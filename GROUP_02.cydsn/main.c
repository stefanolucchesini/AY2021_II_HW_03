/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "InterruptRoutines.h"
#include "Logging.h"
#include "project.h"

/**
* \brief Size of data buffer for I2C slave device
*/
#define SLAVE_BUFFER_SIZE 7


uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; ///< Buffer for the slave device
uint8_t Ctrl_Reg_1 = 0;
uint8_t Ctrl_Reg_2 = 0;
uint16_t Light_array[ARRAY_LENGTH];
uint16_t Temp_array[ARRAY_LENGTH];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Start logging interface */
    Logging_Start();
    
    /* Start EZI2C Component */
    EZI2C_Start();
    
    /* Start MUX */
    AMux_Init();
    
    /* Start timer and associated ISR */
    Timer_Count_Start();
    isr_Count_StartEx(Custom_Timer_Count_ISR);
    
    // Set up Slave Buffer
    slaveBuffer[0] = Ctrl_Reg_1;
    slaveBuffer[1] = Ctrl_Reg_2;
    
    // Set up who am i register
    slaveBuffer[2] = 0xBC;

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
