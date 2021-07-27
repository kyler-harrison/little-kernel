void write_port(unsigned short port, unsigned char data) {
	asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned char read_port(unsigned short port) {
	unsigned char data;
	asm("in %%dx, %%al" : "=a" (data) : "d" (port));  
	return data;
}
