#include <stdint.h>
#include <stdio.h>
#include "CPUHAL.h"

extern volatile uint32_t TimerTicks;
#define CARTRIDGE_STATUS_REG (*(volatile uint32_t *)0x4000001C)
typedef void (*TEntryFunction)(void);

volatile char *text_memory = (volatile char *)(0x50000000 + 0xFE800);

int main() {
    TEntryFunction EntryFunction;

    while(1){
        if(CARTRIDGE_STATUS_REG & 0x1){
            /*
            text_memory[128] = 'C';
            text_memory[129] = 'a';
            text_memory[130] = 'r';
            text_memory[131] = 't';
            text_memory[132] = ' ';
            text_memory[133] = 'I';
            text_memory[134] = 'n';
            text_memory[135] = 's';
            text_memory[136] = 'e';
            text_memory[137] = 'r';
            text_memory[138] = 't';
            text_memory[139] = 'e';
            text_memory[140] = 'd';
            text_memory[141] = '!';
            text_memory[142] = ' ';
            text_memory[143] = ' ';
            text_memory[144] = ' ';
            text_memory[145] = ' ';
            */
            EntryFunction = (TEntryFunction)(CARTRIDGE_STATUS_REG & 0xFFFFFFFC);
            EntryFunction();
        }
        else
        {
            text_memory[128] = 'P';
            text_memory[129] = 'l';
            text_memory[130] = 'e';
            text_memory[131] = 'a';
            text_memory[132] = 's';
            text_memory[133] = 'e';
            text_memory[134] = ' ';
            text_memory[135] = 'I';
            text_memory[136] = 'n';
            text_memory[137] = 's';
            text_memory[138] = 'e';
            text_memory[139] = 'r';
            text_memory[140] = 't';
            text_memory[141] = ' ';
            text_memory[142] = 'C';
            text_memory[143] = 'a';
            text_memory[144] = 'r';
            text_memory[145] = 't';
        }
    }
    return 0;
}

