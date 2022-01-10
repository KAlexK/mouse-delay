/*******************************************************************************
  Timer/Counter(TCC1) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_tcc1.c

  Summary
    TCC1 PLIB Implementation File.

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
#include "plib_tcc1.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: TCC1 Implementation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Initialize the TCC module in Timer mode */
void TCC1_TimerInitialize( void )
{
    /* Reset TCC */
    TCC1_REGS->TCC_CTRLA = TCC_CTRLA_SWRST_Msk;

    while((TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_SWRST_Msk) == TCC_SYNCBUSY_SWRST_Msk)
    {
        /* Wait for Write Synchronization */
    }

    /* Configure counter mode & prescaler */
    TCC1_REGS->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV1024 ;
    /* Configure in Match Frequency Mode */
    TCC1_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM;

    /* Configure timer period */
    TCC1_REGS->TCC_PER = 46875U;

    /* Clear all interrupt flags */
    TCC1_REGS->TCC_INTFLAG = TCC_INTFLAG_Msk;



    while((TCC1_REGS->TCC_SYNCBUSY) != 0U)
    {
        /* Wait for Write Synchronization */
    }
}

/* Enable the TCC counter */
void TCC1_TimerStart( void )
{
    TCC1_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;
    while((TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) == TCC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Disable the TCC counter */
void TCC1_TimerStop( void )
{
    TCC1_REGS->TCC_CTRLA &= ~TCC_CTRLA_ENABLE_Msk;
    while((TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) == TCC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

uint32_t TCC1_TimerFrequencyGet( void )
{
    return (uint32_t)(46875U);
}

void TCC1_TimerCommandSet(TCC_COMMAND command)
{
    TCC1_REGS->TCC_CTRLBSET = (uint8_t)((uint32_t)command << TCC_CTRLBSET_CMD_Pos);
    while((TCC1_REGS->TCC_SYNCBUSY) != 0U)
    {
        /* Wait for Write Synchronization */
    }    
}

/* Get the current timer counter value */
uint32_t TCC1_Timer24bitCounterGet( void )
{
    /* Write command to force COUNT register read synchronization */
    TCC1_REGS->TCC_CTRLBSET |= (uint8_t)TCC_CTRLBSET_CMD_READSYNC;

    while((TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_CTRLB_Msk) == TCC_SYNCBUSY_CTRLB_Msk)
    {
        /* Wait for Write Synchronization */
    }

    while((TCC1_REGS->TCC_CTRLBSET & TCC_CTRLBSET_CMD_Msk) != 0U)
    {
        /* Wait for CMD to become zero */
    }
    
    /* Read current count value */
    return TCC1_REGS->TCC_COUNT;

}

/* Configure timer counter value */
void TCC1_Timer24bitCounterSet( uint32_t count )
{
    TCC1_REGS->TCC_COUNT = count & 0xFFFFFFU;

    while((TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_COUNT_Msk) == TCC_SYNCBUSY_COUNT_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Configure timer period */
void TCC1_Timer24bitPeriodSet( uint32_t period )
{
    TCC1_REGS->TCC_PER = period & 0xFFFFFFU;
    while((TCC1_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_PER_Msk) == TCC_SYNCBUSY_PER_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Read the timer period value */
uint32_t TCC1_Timer24bitPeriodGet( void )
{
    return TCC1_REGS->TCC_PER;
}





/* Polling method to check if timer period interrupt flag is set */
bool TCC1_TimerPeriodHasExpired( void )
{
    uint8_t timer_status = 0U;
    timer_status = (uint8_t)((TCC1_REGS->TCC_INTFLAG) & TCC_INTFLAG_OVF_Msk);
    TCC1_REGS->TCC_INTFLAG = timer_status;
    return (timer_status != 0U);
}
