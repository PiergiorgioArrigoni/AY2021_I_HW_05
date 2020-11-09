/**
*   \file InterruptRoutine.h
*   \brief Header file for the ISR triggered by the button
*   \author Piergiorgio Arrigoni
*/

#ifndef _INTERRUPT_ROUTINE_H_
    #define _INTERRUPT_ROUTINE_H_
    
    #include "project.h"
    
    volatile uint8 flag_button; //flag lifted by the button ISR
    
    CY_ISR_PROTO(Button_ISR); //ISR triggered by the pressing of the button
#endif

/* [] END OF FILE */