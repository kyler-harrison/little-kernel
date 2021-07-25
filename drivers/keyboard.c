#include "keyboard.h"
#include "display.h"
#include "ports.h"

// print input back if valid 
void keypress(unsigned char input) {
	switch (input) {
		case 0x1C :
			print_str("VALID ENTER\n", RED);
			break;
		default:
			break;
	}
}

void repl(void) {
	unsigned char input = 0x0;

	while (1) {
		// read value (byte) at keyboard addr into input variable
		input = read_port(KEYBOARD_ADDR);

		if (input == 0x1C) {
			keypress(input);
			input = 0x0;
			write_port(KEYBOARD_ADDR, input);
		}
	}
}
