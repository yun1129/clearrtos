/******************************************************************************
 *                                                                            *
 * This program is distributed in the hope that it will be useful, but        *
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE. This file and program are licensed    *
 * under the GNU Lesser General Public License Version 3, 29 June 2007.       *
 * The complete license can be accessed from the following location:          *
 * http://opensource.org/licenses/lgpl-3.0.html                               *
 *                                                                            *
 * Author: Yun Li (yunli.open@gmail.com)                                      *
 *   Date: 01/12/2012                                                         *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
  REVISION HISTORY
  ================
  
  Date        Version  Name          Description
  ----------  -------  ------------  -------------------------------------------

  ----------  -------  ------------  -------------------------------------------

 ******************************************************************************/

#include "primitive.h"
#include "interrupt.h"

#ifdef __thumb__
#define SWITCH_REGISTERS u32_t switch_reg
#define SWITCH_TO_ARM    ".align 2\nbx pc\n.arm\n"
#define SWITCH_BACK      "add %[switch_reg], pc, #1\nbx %[switch_reg]\n.thumb\n"
#define SWITCH_OUTPUT    [switch_reg] "=&r" (switch_reg)
#else
#define SWITCH_REGISTERS
#define SWITCH_TO_ARM
#define SWITCH_BACK
#define SWITCH_OUTPUT
#endif

typedef enum {
    EXCEPTION_RESET,
    EXCEPTION_UNDEFINED_INSTRUCTION,
    EXCEPTION_SOFTWARE_INTERRUPT, // SWI
    EXCEPTION_PREFETCH_ABORT,
    EXCEPTION_DATA_ABORT,
    EXCEPTION_RESERVED,
    EXCEPTION_IRQ, // normal interrupt
    EXCEPTION_FIQ, // fast interrupt
    
    // shoud be put at the end of this enum
    EXCEPTION_COUNT
} exception_type_t;

static int g_interrupt_nested_count;
static interrupt_handler_t g_interrupt_vector [CONFIG_MAX_INTERRUPT];
static interrupt_handler_t g_device_interrupt_handler;
static interrupt_exit_callback_t g_interrupt_exit_callback;

static inline void global_interrupt_enable (interrupt_level_t _level)
{
    SWITCH_REGISTERS;

    asm volatile (
        SWITCH_TO_ARM
        "msr cpsr, %[level]\n"
        SWITCH_BACK
        : SWITCH_OUTPUT
        : [_level] "r" (_level)
    );
}

static inline interrupt_level_t global_interrupt_disable ()
{
    u32_t switch_reg;
    u32_t level;

    asm volatile (
        SWITCH_TO_ARM
        "mrs %[level], cpsr\n"
        "orr %[switch_reg], %[level], #0x80\n"
        "msr cpsr, %[switch_reg]\n"
        SWITCH_BACK
        : [switch_reg] "=&r" (switch_reg), [level] "=&r" (level)
    );

    return level;
}

void interrupt_enter ()
{
    interrupt_level_t level;

    level = global_interrupt_disable ();
    g_interrupt_nested_count ++;
    global_interrupt_enable (level);
}

void interrupt_exit ()
{
    interrupt_level_t level;

    level = global_interrupt_disable ();
    g_interrupt_nested_count --;
    global_interrupt_enable (level);
    // give an opportunity to the scheduler for task switch
    if (0 == g_interrupt_nested_count && 0 != g_interrupt_exit_callback) {
        //g_interrupt_exit_callback ();
    }
}

bool is_in_interrupt ()
{
    return (bool) (g_interrupt_nested_count != 0);
}

static void interrupt_handler (int _vector)
{
    interrupt_enter ();
    if (g_device_interrupt_handler != null) {
        g_device_interrupt_handler (_vector);
    }
    else {
        // oops! log error? hold on, we are in interrupt!
    }
    interrupt_exit ();
}

void device_interrupt_handler_install (interrupt_handler_t _handler)
{
    g_device_interrupt_handler = _handler;
}

void interrupt_exit_callback_install (interrupt_exit_callback_t _handler)
{
    g_interrupt_exit_callback = _handler;
}

interrupt_handler_t interrupt_handler_install (int _vector, 
    interrupt_handler_t _handler)
{
    interrupt_handler_t old;

    if (_vector > INTERRUPT_LAST_INDEX) {
        return null;
    }
    
    old = g_interrupt_vector [_vector];
    g_interrupt_vector [_vector] = _handler;
    return old;
}

void interrupt_enable (int _vector)
{
    UNUSED (_vector);
    // do nothing on Linux for simplifying
}

void interrupt_disable (int _vector)
{
    UNUSED (_vector);
    // do nothing on Linux for simplifying
}

error_t module_interrupt (system_state_t _state)
{
    if (STATE_INITIALIZING == _state) {
        for (int vector = 0; vector <= INTERRUPT_LAST_INDEX; vector ++) {
            g_interrupt_vector [vector] = interrupt_handler;
        }
        g_interrupt_nested_count = 0;
        global_interrupt_enable (INTERRUPT_ENABLED);
    }
    else if (STATE_DESTROYING == _state) {
        global_interrupt_enable (INTERRUPT_DISABLED);
    }
    return 0;
}

