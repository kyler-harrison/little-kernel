#define KEYBOARD_ADDR 0x60  
#define NUM_KEYS 77
#define MAX_BUFFER_SIZE 100

void update_buffer(char val);
void clear_buffer(void);
void keypress(unsigned char input);
void repl(void);
