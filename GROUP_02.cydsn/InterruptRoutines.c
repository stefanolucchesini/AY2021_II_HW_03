/**
* This file contains all the source code required
* for the interrupt service routines.
*/

// Include required header files
#include "InterruptRoutines.h" 
#include "Timer_Count.h"
#include "project.h"
#include "stdio.h"

// Our global variables
extern volatile uint8_t slaveBuffer[];
extern volatile uint16_t Light_array[ARRAY_LENGTH];
extern volatile uint16_t Temp_array[ARRAY_LENGTH];
extern volatile uint8_t count;
extern volatile uint8_t samplenumber;
extern volatile uint8_t MSB_Light, LSB_Light;
extern volatile uint8_t MSB_Temp, LSB_Temp;
extern volatile uint8_t flag_send;
extern volatile uint8_t status;
extern volatile uint8_t Ctrl_Reg_1;
extern volatile uint8_t Ctrl_Reg_2;
uint8_t old_timer_period = DEFAULT_TIM_PERIOD;

CY_ISR(Custom_Timer_Count_ISR)
{
    // Read timer status register to pull interrupt line low
    Timer_Count_ReadStatusRegister();
    
    AMux_Select(MUX_TEMP);
    Temp_array[count] = ADC_DelSig_Read32();
    if(Temp_array[count]<0) Temp_array[count] = 0;
    if(Temp_array[count]>65535) Temp_array[count] = 65535;
    
    AMux_Select(MUX_LIGHT);
    Light_array[count] = ADC_DelSig_Read32();
    if(Light_array[count]<0) Light_array[count] = 0;
    if(Light_array[count]>65535) Light_array[count] = 65535;
    
    count++;
    count = (count > ARRAY_LENGTH-1) ? 0 : count; //memory protection

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
    //CONTROL REGISTER 1 and 2 READ
    Ctrl_Reg_1 = slaveBuffer[0];
    Ctrl_Reg_2 = slaveBuffer[1];
    uint8_t cnt1val = Ctrl_Reg_1;  
    //Status bits update
    status = Ctrl_Reg_1 & 0b00000011;
    //Average samples number extraction from register 1
    cnt1val &= 0b00111100;
    cnt1val = (cnt1val >> 2);
    if(cnt1val!=0) samplenumber = cnt1val;  
        
    
    //Timer period update
    if(Ctrl_Reg_2 != 0 && Ctrl_Reg_2 != 1 && Ctrl_Reg_2 != 2) Timer_Count_WritePeriod(Ctrl_Reg_2);
    //Values 0 and 1 are not allowed (no modification of timer period)

    
}
/* [] END OF FILE */
