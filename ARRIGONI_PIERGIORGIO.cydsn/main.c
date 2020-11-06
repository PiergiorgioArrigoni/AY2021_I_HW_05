/**
*   \file main.c
*   \brief Main source file for Homework 5
*   \author Piergiorgio Arrigoni
*/

#include "InterruptRoutine.h"
#include "I2C_Interface.h"

#define LIS3DH_DEVICE_ADDRESS 0x18 //address of the slave device
#define LIS3DH_STATUS_REG 0x27 //address of the Status register
#define LIS3DH_CTRL_REG1 0x20 //address of the control register 1
#define LIS3DH_HIGHRES_MODE_CTRL_REG1 0x47 //value to set high-resolution mode to the accelerator
#define LIS3DH_CTRL_REG4 0x23 //address of the Control register 4

//Accelerometer output registers
#define LIS3DH_OUT_X_L 0x28
#define LIS3DH_OUT_X_H 0x29
#define LIS3DH_OUT_Y_L 0x2A
#define LIS3DH_OUT_Y_H 0x2B
#define LIS3DH_OUT_Z_L 0x2C
#define LIS3DH_OUT_Z_H 0x2D

int main(void)
{
    //Initialize components
    I2C_Master_Start();
    UART_Start();
    EEPROM_Start();
    
    fs_index = EEPROM_ReadByte(EEPROM_ADDRESS); //startup register reading
      
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