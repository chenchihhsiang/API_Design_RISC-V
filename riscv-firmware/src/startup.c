#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern uint8_t _erodata[];
extern uint8_t _data[];
extern uint8_t _edata[];
extern uint8_t _sdata[];
extern uint8_t _esdata[];
extern uint8_t _bss[];
extern uint8_t _ebss[];

// Graphic parameter def.
// Video BackGround Control
volatile uint32_t *BGControl0 = (volatile uint32_t *)(0x50000000 + 0xFF100);      // Background Control 0
volatile uint32_t *BGControl1 = (volatile uint32_t *)(0x50000000 + 0xFF104);      // Background Control 1
volatile uint32_t *BGControl2 = (volatile uint32_t *)(0x50000000 + 0xFF108);      // Background Control 2
volatile uint32_t *BGControl3 = (volatile uint32_t *)(0x50000000 + 0xFF10C);      // Background Control 3
volatile uint32_t *BGControl4 = (volatile uint32_t *)(0x50000000 + 0xFF110);      // Background Control 4

// Video Large Sprite Control
volatile uint32_t *LSControl0 = (volatile uint32_t *)(0x50000000 + 0xFF114);      // Large Sprite Control 0
volatile uint32_t *LSControl1 = (volatile uint32_t *)(0x50000000 + 0xFF118);      // Large Sprite Control 1
volatile uint32_t *LSControl2 = (volatile uint32_t *)(0x50000000 + 0xFF11C);      // Large Sprite Control 2
volatile uint32_t *LSControl3 = (volatile uint32_t *)(0x50000000 + 0xFF120);      // Large Sprite Control 3
volatile uint32_t *LSControl4 = (volatile uint32_t *)(0x50000000 + 0xFF124);      // Large Sprite Control 4

// Video Small Sprite Control
volatile uint32_t *SSControl0 = (volatile uint32_t *)(0x50000000 + 0xFF214);      // Small Sprite Control 0
volatile uint32_t *SSControl1 = (volatile uint32_t *)(0x50000000 + 0xFF218);      // Small Sprite Control 1
volatile uint32_t *SSControl2 = (volatile uint32_t *)(0x50000000 + 0xFF21C);      // Small Sprite Control 2
volatile uint32_t *SSControl3 = (volatile uint32_t *)(0x50000000 + 0xFF220);      // Small Sprite Control 3
volatile uint32_t *SSControl4 = (volatile uint32_t *)(0x50000000 + 0xFF224);      // Small Sprite Control 4

// Video Background Palette Control
volatile uint32_t *BPControl0 = (volatile uint32_t *)(0x50000000 + 0xFC000);      // Background Palette Control 0
volatile uint32_t *BPControl1 = (volatile uint32_t *)(0x50000000 + 0xFC400);      // Background Palette Control 1
volatile uint32_t *BPControl2 = (volatile uint32_t *)(0x50000000 + 0xFC800);      // Background Palette Control 2
volatile uint32_t *BPControl3 = (volatile uint32_t *)(0x50000000 + 0xFCC00);      // Background Palette Control 3

// Video Sprite Palette Control
volatile uint32_t *SPControl0 = (volatile uint32_t *)(0x50000000 + 0xFD000);      // Sprite Palette Control 0
volatile uint32_t *SPControl1 = (volatile uint32_t *)(0x50000000 + 0xFD400);      // Sprite Palette Control 1
volatile uint32_t *SPControl2 = (volatile uint32_t *)(0x50000000 + 0xFD800);      // Sprite Palette Control 2
volatile uint32_t *SPControl3 = (volatile uint32_t *)(0x50000000 + 0xFDC00);      // Sprite Palette Control 3

volatile char *VIDEO_MODE = (volatile char *)(0x50000000 + 0xFF414);              // Mode Control Reg
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);            // text

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
    COLOR = 0,
    RED = 0xFF0000,
    MEDIUMSLATEBLUE = 0x7B68EE,
    LIME = 0x00FF00,
    LIGHTSKYBLUE = 0x87CEFA,
    CORAL = 0xFF7F50,
    GOLD = 0xFFD700,
    BLACK = 0x000000,
};

uint32_t color = 0;

uint32_t V_Pause_Count = 0;

/*   convert function define   */
uint32_t convertBGC();
uint32_t convertLSC();
uint32_t convertSSC();

// Adapted from https://stackoverflow.com/questions/58947716/how-to-interact-with-risc-v-csrs-by-using-gcc-c-code
__attribute__((always_inline)) inline uint32_t csr_mstatus_read(void){
    uint32_t result;
    asm volatile ("csrr %0, mstatus" : "=r"(result));
    return result;
}

__attribute__((always_inline)) inline void csr_mstatus_write(uint32_t val){
    asm volatile ("csrw mstatus, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_write_mie(uint32_t val){
    asm volatile ("csrw mie, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_enable_interrupts(void){
    asm volatile ("csrsi mstatus, 0x8");
}

__attribute__((always_inline)) inline void csr_disable_interrupts(void){
    asm volatile ("csrci mstatus, 0x8");
}

__attribute__((always_inline)) inline uint32_t csr_mcause_read(void){
    uint32_t result;
    asm volatile ("csrr %0, mcause" : "=r"(result));
    return result;
}

#define INTERRRUPT_ENABLE (*((volatile uint32_t *)0x40000000))
#define INTERRUPT_PENDING (*((volatile uint32_t *)0x40000004))
#define MTIME_LOW       (*((volatile uint32_t *)0x40000008))
#define MTIME_HIGH      (*((volatile uint32_t *)0x4000000C))
#define MTIMECMP_LOW    (*((volatile uint32_t *)0x40000010))
#define MTIMECMP_HIGH   (*((volatile uint32_t *)0x40000014))
#define CONTROLLER      (*((volatile uint32_t *)0x40000018))
#define CARTRIDGE_STATUS (*((volatile uint32_t *)0x4000001C))
#define CARTRIDGE_ROM (*((volatile uint32_t *)0x20000000))
volatile char *text = (volatile char *)(0x50000000 + 0xFE800);

void init(void){
    uint8_t *Source = _erodata;
    uint8_t *Base = _data < _sdata ? _data : _sdata;
    uint8_t *End = _edata > _esdata ? _edata : _esdata;

    while(Base < End){
        *Base++ = *Source++;
    }
    Base = _bss;
    End = _ebss;
    while(Base < End){
        *Base++ = 0;
    }

    //INTERRRUPT_ENABLE = 0x7;    // Enable every external interrupt, including CMD, video, and cartridge

    INTERRRUPT_ENABLE = INTERRRUPT_ENABLE | 0x4;        // enable CMIE
    
    INTERRRUPT_ENABLE = INTERRRUPT_ENABLE | 0x2;        // enable VIE

    INTERRRUPT_ENABLE = INTERRRUPT_ENABLE | 0x1;        // enable CIE

    csr_write_mie(0x888);       // Enable all interrupt soruces
    csr_enable_interrupts();    // Global interrupt enable
    MTIMECMP_LOW = 100;
    MTIMECMP_HIGH = 0;
}

volatile uint32_t TimerTicks;
volatile uint32_t CompareTicks;

volatile uint32_t Ctr_Status;

void c_interrupt_handler(void){

    /*
    uint64_t NewCompare = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
    NewCompare = NewCompare + CompareTicks + 100;
    MTIMECMP_HIGH = NewCompare>>32;
    MTIMECMP_LOW = NewCompare;
    TimerTicks++;
    */

    // get the exception code of mcause
    uint32_t csr_mcause = csr_mcause_read() & 0xF;
    
    // if mcause = Timer interrupt
    if (csr_mcause == 0x7)
    {
        //text[305] = 'T';
        uint64_t NewCompare = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
        NewCompare = NewCompare + CompareTicks + 100;
        MTIMECMP_HIGH = NewCompare>>32;
        MTIMECMP_LOW = NewCompare;
        TimerTicks++;
    }
    // if mcause = External interrupt && CMD interrupt
    else if ((csr_mcause == 0xb) && (INTERRUPT_PENDING & 0x4))
    {
        if (VIDEO_MODE[0] == 0x0)
        {
            VIDEO_MODE[0] = 0x1;
        }
        else if (VIDEO_MODE[0] == 0x1)
        {
            VIDEO_MODE[0] = 0x0;
        }
        INTERRUPT_PENDING = INTERRUPT_PENDING | 0x4;
        return;
    }
    // if mcause = External interrupt && Video interrupt
    else if ((csr_mcause == 0xb) && (INTERRUPT_PENDING & 0x2))
    {
        //text[300] = 'V';

        color ++;

        if(INTERRUPT_PENDING & 0x2)
        {
            INTERRUPT_PENDING = INTERRUPT_PENDING | 0xfffffff2;
        }
    }
}

uint32_t c_syscall(uint32_t func, uint32_t type, char Chr, uint32_t x, uint32_t y, uint32_t z, uint32_t w, uint32_t h, uint32_t palette, uint64_t pColor){
    /*
    if(param == 3){
        return TimerTicks;
    }
    */
    switch (func)
    {
        case SYSTIMER:
            return TimerTicks;
            break;

        case TMSET:
            CompareTicks = type;
            break;
        
        case TextMode:
            //VIDEO_MODE[0] = 0x0;
            text[192] = 'T';
            text[193] = 'e';
            text[194] = 'x';
            text[195] = 't';
            text[196] = ' ';
            text[197] = 'M';
            text[198] = 'o';
            text[199] = 'd';
            text[200] = 'e';
            text[201] = ' ';
            text[202] = 'S';
            text[203] = 'y';
            text[204] = 's';
            text[205] = 'C';
            text[206] = 'a';
            text[207] = 'l';
            text[208] = 'l';
            text[209] = ' ';
            break;

        case VideoMode:
            if (type == BACKGROUND)
            {
                //VIDEO_MODE[0] = 0x1;
                BPControl0[0] = pColor;
                BGControl0[0] = convertBGC(x, y, z, palette);

            }
            else if (type == LARGESPRITE)
            {
                if(palette == 0)
                {
                    //VIDEO_MODE[0] = 0x1;
                    SPControl0[0] = pColor;
                    LSControl0[0] = convertLSC(x, y, w, h, palette);
                }
                else if (palette == 1)
                {
                    //VIDEO_MODE[0] = 0x1;
                    SPControl1[0] = pColor;
                    LSControl1[0] = convertLSC(x, y, w, h, palette);
                }
            }
            else if (type == SMALLSPRITE)
            {
                if (palette == 0)
                {
                    //VIDEO_MODE[0] = 0x1;
                    SPControl0[0] = pColor;
                    SSControl0[0] = convertSSC(x, y, z, w, h, palette);
                }
                else if (palette == 1)
                {
                    //VIDEO_MODE[0] = 0x1;
                    SPControl1[0] = pColor;
                    SSControl1[0] = convertSSC(x, y, z, w, h, palette);
                }
                else if (palette == 2)
                {
                    //VIDEO_MODE[0] = 0x1;
                    SPControl2[0] = pColor;
                    SSControl2[0] = convertSSC(x, y, z, w, h, palette);
                }

            }            
            break;

        case CTL_STATUS:
            return CONTROLLER;
            break;

        case MODE_STATUS:
            return VIDEO_MODE[0];
            break;

        case TEXT_PRINT:
           text[type] = Chr;
           break;

        case MODE:
            if(type == TextMode)
            {
                VIDEO_MODE[0] = 0x0;
            }
            else if (type == VideoMode)
            {
                VIDEO_MODE[0] = 0x1;
            }

        case COLOR_CHANGE:
            SPControl0[0] = color;
            break;

        case CLEAN_DOT:
            SPControl1[0] = BLACK;
            break;    

        default:
            break;
    }
    return 0;
}

uint32_t convertBGC (uint32_t x, uint32_t y, uint32_t z, uint32_t palette)
{
    x = x + 512;
    y = y + 288;
    uint32_t binaryNumber = (x << 2) + (y << 12) + (z << 22) + palette;
    return binaryNumber;
}

uint32_t convertLSC (uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t palette)
{
    x = x + 64;
    y = y + 64;
    w = w - 33;
    h = h - 33;
    uint32_t binaryNumber = (x << 2) + (y << 12) + (w << 21) + (h << 26) + palette;
    return binaryNumber;
}

uint32_t convertSSC (uint32_t x, uint32_t y, uint32_t z, uint32_t w, uint32_t h, uint32_t palette)
{
    x = x + 16;
    y = y + 16;
    w = w - 1;
    h = h - 1;
    uint32_t binaryNumber = (x << 2) + (y << 12) + (z << 29) + (w << 21) + (h << 25) + palette;
    return binaryNumber;
}