//cause standard PCI parallel port card to go to EPP mode
#include <sys/io.h>
#include <unistd.h>
#include <stdio.h>

// modified from a program from Jon Elson, can put parallel port
// on command line, or will default to 0x378
// John A. Stewart, Jan 2013

int main(int argc, char *argv[]) {
        int base_addr, extctrl_addr;
        if (argc > 1) {
                if (1!=sscanf(argv[1],"%x",&base_addr)) {
                        printf ("issue - have params, but expected number\n");
                        exit(1);
                }
        } else {
                base_addr = 0x378;
        }

        iopl(3) ;//turn on access to all I/O
        extctrl_addr = base_addr + 0x402;
        printf ("current byte %x\n",inb(extctrl_addr));
        outb(0x80,extctrl_addr);  // set for EPP mode
        printf("wrote 0x80 to 0x%x\n",extctrl_addr);
        return 0;
}

