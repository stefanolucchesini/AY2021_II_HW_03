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
uint8_t MSB_Light = 0, LSB_Light = 0;  //invalid values
uint8_t MSB_Temp = 0, LSB_Temp = 0;    //invalid values
uint16_t Light_array[ARRAY_LENGTH];
uint16_t Temp_array[ARRAY_LENGTH];
uint8_t count = 0;

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
    //Ch0 Bit 15-8
    slaveBuffer[3] = MSB_Temp;
    //Ch0 Bit 7-0
    slaveBuffer[4] = LSB_Temp;
    //Ch1 Bit 15-8
    slaveBuffer[5] = MSB_Light;
    //Ch1 Bit 7-0
    slaveBuffer[6] = LSB_Light;
    
    // Set up EZI2C buffer
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 1 ,slaveBuffer);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {      
      
        if( count == ARRAY_LENGTH){
            count = 0;
            uint16_t lightvalue = 0, tempvalue = 0;

            for(int i = 0; i< ARRAY_LENGTH; i++){
                lightvalue += Light_array[i];
                tempvalue += Temp_array[i];
            }
            
            lightvalue /= ARRAY_LENGTH;
            tempvalue /= ARRAY_LENGTH;
            //COunt 5 samples --> Send data via I2C
        }
    }
}

/* [] END OF FILE */
