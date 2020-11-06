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
    
    //Read EEPROM value
    uint8 eeprom_value = EEPROM_ReadByte(EEPROM_BYTE);    
    
    //Enable button interrupt
    CyGlobalIntEnable;
    ISR_Button_StartEx(Button_ISR);

    for(;;)
    {
    }
}

/* [] END OF FILE */