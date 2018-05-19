#Output Files==================================================================
PROJECT = firmware
EXECUTABLE = $(PROJECT).elf
BIN_IMAGE = $(PROJECT).bin
HEX_IMAGE = $(PROJECT).hex
#Cross Complier================================================================
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

#Complier Flags================================================================
CFLAGS_INC = 

CFLAGS_DEF = 

CFLAGS_OPT = \
	-O2

CFLAGS_WRN = \
	-Wall \
	-Wextra \
	-Wdouble-promotion \
	-Wshadow \
	-Werror=array-bounds \
	-Wfatal-errors \
	-Wmissing-prototypes \
	-Wbad-function-cast  \
	-Wstrict-prototypes \
	-Wmissing-parameter-type

ARCH_FLAGS = \
	-mlittle-endian \
	-mthumb \
	-mcpu=cortex-m4 \
	-mfpu=fpv4-sp-d16 \
	-mfloat-abi=hard

CFLAGS = \
	-g \
	${CFLAGS_INC} \
	${CFLAGS_DEF} \
	${CFLAGS_OPT} \
	${CFLAGS_WRN} \
	${ARCH_FLAGS}

#Linker Flags==================================================================
CFLAGS_NEW_LIB_NANO= \
	--specs=nano.specs \
	--specs=nosys.specs \
	-u _printf_float

LDFLAGS += $(CFLAGS_NEW_LIB_NANO) --static -Wl,--gc-sections
LDFLAGS += -T ./linkerscript.ld
LDLIBS += -Wl,--start-group -lm -Wl,--end-group

#Compile Files=================================================================
PLATFORM = ./Platform

PLATFORM_FILE = \
	${PLATFORM}/stm32f4_sys.o \
	${PLATFORM}/stm32f4_rcc.o \
	${PLATFORM}/stm32f4_gpio.o \
	${PLATFORM}/stm32f4_usart.o

BOOT = ./Boot

BOOT_FILE = \
	${BOOT}/startup.o \
	${BOOT}/init.o \
	${BOOT}/peripherals.o \
	${BOOT}/boot.o\
	${BOOT}/print.o

OBJS =	${BOOT_FILE} \
		${PLATFORM_FILE} \
		./main.o

#Rule==========================================================================
$(BIN_IMAGE):$(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo 'OBJCOPY $(BIN_IMAGE)'

$(EXECUTABLE): $(OBJS)
	@$(LD) $(LDFLAGS) $(ARCH_FLAGS) $(OBJS) $(LDLIBS) -o $@ 
	@echo 'LD $(EXECUTABLE)'

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo 'CC $<'

%.o: %.s
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo 'CC $<'

#Command=======================================================================
all:
	$(BIN_IMAGE)

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
	rm -rf $(OBJS)

flash_openocd:
	openocd -f interface/stlink-v2.cfg \
	-c "transport select hla_swd" \
	-f target/stm32f4x_stlink.cfg \
	-c "init" \
	-c "reset init" \
	-c "halt" \
	-c "flash write_image erase $(PROJECT).elf" \
	-c "verify_image $(PROJECT).elf" \
	-c "reset run" -c shutdown

#PHONY=========================================================================
.PHONY:all clean flash_openocd