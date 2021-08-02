# Little Kernel
A simple kernel image that boots into a terminal and echoes input. It may be a bit of a stretch to call this a kernel, but I am anyways.

The purpose of completing this project (is any project ever really complete?) was to learn about how software interacts with physical hardware, and I sort of figured some of that out. Sort of. Abstraction upon abstraction, man. 
![The running kernel](/imgs/running.PNG)

## Building
To build the image and rm uneeded files:

'''
make
make clean
'''

This creates an image file called little-kernel.iso.

## Booting
I booted this image in VirtualBox. It may be bootable on other platforms, but I have not tested any others (nor do I want to, for that matter).

In VirtualBox, I created a new machine with these settings: 
![VirtualBox settings](/imgs/vbox_settings.PNG)

## References
* https://www.linuxjournal.com/content/what-does-it-take-make-kernel-0
* https://dev.to/frosnerd/series/9585
* https://wiki.osdev.org/Text_Mode_Cursor#Moving_the_Cursor
