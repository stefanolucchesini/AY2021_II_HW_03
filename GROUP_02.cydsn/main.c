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

char message[30];

volatile uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; ///< Buffer for the slave device
volatile uint8_t Ctrl_Reg_1 = 0;
volatile uint8_t Ctrl_Reg_2 = 0;
volatile uint8_t MSB_Light = 0, LSB_Light = 0;  //invalid values
volatile uint8_t MSB_Temp = 0, LSB_Temp = 0;    //invalid values
volatile uint16_t Light_array[ARRAY_LENGTH];
volatile uint16_t Temp_array[ARRAY_LENGTH];
volatile uint8_t count = 0;
volatile uint8_t samplenumber = 5;
volatile uint8_t flag_send = 0;
volatile uint8_t status = 0;

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
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, RWSIZE, slaveBuffer);
    
        /* Start timer and associated ISR */
    Timer_Count_Start();
    isr_Count_StartEx(Custom_Timer_Count_ISR);
    
    //Start timer for updating registers at 50Hz
    Timer_Send_Start();
    isr_Send_StartEx(Custom_Timer_Send_ISR);
    

    for(;;)
    {
        sprintf(message, "Timer period: %d\r\n", Timer_Count_ReadPeriod());
        UART_Debug_PutString(message);
        if(count >= samplenumber){
        count = 0;
        uint16_t lightvalue = 0, tempvalue = 0;
        
        //Average of values
        for(int i = 0; i< samplenumber; i++){
            lightvalue += Light_array[i];
            tempvalue += Temp_array[i];
        }
        lightvalue /= samplenumber;
        tempvalue /= samplenumber;
        
        //Split light and temp values into MSB and LSB
        MSB_Temp = (tempvalue & 0xFF00) >> 8;    
        LSB_Temp = tempvalue & 0x00FF;
        
        MSB_Light = (lightvalue & 0xFF00) >> 8;    
        LSB_Light = lightvalue & 0x00FF;
        }
        
        //Update slave registers
        if(flag_send){
        flag_send=0;
        switch (status){
        case DEVICE_STOPPED:
            break;
        case CH0:
            //Ch0 Bit 15-8
            slaveBuffer[3] = MSB_Temp;
            //Ch0 Bit 7-0
            slaveBuffer[4] = LSB_Temp;
            //Ch1 Bit 15-8
            slaveBuffer[5] = 0;
            //Ch1 Bit 7-0
            slaveBuffer[6] = 0;
            Pin_LED_Write(LED_OFF); //Control LED
            break;
        case CH1:
            //Ch0 Bit 15-8
            slaveBuffer[3] = 0;
            //Ch0 Bit 7-0
            slaveBuffer[4] = 0;
            //Ch1 Bit 15-8
            slaveBuffer[5] = MSB_Light;
            //Ch1 Bit 7-0
            slaveBuffer[6] = LSB_Light;
            Pin_LED_Write(LED_OFF); //Control LED
            break;
        case BOTH_CHANNELS:
            //Ch0 Bit 15-8
            slaveBuffer[3] = MSB_Temp;
            //Ch0 Bit 7-0
            slaveBuffer[4] = LSB_Temp;
            //Ch1 Bit 15-8
            slaveBuffer[5] = MSB_Light;
            //Ch1 Bit 7-0
            slaveBuffer[6] = LSB_Light;
            Pin_LED_Write(LED_ON); //Control LED
            break;
        default: break;    
            }
          
        } 

    }
}

/* [] END OF FILE */
