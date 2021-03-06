#******************************************************************************
#    Noureddine Ait Said
#    TIMA Laboratory
#    nouredddine.aitsaid0@gmail.com
#
#
#	Parameters are: PLATFORM := defaults to LOWRISCV
#					VERBOSE :=  defaults to 0
#
#------------------------------------------------------------------------------
SRC_DIR = ./src/
BUILD_DIR = .

SOURCES = $(SRC_DIR)entry.S \
			$(SRC_DIR)main.c \
			$(SRC_DIR)plic.c \
			$(SRC_DIR)interrupt.c \
			$(SRC_DIR)syscalls.c 
		#	$(SRC_DIR)pthread_lib.c 
		#	\
		#	$(SRC_DIR)pthread_barrier.c

INCLUDES = -I./include

#include sources.mk
##################################################################

TARGET = $(BUILD_DIR)/baremetal-example

LINKER = test.ld
LOG_FILE = pthread_log.txt

ISA ?= rv64imafdc
#ISA ?= rv32imac
#ABI ?= ilp32
ABI ?= lp64
N_PROC ?= 4
#SPIKE_SIMULATION ?= -DSPIKE_SIMULATION

ifndef RISCV
$(error "[ ERROR ] - RISCV variable not set!")
endif


CC = $(RISCV)/bin/riscv64-unknown-elf-gcc
#CC = $(RISCV)/bin/riscv32-unknown-elf-gcc
LD = $(RISCV)/bin/riscv64-unknown-elf-ld
#LD = $(RISCV)/bin/riscv32-unknown-elf-ld
DUMPER = $(RISCV)/bin/riscv64-unknown-elf-objdump
#DUMPER = $(RISCV)/bin/riscv32-unknown-elf-objdump
SIZE = $(RISCV)/bin/riscv64-unknown-elf-size
#SIZE = $(RISCV)/bin/riscv32-unknown-elf-size
SPIKE = $(RISCV)/bin/spike


CFLAGS = -O1 -march=$(ISA) -mabi=$(ABI) -mcmodel=medany -ffreestanding -static -nostdlib -nostartfiles -lgcc $(INCLUDES) 
LDFLAGS = -T $(LINKER)



OBJS = $(SOURCES:.c=.o)
DEPS = $(SOURCES:.c=.d)

%.i : %.c
	#------>>  Preprocessing
	$(CC) -E $(CFLAGS) $(INCLUDES) $(SPIKE_SIMULATION) $< -o $@

%.S : %.c
	#------>>  Generating Assembly files
	$(CC) -S $(CFLAGS) $(INCLUDES) $(SPIKE_SIMULATION) $< -o $@

%.o : %.S %.c
	#------>>  Generating object file
	$(CC) -c $(CFLAGS) $(INCLUDES)  $(SPIKE_SIMULATION) $< -o $@
	
%.dump : %.out
	$(DUMPER) -S -D $<


.PHONY: compile-all
compile-all: $(OBJS)

.PHONY: build
build: all

.PHONY: all
all: $(TARGET).out

$(TARGET).out: compile-all
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) $(SPIKE_SIMULATION) -o $@
	$(SIZE) $@

.PHONY: sim
sim:
	@echo "-------------  Starting Spike ISS Simulation  -------------"
	$(SPIKE) -p$(N_PROC) --isa=$(ISA) $(TARGET).out 
	
.PHONY: debug
debug:
	@echo "-------------------  Starting Debugging  -------------------"
	@$(SPIKE) -d -p$(N_PROC) --isa=$(ISA) $(TARGET).out

.PHONY: log
log:
	@echo "-------------------  Starting Debugging  -------------------"
	@$(SPIKE) -p$(N_PROC) --isa=$(ISA) $(TARGET).out 2> $(LOG_FILE)


.PHONY: build-sim
build-sim: $(TARGET).out
	@echo ""
	@echo "-------------  Build done, starting simulation  -------------"
	@$(SPIKE) -p$(N_PROC) --isa=$(ISA)  $(TARGET).out

.PHONY: build-sim-cache
build-sim-cache: $(TARGET).out
#  --hartids=<a,b,...>   Explicitly specify hartids, default is 0,1,...
#  --ic=<S>:<W>:<B>      Instantiate a cache model with S sets,
#  --dc=<S>:<W>:<B>        W ways, and B-byte blocks (with S and
#  --l2=<S>:<W>:<B>        B both powers of 2).
#	DEFAULT lowRISC values
#  --ic=64:4:8      Instantiate a cache model with S sets,
#  --dc=64:4:8        W ways, and B-byte blocks (with S and
#  --l2=256:8:8        B both powers of 2).
	@echo ""
	@echo "-------------  Build done, starting simulation  -------------"
	@$(SPIKE) -p$(N_PROC) --isa=$(ISA) --ic=64:4:8 --dc=64:4:8 --l2=256:8:8 $(TARGET).out

.PHONY: clean
clean:
#	rm -f $(SRC_DIR)*.i $(SRC_DIR)*.s $(SRC_DIR)*.asm $(SRC_DIR)*.o $(SRC_DIR)*.d $(SRC_DIR)*.out $(SRC_DIR)*.map $(TARGET).out
	rm -f $(SRC_DIR)*.i $(SRC_DIR)*.asm $(SRC_DIR)*.o $(SRC_DIR)*.d $(SRC_DIR)*.out $(SRC_DIR)*.map $(TARGET).out
	
