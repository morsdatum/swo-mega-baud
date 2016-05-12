
all: clean build

clean:
	scons -c -j 2

build:
	scons -j 3

openocd:
	cd cortex && sudo /opt/arm-openocd-0.10dev200/bin/openocd -f board/stm32f4discovery.cfg

swo:
	cd cortex && sudo /opt/arm-openocd-0.10dev200/bin/openocd -f board/stm32f4discovery-swo.cfg

mspdebug:
	cd msp430 && /opt/msp430-gcc-linux-4.0.1/bin/mspdebug rf2500 "prog tx-main.elf" gdb

.PHONY: all clean build openocd mspdebug swo
