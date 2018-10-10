/*
 * MODBUS Library: ARM STM32 Port (FWLIB 2.0x)
 * Copyright (c) Christian Walter <cwalter@embedded-solutions.at>
 * All rights reserved.
 *
 * ARM STM32 Port by Niels Andersen, Elcanic A/S <niels.andersen.elcanic@gmail.com>
 *
 * $Id: mbportother.c,v 1.1 2008-12-14 19:33:32 cwalter Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include <stdlib.h>

/* ----------------------- Platform includes --------------------------------*/
#include "mbport.h"
#include "common/mbtypes.h"
#include "common/mbframe.h"
#include "common/mbutils.h"

/* ----------------------- Modbus includes ----------------------------------*/

/* ----------------------- Defines ------------------------------------------*/
#define PORT_INTERRUPT_PRIORITY_MAX     ( 1 )

/* ----------------------- Type definitions ---------------------------------*/

/* ----------------------- Static variables ---------------------------------*/

static UBYTE    ubNesting = 0;

/* ----------------------- Static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/

void
assert_failed( uint8_t * file, uint32_t line )
{
    ( void )file;
    ( void )line;
    vMBPAssert(  );
}

void
vMBPAssert( void )
{
    volatile BOOL   bBreakOut = FALSE;

    vMBPEnterCritical(  );
    while( !bBreakOut );
}

void
vMBPEnterCritical( void )
{
//    __asm volatile  ( "mov r0, %0\n" "msr basepri, r0\n"::"i" ( PORT_INTERRUPT_PRIORITY_MAX ):"r0" );
    __disable_irq(  );

    ubNesting++;
}

void
vMBPExitCritical( void )
{
    ubNesting--;
    if( 0 == ubNesting )
    {
//        __asm volatile  ( "mov r0, #0\n" "msr basepri, r0\n":::"r0" );

        __enable_irq(  );
    }
}

void
vMBPSetDebugPin( eMBPDebugPin ePinName, BOOL bTurnOn )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    STATIC BOOL     bIsInitalized = FALSE;

    if( !bIsInitalized )
    {
        vMBPEnterCritical(  );

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_Init( GPIOC, &GPIO_InitStructure );

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_Init( GPIOA, &GPIO_InitStructure );

        bIsInitalized = TRUE;
        vMBPExitCritical(  );
    }

    switch ( ePinName )
    {
    case MBP_DEBUGPIN_0:
        GPIO_WriteBit( GPIOC, GPIO_Pin_2, bTurnOn ? Bit_SET : Bit_RESET );
        break;
    case MBP_DEBUGPIN_1:
        GPIO_WriteBit( GPIOA, GPIO_Pin_0, bTurnOn ? Bit_SET : Bit_RESET );
        break;
    default:
        break;
    }
}
