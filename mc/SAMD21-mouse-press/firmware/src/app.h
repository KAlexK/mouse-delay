/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

#ifndef _APP_H
#define _APP_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions

// Last word in RAM
// Unlike for ordinary applications, our link script doesn't place the stack at the bottom
// of the RAM, but instead after all allocated BSS.
// In other words, this word should survive reset.

#define DBL_TAP_PTR ((volatile uint32_t *)(HMCRAMC0_ADDR + HMCRAMC0_SIZE - 4))
#define DBL_TAP_MAGIC 0xf01669ef // Randomly selected, adjusted to have first and last bit set

#define DEV_ID 60  // ID ?????????? - Seeeduino XIAO (ATSAMD21G18A-MU)

// Список команд, которые передаются по USB
#define TOGGLE_LED 0x80 // ????????-????????? ????????? ?????????
#define SW_STATE   0x81 // Передаем значение таймера - задержка
#define SW_ON      0x82 // Включаем кнопку мыши
#define UPDATE_FW  0x85    // ???????????? ? ????? ?????????? ??????????????
#define GET_DEV_ID 0xA0   // ???????? ID ??????????

#if defined (__PIC32MZ__) || defined (__PIC32MX__) || defined (_SAMD21J18A_H_) || defined (_SAMD21G18A_H_) || defined (_SAME54P20A_H_) ||  defined (__PIC32MM__) ||  defined (_PIC32CM5164LE00100_H_)
#define APP_EP_BULK_OUT 1
#define APP_EP_BULK_IN 1
//#warning "EP 1 & 1"
#else
#define APP_EP_BULK_OUT 1
#define APP_EP_BULK_IN 2
//#warning "EP 1 & 2"
#endif 

#define LED_Off() (LED_Set())  // Blue LED
#define LED_On() (LED_Clear())
#define LED_YELLOW_Off() (LED_YELLOW_Set())  // Yellow LED
#define LED_YELLOW_On() (LED_YELLOW_Clear())
#define LED_BLUE_Off() (LED_BLUE_Set())  // Second Blue LED
#define LED_BLUE_On() (LED_BLUE_Clear())
#define MOUSE_Off() (SWITCH_Clear())  // Кнопка мыши, для выключения сажаем на землю
#define MOUSE_On() (SWITCH_Set()) // Кнопка мыши, для включения устанавливаем высокий уровень

#define SWITCH_STATE_PRESSED   0
#define SWITCH_STATE_RELEASED  1

void resetIntoBootloader(void);
void EIC11_Callback(uintptr_t context);
void TCC0_Callback(uint32_t status, uintptr_t context);
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

/* Application States

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
 */

typedef enum 
{
    /* Application's state machine's initial state. */
    APP_STATE_INIT = 0,
    /* Application waits for device configuration */
    APP_STATE_WAIT_FOR_CONFIGURATION,
    /* Application runs the main task */
    APP_STATE_MAIN_TASK,
    /* Application error occurred */
    APP_STATE_ERROR
} APP_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
   /* Device layer handle returned by device layer open function */
    USB_DEVICE_HANDLE usbDevHandle;
    /* Application state*/
    APP_STATES state;
    /* Track device configuration */
    bool deviceIsConfigured;
    /* Configuration value */
    uint8_t configValue;
    /* speed */
    USB_SPEED speed;
    /* ep data sent */
    bool epDataWritePending;
    /* ep data received */
    bool epDataReadPending;
    /* Transfer handle */
    USB_DEVICE_TRANSFER_HANDLE writeTranferHandle;
    /* Transfer handle */
    USB_DEVICE_TRANSFER_HANDLE readTranferHandle;
    /* The transmit endpoint address */
    USB_ENDPOINT_ADDRESS endpointTx;
    /* The receive endpoint address */
    USB_ENDPOINT_ADDRESS endpointRx;
    /* Tracks the alternate setting */
    uint8_t altSetting;
    uint32_t delay1_val; // Сохраняем значение первой задержки - от нажатия кнопки до замыкания контактов
    bool delayStart; // Запущено измерение задержки
} APP_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks ( void );


#endif /* _APP_H */
/*******************************************************************************
 End of File
 */

