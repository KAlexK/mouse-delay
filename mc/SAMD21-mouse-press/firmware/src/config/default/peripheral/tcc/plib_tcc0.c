/*******************************************************************************
  Timer/Counter(TCC0) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_tcc0.c

  Summary
    TCC0 PLIB Implementation File.

  Description
    This file defines the interface to the TCC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/

#include "interrupts.h"
#include "plib_tcc0.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

static TCC_CALLBACK_OBJECT TCC0_CallbackObject;

// *****************************************************************************
// *****************************************************************************
// Section: TCC0 Implementation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Initialize the TCC module in Timer mode */
void TCC0_TimerInitialize( void )
{
    /* Reset TCC */
    TCC0_REGS->TCC_CTRLA = TCC_CTRLA_SWRST_Msk;

    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_SWRST_Msk) == TCC_SYNCBUSY_SWRST_Msk)
    {
        /* Wait for Write Synchronization */
    }

    /* Configure counter mode & prescaler */
    TCC0_REGS->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV64 ;
    /* Configure in Match Frequency Mode */
    TCC0_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM;

    /* Configure timer one shot mode */
    TCC0_REGS->TCC_CTRLBSET = (uint8_t)TCC_CTRLBSET_ONESHOT_Msk;
    /* Configure timer period */
    TCC0_REGS->TCC_PER = 187500U;

    /* Clear all interrupt flags */
    TCC0_REGS->TCC_INTFLAG = TCC_INTFLAG_Msk;

    TCC0_CallbackObject.callback_fn = NULL;
    /* Enable interrupt*/
    TCC0_REGS->TCC_INTENSET = (TCC_INTENSET_OVF_Msk);


    while((TCC0_REGS->TCC_SYNCBUSY) != 0U)
    {
        /* Wait for Write Synchronization */
    }
}

/* Enable the TCC counter */
void TCC0_TimerStart( void )
{
    /* In one-shot timer mode, first disable the timer */
    TCC0_REGS->TCC_CTRLA &= ~TCC_CTRLA_ENABLE_Msk;
    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) == TCC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
    TCC0_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;
    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) == TCC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Disable the TCC counter */
void TCC0_TimerStop( void )
{
    TCC0_REGS->TCC_CTRLA &= ~TCC_CTRLA_ENABLE_Msk;
    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) == TCC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

uint32_t TCC0_TimerFrequencyGet( void )
{
    return (uint32_t)(750000U);
}

void TCC0_TimerCommandSet(TCC_COMMAND command)
{
    TCC0_REGS->TCC_CTRLBSET = (uint8_t)((uint32_t)command << TCC_CTRLBSET_CMD_Pos);
    while((TCC0_REGS->TCC_SYNCBUSY) != 0U)
    {
        /* Wait for Write Synchronization */
    }    
}

/* Get the current timer counter value */
uint32_t TCC0_Timer24bitCounterGet( void )
{
    /* Write command to force COUNT register read synchronization */
    TCC0_REGS->TCC_CTRLBSET |= (uint8_t)TCC_CTRLBSET_CMD_READSYNC;

    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_CTRLB_Msk) == TCC_SYNCBUSY_CTRLB_Msk)
    {
        /* Wait for Write Synchronization */
    }

    while((TCC0_REGS->TCC_CTRLBSET & TCC_CTRLBSET_CMD_Msk) != 0U)
    {
        /* Wait for CMD to become zero */
    }
    
    /* Read current count value */
    return TCC0_REGS->TCC_COUNT;

}

/* Configure timer counter value */
void TCC0_Timer24bitCounterSet( uint32_t count )
{
    TCC0_REGS->TCC_COUNT = count & 0xFFFFFFU;

    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_COUNT_Msk) == TCC_SYNCBUSY_COUNT_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Configure timer period */
void TCC0_Timer24bitPeriodSet( uint32_t period )
{
    TCC0_REGS->TCC_PER = period & 0xFFFFFFU;
    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_PER_Msk) == TCC_SYNCBUSY_PER_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Read the timer period value */
uint32_t TCC0_Timer24bitPeriodGet( void )
{
    return TCC0_REGS->TCC_PER;
}



/* Register callback function */
void TCC0_TimerCallbackRegister( TCC_CALLBACK callback, uintptr_t context )
{
    TCC0_CallbackObject.callback_fn = callback;

    TCC0_CallbackObject.context = context;
}

/* Timer Interrupt handler */
void TCC0_InterruptHandler( void )
{
    uint32_t status;
    status = (uint32_t) TCC0_REGS->TCC_INTFLAG;
    /* Clear interrupt flags */
    TCC0_REGS->TCC_INTFLAG = TCC_INTFLAG_Msk;
    if( TCC0_CallbackObject.callback_fn != NULL)
    {
        TCC0_CallbackObject.callback_fn(status, TCC0_CallbackObject.context);
    }

}
  


