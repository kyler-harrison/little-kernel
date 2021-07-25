#include "../drivers/display.h"
#include "../drivers/keyboard.h"

void main(void) {
	clear_screen();
	print_str("> ", GREEN);
	repl();

	return;
}
