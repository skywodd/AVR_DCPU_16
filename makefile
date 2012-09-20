# AVR mcu target, should not be changed 
TARGET = atmega328p

# System clock frequency
F_CPU = 16000000

# Avrdude serial port for upload
SERIAL_PORT = COM7

# Avrdude serial baudrate for upload
SERIAL_SPEED = 115200
#57600

# Source files to compile
SOURCES = button.c buzzer.c dcpu.c debug.c led.c main.c microvga.c ram.c rom.c spi.c uart.c

# Output project name
OUTPUT = dcpu

# Bootloader hex file location
BOOTLOADER_HEX = bootloader_hex/optiboot_atmega328.hex

# Avrdude programmer for bootloading
AVRDUDE_BOOTLOAD_PROGRAMMER = usbtiny

# Avrdude programmer for uploading
AVRDUDE_UPLOAD_PROGRAMMER = arduino

# Compilation switch
EXTRA_DEFINE = -D__EXTRA_DEFINE__
#EXTRA_DEFINE += -DSERIAL_DEBUG_SUPPORT

EXTRA_DEFINE += -DKEYBOARD_INPUT_SUPPORT
EXTRA_DEFINE += -DUART_FLOW_CONTROL_SUPPORT
EXTRA_DEFINE += -DBUTTON_STATE_SUPPORT
#EXTRA_DEFINE += -DO_REGISTER_SUPPORT
EXTRA_DEFINE += -DBLINKING_BIT_SUPPORT
EXTRA_DEFINE += -DFOREGROUND_COLOR_SUPPORT
EXTRA_DEFINE += -DBACKGROUND_COLOR_SUPPORT
EXTRA_DEFINE += -DLED_STATE_SUPPORT
EXTRA_DEFINE += -DBUZZER_BEEP_SUPPORT
#EXTRA_DEFINE += -DHARDWARE_SPI_SS_SUPPORT
EXTRA_DEFINE += -DONBOARD_MEMORY

# Compilator switch
CCFLAGS = -Wall -Os -ffunction-sections
LDFLAGS = -Wl,-gc-sections

# ----- Nothing to change after this line
OBJECTS =  $(SOURCES:.c=.o)
COMPILER = avr-gcc -mmcu=$(TARGET) -std=c99 -DF_CPU=$(F_CPU)UL
OBJCOPY = avr-objcopy -O ihex
OBJSIZE = avr-size -C --mcu=$(TARGET)
AVRDUDE_UPLOAD = avrdude -p $(TARGET) -c $(AVRDUDE_UPLOAD_PROGRAMMER) -P $(SERIAL_PORT) -b $(SERIAL_SPEED)
AVRDUDE_ICSP = avrdude -p $(TARGET) -c $(AVRDUDE_BOOTLOAD_PROGRAMMER)

all: elf hex size

%.o: %.c
	$(COMPILER) -c $< -o $@ $(CCFLAGS) $(EXTRA_DEFINE)
	
elf: $(OBJECTS)
	$(COMPILER) -o $(OUTPUT).elf $(LDFLAGS) $(EXTRA_DEFINE) $^
	
hex: $(OUTPUT).elf
	$(OBJCOPY) $(OUTPUT).elf $(OUTPUT).hex
	
size: $(OUTPUT).elf
	$(OBJSIZE) $(OUTPUT).elf

flash: $(OUTPUT).hex
	$(AVRDUDE_UPLOAD) -U flash:w:$(OUTPUT).hex

lock:
	$(AVRDUDE_ICSP) -e -U lock:w:0x0f:m
	
fuses:
	$(AVRDUDE_ICSP) -U lfuse:w:0xff:m -U hfuse:w:0xde:m -U efuse:w:0x06:m  -U lock:w:0x0f:m
	
bootloader:
	$(AVRDUDE_ICSP) -U flash:w:$(BOOTLOADER_HEX)
	
clean:
	rm -f *.o *.elf *.hex