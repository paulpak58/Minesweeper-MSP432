/*
 * task_mouse.c
 *
 *  Created on: Nov 25, 2020
 *      Author: breit
 */

#include <main.h>

#define MOUSE_QUEUE_LEN  2

TaskHandle_t Task_Mouse_Handle;
QueueHandle_t Queue_Mouse;

// Global vars

int Speed = 25;
uint8_t x = 72;
uint8_t y = 59;
uint8_t prev_x = 72;
uint8_t prev_y = 59;

/******************************************************************************
 * This function will initialize Queue_Space_Ship and initialize the LCD
 ******************************************************************************/
void Task_Mouse_Init(void)
{
    Queue_Mouse = xQueueCreate(MOUSE_QUEUE_LEN,sizeof(MOUSE_MSG_t));
}


//Helper method to draw the mouse
void draw_mouse(uint8_t x, uint8_t y) {
    lcd_draw_rectangle(x,y,10,10,LCD_COLOR_GRAY);
    if(myBoard[x/13][y/13] == '1')
    {
        lcd_draw_image(x,y,ONE_WIDTH,ONE_HEIGHT,Bitmap_One,LCD_COLOR_WHITE,LCD_COLOR_GRAY);
    }
    else if(myBoard[x/13][y/13] == '2')
    {
        lcd_draw_image(x,y,TWO_WIDTH,TWO_HEIGHT,Bitmap_Two,LCD_COLOR_WHITE,LCD_COLOR_GRAY);
    }
    else if(myBoard[x/13][y/13] == '3')
    {
        lcd_draw_image(x,y,THREE_WIDTH,THREE_HEIGHT,Bitmap_Three,LCD_COLOR_WHITE,LCD_COLOR_GRAY);
    }
    else if(myBoard[x/13][y/13] == '4')
    {
        lcd_draw_image(x,y,FOUR_WIDTH,FOUR_HEIGHT,Bitmap_Four,LCD_COLOR_WHITE,LCD_COLOR_GRAY);
    }
    else if(myBoard[x/13][y/13] == '5')
    {
        lcd_draw_image(x,y,FIVE_WIDTH,FIVE_HEIGHT,Bitmap_Five,LCD_COLOR_WHITE,LCD_COLOR_GRAY);
    }
    else if(myBoard[x/13][y/13] == '6')
    {
        lcd_draw_image(x,y,SIX_WIDTH,SIX_HEIGHT,Bitmap_Six,LCD_COLOR_WHITE,LCD_COLOR_GRAY);
    }
    else if(myBoard[x/13][y/13] == '7')
    {
        lcd_draw_image(x,y,SEVEN_WIDTH,SEVEN_HEIGHT,Bitmap_Seven,LCD_COLOR_WHITE,LCD_COLOR_GRAY);
    }
    else if(myBoard[x/13][y/13] == '8')
    {
        lcd_draw_image(x,y,EIGHT_WIDTH,EIGHT_HEIGHT,Bitmap_Eight,LCD_COLOR_WHITE,LCD_COLOR_GRAY);
    }
    else if(myBoard[x/13][y/13] == 'f')
    {
        lcd_draw_image(x,y,FLAG_WIDTH,FLAG_HEIGHT,Bitmap_Flag,LCD_COLOR_WHITE,LCD_COLOR_GRAY);
    }
    else
    {
        lcd_draw_image(
                x,
                y,
                MOUSE_WIDTH,
                MOUSE_HEIGHT,
                Bitmap_Mouse,
                LCD_COLOR_BLACK,
                LCD_COLOR_GRAY
        );
    }
}

/******************************************************************************
 * This task manages the movement of the space ship. The joystick task or the
 * console task can send messages to SHIP_QUEUE_LEN
 ******************************************************************************/
void Task_Mouse(void *pvParameters)
{
    MOUSE_MSG_t msg;
    BOARD_MSG_t board_msg;
    board_msg.action = MOVE;
    BaseType_t status;

    while(1)
    {
        status = xQueueReceive(Queue_Mouse, &msg, portMAX_DELAY);

        if(msg.cmd == MOUSE_CMD_LEFT)
        {
            prev_x = x;
            if(x == 7) {
                x = 124;
            }
            else {
                x = x - 13;
            }

            draw_mouse(x,y);
            draw_user_image(prev_x, y, myBoard[prev_x/13][y/13]);
        }
        else if(msg.cmd == MOUSE_CMD_RIGHT)
        {
            prev_x = x;
            x = x + 13;
            if (x > 130) {
                x = 7;
            }
            draw_mouse(x,y);
            draw_user_image(prev_x, y, myBoard[prev_x/13][y/13]);
        }
        else if(msg.cmd == MOUSE_CMD_DOWN)
        {
            prev_y = y;
            y = y + 13;
            if (y > 130) {
                y = 7;
            }
            draw_mouse(x,y);
            draw_user_image(x, prev_y, myBoard[x/13][prev_y/13]);
        }
        else if(msg.cmd == MOUSE_CMD_UP)
        {
            prev_y = y;
            if(y == 7) {
                y = 124;
            }
            else {
                y = y - 13;
            }
            draw_mouse(x,y);
            draw_user_image(x, prev_y, myBoard[x/13][prev_y/13]);
        }
        else {
            draw_mouse(x,y);
        }
        //Delay for the correct amount of time depending on the Speed
        vTaskDelay(pdMS_TO_TICKS(Speed));
    }
}
