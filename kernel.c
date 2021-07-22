#define VGA_ADDR 0xB8000  // video memory starts here
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE 15

unsigned short *terminal_buff;
unsigned int vga_idx;
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

// char in output defined by 2 bytes: str byte and color byte
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
	vga_idx = 0;
	clear_screen();

	print_str("welcome \nto screen\nmore chars and chars and chars and is that eighty yet i dont know i think\nthis will just wrap anyways", GREEN);
	print_str("fare\nwell", RED);

	return;
}
