# README -- by Chih-Hsiang Chen, 15 Mar. 2021

The "test.sh" is the shell script document written for operating Makefile in both riscv-firmware and riscv-cartridge files automatically.

About the Final phase, there are some requirement as following:
1. Launch the application program when cartridge is entered;
2. Access multi-button controller input;
3. React to periodic timer;
4. React to video interrupt;
5. Draw to the background, large and small sprites.

In the README, in order to demonstrate my result, there are three parts of this README.md: the descriptions about the requirements one-by-one, detailed explanation of APIs, and the demonstration of my small game, the implementation.

A.  Requirements
1.  The content of cartridge is about the game I designed. The cartridge interrupt would be triggered automatically by hitting “cartridge button”. And all of the sys_calls in the cartridge meet  
    the requirement of being APIs — don’t access the hardware address directly. These API would be described in the second section of README.md. 
2.  Most of buttons on the controller have each function basing on different modes, and all of them are read and reacted in the program.
3.  There are two ways I demonstrate the reaction to periodic timer. First is polling inputs from controller regularly. Second is by hitting button_U, the color of large sprite would be change 
    regularly. And the timer interval could be set to adjust the frequency by TimerInterval( ) as well.
4.  About the reaction to video interrupt, I design Color_Random( ) to demonstrate it. This function is combining timer interrupt and video interrupt together to achieve the goal.
5.  The APIs about drawing background, large and small sprites are VideoCtrBG( ), VideoCtrLS( ), and VideoCtrSS( ), which are designed for setting the required parameters of these elements.

B.  APIs
1.  void TimerInterval(func, interval)
    The first system call I designed is to set the timer interval. By setting "func" to TMSET and "interval" to the desired interval, user can easily change the value of the interval.

2.  void ModeSelect(func)
    The original purpose of this system call is to set the "Mode" in Mode Control Register. However, I finally decide to design this system call for setting TEXT mode and design VIDEO mode separately.

3.  int Ctr_Status, char Mode_Status(func)
    The functions is designed for allowing cartridge to communicate with firmware about the mode status -- TEXT mode or GRAPHS mode. In this situation, the cartridge do not need to obtain the status of cartridge by accessing memory address.

4.  VideoCtrBG, VideoCtrLS, VideoCtrSS(func, type, x, y, z, w, h, palette, color)
    These system calls are separated from Modeselect(func).
    "func" would be set as "VideoMode", which set the "Mode" in Mode Control Register to VIDEO mode;
    "type" is the bit used to set three different video types, which are background, large sprite, and small sprite;
    "x, y, z, w, h, palette, color" are the parameters for three different video types' setting.

5.  void Text_Convert(func, Posi, Chr)
    The goal of the function is similar to Ctr_Status, char Mode_Status, which allowing cartridge to communicate with firmware without accessing memory address. The function can easily set the position and the displayed char one by one.

6.  void TextPrint(line, str[])
    The function is packaging Text_Convert() in another form. Importantly, by using this function, users can print "string" at once rather than calling Text_Convert() for several times.

7.  void Color_Random(func, palette)
    The function is designed to react to video interrupt primarily. By hitting button_U, the interrupt would be reacted and the color of the large sprite would be switched between keeping changing colors and staying at the same color. The parameters are function category and the specified palette which is going to be controlled.

8.  void CleanDot(func)
    The function is called when the game is completed, and later, clean the signal sprite used to determine player's score.

C.  Application -- Traffic Light Game
a.  Rules
    1. Eating the randomly appear small golden small sprite. 
    2. Each small golden small sprite get 1 score.
    3. The color of signal at bottom right would change. From green, coral, to red while the score is increasing.
    4. You will win the game while you get three points, which is the signal turns to red.
    5. After winning the game, you'll be transferred to HOME page.
    6. Hitting button K for resatartting the game.

b. Controls
    1. Button WADX is for controlling the large sprite.
    2. Button CMD is for switching between HOME and GAME page.
    3. Button K is for resetting the score.
    4. Button U is for switching between keeping and stopping changing the color of large sprite.
    5. Button I is for switching between having BackGround or not.
