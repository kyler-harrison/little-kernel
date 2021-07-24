#define VGA_ADDR 0xB8000  
#define ROWS 25
#define COLS 80
#define MAX_IDX ROWS * COLS
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE 15

void clear_screen(void);
void vga_newline(void);
void print_str(char *str, unsigned char color);
