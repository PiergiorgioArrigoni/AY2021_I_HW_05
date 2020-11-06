/**
*   \file InterruptRoutine.h
*   \brief Header file for the ISR triggered by the button
*   \author Piergiorgio Arrigoni
*/

#ifndef _INTERRUPT_ROUTINE_H_
    #define _INTERRUPT_ROUTINE_H_
    
    #include "project.h"
    
    #define EEPROM_BYTE 0x00
    #define NUMB_OF_FREQ 6
    
    volatile uint8 eeprom_index;
    
    CY_ISR_PROTO(Button_ISR); //ISR triggered by button push (with debouncer component implemented)
#endif

/* [] END OF FILE */