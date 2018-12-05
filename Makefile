program: main.o USART.o
	avr-gcc -o program main.o USART.o

main.o: main.c USART.h
	avr-gcc -g -Os -mmcu=atmega128 main.c -o main.elf

	avr-objcopy -O binary main.elf main.bin
	avrdude -p m128 -U main.bin -b 115200 -c jtagmkI -P /dev/ttyUSB0 -v

USART.o USART.c USART.h
	avr-gcc -c USART.c

