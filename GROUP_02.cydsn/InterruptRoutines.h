/**
* \file InterruptRoutines.h
*
* \brief Header file for interrupt routines
*
* This header file contains the function declarations
* for the interrupt services routines.
*
* \authors Stefano Lucchesini, Giovanni Marelli
* \date April 2020
*/

#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include <cytypes.h>
    
    #define MUX_TEMP 0
    #define MUX_LIGHT 1
    #define ARRAY_LENGTH 15
    #define SLAVE_BUFFER_SIZE 7
    #define WHO_AM_I 0xBC
    #define MIN_ALLOWED_PERIOD 2
    #define DEFAULT_SAMPLE_NUMBER 5
    #define DEVICE_STOPPED 0
    #define CH0 1
    #define CH1 2
    #define BOTH_CHANNELS 3
    #define RWSIZE 2
    #define LED_ON 1
    #define LED_OFF 0
    #define DEFAULT_TIM_PERIOD 51
    
    /**
    *   \brief ISR called on timers overflow
    */
    CY_ISR_PROTO(Custom_Timer_Count_ISR);
    CY_ISR_PROTO(Custom_Timer_Send_ISR);
    
 #endif
/* [] END OF FILE */
