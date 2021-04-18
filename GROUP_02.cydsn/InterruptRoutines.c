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
    
}

CY_ISR(Custom_Timer_Send_ISR)
{
    // Read timer status register to pull interrupt line low
    Timer_Send_ReadStatusRegister();
    
    
}

/**
*   This function is called when exiting the EZI2C_ISR. Here we
*   perform all the tasks based on the requests.
*/
void EZI2C_ISR_ExitCallback(void)
{
    //Average samples number extraction from register 1
    uint8_t cnt1val = slaveBuffer[0];  
    cnt1val &= 0b00111100;
    cnt1val = (cnt1val >> 2);
    samplenumber = cnt1val;  
    
    //Timer period update
    if(slaveBuffer[1] == 0 || slaveBuffer[1] == 1) Timer_Count_WritePeriod(MIN_ALLOWED_PERIOD);
    else Timer_Count_WritePeriod(slaveBuffer[1]);
    
    //Status bits update
    
    
    
    if(slaveBuffer[2] != WHO_AM_I) slaveBuffer[2] = WHO_AM_I;
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 1, slaveBuffer);
    
}
/* [] END OF FILE */
