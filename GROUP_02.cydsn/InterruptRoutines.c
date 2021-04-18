/**
* This file contains all the source code required
* for the interrupt service routines.
*/

// Include required header files
#include "InterruptRoutines.h" 
#include "Timer_Count.h"
#include "project.h"

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

CY_ISR(Custom_Timer_Count_ISR)
{
    // Read timer status register to pull interrupt line low
    Timer_Count_ReadStatusRegister();
    AMux_Select(MUX_LIGHT);
    Light_array[count] = ADC_DelSig_Read32();
    
    AMux_Select(MUX_TEMP);
    Temp_array[count] = ADC_DelSig_Read32();
    
    count++;
    count = (count > 14) ? 0 : count;

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
    //samplenumber = cnt1val;  
        
    
    //Timer period update
    //if(Ctrl_Reg_2 == 0 || Ctrl_Reg_2 == 1) Timer_Count_WritePeriod(MIN_ALLOWED_PERIOD);
    //else Timer_Count_WritePeriod(Ctrl_Reg_2);
  
    
}
/* [] END OF FILE */
