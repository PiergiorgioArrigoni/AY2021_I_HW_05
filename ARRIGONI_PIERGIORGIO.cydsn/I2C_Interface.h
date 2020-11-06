/** 
 * \file I2C_Interface.h
 * \brief Header file for the I2C interface
 * \author Piergiorgio Arrigoni
*/

#ifndef _I2C_Interface_H_
    #define _I2C_Interface_H_
    
    #include "project.h"
    
    uint8 I2C_IsDeviceConnected(uint8_t device_address);
    uint8 I2C_ReadRegister(uint8 device_address, uint8 register_address, uint8* data);
    uint8 I2C_WriteRegister(uint8 device_address, uint8 register_address, uint8 data);
#endif

/* [] END OF FILE */