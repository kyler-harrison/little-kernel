#define VGA_ADDR 0xB8000  // video memory starts here
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE 15

unsigned short *terminal_buff;
unsigned int vga_idx;
unsigned int new_line = 80;  // how much to add to vga_idx to get to next line

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
		if (str[idx] != '\n') {
			terminal_buff[vga_idx] = (unsigned short) str[idx] | (unsigned short) color << 8;
			vga_idx++;
			// TODO add check for line wrapping
		} else {
			vga_idx = (vga_idx / new_line + 1) * new_line;  // get to start idx of current line, get to start idx of next
		}

		idx++;
	}
} 

void main(void) {
	terminal_buff = (unsigned short *) VGA_ADDR;
	vga_idx = 0;
	clear_screen();

	print_str("welcome \nto screen\n", GREEN);
	print_str("fare\nwell", RED);

	return;
}
