# Bort2
The bort2 project is motor driver software for an embedded microcontroller. It has two components: a set of generated ST code from the STMCubeMX software and a motorlib folder. The ST generated code is convenient for pin configuration and peripheral initialization but its library functions are a bit heavy to run during the application. So at the end of the ST generated main file is a call into motorlib. And the ST generated interrupts are also replaced by calls to motorlib. Motorlib has both generic motor control functions as well as code specific to the ST peripherals. Special care is taken to allow STMCubeMX to be able to regenerate code in place without affecting the combined program. Rather than supporting configurable peripherals at runtime the goal here is to keep the executable simplier and utilize the STMCubeMX software for that functionality and thus recompile for different hardware configurations. An example is configuring to communicate with a specific SPI encoder. Bit rate, polarity and phase are all set up from STMCubeMX. Then that encoder could be linked to the motor encoder or output encoder used in motorlib either through the SPIEncoder class or be creating a new class derived from Encoder depending on the specifics of communication. 

Links to all relevant software downloads are provided at the end of this document.

## Build
The current build system is gcc due to it being supported by STMCubeMX. The specific setup I've been using that works is the build of the gcc compiler available on the ARM website. This complier will have a prefix of arm-none-eabi, so for example arm-none-eabi-gcc should be on your path. Run make to build. Then programming can be accomplished either through the built in bootloader or through a debugger connection, both described below.

## Motorlib
Motorlib assumes a specific actuator, sensor, and communication setup that has been found to be common in robotics. And it sets up control loops that have been found to work well for this scenario. This is a recommended starting point for control of robotic system, however one solution can not solve every problem. The recommended approach is to branch and modify motorlib for your situation but to maintain a link back to the main repository such that upgrades are available. And also to push non specific code like sensor drivers and control algorithms to the main repository.

Actuator block diagram.

Actuator control diagram.

Software diagram.

### Communication
The assumed usage case for this software is multiple motor drivers that are connected to a central control host. The currently recommended communication system is USB to a linux host and there are custom drivers, utilities, and example code for this in the repositories usb_rt_driver and realtime-tmp.

### Programming
The firmware can be upgraded through USB in Device Firmware Upgrade (DFU) mode. DFU mode is built into the ST bootloader. For an unprogrammed or board with corrupted firmware the ST bootloader can be accessed with a combination of external pin settings. For an already programmed board you can switch to DFU mode at runtime by sending a DFU_DETACH packet over USB. This and DFU programming are available through the dfu-util software package. The output of the make process described in build is a bin file. This can be programmed with the command:
```console
> dfu-util -a0 -s 0x8000000 -D build/bort2.bin
```

### Parameters
Various motor control parameters that may be specific to the application are placed in a separate flash block that can be reprogrammed independently of the main firmware. The format is the struct Param.

### Tuning and debugging
With the gcc build system described above I also use Visual Studio Code and the Cortex Debug extension. In addition I prefer the SEGGER JLink debugger as it has a low cost edu mini version that is full featured but with a non commercial license. There is an install file for SEGGER JLink that must be installed as well as having the debugger. The debugger connection allows for fast programming and source debugging through Visual Studio Code. In addition I have also found it very handy to use arm-none-eabi-gdb directly, which essentially provides a command line interface to the internals of the software while running with no implementation effort or runtime overhead. To use the command line gdb version separate from Visual Studio Code and Cortex Debug you can run the SEGGER GDBServer executable separately, then run gdb with an example below.
```console
> arm-none-eabi-gdb build/bort2.elf
(gdb) monitor remote :2331
(gdb) load
(gdb) monitor reset
(gdb) monitor go
(gdb) print working_param.main_loop_param
(gdb) set working_param.main_loop_param.controller_param.kp = 2
```
One caveat to the debugging through gdb is that it is not available when running with link time optimization (LTO) turned on. LTO should be disabled through the Makefile, in order to do the above debugging. However LTO turned off will adversely effect control loop timing.


## Software downloads
Software downloads as of 12/2019:
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
- [gcc-arm](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
  - Unzip and place on path. E.g. `sudo chown $USER:$USER /opt; tar -C /opt xf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2; echo "export PATH = $PATH:/opt/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux >> ~/.bashrc`
- [Visual Studio Code](https://code.visualstudio.com/download)
  - Get the Cortex Debug extension in Visual Studio Code, View menu&rarr;Extensions
- [SEGGER J-Link Software](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)