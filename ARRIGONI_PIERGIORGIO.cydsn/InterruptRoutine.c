/**
*   \file InterruptRoutine.c
*   \brief Source file for the ISR triggered by the button
*   \author Piergiorgio Arrigoni
*/

#include "InterruptRoutine.h"

CY_ISR(Button_ISR)
{   
    fs_index++;
    
    if(fs_index == NUMB_OF_FREQ) //cycle must be started again
        fs_index = 0;
    
    flag_button = 1;
}

/* [] END OF FILE */