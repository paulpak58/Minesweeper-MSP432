/*
 * task_mouse.h
 *
 *  Created on: Nov 25, 2020
 *      Author: breit
 */

#ifndef TASK_MOUSE_H_
#define TASK_MOUSE_H_

#include <main.h>

typedef enum {
    MOUSE_CMD_LEFT,
    MOUSE_CMD_RIGHT,
    MOUSE_CMD_UP,
    MOUSE_CMD_DOWN,
    MOUSE_CMD_CENTER,
    MOUSE_CMD_SPEED
} MOUSE_CMD_t;
typedef struct {
    MOUSE_CMD_t cmd;
    uint8_t  value;
}MOUSE_MSG_t;


extern TaskHandle_t Task_Mouse_Handle;
extern QueueHandle_t Queue_Mouse;
//Value used to set how many ms between updates
extern int Speed;
extern uint8_t x;
extern uint8_t y;
extern uint8_t prev_x;
extern uint8_t prev_y;

/******************************************************************************
 * This function will initialize Queue_Space_Ship
 ******************************************************************************/
void Task_Mouse_Init(void);

/******************************************************************************
 * This task manages the movement of the space ship. The joystick task or the
 * console task can send messages to the
 ******************************************************************************/
void Task_Mouse(void *pvParameters);

//Helper method to draw the spaceship
void draw_mouse(uint8_t x, uint8_t y);



#endif /* TASK_MOUSE_H_ */
