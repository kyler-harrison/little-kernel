#define VGA_ADDR 0xB8000  // video memory starts here
#define KEYBOARD_ADDR 0x60  // is this the key press buffer?
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE 15

unsigned short *terminal_buff;  // vga terminal display
unsigned int vga_idx;  // track location in terminal
unsigned int newline = 80;  // how many characters fit in one line of screen 
unsigned short *keyboard_buff;

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
	if (idx % newline > 0 && idx != 0) {
		vga_newline();
	}
} 

void main(void) {
	terminal_buff = (unsigned short *) VGA_ADDR;
	//keyboard_buff = (unsigned short *) KEYBOARD_ADDR;

	vga_idx = 0;
	clear_screen();

	//char c = (char) keyboard_buff[0] + 48;
	//key[0] = c;
	//
	
	print_str("welcome to screen\n", GREEN);
	//char *key_buff = (char *) 0x60;

	unsigned char first_result;
	unsigned char result;
	asm("in %%dx, %%al" : "=a" (first_result) : "d" (0x60));
	char *c;
	//c[0] = first_result;
	//print_str(c, RED);

	while (1) {
		asm("in %%dx, %%al" : "=a" (result) : "d" (0x60));

		if (result != first_result) {
			c[0] = result;
			print_str(c, RED);
			first_result = result;
		}

		/*
		if (key_buff[0] == 0x1) {
			print_str("success\n", RED);
		}
		*/
	}

	return;
}
