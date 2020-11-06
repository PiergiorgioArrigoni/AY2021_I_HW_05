/**
*   \file InterruptRoutine.c
*   \brief Source file for the ISR triggered by the button
*   \author Piergiorgio Arrigoni
*/

#include "InterruptRoutine.h"

CY_ISR(Button_ISR)
{   
    fs_index++;
    
    if(fs_index == NUMB_OF_FREQ) //cycle must be started again
        fs_index = 0;
    
    EEPROM_UpdateTemperature(); //security measure if temperature has changed of few degrees
    EEPROM_WriteByte(fs_index, EEPROM_ADDRESS); //startup register writing
}

/* [] END OF FILE */