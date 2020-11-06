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
    
    fs_index = EEPROM_ReadByte(EEPROM_BYTE); //startup register reading
      
    uint16 fs_values[NUMB_OF_FREQ] = {1, 10, 25, 50, 100, 200}; //sampling frequency values
    uint16 fs_current = fs_values[fs_index]; //sampling frequency value at startup
    
    //Enable button interrupt
    CyGlobalIntEnable;
    ISR_Button_StartEx(Button_ISR);

    for(;;)
    {
    }
}

/* [] END OF FILE */