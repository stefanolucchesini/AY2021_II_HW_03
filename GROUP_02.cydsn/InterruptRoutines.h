/**
* \file InterruptRoutines.h
*
* \brief Header file for interrupt routines
*
* This header files contains the function declarations
* for the interrupt services routines.
*
* \author Davide Marzorati
* \date October 14, 2019
*/

#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include <cytypes.h>
    
    #define MUX_TEMP 0
    #define MUX_LIGHT 1
    #define ARRAY_LENGTH 16
    #define SLAVE_BUFFER_SIZE 7
    #define WHO_AM_I 0xBC
    #define MIN_ALLOWED_PERIOD 2
    #define DEVICE_STOPPED 0
    #define CH0 1
    #define CH1 2
    #define BOTH_CHANNELS 3
    
    /**
    *   \brief ISR called on timer overflow
    */
    CY_ISR_PROTO(Custom_Timer_Count_ISR);
    CY_ISR_PROTO(Custom_Timer_Send_ISR);
    
 #endif
/* [] END OF FILE */
