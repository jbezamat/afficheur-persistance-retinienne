F_CPU=13000000
COMPIL_OPT=-g -mmcu=atmega128
PORT=/dev/ttyUSB0

ALL=main
DEP= main.o SPI.o USART.o hall_sensor.o curved.o interface.o

all: $(ALL)

main: main.elf
	avr-objcopy -O binary main.elf main.bin

main.elf: $(DEP)
	avr-gcc --std=c99 $(COMPIL_OPT) -o $@ $^

%.o: %.c
	avr-gcc --std=c99 $(COMPIL_OPT) -Os -c -DF_CPU=$(F_CPU) -o $@ $<

clean:
	rm -rf *.o
	rm -rf *.elf

install: main
	avrdude -b 115200 -p m128 -P $(PORT) -c jtagmkI -U main.bin -v -F

mrproper: clean
	rm -rf test
