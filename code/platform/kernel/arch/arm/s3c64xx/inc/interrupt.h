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
 *   Date: 07/30/2010                                                         *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
  REVISION HISTORY
  ================
  
  Date     Version  Name          Description
  -------- -------  ------------  --------------------------------------------

  -------- -------  ------------  --------------------------------------------

 ******************************************************************************/
 
#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "primitive.h"
#include "error.h"
#include "module.h"
#include "clib.h"

#define CONFIG_MAX_INTERRUPT    64
#define INTERRUPT_LAST_INDEX    (CONFIG_MAX_INTERRUPT - 1)
#define INTERRUPT_NONE          (-1)

// currently, we don't support multi-core process, so, memory barrier isn't 
// required. read on at http://en.wikipedia.org/wiki/memory_barrier.
#define memory_barrier() asm volatile ("" ::: "memory")
#define interrupt_flash() interrupt_enable (level); \
    memory_barrier (); \
    level = interrupt_disable ();

typedef void (*interrupt_handler_t) (int _vector);
typedef void (*interrupt_exit_callback_t) ();

typedef enum {
    INTERRUPT_ENABLED,
    INTERRUPT_DISABLED
} interrupt_level_t;

#ifdef  __cplusplus
extern "C" {
#endif

error_t module_interrupt (system_state_t _state);

static inline void global_interrupt_enable (interrupt_level_t _level);
static inline interrupt_level_t global_interrupt_disable ();

void device_interrupt_handler_install (interrupt_handler_t _handler);
void interrupt_exit_callback_install (interrupt_exit_callback_t _handler);
interrupt_handler_t interrupt_handler_install (int _vector, 
    interrupt_handler_t _handler);

void interrupt_enable (int _vector);
void interrupt_disable (int _vector);

void interrupt_enter ();
void interrupt_exit ();
bool is_in_interrupt ();

#ifdef __cplusplus
}
#endif

#endif

