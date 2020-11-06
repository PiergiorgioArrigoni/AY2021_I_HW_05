/**
*   \file InterruptRoutine.h
*   \brief Source file for the ISR triggered by the button
*   \author Piergiorgio Arrigoni
*/

#include "InterruptRoutine.h"

CY_ISR(Button_ISR) //ISR triggered by the pressing of the button
{   
    eeprom_index++;
    
    if(eeprom_index == NUMB_OF_FREQ)
        eeprom_index = 0;
    
    EEPROM_UpdateTemperature();
    EEPROM_WriteByte(eeprom_index, EEPROM_BYTE);
}

/* [] END OF FILE */