#include "display.h"
#include "ports.h"
#include "utils.h"

unsigned short *terminal_buff = (unsigned short *) VGA_ADDR;  // vga terminal display
unsigned int vga_idx = 0;  // track end of line
int lower_bound = 1;  // refers to user start idx of a single line, i.e. the ">" in terminal (will need to update once dirs implemented) 
int current_line = 0;
char input_buffer[MAX_BUFFER_SIZE];
int buffer_idx = 0;  // current idx of buffer to alter
int buffer_size = 0;  // current size of input buffer

// add char to input_buffer array (string)
void update_buffer(char val) {
	input_buffer[buffer_idx] = val;
}

// kind of a lie, but input_buffer will only be read up to buffer_idx
void clear_buffer(void) {
	buffer_idx = 0;
}

char * get_buffer() {
	return input_buffer;
}

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
void update_vga_idx(int offset) {
	int check = vga_idx + offset;

	// cursor is bounded to line
	if ((offset == -1 && check > (current_line * COLS + 1 + offset)) || offset == 1) {
		vga_idx += offset;
		set_cursor(vga_idx);
	}
}

// move to next line on screen
void vga_newline(void) {
	vga_idx = (vga_idx / COLS + 1) * COLS;  // get to start idx of current line then get to start idx of next
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
	current_line = vga_idx / COLS;
}

// char in output defined by 2 bytes: str byte and color byte
void print_str(char *str, unsigned char color) {
	int update = 1;

	// note that you can't actually edit if you move cursor
	if (str_comp(str, "BACKSPACE")) {
		update_vga_idx(-1);
		str = " ";
		update = 0;

		if (buffer_size > 0) {
			update_buffer(str[0]);
			buffer_idx--;
		}

	} else if (str_comp(str, "LARROW")) {
		update_vga_idx(-1);

		if (buffer_idx > 0) {
			buffer_idx--;
		}

		return;

	} else if (str_comp(str, "RARROW")) {
		update_vga_idx(1);

		if ((buffer_idx + 1) < buffer_size) {
			buffer_idx++;
		}

		return;
	}

	// TODO update buffer as chars inputted
	int idx = 0;
	while (str[idx]) {
		// 2 byte input where first byte represents color and second represents character
		if (str[idx] != '\n') {
			
			// bottom of screen reached 
			if (vga_idx >= MAX_IDX) {
				shift();
			}

			terminal_buff[vga_idx] = (unsigned short) color << 8 | (unsigned short) str[idx];

			if (update) {
				vga_idx++;
				set_cursor(vga_idx);
				update_buffer(str[idx]);
				buffer_idx++;
				buffer_size++;
			}

		} else {
			vga_newline();
			set_cursor(vga_idx);
		}

		idx++;
	}

	// a single str automatically wraps, but a newline may still be needed at the end
	if (idx / COLS > 0 && idx != 0 && str[idx - 1] != '\n') {
		vga_newline();
		set_cursor(vga_idx);
	}
} 

