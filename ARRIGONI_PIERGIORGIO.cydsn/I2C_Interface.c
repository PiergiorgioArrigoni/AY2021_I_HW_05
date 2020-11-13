/**
*   \file I2C_Interface.c
*   \brief Source file for the I2C interface
*   \author Piergiorgio Arrigoni
*   \credits Davide Marzorati
*/

#include "I2C_Interface.h"

uint8 I2C_IsDeviceConnected(uint8 device_address)
{
    //send a start condition followed by a stop condition
    uint8 connection = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE);
    I2C_Master_MasterSendStop();
    
    if (connection == I2C_Master_MSTR_NO_ERROR) //return 1 if the device is connected, 0 otherwise
        return 1; 
    else
        return 0;
}

uint8 I2C_ReadRegister(uint8 device_address, uint8 register_address, uint8* data)
{
    uint8 error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE); //send start condition
    if (error == I2C_Master_MSTR_NO_ERROR)
    { 
        error = I2C_Master_MasterWriteByte(register_address); // write address of register to be read
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            error = I2C_Master_MasterSendRestart(device_address, I2C_Master_READ_XFER_MODE); //send restart condition
            if (error == I2C_Master_MSTR_NO_ERROR)
            {
                *data = I2C_Master_MasterReadByte(I2C_Master_NAK_DATA); // read data without acknowledgement
            }
        }
    }
    I2C_Master_MasterSendStop(); //send stop condition
    
    return error ? 1:0; //return 1 if there is an error, 0 otherwise
}

uint8 I2C_WriteRegister(uint8 device_address, uint8 register_address, uint8 data)
{
    uint8 error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE); //send start condition
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        error = I2C_Master_MasterWriteByte(register_address); //write register address
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            error = I2C_Master_MasterWriteByte(data); //write byte of interest
        }
    }
    I2C_Master_MasterSendStop(); //send stop condition
    
    return error ? 1:0; //return 1 if there is an error, 0 otherwise
}

uint8 I2C_ReadRegisterMulti(uint8 device_address, uint8 register_address, uint8 register_count, uint8* data)
{
    uint8 error = I2C_Master_MasterSendStart(device_address,I2C_Master_WRITE_XFER_MODE);//send start condition
    if (error == I2C_Master_MSTR_NO_ERROR)
    {
        register_address |= 0x80; //datasheet indication for multi read (autoincrement)
        error = I2C_Master_MasterWriteByte(register_address); //write address of register to be read with the MSb equal to 1
        if (error == I2C_Master_MSTR_NO_ERROR)
        {
            error = I2C_Master_MasterSendRestart(device_address, I2C_Master_READ_XFER_MODE); //send restart condition
            if (error == I2C_Master_MSTR_NO_ERROR)
            {
                uint8 counter = register_count; 
                while(counter>1) //continue reading until we have register to read
                {
                    data[register_count-counter] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
                    counter--;
                }
                data[register_count-1] = I2C_Master_MasterReadByte(I2C_Master_NAK_DATA); //read last data without acknowledgement
            }
        }
    }
    I2C_Master_MasterSendStop(); // Send stop condition
    
    return error ? 1:0; // Return error code
}

/* [] END OF FILE */