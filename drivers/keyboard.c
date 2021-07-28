#include "keyboard.h"
#include "display.h"
#include "ports.h"
#include "utils.h"

// hex keyboard scancodes: https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
// space either means space or tab 
// special key indexes: not:err, 0:esc, 11:backspace, 25:enter, 26: Lctrl, 41:Lshift, 53:Rshift, 54:prntscrn, 55:LALT, -1:RARROW, -3:LARROW
// blank strings are keys im not implementing 
char *keys[NUM_KEYS] = {"", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACKSPACE", " ", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "ENTER", "", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", "LSHIFT", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "RSHIFT", "", "", " ", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "LARROW", "", "RARROW"};
char input_buffer[MAX_BUFFER_SIZE];
int buffer_idx = 0;

// add char to input_buffer array (string)
void append_buffer(char val) {
	input_buffer[buffer_idx] = val;
	buffer_idx++;
}

// kind of a lie, but input_buffer will only be read up to buffer_idx
void clear_buffer(void) {
	buffer_idx = 0;
}

// process keyboard input 
void keypress(unsigned char input) {
	int idx = ((int) input) - 1;  // scancodes are conveniently in order (except for 0th scancode)

	if (idx < NUM_KEYS) {
		char *key = keys[idx];

		// TODO actually handle stuff
		if (str_comp(key, "BACKSPACE")) {
			buffer_idx--;
			// TODO
			// either need to track current cursor offset or get it in display, then decrement it here
			// rm char on display (go to vga_idx in vga and make '') and then shift any chars ahead back
			// vga_idx--
		} else if (str_comp(key, "ENTER")) {
			print_str("special\n", WHITE);
		} else if (str_comp(key, "LARROW")) {
			print_str("special\n", WHITE);
		} else if (str_comp(key, "RARROW")) {
			print_str("special\n", WHITE);
		} else if (str_comp_key(key, "LSHIFT") || str_comp(key, "RSHIFT")) {
			print_str("special\n", WHITE);
		}
		} else if (str_comp(key, "")) {
			print_str("other guy\n", WHITE);
		} else {
			print_str(key, WHITE);
			char key_char = key[0];

			// a lazy catch, but better than nothing
			if (buffer_idx < MAX_BUFFER_SIZE) {
				append_buffer(key_char);
			} else {
				clear_buffer();
				append_buffer(key_char);
			}

			// TODO rm
			print_str("\nbuffer:\n", WHITE);
			print_str(input_buffer, WHITE);
			print_str("\n", WHITE);
		}
	} 
}

void repl(void) {
	unsigned char input = 0x0;

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
