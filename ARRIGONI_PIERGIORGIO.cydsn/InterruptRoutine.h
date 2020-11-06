/**
*   \file InterruptRoutine.h
*   \brief Header file for the ISR triggered by the button
*   \author Piergiorgio Arrigoni
*/

#ifndef _INTERRUPT_ROUTINE_H_
    #define _INTERRUPT_ROUTINE_H_
    
    #include "project.h"
    
    #define EEPROM_BYTE 0x00
    
    CY_ISR_PROTO(Button_ISR);
#endif

/* [] END OF FILE */