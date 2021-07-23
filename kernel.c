#define VGA_ADDR 0xB8000  // video memory starts here
#define KEYBOARD_ADDR 0x60  // where keypresses go (is this just a port?)
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE 15

unsigned short *terminal_buff;  // vga terminal display
unsigned int vga_idx;  // track location in terminal
unsigned int newline = 80;  // how many characters fit in one line of screen 

// empty screen
void clear_screen(void) {
	int idx = 0;
	// 25 lines of 80 columns (ascii chars), each elem is 2 bytes

	while (idx < 80 * 25 * 2) {
		terminal_buff[idx] = ' ';
		idx += 2;
	}
}

// move to next line on screen
void vga_newline(void) {
	vga_idx = (vga_idx / newline + 1) * newline;  // get to start idx of current line, get to start idx of next
}

// char in output defined by 2 bytes: color byte and str byte
void print_str(char *str, unsigned char color) {
	int idx = 0;

	while (str[idx]) {
		// this makes a 2 byte (16 bit) input where the first 8 bits represent the color
		// and the second 8 bits represent the character
		if (str[idx] != '\n') {
			terminal_buff[vga_idx] = (unsigned short) str[idx] | (unsigned short) color << 8;
			vga_idx++;

		} else {
			vga_newline();
		}

		idx++;
	}

	// a single str automatically wraps, but a newline may still be needed at the end
	if (idx / newline > 0 && idx != 0 && str[idx - 1] != '\n') {
		vga_newline();
	}
} 

void main(void) {
	terminal_buff = (unsigned short *) VGA_ADDR;
	vga_idx = 0;
	clear_screen();

	print_str("welcome to screen\n", GREEN);
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

	return;
}
