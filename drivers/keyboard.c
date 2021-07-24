#include "keyboard.h"
#include "display.h"

void repl(void) {
	unsigned char input;

	while (1) {
		// read value (byte) at keyboard addr into input variable
		asm("in %%dx, %%al" : "=a" (input) : "d" (KEYBOARD_ADDR));  

		if (input == 0x1C) {
			print_str("ENTER\n", RED);
			print_str("BREAKING", RED);
			input = 0x0;
			break;
		}
	}
}
