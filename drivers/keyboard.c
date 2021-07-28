#include "keyboard.h"
#include "display.h"
#include "ports.h"
#include "utils.h"

// hex keyboard scancodes: https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
// space either means space or tab 
// special key indexes: not:err, 0:esc, 11:backspace, 25:enter, 26: Lctrl, 41:Lshift, 53:Rshift, 54:prntscrn, 55:LALT, -1:RARROW, -3:LARROW
// blank strings are keys im not implementing 
char *keys[num_keys] = {"", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACKSPACE", " ", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "ENTER", "", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", "", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "", "", "", " ", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "LARROW", "", "RARROW"};

// process keyboard input 
void keypress(unsigned char input) {
	int idx = ((int) input) - 1;  // scancodes are conveniently in order (except for 0th scancode)
	char *key;

	if (idx < num_keys) {
		key = keys[idx];

		// TODO actually handle stuff
		if (str_comp(key, "BACKSPACE")) {
			print_str("special\n", WHITE);
		} else if (str_comp(key, "ENTER")) {
			print_str("special\n", WHITE);
		} else if (str_comp(key, "LARROW")) {
			print_str("special\n", WHITE);
		} else if (str_comp(key, "RARROW")) {
			print_str("special\n", WHITE);
		} else if (str_comp(key, "")) {
			print_str("other guy\n", WHITE);
		} else {
			print_str(key, WHITE);
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
