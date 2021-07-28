#define KEYBOARD_ADDR 0x60  
#define NUM_KEYS 77
#define MAX_BUFFER_SIZE 1000

void append_buffer(char val);
void clear_buffer(void);
int keypress(unsigned char input);
void repl(void);
