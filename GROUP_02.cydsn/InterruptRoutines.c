/**
* This file contains all the source code required
* for the interrupt service routines.
*/

// Include required header files
#include "InterruptRoutines.h" 
#include "Timer_Count.h"
#include "project.h"

// Our global variables
extern uint8_t slaveBuffer[];
extern uint16_t Light_array[ARRAY_LENGTH];
extern uint16_t Temp_array[ARRAY_LENGTH];
extern uint8_t count;
extern uint8_t samplenumber;
extern uint8_t MSB_Light, LSB_Light;
extern uint8_t MSB_Temp, LSB_Temp;
extern uint8_t flag_send;
extern uint8_t status;

CY_ISR(Custom_Timer_Count_ISR)
{
    // Read timer status register to pull interrupt line low
    Timer_Count_ReadStatusRegister();
    
    // Sample ADC both light and temp
    AMux_Select(MUX_LIGHT);
    Light_array[count] = ADC_DelSig_Read32();
    
    AMux_Select(MUX_TEMP);
    Temp_array[count] = ADC_DelSig_Read32();
    
    count++;
    
    if(count == samplenumber){
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

}

CY_ISR(Custom_Timer_Send_ISR)
{
    // Read timer status register to pull interrupt line low
    Timer_Send_ReadStatusRegister();
    
    //Raise flag to send data via I2C
    flag_send = 1;
}

/**
*   This function is called when exiting the EZI2C_ISR. Here we
*   perform all the tasks based on the requests.
*/
void EZI2C_ISR_ExitCallback(void)
{
    //CONTROL REGISTER 1 READ
    uint8_t cnt1val = slaveBuffer[0];  
    //Status bits update
    status = slaveBuffer[0] & 0b00000011;
    //Average samples number extraction from register 1
    cnt1val &= 0b00111100;
    cnt1val = (cnt1val >> 2);
    samplenumber = cnt1val;  
        
    
    //Timer period update
    if(slaveBuffer[1] == 0 || slaveBuffer[1] == 1) Timer_Count_WritePeriod(MIN_ALLOWED_PERIOD);
    else Timer_Count_WritePeriod(slaveBuffer[1]);
        
    
    //WHO_AM_I register change not allowed
    if(slaveBuffer[2] != WHO_AM_I) slaveBuffer[2] = WHO_AM_I;
    
    //Update I2C slave registers
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 1, slaveBuffer);
    
}
/* [] END OF FILE */
