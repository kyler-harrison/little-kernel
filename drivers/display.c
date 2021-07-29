#include "display.h"
#include "ports.h"

unsigned short *terminal_buff = (unsigned short *) VGA_ADDR;  // vga terminal display
unsigned int vga_idx = 0;  // track end of line
int cursor_idx = 0;  // track cursor in terminal
int lower_bound = 1;  // refers to idx of a single line, i.e. the ">" in terminal (will need to update once dirs implemented) 
int current_line = 0;

// empty screen
void clear_screen(void) {
	// screen is 25 lines of 80 elems, elems are unsigned shorts (2 bytes to represent character and its color)
	
	int i;
	for (i = 0; i < COLS * ROWS; i++) {
		terminal_buff[i] = (unsigned short) WHITE << 8 | (unsigned short) ' ';
	}
}

// set blinking cursor to position in vga
void set_cursor(int pos) {
	// https://wiki.osdev.org/Text_Mode_Cursor#Moving_the_Cursor
	write_port(CURSOR_CTRL_ADDR, VGA_LOW);
	write_port(CURSOR_DATA_ADDR, (unsigned char) (pos & 0xFF));
	write_port(CURSOR_CTRL_ADDR, VGA_HIGH);
	write_port(CURSOR_DATA_ADDR, (unsigned char) ((pos >> 8) & 0xFF));
}

// update the index tracking the current cursor position
void update_cursor_idx(int offset) {
	int check = cursor_idx + offset;

	// cursor is bounded to line
	if ((offset == -1 && check > (current_line * COLS + 1 + offset)) || (offset == 1 && check <= vga_idx)) {
		cursor_idx += offset;
		set_cursor(cursor_idx);
	}
}

// move to next line on screen
void vga_newline(void) {
	vga_idx = (vga_idx / COLS + 1) * COLS;  // get to start idx of current line then get to start idx of next
	cursor_idx = vga_idx;
	current_line = vga_idx / COLS;
}

// shifts screen down when output reaches end of terminal screen
void shift(void) {
	unsigned short *terminal_cp;  
	int i;

	// basically just removes first vga row, shifts each row by -1 index, blanks last row 
	for (i = 0; i < MAX_IDX; i++) {
		if (i < (MAX_IDX - COLS) * COLS) {
			terminal_buff[i] = terminal_buff[i + COLS];
		} else {
			terminal_buff[i] = (unsigned short) WHITE << 8 | (unsigned short) ' ';
		}
	}

	vga_idx = MAX_IDX - COLS;
	cursor_idx = cursor_idx - COLS;  // TODO check if this is right
	current_line = vga_idx / COLS;
}

// char in output defined by 2 bytes: str byte and color byte
void print_str(char *str, unsigned char color) {
	int idx = 0;

	while (str[idx]) {
		// 2 byte input where first byte represents color and second represents character
		if (str[idx] != '\n') {
			
			// bottom of screen reached 
			if (vga_idx >= MAX_IDX) {
				shift();
			}

			terminal_buff[vga_idx] = (unsigned short) color << 8 | (unsigned short) str[idx];
			vga_idx++;
			cursor_idx++;
			set_cursor(cursor_idx);

		} else {
			vga_newline();
			set_cursor(cursor_idx);
		}

		idx++;
	}

	// a single str automatically wraps, but a newline may still be needed at the end
	if (idx / COLS > 0 && idx != 0 && str[idx - 1] != '\n') {
		vga_newline();
		set_cursor(cursor_idx);
	}
} 

