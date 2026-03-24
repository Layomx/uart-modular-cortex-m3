CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m3 -mthumb \
         -nostdlib -nostartfiles \
         -Wall -Wextra \
         -g -O0 \
         -ffunction-sections -fdata-sections \
         -I./src -I./drivers

LDFLAGS = -T linker.ld \
          -Wl,--gc-sections \
          -Wl,--print-memory-usage

SRCS = $(wildcard src/*.c) $(wildcard drivers/*.c)
OBJS = $(patsubst src/%.c, build/%.o, $(wildcard src/*.c)) \
       $(patsubst drivers/%.c, build/%.o, $(wildcard drivers/*.c))
TARGET = build/firmware

all: $(TARGET).elf $(TARGET).bin
	@echo "Build complete."
	@$(SIZE) $(TARGET).elf

$(TARGET).elf: $(OBJS) linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: drivers/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	@mkdir -p build

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

.PHONY: all clean run debug size

clean:
	@rm -rf build

run: $(TARGET).elf
	qemu-system-arm \
		-machine mps2-an385 \
		-cpu cortex-m3 \
		-nographic \
		-kernel $(TARGET).elf

debug: $(TARGET).elf
	qemu-system-arm \
		-machine mps2-an385 \
		-cpu cortex-m3 \
		-nographic \
		-kernel $(TARGET).elf \
		-S -gdb tcp::3333

size: $(TARGET).elf
	@$(SIZE) $(TARGET).elf
