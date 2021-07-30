#include "keyboard.h"
#include "display.h"
#include "ports.h"
#include "utils.h"
#include "../kernel/state.h"

// hex keyboard scancodes: https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
// space either means space or tab 
// special key indexes: not:err, 0:esc, 11:backspace, 25:enter, 26: Lctrl, 41:Lshift, 53:Rshift, 54:prntscrn, 55:LALT, -1:RARROW, -3:LARROW
// blank strings are keys im not implementing 
char *keys[NUM_KEYS] = {"", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACKSPACE", " ", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "ENTER", "", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", "LSHIFT", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "RSHIFT", "", "", " ", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "LARROW", "", "RARROW"};
//char *shift_keys[NUM_KEYS] = {};  // TODO
int shift_on = 0;
char input_buffer[MAX_BUFFER_SIZE];
int buffer_end = 0;  // current idx of buffer to alter

// add char to input_buffer array (string)
void update_buffer(char val) {
	input_buffer[buffer_end] = val;
	buffer_end++;
}

// kind of a lie, but input_buffer will only be read up to idx of buffer_size - 1
void clear_buffer(void) {
	buffer_end = 0;
}

// process keyboard input, returns 1 if newline needed (i.e. print out terminal pwd prompt again)
void keypress(unsigned char input) {
	int idx = ((int) input) - 1;  // scancodes are conveniently in order (except for 0th scancode)

	if (idx < NUM_KEYS) {
		char *key = keys[idx];

		if (str_comp(key, "LSHIFT") || str_comp(key, "RSHIFT")) {
			shift_on = shift_on ? 0 : 1;  // shift will work like caps lock key
		} else if (str_comp(key, "LARROW") || str_comp(key, "RARROW")) {
			// not handling these for the moment
		} else if (str_comp(key, "ENTER")) {
			// fun from somewhere to process buffer (input is input_buffer[0] to input_buffer[buffer_end - 1])
			print_str("\n", WHITE);
			print_str("buffer: ", WHITE);
			input_buffer[buffer_end] = '\0';  // this is safe right?
			print_str(input_buffer, WHITE);
			print_str("\n", WHITE);
			print_str(TERM_PROMPT, TERM_COLOR);
			clear_buffer();

		} else if (str_comp(key, "")) {
			// other unhandled special keys
		} else if (str_comp(key, "BACKSPACE")) {
			if (buffer_end > 0) {
				buffer_end--;  // basically just shifts where the end of the expected input is
			}

			update_vga_idx(-1);
			print_str(" ", WHITE);
			update_vga_idx(-1);

		} else {
			if (shift_on) {
				//key = shift_keys[idx];  // TODO
			}

			update_buffer(key[0]);  // safe to do bc key should only be a single char here
			print_str(key, WHITE);
		}
	} 
}

void repl(void) {
	unsigned char input = 0x0;
	print_str(TERM_PROMPT, TERM_COLOR);

	while (1) {
		// read value (byte) at keyboard addr into input variable
		input = read_port(KEYBOARD_ADDR);

		// haven't figured out the actual default scancode yet, so this max is kinda arbitrary
		if (input != 0x0 && input <= 0x80) {
			keypress(input);
			input = 0x0;
			write_port(KEYBOARD_ADDR, input);  // set back to err code
		}
	}
}
