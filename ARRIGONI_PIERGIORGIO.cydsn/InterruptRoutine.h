/**
*   \file InterruptRoutine.h
*   \brief Header file for the ISR triggered by the button
*   \author Piergiorgio Arrigoni
*/

#ifndef _INTERRUPT_ROUTINE_H_
    #define _INTERRUPT_ROUTINE_H_
    
    #include "project.h"
    
    #define EEPROM_BYTE 0x00 //eeprom byte on which the index of the startup sampling frequency is saved
    #define NUMB_OF_FREQ 6 //number of sampling frequency values
    
    volatile uint8 fs_index; //index of the current sampling frequency
    
    CY_ISR_PROTO(Button_ISR); //ISR triggered by button push (with debouncer component implemented)
#endif

/* [] END OF FILE */