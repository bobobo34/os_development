#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//check if compiler thinks you are targeting wrong os
#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif
//this tutorial will only work for 32-bit ix86 targets
#if !defined(__i386__)
#error "This (kernel.c) needs to be compiled with an ix86-elf compiler"
#endif

//Hardware text mode color constants
enum vga_color {
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while(str[len])
        ++len;
    return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000; // VGA text mode buffer
    for(size_t y = 0; y < VGA_HEIGHT; ++y) {
        for(size_t x = 0; x < VGA_WIDTH; ++x) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void scroll_terminal(void) {
    
}
void terminal_putchar(char c) {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if(++terminal_column == VGA_WIDTH) { //reset column(x) if it overflows
        terminal_column = 0;
        if(++terminal_row == VGA_HEIGHT) { //same here
            terminal_row = 0;
        }
    }
}

void terminal_write(const char* data, size_t size) {
    for(size_t i = 0; i < size; ++i) {
        char c = data[i];
        if(c == '\n') {
            terminal_column = 0;
            if(++terminal_row == VGA_HEIGHT) {
                terminal_row = 0;
            }
        }
        else {
            terminal_putchar(c);
        }
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}


void kernel_main(void) {
    //init terminal interface
    terminal_initialize();

    //newline support TODO
    terminal_writestring("Hello, kernel World!\nIs this on a new line?");
}
