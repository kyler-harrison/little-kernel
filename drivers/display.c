#include "display.h"

unsigned short *terminal_buff = (unsigned short *) VGA_ADDR;  // vga terminal display
unsigned int vga_idx = 0;  // track cursor in terminal

// empty screen
void clear_screen(void) {
	// screen is 25 lines of 80 elems, elems are unsigned shorts (2 bytes to represent character and its color)
	
	int i;
	for (i = 0; i < COLS * ROWS; i++) {
		terminal_buff[i] = (unsigned short) BLACK << 8 | (unsigned short) ' ';
	}
}

// move to next line on screen
void vga_newline(void) {
	vga_idx = (vga_idx / COLS + 1) * COLS;  // get to start idx of current line then get to start idx of next
}

// shifts screen down when output reaches end of terminal screen
void shift(void) {
}

// char in output defined by 2 bytes: str byte and color byte
void print_str(char *str, unsigned char color) {
	int idx = 0;

	while (str[idx]) {
		// 2 byte input where first byte represents color and second represents character
		if (str[idx] != '\n') {
			terminal_buff[vga_idx] = (unsigned short) color << 8 | (unsigned short) str[idx];
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

