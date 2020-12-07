/*
 * task_mkII_s2.c
 *
 *  Created on: Oct 14, 2020
 *      Author: Joe Krachey
 */

#include "task_mkII_s2.h"

TaskHandle_t Task_mkII_s2_Handle = NULL;

void init_mkII_s2()
{
    P3->DIR &= ~BIT5;
    P3->REN |= BIT5;
    P3->OUT |= BIT5;
}

/******************************************************************************
 * De-bounce switch S2.
 *****************************************************************************/
bool debounce_s2(void)
{
    static uint8_t debounce_state = 0x00;

    // Shift the de-bounce variable to the right
    debounce_state = debounce_state << 1;

    // If S2 is being pressed, set the LSBit of debounce_state to a 1;
    if((P3->IN & BIT5) == 0)
    {
        debounce_state |= 0x01;
    }

    // If the de-bounce variable is equal to 0x7F, change the color of the tri-color LED.
    if(debounce_state == 0x7F)
    {
        return true;
    }
    else
    {
        return false;
    }

}


/******************************************************************************
 * De-bounce switch S2.  If is has been pressed, change the tri-Color LED on
 * the MSP432 Launchpad. Everytime S2 is pressed, the color should change in
 * the following order:
 *      RED->GREEN->BLUE->RED->GREEN....
 *****************************************************************************/
void task_mkII_s2(void *pvParameters)
{
    BaseType_t status;
    BOARD_MSG_t msg;

    while(1)
    {
        if(debounce_s2())
        {
            msg.action = FLAG;
            status = xQueueSendToBack(Queue_Board, &msg, portMAX_DELAY);
        }
        // Delay for 10mS using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
