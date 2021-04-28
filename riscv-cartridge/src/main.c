#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*   enum define   */
enum funcCode{
    SYSTIMER = 1,
    TextMode = 2,
    VideoMode = 3,
    TMSET = 4,
    CTL_STATUS = 5,
    MODE_STATUS = 6,
    TEXT_PRINT = 7,
    MODE = 8,
    COLOR_CHANGE = 9,
    CLEAN_DOT = 10,
};

enum videoType{
    BACKGROUND = 1,
    LARGESPRITE = 2,
    SMALLSPRITE = 3,
};

enum color{
    COLOR,
    RED = 0xFF0000,
    MEDIUMSLATEBLUE = 0x7B68EE,
    LIME = 0x00FF00,
    LIGHTSKYBLUE = 0x87CEFA,
    CORAL = 0xFF7F50,
    GOLD = 0xFFD700,
    BLACK = 0x000000,
};

uint32_t Timer(uint32_t func);
int random_number(int min_num, int max_num);
void TextPrint(int line, char str[]);

/*   syscall function define   */
void TimerInterval(uint32_t func, uint32_t interval);
void ModeSelect(uint32_t func, uint32_t Type);
void VideoCtrBG(uint32_t func, uint32_t Type, char Chr, uint32_t x, uint32_t y, uint32_t z, uint32_t w, uint32_t h, uint32_t palette, uint64_t pColor);
void VideoCtrLS(uint32_t func, uint32_t Type, char Chr, uint32_t x, uint32_t y, uint32_t z,uint32_t w, uint32_t h, uint32_t palette, uint64_t pColor);
void VideoCtrSS(uint32_t func, uint32_t Type, char Chr, uint32_t x, uint32_t y, uint32_t z, uint32_t w, uint32_t h, uint32_t palette, uint64_t pColor);

int Ctr_Status(uint32_t func);
char Mode_Status(uint32_t func);
void Text_Convert(uint32_t func, uint32_t Posi, char Chr);
void Color_Random(uint32_t func, uint32_t palette);
void CleanDot(uint32_t func);

/*   main   */
int main() {
    int XPos = 12;
    int x_pos = 0;
    int y_pos = 0;
    int x_pos_R = 0;
    int y_pos_R = 0;
    uint32_t count = 0;
    uint32_t count_BG = 0;
    int32_t i = 0;
    uint32_t ControllerStatus;
    uint32_t CurrentTicks;
    uint32_t LastGlobalTicks = 0;
    uint32_t score = 0;

    TextPrint(0, "HOME PAGE !!!");
    TextPrint(1, "This is a Traffic Light game!");
    TextPrint(2, "                       ");

    TextPrint(3, "***READ BEFORE START***");
    
    TextPrint(5, "A. Rules");
    TextPrint(6, "1. Eatting the ramdonly appear small golden small sprite.");
    TextPrint(7, "2. Each small golden small sprite get 1 score.");
    TextPrint(8, "3. The color of signal at bottom right would change.");
    TextPrint(9, "   From green, coral, to red while the score is increasing.");
    TextPrint(10, "4. You will win the game while you get three points,");
    TextPrint(11, "   which is the signal turns to red.");
    TextPrint(12, "5. After winning the game, you'll be transferred to HOME page.");

    TextPrint(14, "B. Controls");
    TextPrint(15, "1. btn WADX is for controlling the large spirite.");
    TextPrint(16, "2. btn CMD is for switching between HOME and GAME page.");
    TextPrint(17, "3. btn K is for resetting the score.");
    TextPrint(18, "4. btn U is for switching between keeping and stopping");
    TextPrint(19, "         changing the color of large sprite.");
    TextPrint(20, "5. btn I is for switching betwen having BackGround or not.");

    TimerInterval(TMSET, 50);

    while (1) {
        CurrentTicks = Timer(SYSTIMER);

        VideoCtrLS(VideoMode, LARGESPRITE, ' ', x_pos, y_pos, 0, 50, 50, 0, LIGHTSKYBLUE);
        
        if (count % 10 < 5)
        {
            Color_Random(COLOR_CHANGE, 0);
        }
        else if(count % 10 >= 5)
        {
            VideoCtrLS(VideoMode, LARGESPRITE, ' ', x_pos, y_pos, 0, 50, 50, 0, LIGHTSKYBLUE);
        }

        if (count_BG % 10 < 5)
        {
            VideoCtrBG(VideoMode, BACKGROUND, ' ', 0, 0, 0, 0, 0, 0, MEDIUMSLATEBLUE);
        }
        else if(count_BG % 10 >= 5)
        {
            VideoCtrBG(VideoMode, BACKGROUND, ' ', 0, 0, 0, 0, 0, 0, BLACK);
        }

        if ( ((x_pos <= x_pos_R && ( x_pos_R<(x_pos+50))) && (y_pos <= y_pos_R && ( y_pos_R<(y_pos+50)))) || ((x_pos <= (x_pos_R+15) && ( x_pos_R<(x_pos+50))) && (y_pos <= (y_pos_R+15) && ( y_pos_R<(y_pos+50)))))
        {
            CleanDot(CLEAN_DOT);

            x_pos_R = random_number(1, 460);
            y_pos_R = random_number(1, 235);
            
            if(score < 5)
            {
                VideoCtrSS(VideoMode, SMALLSPRITE, ' ', x_pos_R, y_pos_R, 0, 15, 15, 1, GOLD);
                score ++;
            }

        }

        if (score == 0)
        {
            // all black
            VideoCtrSS(VideoMode, SMALLSPRITE, ' ', 320, 170, 0, 10, 10, 2, BLACK);
        }
        else if (score == 1)
        {
            VideoCtrSS(VideoMode, SMALLSPRITE, ' ', 320, 170, 0, 10, 10, 2, LIME);
        }
        else if (score == 2)
        {
            VideoCtrSS(VideoMode, SMALLSPRITE, ' ', 320, 170, 0, 10, 10, 2, CORAL);

        }
        else if (score == 3)
        {
            VideoCtrSS(VideoMode, SMALLSPRITE, ' ', 320, 170, 0, 10, 10, 2, RED);
        }
        else if (score == 5)
        {
            ModeSelect(MODE, TextMode);
            TextPrint(22, "***");
            TextPrint(23, "Congradulations!!!!!!");
            TextPrint(24, "                  ***");
        }

        if(CurrentTicks != LastGlobalTicks){
            ControllerStatus = Ctr_Status(CTL_STATUS);

            if(ControllerStatus){
                if (Mode_Status(MODE_STATUS) == 0x0)
                {
                    if(ControllerStatus & 0x1){             // LDD
                        if(XPos & 0x3F){
                        }
                    }
                    if(ControllerStatus & 0x2){             // UDD
                        if(XPos >= 0x40){
                        }
                    }
                    if(ControllerStatus & 0x4){             // DDD
                        if(XPos < 0x8C0){
                        }
                    }
                    if(ControllerStatus & 0x8){             // RDD
                        if((XPos & 0x3F) != 0x3F){
                        }
                    }
                    if(ControllerStatus & 0x10)             // B4D (u)
                    {

                    }
                    if(ControllerStatus & 0x20)             // B2D (i)
                    {

                    }
                    if(ControllerStatus & 0x40)             // B2D (j)
                    {

                    }
                    if(ControllerStatus & 0x80)             // B3D (k)
                    {
                        score = 0;
                        TextPrint(22, "                                ");
                        TextPrint(23, "                                ");
                        TextPrint(24, "                                ");
                    }                                   

                }
                else if (Mode_Status(MODE_STATUS) == 0x1)
                {
                    if(ControllerStatus & 0x1){             // LDD
                        if(x_pos > 12){
                            x_pos -=2;
                        }
                    }
                    if(ControllerStatus & 0x2){             // UDD
                        if(y_pos > 12){
                            y_pos -=2;
                        }
                    }
                    if(ControllerStatus & 0x4){             // DDD
                        if(y_pos < 240){
                            y_pos +=2;
                        }
                    }
                    if(ControllerStatus & 0x8){             // RDD
                        if(x_pos < 480){
                            x_pos +=2;
                        }
                    }
                    if(ControllerStatus & 0x10)             // B4D (u)
                    {
                        count ++;
                    }
                    if(ControllerStatus & 0x20)             // B2D (i)
                    {
                        count_BG ++;
                    }
                    if(ControllerStatus & 0x40)             // B2D (j)
                    {

                    }
                    if(ControllerStatus & 0x80)             // B3D (k)
                    {

                        score = 0;

                    }

                }
            }
            
            LastGlobalTicks = CurrentTicks;
        }
    }
    return 0;
}


int random_number(int min, int max)
{
    int result = 0, low = 0, hi = 0;

    if (min < max)
    {
        low = min;
        hi = max + 1; // include max_num in output
    } else {
        low = max + 1; // include max_num in output
        hi = min;
    }

    result = (rand() % (hi - low)) + low;
    return result;
}

void TextPrint(int line, char str[])
{
    int LineNum = line * 64;
    for (int i = 0; i < strlen(str); i++)
    {
        Text_Convert(TEXT_PRINT, i + LineNum, str[i]);
    }
}