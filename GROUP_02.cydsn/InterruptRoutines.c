/**
* \file InterruptRoutines.c
*
* \brief Source file for interrupt routines
*
* This source file contains the function definitions
* for the interrupt services routines.
*
* \authors Stefano Lucchesini, Giovanni Marelli
* \date April 2020
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
extern volatile uint8_t status;
extern volatile uint8_t Ctrl_Reg_1;
extern volatile uint8_t Ctrl_Reg_2;

/*
*   This function is called when Timer_Count overflows. Here we
*   read ADC values from the 2 sensors.
*/
CY_ISR(Custom_Timer_Count_ISR)
{
    // Read timer status register to pull interrupt line low
    Timer_Count_ReadStatusRegister();
    
    //Read ADC values
    //Read Channel 0
    AMux_Select(MUX_TEMP);
    Temp_array[count] = ADC_DelSig_Read32();
    //Read Channel 1
    AMux_Select(MUX_LIGHT);
    Light_array[count] = ADC_DelSig_Read32();
    
    count++;
    //Avoid to write outside array size
    count = (count > ARRAY_LENGTH) ? 0 : count;
}

/*
*   This function is called when exiting the EZI2C_ISR. Here we
*   perform all the tasks based on the requests.
*/
void EZI2C_ISR_ExitCallback(void)
{
    //Control Registers 1 and 2 read
    Ctrl_Reg_1 = slaveBuffer[0];
    Ctrl_Reg_2 = slaveBuffer[1];
    uint8_t cnt1val = Ctrl_Reg_1;  
    //Status bits update
    status = Ctrl_Reg_1 & 0b00000011;
    //Average samples number extraction from register 1
    cnt1val &= 0b00111100;
    cnt1val = (cnt1val >> 2);
    if(cnt1val!=0) samplenumber = cnt1val; //Avoid number of samples 0 (value unchanged)
    
    //Timer period update
    if(Ctrl_Reg_2 != 0 && Ctrl_Reg_2 != 1 && Ctrl_Reg_2 != 2) Timer_Count_WritePeriod(Ctrl_Reg_2);
    //Values 0, 1 and 2 are not allowed (Timer Period is unchanged)
}

/* [] END OF FILE */
