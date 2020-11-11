/**
*   \file main.c
*   \brief Main source file for Homework 5
*   \author Piergiorgio Arrigoni
*/

/* WARNING: The code may sometimes cycles indefinitely in reading the status register, because the wrong device address is read, for unknown reasons;
    in this case the user has to just PUSH the reset button on the PSoC device ONCE or disconnect and reconnect power supply if the problem persists */
/* NOTE: All "sprintf" and "UART_PutString" instances were used to debug the code with CoolTerm, 
    when interfacing with BCP they fill up the bandwidth and so they were put as comments */

#include "InterruptRoutine.h"
#include "I2C_Interface.h"
#include "stdio.h"

#define EEPROM_ADDRESS 0x00 //eeprom address on which the index of the startup sampling frequency is saved
#define NUMB_OF_FREQ 6 //number of sampling frequency values

//LIS3DH control/status registers
#define LIS3DH_DEVICE_ADDRESS 0x18 //address of the slave device
#define LIS3DH_CTRL_REG1 0x20 //address of the control register 1
#define LIS3DH_SET_CTRL_REG1 0x07 //low mode disabled, all three axes enabled (4 lsbs of control register 1)
#define LIS3DH_CTRL_REG4 0x23 //address of the Control register 4
#define LIS3DH_SET_CTRL_REG4 0x88 //bdu and high-resolution mode enabled, full scale ±2g 
#define LIS3DH_STATUS_REG 0x27 //address of the status register
#define LIS3DH_READY_STATUS_REG 0x08 //bit that goes to 1 when a new set of data is available

//Accelerometer output registers
#define LIS3DH_OUT_X_L 0x28 //with multi-register reading, this is the only define actually needed
#define LIS3DH_OUT_X_H 0x29
#define LIS3DH_OUT_Y_L 0x2A
#define LIS3DH_OUT_Y_H 0x2B
#define LIS3DH_OUT_Z_L 0x2C
#define LIS3DH_OUT_Z_H 0x2D

//Data rate configurations (4 msbs of control register 1)
#define FS_1 0x01
#define FS_10 0x02
#define FS_25 0x03
#define FS_50 0x04
#define FS_100 0x05
#define FS_200 0x06
#define FS_400 0x07 //unused

int main(void)
{
    //Initialize components
    I2C_Master_Start();
    UART_Start();
    EEPROM_Start();
    
    CyDelay(5); //LIS3DH boot procedure takes 5 ms to complete
    
    uint8 fs_index = EEPROM_ReadByte(EEPROM_ADDRESS); //startup register reading of the index of the current sampling frequency    
    if(fs_index >= NUMB_OF_FREQ) //if frequency index is out of range (e.g. it's the first time we use the device) we set it at the first value
        fs_index = 0;
    
    uint16 fs_values[NUMB_OF_FREQ] = {1, 10, 25, 50, 100, 200}; //sampling frequency values
    uint8 fs_config[NUMB_OF_FREQ] = {FS_1, FS_10, FS_25, FS_50, FS_100, FS_200}; //sampling frequency register configuration values
    
    //Enable button interrupt
    CyGlobalIntEnable;
    ISR_Button_StartEx(Button_ISR);
    
    char message[70]; //string to be sent to the UART
    uint8 error; //used to check errors in I2C communications
    uint8 i;

    //Checking which devices are present on the I2C bus (LIS3DH_DEVICE_ADDRESS should be present)
    for(i=0; i<128; i++)
    {
        if(I2C_IsDeviceConnected(i))
        {
            sprintf(message, "\n\nDevice 0x%02X is connected.\n", i);
            //UART_PutString(message); 
        } 
    }
    
    //CONTROL REGISTER 1 setting
    uint8 ctrl_reg1; 
    error = I2C_ReadRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_CTRL_REG1, &ctrl_reg1);
    
    if (!error)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X \n", ctrl_reg1);
        //UART_PutString(message); 
    }
    else
    {
        //UART_PutString("An error occurred during attempt to read control register 1.\n");   
    }
    
    if (ctrl_reg1 != (LIS3DH_SET_CTRL_REG1 | (fs_config[fs_index]<<4)))
    {
        ctrl_reg1 = (LIS3DH_SET_CTRL_REG1 | (fs_config[fs_index]<<4));
        error = I2C_WriteRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_CTRL_REG1, ctrl_reg1);
    
        if (!error)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X \n", ctrl_reg1);
            //UART_PutString(message); 
        }
        else
        {
            //UART_PutString("An error occurred during attempt to set control register 1.\n");   
        }
    }
    
    sprintf(message, "Frequency sampling value: %d\n", fs_values[fs_index]);
    //UART_PutString(message);
    
    //CONTROL REGISTER 4 setting
    uint8 ctrl_reg4; 
    error = I2C_ReadRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_CTRL_REG4, &ctrl_reg4);
    
    if (!error)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        //UART_PutString(message); 
    }
    else
    {
        //UART_PutString("An error occurred during attempt to read control register 4.\n");   
    }
    
    if (ctrl_reg4 != LIS3DH_SET_CTRL_REG4)
    {
        ctrl_reg4 = LIS3DH_SET_CTRL_REG4;
        error = I2C_WriteRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_CTRL_REG4, ctrl_reg4);
    
        if (!error)
        {
            sprintf(message, "CONTROL REGISTER 4 successfully written as: 0x%02X\n", ctrl_reg4);
            //UART_PutString(message); 
        }
        else
        {
            //UART_PutString("An error occurred during attempt to set control register 1.\n");   
        }
    }
    
    //Measurement variables definition
    uint8 AccelData[6]; //readings from accelerometer
    int16 value[3]; //raw data from accelerometer
    float acc[3]; //values of acceleration in m/s^2
    uint16 scale = 1000; //3 significant figures to be kept
    
    //Data packet to be sent to the UART
    uint8 DataBuffer[8];
    DataBuffer[0] = 0xA0; //header byte
    DataBuffer[7] = 0xC0; //tail byte
    
    uint8 new; //used to check if a set of new values is present in the registers
    flag_button = 0;
     
    for(;;)
    {
        if(flag_button)
        {
            flag_button = 0;
            fs_index++;
            if(fs_index == NUMB_OF_FREQ) //cycle must be started again
                fs_index = 0;
    
            EEPROM_UpdateTemperature(); //security measure because temperature might have changed of more than 10 degrees
            EEPROM_WriteByte(fs_index, EEPROM_ADDRESS); //startup register writing
            
            ctrl_reg1 = (LIS3DH_SET_CTRL_REG1 | (fs_config[fs_index]<<4));
            error = I2C_WriteRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_CTRL_REG1, ctrl_reg1);
            if (!error)
            {
                sprintf(message, "\n\nCONTROL REGISTER 1 successfully written as: 0x%02X \n", ctrl_reg1);
                //UART_PutString(message); 
            }
            else
            {
                //UART_PutString("An error occurred during attempt to set control register 1.\n");   
            }
            
            sprintf(message, "Frequency sampling value: %d\n", fs_values[fs_index]);
            //UART_PutString(message);
        }

        error = I2C_ReadRegister(LIS3DH_DEVICE_ADDRESS, LIS3DH_STATUS_REG, &new);
        if (!error)
        {
            if(new &= LIS3DH_READY_STATUS_REG) //checking if a set of new values is present in the accelerometer registers
            {           
                error = I2C_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS, LIS3DH_OUT_X_L, 6, AccelData);    
                if(!error)
                {  
                    //Fixing X output for transmission
                    value[0] = (int16) (AccelData[0] | (AccelData[1]<<8)) >> 4; //high resolution mode is 12 bit left-aligned
                    acc[0] = value[0] * (2*9.81)/2048.0; //full scale range is ±2g (g = 9.81 m/s^2) on 12 bits signed
                    sprintf(message, "\nX output in m/s^2 (with raw output): %d (%d)\n", (int16) acc[0], value[0]); //cannot display float in coolterm
                    //UART_PutString(message);
                    value[0] = (int16) (acc[0]*scale); //scaling in order to carry out transmission and save significant figures
                    DataBuffer[1] = (uint8) (value[0] & 0xFF);
                    DataBuffer[2] = (uint8) (value[0] >> 8);
                    
                    //Fixing Y output for transmission
                    value[1] = (int16) (AccelData[2] | (AccelData[3]<<8)) >> 4;
                    acc[1] = value[1] * (4*9.81)/4096.0;
                    sprintf(message, "Y output in m/s^2 (with raw output): %d (%d)\n", (int16) acc[1], value[1]);
                    //UART_PutString(message);
                    value[1] = (int16) (acc[1]*scale);
                    DataBuffer[3] = (uint8) (value[1] & 0xFF);
                    DataBuffer[4] = (uint8) (value[1] >> 8);
                    
                    //Fixing Z output for transmission
                    value[2] = (int16) (AccelData[4] | (AccelData[5]<<8)) >> 4;
                    acc[2] = value[2] * (4*9.81)/4096.0;
                    sprintf(message, "Z output in m/s^2 (with raw output): %d (%d)\n", (int16) acc[2], value[2]);
                    //UART_PutString(message);
                    value[2] = (int16) (acc[2]*scale);
                    DataBuffer[5] = (uint8) (value[2] & 0xFF);
                    DataBuffer[6] = (uint8) (value[2] >> 8);             
                    
                    UART_PutArray(DataBuffer, 8); //transmitting packet to bridge control panel
                }
                else
                {
                    //UART_PutString("One or more errors occurred during attempt to read accelerometer output registers.\n");   
                }
            }
        }
        else
        {
            //UART_PutString("An error occurred during attempt to read status register.\n");   
        } 
    }
}

/* [] END OF FILE */