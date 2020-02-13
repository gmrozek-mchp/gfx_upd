/*******************************************************************************
 Module for Microchip Legato Graphics Library

  Company:
    Microchip Technology Inc.

  File Name:
    generated/le_gen_harmony.h

  Summary:
    Autogenerated header file for the Legato graphics library.

  Description:
    This file contains autogenerated code that binds the Legato graphics library
    and the MPLAB Harmony framework together.
*******************************************************************************/

#ifndef LE_HARMONY_H
#define LE_HARMONY_H

#include "gfx/legato/generated/le_gen_init.h"

typedef enum
{
	LEGATO_STATE_INIT = 0,
	LEGATO_STATE_RUNNING,
	LEGATO_STATE_ERROR
} LegatoState;

/******************************************************************************
  Function:
	void Legato_Initialize(void)

  Summary:
	This function initializes the Legato state machine.
 */
void Legato_Initialize(void);


/******************************************************************************
  Function:
	void Legato_Tasks(void);

  Summary:
	This function is called in SYS_Tasks.
*/
void Legato_Tasks(void);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // LE_HARMONY_H
