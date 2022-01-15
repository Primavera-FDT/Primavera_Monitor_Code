/**********************************************************************
* File:             uart_receive.h
* Author:           D. Theran. 
**********************************************************************/

#ifndef __PRIMAVERA_UART_RECEIVE_H__  
#define __PRIMAVERA_UART_RECEIVE_H__

/**********************************************************************
*                           USER COMMANDS                            *
**********************************************************************/
/************************************************************
* The possible user commands and its translation will be:
* 
* --> 'S' = SEND
* --> 'I' = STREAM_START
* --> 'O' = STREAM_STOP
* --> 'U' = STATUS
* --> 'L' = LOG
* --> 'P' = EJECT_SD
* --> 'M' = MOUNT_SD
* --> 'T' = TIME_SET
*************************************************************/

/**********************************************************************
*                            DEPENDENCIES                            *
**********************************************************************/

#include "controller.h"

/**********************************************************************
*                      GLOBALS AND DEFINITIONS                       *
**********************************************************************/
#define QUEUE_SEND_TO_MAIN 1    /* Queue where the command will be 
                                   sent to the main */

/* enum command
 *
 * Commands that will be sent to the main task  
 */
enum command {

    SEND,
    STREAM_START,
    STREAM_STOP,
    STATUS,
    LOG,
    EJECT_SD,
    MOUNT_SD,
    TIME_SET
};

/**********************************************************************
*                  PROTOTYPES OF EXPORTED FUNCTIONS                  *
**********************************************************************/

/***********************************************
*  Task to receive the command from the user  *
***********************************************/
void vUartReceive(void *pvParameters);

/********************************************************************
*  Initialize all the configuration parameters of the UART module  *
********************************************************************/
void UART_init(void);

/*****************************************************
*  Send the correspondent command to the main task  *
*****************************************************/
void SendCommand(char command);

#endif //__PRIMAVERA_UART_RECEIVE_H__

// end of the file uart_receive.h
