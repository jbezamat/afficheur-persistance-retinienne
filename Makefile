CXXFLAGS=-Wall -Wextra

.PHONY: all 
all: main



main:
	#avr-gcc -g -Os -mmcu=atmega128 USART.c -o usart.elf
	avr-gcc -g -Os -mmcu=atmega128 main.c -o main.elf
	
	avr-objcopy -O binary main.elf main.bin
	avrdude -p m128 -U main.bin -b 115200 -c jtagmkI -P /dev/ttyUSB0 -v

.PHONY: clean
clean:
	rm *.elf *.bin

