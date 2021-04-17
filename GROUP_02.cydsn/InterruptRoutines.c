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

/**
*   This function is called when exiting the EZI2C_ISR. Here we
*   perform all the tasks based on the requests.
*/
void EZI2C_ISR_ExitCallback(void)
{
    
}
/* [] END OF FILE */
