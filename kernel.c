#define VGA_ADDR 0xB8000  // video memory starts here
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE 15

unsigned short *terminal_buff;
unsigned int vga_idx;

void clear_screen(void) {
	int idx = 0;
	// 25 lines of 80 columns (ascii chars), each elem is 2 bytes

	while (idx < 80 * 25 * 2) {
		terminal_buff[idx] = ' ';
		idx += 2;
	}
}

// char in output defined by 2 bytes: str byte and color byte
void print_str(char *str, unsigned char color) {
	int idx = 0;

	while (str[idx]) {
		// this makes a 2 byte (16 bit input) where the first 8 bits represent the color
		// and the second 8 bits represent the character
		terminal_buff[vga_idx] = (unsigned short) str[idx] | (unsigned short) color << 8;
		idx++;
		vga_idx++;
	}
} 

void main(void) {
	terminal_buff = (unsigned short *) VGA_ADDR;
	vga_idx = 0;

	clear_screen();
	print_str("welcome to screen", GREEN);
	vga_idx = 80;  // go to new line
	print_str("farewell", RED);

	return;
}
