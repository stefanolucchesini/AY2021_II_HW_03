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
#include "stdio.h"

/**
* \brief Size of data buffer for I2C slave device
*/

uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; ///< Buffer for the slave device
uint8_t Ctrl_Reg_1 = 0;
uint8_t Ctrl_Reg_2 = 0;
uint8_t MSB_Light = 0, LSB_Light = 0;  //invalid values
uint8_t MSB_Temp = 0, LSB_Temp = 0;    //invalid values
uint16_t Light_array[ARRAY_LENGTH];
uint16_t Temp_array[ARRAY_LENGTH];
uint8_t count = 0;
uint8_t samplenumber = 5;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Start logging interface */
    Logging_Start();
    /* UART initialization */
    UART_Debug_Start();
    
    /* Start EZI2C Component */
    EZI2C_Start();
    
    /* Start MUX */
    AMux_Init();
    
    /* Start ADC */
    ADC_DelSig_Start();
    ADC_DelSig_StartConvert();
    
    // Set up Slave Buffer
    
    //Control registers default values
    slaveBuffer[0] = Ctrl_Reg_1;
    slaveBuffer[1] = Ctrl_Reg_2;
    // Set up who am i register
    slaveBuffer[2] = WHO_AM_I;
    //Ch0 Bit 15-8
    slaveBuffer[3] = MSB_Temp;
    //Ch0 Bit 7-0
    slaveBuffer[4] = LSB_Temp;
    //Ch1 Bit 15-8
    slaveBuffer[5] = MSB_Light;
    //Ch1 Bit 7-0
    slaveBuffer[6] = LSB_Light;
    
    // Set up EZI2C buffer
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 1, slaveBuffer);
    
        /* Start timer and associated ISR */
    Timer_Count_Start();
    isr_Count_StartEx(Custom_Timer_Count_ISR);
    
    //Start timer for updating registers at 50Hz
    Timer_Send_Start();
    isr_Send_StartEx(Custom_Timer_Send_ISR);

    for(;;)
    {      
      
        if( count >= samplenumber){
            count = 0;
            uint16_t lightvalue = 0, tempvalue = 0;

            for(int i = 0; i< samplenumber; i++){
                lightvalue += Light_array[i];
                tempvalue += Temp_array[i];
            }
            
            lightvalue /= samplenumber;
            tempvalue /= samplenumber;
            
            //Debug to see if ADC works properly
            char message[30];
            sprintf(message, "Temp value: %x\r\n", tempvalue);
            UART_Debug_PutString(message);
            sprintf(message, "Light value: %x\r\n", lightvalue);
            UART_Debug_PutString(message);
            
            //Split light and temp values into MSB and LSB
            MSB_Temp = (tempvalue & 0xFF00) >> 8;    
            LSB_Temp = tempvalue & 0x00FF;
            sprintf(message, "Binary TEMPERATURE MSB: %x LSB: %x\r\n",MSB_Temp,LSB_Temp);
            UART_Debug_PutString(message);
            
            MSB_Light = (lightvalue & 0xFF00) >> 8;    
            LSB_Light = lightvalue & 0x00FF;
            sprintf(message, "Binary LIGHT MSB: %x LSB: %x\r\n",MSB_Light,LSB_Light);
            UART_Debug_PutString(message);
            
        }
    }
}

/* [] END OF FILE */
