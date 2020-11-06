/**
*   \file main.c
*   \brief Main source file for Homework 5
*   \author Piergiorgio Arrigoni
*/

#include "InterruptRoutine.h"

int main(void)
{
    //Initialize components
    I2C_Master_Start();
    UART_Start();
    EEPROM_Start();
    
    eeprom_index = EEPROM_ReadByte(EEPROM_BYTE);
      
    uint16 eeprom_values[NUMB_OF_FREQ] = {1, 10, 25, 50, 100, 200};
    uint16 eeprom_freq = eeprom_values[eeprom_index];
    
    //Enable button interrupt
    CyGlobalIntEnable;
    ISR_Button_StartEx(Button_ISR);

    for(;;)
    {
    }
}

/* [] END OF FILE */