#include "kernel.h"
#include "shell.h"      
#include "std_type.h"   

#define VIDEO_MEMORY_SEGMENT 0xB800
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define DEFAULT_TEXT_ATTRIBUTE 0x07 

static int g_cursor_x = 0;
static int g_cursor_y = 0;
static byte g_current_text_attribute = DEFAULT_TEXT_ATTRIBUTE;


static void update_cursor_bios() {
   
    int ax = (0x02 << 8); 
    int bx = 0x0000;      
    int dx = (g_cursor_y << 8) | g_cursor_x; 
    _interrupt_invoke(0x10, ax, bx, 0, dx);
}

static void scroll_screen() {
    int x, y;
    unsigned int offset_dst; 


    y = SCREEN_HEIGHT - 1;
    for (x = 0; x < SCREEN_WIDTH; x++) {
        offset_dst = (y * SCREEN_WIDTH + x) * 2;
        _putInMemory(VIDEO_MEMORY_SEGMENT, offset_dst, ' '); 
        _putInMemory(VIDEO_MEMORY_SEGMENT, offset_dst + 1, g_current_text_attribute);
    }

    g_cursor_y = SCREEN_HEIGHT - 1;
    g_cursor_x = 0;
}



void setGlobalTextColor(byte attribute) {
    g_current_text_attribute = attribute;
}

void printChar(char c) {
    unsigned int offset;

    if (c == '\n') {
        g_cursor_y++;
        g_cursor_x = 0;
    } else if (c == '\r') {
        g_cursor_x = 0;
    } else if (c == '\b') {
        if (g_cursor_x > 0) {
            g_cursor_x--;
            offset = (g_cursor_y * SCREEN_WIDTH + g_cursor_x) * 2;
            _putInMemory(VIDEO_MEMORY_SEGMENT, offset, ' ');
            _putInMemory(VIDEO_MEMORY_SEGMENT, offset + 1, g_current_text_attribute);
        } else if (g_cursor_y > 0) {
            g_cursor_y--;
            g_cursor_x = SCREEN_WIDTH - 1;
        }
    } else if (c >= ' ') {
        offset = (g_cursor_y * SCREEN_WIDTH + g_cursor_x) * 2;
        _putInMemory(VIDEO_MEMORY_SEGMENT, offset, (byte)c);
        _putInMemory(VIDEO_MEMORY_SEGMENT, offset + 1, g_current_text_attribute);
        g_cursor_x++;
    }

    if (g_cursor_x >= SCREEN_WIDTH) {
        g_cursor_x = 0;
        g_cursor_y++;
    }
    if (g_cursor_y >= SCREEN_HEIGHT) {
        scroll_screen();
    }
    update_cursor_bios();
}

void printString(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        printChar(str[i]);
        i++;
    }
}

void clearScreen() {
    int x, y;
    unsigned int offset;
    for (y = 0; y < SCREEN_HEIGHT; y++) {
        for (x = 0; x < SCREEN_WIDTH; x++) {
            offset = (y * SCREEN_WIDTH + x) * 2;
            _putInMemory(VIDEO_MEMORY_SEGMENT, offset, ' ');
            _putInMemory(VIDEO_MEMORY_SEGMENT, offset + 1, g_current_text_attribute);
        }
    }
    g_cursor_x = 0;
    g_cursor_y = 0;
    update_cursor_bios();
}

char readKeyboardChar() {
    int ax_input = (0x00 << 8);
    int returned_ax = _interrupt_invoke(0x16, ax_input, 0, 0, 0);
    return (char)(returned_ax & 0xFF);
}

void readString(char *buf) {
    int i = 0;
    char c;
    while (1) {
        c = readKeyboardChar();
        if (c == '\r') {
            buf[i] = '\0';
            printChar('\n');
            break;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                printChar('\b');
            }
        } else if (c >= ' ' && c <= '~') {

            buf[i] = c;
            printChar(c);
            i++;
        }
    }
}

int main() {
  
    int ax_debug = (0x0E << 8) | 'K'; 
    int bx_debug = 0x0007;            
    _interrupt_invoke(0x10, ax_debug, bx_debug, 0, 0); 
 

    clearScreen(); 
    setGlobalTextColor(DEFAULT_TEXT_ATTRIBUTE); 
    
    printString("Welcome to EorzeOS!\n");
    
    shell(); 
    
    while(1); 
    return 0; 
}
