#include "display.h"

unsigned short *terminal_buff = (unsigned short *) VGA_ADDR;  // vga terminal display
unsigned int vga_idx = 0;  // track cursor in terminal

// make empty screen
void clear_screen(void) {
	int idx = 0;
	// 25 lines of 80 columns (ascii chars), each elem is 2 bytes

	while (idx < COLS * ROWS * 2) {
		terminal_buff[idx] = ' ';
		idx += 2;
	}
}

// move to next line on screen
void vga_newline(void) {
	vga_idx = (vga_idx / COLS + 1) * COLS;  // get to start idx of current line, get to start idx of next
}

// char in output defined by 2 bytes: str byte and color byte
void print_str(char *str, unsigned char color) {
	int idx = 0;

	while (str[idx]) {
		// this makes a 2 byte (16 bit) input where the first 8 bits represent the color
		// and the second 8 bits represent the character
		if (str[idx] != '\n') {
			terminal_buff[vga_idx] = (unsigned short) str[idx] | (unsigned short) color << 8;
			vga_idx++;

		} else {
			vga_newline();
		}

		idx++;
	}

	// a single str automatically wraps, but a newline may still be needed at the end
	if (idx / COLS > 0 && idx != 0 && str[idx - 1] != '\n') {
		vga_newline();
	}
} 

