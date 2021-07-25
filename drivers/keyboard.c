#include "keyboard.h"
#include "display.h"

void repl(void) {
	unsigned char input = 0x0;

	while (1) {
		// read value (byte) at keyboard addr into input variable
		asm("in %%dx, %%al" : "=a" (input) : "d" (KEYBOARD_ADDR));  

		if (input == 0x1C) {
			print_str("ENTER\n", RED);
			print_str("afterthought\n", RED);
			//print_str("BREAKING", RED);
			input = 0x0;
			asm("out %%al, %%dx" : : "a" (input), "d" (KEYBOARD_ADDR));
		}
	}
}
