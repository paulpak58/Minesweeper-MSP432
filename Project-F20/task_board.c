/*
 * task_board.c
 *
 *  Created on: Dec 2, 2020
 *      Author: paul
 */

#include "task_board.h"

// private function headers
void display_background();
void generate_mines();
bool cellValid(int row, int col);
int countAdjacentMines(int row, int col);


#define BOARD_QUEUE_LEN 2
#define NUM_MINES 10
#define MAX_DIMENSION 10

TaskHandle_t Task_Board_Handle;
QueueHandle_t Queue_Board;

char realBoard[MAX_DIMENSION][MAX_DIMENSION];        // board containing the mines
int mines[NUM_MINES][2];                             // array of mines
char myBoard[MAX_DIMENSION][MAX_DIMENSION];          // current player board

void Task_Board_Init(void)
{
    Queue_Board = xQueueCreate(BOARD_QUEUE_LEN, sizeof(BOARD_MSG_t));

    // Initialize the LCD
    HAL_LCD_PortInit();
    Crystalfontz128x128_Init();
    display_background();
    // Initialize global vars
    int i,j;
    for(i = 0; i < MAX_DIMENSION; ++i)
    {
        for(j = 0; j < MAX_DIMENSION; ++j)
        {
            realBoard[i][j] = '\0';
            myBoard[i][j] = '\0';
        }
    }
}


void Task_Board(void *pvParameters)
{
    BaseType_t status;
    BOARD_MSG_t msg;

    //display_background();
    lcd_draw_image(SCREEN_CENTER_COL + 7, SCREEN_CENTER_ROW - 6,MOUSE_WIDTH,MOUSE_HEIGHT,Bitmap_Mouse,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
    generate_mines();

    while(1)
    {
        status = xQueueReceive(Queue_Board, &msg, portMAX_DELAY);
        if(msg.action == UNCOVER)
        {
            display_background();           // Replace w/ UNCOVER ACTION
        }
        else if (msg.action == FLAG)
        {
            display_background();           // Replace w/ FLAG ACTION
        }
        else {
//            display_background();           //This is for moving the mouse
//            MOUSE_MSG_t msg;
//            msg.cmd = MOUSE_CMD_CENTER;
//            msg.value = 1;
//            xQueueSendToBack(Queue_Mouse, &msg, portMAX_DELAY);

        }
    }
}

void display_background()
{
    // Displays gray background
    lcd_draw_rectangle(SCREEN_CENTER_COL,SCREEN_CENTER_ROW,SCREEN_X,SCREEN_Y,LCD_COLOR_GRAY);

//    lcd_draw_image(SCREEN_CENTER_COL + 7,SCREEN_CENTER_ROW + 7,BOMB_WIDTH,BOMB_HEIGHT,Bitmap_Bomb,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
//
//    lcd_draw_image(SCREEN_CENTER_COL - 6,SCREEN_CENTER_ROW + 7,FLAG_WIDTH,FLAG_HEIGHT,Bitmap_Flag,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
//
//    lcd_draw_image(SCREEN_CENTER_COL - 44,SCREEN_CENTER_ROW - 6,ONE_WIDTH,ONE_HEIGHT,Bitmap_One,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
//
//    lcd_draw_image(SCREEN_CENTER_COL - 32,SCREEN_CENTER_ROW - 6,TWO_WIDTH,TWO_HEIGHT,Bitmap_Two,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
//
//    lcd_draw_image(SCREEN_CENTER_COL - 19,SCREEN_CENTER_ROW - 6,THREE_WIDTH,THREE_HEIGHT,Bitmap_Three,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
//
//    lcd_draw_image(SCREEN_CENTER_COL - 6, SCREEN_CENTER_ROW - 6,FOUR_WIDTH,FOUR_HEIGHT,Bitmap_Four,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
//
//    lcd_draw_image(SCREEN_CENTER_COL + 7, SCREEN_CENTER_ROW - 6,FIVE_WIDTH,FIVE_HEIGHT,Bitmap_Five,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
//
//    lcd_draw_image(SCREEN_CENTER_COL + 20, SCREEN_CENTER_ROW - 6,SIX_WIDTH,SIX_HEIGHT,Bitmap_Six,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
//
//    lcd_draw_image(SCREEN_CENTER_COL + 33, SCREEN_CENTER_ROW - 6,SEVEN_WIDTH,SEVEN_HEIGHT,Bitmap_Seven,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
//
//    lcd_draw_image(SCREEN_CENTER_COL + 46, SCREEN_CENTER_ROW - 6,EIGHT_WIDTH,EIGHT_HEIGHT,Bitmap_Eight,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
//
//    lcd_draw_image(SCREEN_CENTER_COL + 20, SCREEN_CENTER_ROW + 33,MOUSE_WIDTH,MOUSE_HEIGHT,Bitmap_Mouse,LCD_COLOR_BLACK,LCD_COLOR_GRAY);



    // Horizontal lines
    int i = 0;
    for(i = 0; i <72; i+=13)
    {
        lcd_draw_rectangle(SCREEN_CENTER_COL,SCREEN_CENTER_ROW+i,LINE_LENGTH,LINE_WIDTH,LCD_COLOR_BLACK);
        lcd_draw_rectangle(SCREEN_CENTER_COL,SCREEN_CENTER_ROW-i,LINE_LENGTH,LINE_WIDTH,LCD_COLOR_BLACK);
    }
    // Vertical Lines
    i = 0;
    for(i = 0; i <72; i+=13)
    {
        lcd_draw_rectangle(SCREEN_CENTER_COL+i,SCREEN_CENTER_ROW,LINE_WIDTH,LINE_LENGTH,LCD_COLOR_BLACK);
        lcd_draw_rectangle(SCREEN_CENTER_COL-i,SCREEN_CENTER_ROW,LINE_WIDTH,LINE_LENGTH,LCD_COLOR_BLACK);
    }
}

void generate_mines()
{
    bool mark[MAX_DIMENSION * MAX_DIMENSION];
    memset(mark,false,sizeof(mark));

    int i;
    for(i = 0; i < NUM_MINES;)
    {
       int random = rand() % (MAX_DIMENSION * MAX_DIMENSION);
       int x = random / MAX_DIMENSION;
       int y = random % MAX_DIMENSION;

       // Add mine if no mine is placed on the board
       if(mark[random] == false)
       {
           mines[i][0] = x;     // row index of mine
           mines[i][1] = y;     // col index of mine

           // Place the mine
           realBoard[mines[i][0]][mines[i][1]] = '*';
           mark[random] = true;
           i++;
       }
    }
}

bool cellValid(int row, int col)
{
    return (row >= 0)  && (row < MAX_DIMENSION) && (col >= 0) && (col < MAX_DIMENSION);
}

int countAdjacentMines(int row, int col)
{
    int count = 0;          // # of adjacent mines

    // From a current cell, there are 8 surrounding cells to check
    if(cellValid(row-1,col) == true)        // NORTH
    {
        if(realBoard[row-1][col] == '*')
        {
            count++;
        }
    }
    if(cellValid(row+1,col) == true)        // SOUTH
    {
        if(realBoard[row+1][col] == '*')
        {
            count++;
        }
    }
    if(cellValid(row-1,col-1) == true)      // NORTHWEST
    {
        if(realBoard[row-1][col-1] == '*')
        {
            count++;
        }
    }
    if(cellValid(row,col-1) == true)        // WEST
    {
        if(realBoard[row][col-1] == '*')
        {
            count++;
        }
    }
    if(cellValid(row+1,col-1) == true)      // SOUTHWEST
    {
        if(realBoard[row+1][col-1] == '*')
        {
            count++;
        }
    }
    if(cellValid(row-1,col+1) == true)      // NORTHEAST
    {
        if(realBoard[row-1][col+1] == '*')
        {
            count++;
        }
    }
    if(cellValid(row,col+1) == true)        //EAST
    {
        if(realBoard[row][col+1] == '*')
        {
            count++;
        }
    }
    if(cellValid(row+1,col+1) == true)      // SOUTHEAST
    {
        if(realBoard[row+1][col+1] == '*')
        {
            count++;
        }
    }
    return count;
}
