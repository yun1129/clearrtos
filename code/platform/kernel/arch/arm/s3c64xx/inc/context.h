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
  
  Date     Version  Name          Description
  -------- -------  ------------  --------------------------------------------

  -------- -------  ------------  --------------------------------------------

 ******************************************************************************/
 
#ifndef __CPU_H
#define __CPU_H

#include "primitive.h"
#include "interrupt.h"

#ifndef __register_t_defined
typedef int register_t;
#endif

typedef struct {
    register_t cpsr_; // stored via r2 indirectly
    register_t r4_;
    register_t r5_;
    register_t r6_;
    register_t r7_;
    register_t r8_;
    register_t r9_;
    register_t r10_;
    register_t ip_;   // r11: Intra-Procedure-call scratch register
    register_t sp_;   // r13: Stack Pointer
    register_t lr_;   // r14: Link Register (function call return address)
    register_t pc_;   // r15: Program Counter (we don't save it indeed)
} general_purpose_registers_t;

typedef struct {
    general_purpose_registers_t gpr_;
} task_context_t;

#ifdef  __cplusplus
extern "C"
{
#endif

void context_init (task_context_t *_p_context, address_t _stack_base, 
    usize_t _stack_size, address_t _task_entry);
void context_switch (task_context_t *_p_current, task_context_t *_p_next);
void context_switch_in_interrupt (task_context_t *_p_current, 
    task_context_t *_p_next);
void context_restore (task_context_t *_p_context, int _value);

#ifdef __cplusplus
}
#endif

#endif

