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
 *   Date: 01/20/2013                                                         *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
  REVISION HISTORY
  ================
  
  Date        Version  Name          Description
  ----------  -------  ------------  -------------------------------------------

  ----------  -------  ------------  -------------------------------------------

 ******************************************************************************/

#include "clib.h"
#include "context.h"

#define CPU_STACK_ALIGNMENT     4

static u32_t g_cpu_mode = 0x13;

void context_init (task_context_t *_p_context, address_t _stack_base, 
    usize_t _stack_size, address_t _task_entry)
{
    _p_context->sp_ = _stack_base + _stack_size ;
    _p_context->lr_ = _task_entry;
    _p_context->cpsr_ = g_cpu_mode;
}

void context_switch_in_interrupt (task_context_t *_p_current, 
    task_context_t *_p_next)
{
    UNUSED (_p_current);
    UNUSED (_p_next);
    // TBD
}

