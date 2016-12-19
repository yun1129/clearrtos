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
  
  Date     Version  Name          Description
  -------- -------  ------------  --------------------------------------------

  -------- -------  ------------  --------------------------------------------

 ******************************************************************************/

#include "offset.h"

/*
 *  void context_restore (task_context_t *_p_context)
 *
 */

    .globl _context_restore
    .globl context_restore
    .align 4

_context_restore:
context_restore:
    ldmia   r0,  {r2, r4, r5, r6, r7, r8, r9, r10, r11, r13, r14}
    msr cpsr, r2
#ifdef __thumb__
    bx  lr
    nop
#else
    mov pc, lr
#endif
    
