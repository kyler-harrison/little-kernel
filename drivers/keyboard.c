#include "keyboard.h"
#include "display.h"
#include "ports.h"

#define num_keys 88  // TODO move to .h

// hex keyboard scancodes: https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
// space either means space or tab 
// special key indexes: 0:err, 1:esc, 12:backspace, 26:enter, 27: Lctrl, 42:Lshift, 54:Rshift, 55:prntscrn, 56:LALT
char *keys[num_keys] = {"ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACKSPACE", " ", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "ENTER", "LCTRL", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", "LSHIFT", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "RSHIFT", "PRNTSCRN", "LALT", " "};

// print input back if valid 
void keypress(unsigned char input) {
	int idx = ((int) input) - 1;

	if (idx < num_keys) {
		// TODO catch idx and handle LSHIFT/RSHIFT, ENTER, BACKSPACE
		print_str(keys[idx], WHITE);
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
