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
bool cellValid(int row, int col);
void positiveExplosion(int row, int col);

TaskHandle_t Task_Board_Handle;
QueueHandle_t Queue_Board;

char realBoard[MAX_DIMENSION][MAX_DIMENSION];        // board containing the mines
int mines[NUM_MINES][2];                             // array of mines
char myBoard[MAX_DIMENSION][MAX_DIMENSION];          // current player board
int numFlags = 0;                                    // capped by the # of mines

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

    srand(time(NULL));
    generate_mines();
    display_background();
    draw_mouse(x, y);

    while(1)
    {
        status = xQueueReceive(Queue_Board, &msg, portMAX_DELAY);

        if(msg.action == UNCOVER)
        {
            if(myBoard[x/13][y/13] == '\0')
            {
                int adjacentMines = countAdjacentMines(x/13, y/13);
                if(adjacentMines == 0)
                {
                    positiveExplosion(x/13,y/13);
                }
                myBoard[x/13][y/13] = adjacentMines + 48;
                draw_user_image(x,y,myBoard[x/13][y/13]);
                draw_mouse(x,y);
            }
            if(realBoard[x/13][y/13] == '*')        // LOSS
            {
                lcd_draw_rectangle(SCREEN_CENTER_COL,SCREEN_CENTER_ROW,SCREEN_X,SCREEN_Y,LCD_COLOR_RED);
            }
            bool win = true;
            int a,b;
            for(a = 0; a < MAX_DIMENSION; ++a)
            {
                for(b = 0; b < MAX_DIMENSION; ++b)
                {
                    if(myBoard[a][b] == '\0' && realBoard[a][b] != '*')
                    {
                        win = false;
                    }
                }
            }
            if(win)                             // WIN
            {
                lcd_draw_rectangle(SCREEN_CENTER_COL,SCREEN_CENTER_ROW,SCREEN_X,SCREEN_Y,LCD_COLOR_BLUE);
            }
        }
        else if (msg.action == FLAG)
        {
            if(myBoard[x/13][y/13] == 'f')
            {
                myBoard[x/13][y/13] = '\0';
                numFlags--;
                draw_user_image(x,y,'\0');
                draw_mouse(x,y);
            }
            else if(myBoard[x/13][y/13] == '\0' && numFlags < NUM_MINES)
            {
                myBoard[x/13][y/13] = 'f';
                numFlags++;
                draw_user_image(x,y,myBoard[x/13][y/13]);
                draw_mouse(x,y);
            }
        }else {
//            display_background();           //This is for moving the mouse
//            MOUSE_MSG_t msg;
//            msg.cmd = MOUSE_CMD_CENTER;
//            msg.value = 1;
//            xQueueSendToBack(Queue_Mouse, &msg, portMAX_DELAY);

        }
    }
}

void draw_user_image(int x, int y, char input)
{
    lcd_draw_rectangle(x,y,10,10,LCD_COLOR_GRAY);
    if(input == '1')
    {
        lcd_draw_image(x,y,ONE_WIDTH,ONE_HEIGHT,Bitmap_One,LCD_COLOR_BLUE,LCD_COLOR_GRAY);
    }
    else if(input == '2')
    {
        lcd_draw_image(x,y,TWO_WIDTH,TWO_HEIGHT,Bitmap_Two,LCD_COLOR_GREEN,LCD_COLOR_GRAY);
    }
    else if(input == '3')
    {
        lcd_draw_image(x,y,THREE_WIDTH,THREE_HEIGHT,Bitmap_Three,LCD_COLOR_RED,LCD_COLOR_GRAY);
    }
    else if(input == '4')
    {
        lcd_draw_image(x,y,FOUR_WIDTH,FOUR_HEIGHT,Bitmap_Four,LCD_COLOR_MAGENTA,LCD_COLOR_GRAY);
    }
    else if(input == '5')
    {
        lcd_draw_image(x,y,FIVE_WIDTH,FIVE_HEIGHT,Bitmap_Five,LCD_COLOR_BROWN,LCD_COLOR_GRAY);
    }
    else if(input == '6')
    {
        lcd_draw_image(x,y,SIX_WIDTH,SIX_HEIGHT,Bitmap_Six,LCD_COLOR_CYAN,LCD_COLOR_GRAY);
    }
    else if(input == '7')
    {
        lcd_draw_image(x,y,SEVEN_WIDTH,SEVEN_HEIGHT,Bitmap_Seven,LCD_COLOR_ORANGE,LCD_COLOR_GRAY);
    }
    else if(input == '8')
    {
        lcd_draw_image(x,y,EIGHT_WIDTH,EIGHT_HEIGHT,Bitmap_Eight,LCD_COLOR_YELLOW,LCD_COLOR_GRAY);
    }
    else if(input == 'f')
    {
        lcd_draw_image(x,y,FLAG_WIDTH,FLAG_HEIGHT,Bitmap_Flag,LCD_COLOR_RED,LCD_COLOR_GRAY);
    }
}

void display_background()
{
    // Displays gray background
    lcd_draw_rectangle(SCREEN_CENTER_COL,SCREEN_CENTER_ROW,SCREEN_X,SCREEN_Y,LCD_COLOR_GRAY);

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

    // Draw out Bombs
    int count;
    float x,y;

    for(count = 0; count < NUM_MINES; ++count)
    {
        x = (mines[count][0] * 2 + 1) * 6.5;
        y = (mines[count][1] * 2 + 1) * 6.5;
        if(mines[count][0] == 0)
        {
            x = 6.5;
        }
        if(mines[count][1] == 0)
        {
            y = 6.5;
        }
        lcd_draw_image((int)x,(int)y,BOMB_WIDTH,BOMB_HEIGHT,Bitmap_Bomb,LCD_COLOR_BLACK,LCD_COLOR_GRAY);
    }
}

void positiveExplosion(int row, int col)
{
    int adjacentMines;
    int x;
    int y;
    realBoard[row][col] = '0';
    myBoard[row][col] = '0';
    if(cellValid(row-1,col) == true)        // NORTH
    {
        adjacentMines = countAdjacentMines(row-1, col);
       if(adjacentMines == 0 && realBoard[row-1][col] != '0')
        {
            positiveExplosion(row-1,col);
        }
        myBoard[row-1][col] = adjacentMines + 48;
        x = ((row-1) * 2 + 1) * 6.5;
        y = (col * 2 + 1) * 6.5;
        draw_user_image(x,y,myBoard[row-1][col]);
    }
    if(cellValid(row+1,col) == true)        // SOUTH
    {
        adjacentMines = countAdjacentMines(row+1, col);
        if(adjacentMines == 0  && realBoard[row+1][col] != '0')
        {
            positiveExplosion(row+1,col);
        }
        myBoard[row+1][col] = adjacentMines + 48;
        x = ((row+1) * 2 + 1) * 6.5;
        y = (col * 2 + 1) * 6.5;
        draw_user_image(x,y, myBoard[row+1][col]);
    }
    if(cellValid(row-1,col-1) == true)      // NORTHWEST
    {
        adjacentMines = countAdjacentMines(row-1, col-1);
        if(adjacentMines == 0  && realBoard[row-1][col-1] != '0')
        {
            positiveExplosion(row-1,col-1);
        }
        myBoard[row-1][col-1] = adjacentMines + 48;
        x = ((row-1) * 2 + 1) * 6.5;
        y = ((col-1) * 2 + 1) * 6.5;
        draw_user_image(x,y, myBoard[row-1][col-1]);
    }
    if(cellValid(row,col-1) == true)        // WEST
    {
        adjacentMines = countAdjacentMines(row, col-1);
        if(adjacentMines == 0  && realBoard[row][col-1] != '0')
        {
            positiveExplosion(row,col-1);
        }
        myBoard[row][col-1] = adjacentMines + 48;
        x = (row * 2 + 1) * 6.5;
        y = ((col-1) * 2 + 1) * 6.5;
        draw_user_image(x,y, myBoard[row][col-1]);
    }
    if(cellValid(row+1,col-1) == true)      // SOUTHWEST
    {
        adjacentMines = countAdjacentMines(row+1, col-1);
        if(adjacentMines == 0  && realBoard[row+1][col-1] != '0')
        {
            positiveExplosion(row+1,col-1);
        }
        myBoard[row+1][col-1] = adjacentMines + 48;
        x = ((row+1) * 2 + 1) * 6.5;
        y = ((col-1) * 2 + 1) * 6.5;
        draw_user_image(x, y, myBoard[row+1][col-1]);
    }
    if(cellValid(row-1,col+1) == true)      // NORTHEAST
    {
        adjacentMines = countAdjacentMines(row-1, col+1);
        if(adjacentMines == 0  && realBoard[row-1][col+1] != '0')
        {
            positiveExplosion(row-1,col+1);
        }
        myBoard[row-1][col+1] = adjacentMines + 48;
        x = ((row-1) * 2 + 1) * 6.5;
        y = ((col+1) * 2 + 1) * 6.5;
        draw_user_image(x, y, myBoard[row-1][col+1]);
    }
    if(cellValid(row,col+1) == true)        //EAST
    {
        adjacentMines = countAdjacentMines(row, col+1);
        if(adjacentMines == 0 && realBoard[row][col+1] != '0')
        {
            positiveExplosion(row,col+1);
        }
        myBoard[row][col+1] = adjacentMines + 48;
        x = (row * 2 + 1) * 6.5;
        y = ((col+1) * 2 + 1) * 6.5;
        draw_user_image(x, y, myBoard[row][col+1]);
    }
    if(cellValid(row+1,col+1) == true)      // SOUTHEAST
    {
        adjacentMines = countAdjacentMines(row+1, col+1);
        if(adjacentMines == 0 && realBoard[row+1][col+1] != '0')
        {
            positiveExplosion(row+1,col+1);
        }
        myBoard[row+1][col+1] = adjacentMines + 48;
        x = ((row+1) * 2 + 1) * 6.5;
        y = ((col+1) * 2 + 1) * 6.5;
        draw_user_image(x, y, myBoard[row+1][col+1]);
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
