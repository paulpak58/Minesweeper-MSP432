/*
 * task_board.h
 *
 *  Created on: Dec 2, 2020
 *      Author: paul
 */

#ifndef TASK_BOARD_H_
#define TASK_BOARD_H_

#define SCREEN_X            132
#define SCREEN_Y            132

#define SCREEN_CENTER_COL    ((SCREEN_X/2)-1)
#define SCREEN_CENTER_ROW    ((SCREEN_Y/2)-1)

#define LINE_WIDTH          1
#define LINE_LENGTH         (SCREEN_X - 1)

#define SQUARE_SIZE         32

#define PADDING             2

#define LEFT_COL                      (SCREEN_CENTER_COL - SQUARE_SIZE - (2*PADDING) - LINE_WIDTH)
#define CENTER_COL                    (SCREEN_CENTER_COL)
#define RIGHT_COL                     (SCREEN_CENTER_COL + SQUARE_SIZE + (2*PADDING) + LINE_WIDTH)

#define UPPER_ROW                     (SCREEN_CENTER_ROW - SQUARE_SIZE - (2*PADDING) - LINE_WIDTH)
#define CENTER_ROW                    (SCREEN_CENTER_ROW)
#define LOWER_ROW                     (SCREEN_CENTER_ROW + SQUARE_SIZE + (2*PADDING) + LINE_WIDTH)

#define UPPER_HORIZONTAL_LINE_Y     (SCREEN_CENTER_ROW - (SQUARE_SIZE/2) - PADDING - LINE_WIDTH/2)
#define LOWER_HORIZONTAL_LINE_Y     (SCREEN_CENTER_ROW + (SQUARE_SIZE/2) + PADDING + LINE_WIDTH/2)

#define LEFT_HORIZONTAL_LINE_X      (SCREEN_CENTER_COL - (SQUARE_SIZE/2) - PADDING - LINE_WIDTH/2)
#define RIGHT_HORIZONTAL_LINE_X     (SCREEN_CENTER_COL + (SQUARE_SIZE/2) + PADDING + LINE_WIDTH/2)

#include <stdbool.h>
#include "main.h"
#include "minesweeper_images.h"

typedef enum {UNCOVER,FLAG,MOVE} BOARD_ACTION;

typedef struct{
    BOARD_ACTION action;
}BOARD_MSG_t;

extern TaskHandle_t Task_Board_Handle;
extern QueueHandle_t Queue_Board;

void Task_Board_Init(void);

void Task_Board(void *pvParameters);

#endif /* TASK_BOARD_H_ */
