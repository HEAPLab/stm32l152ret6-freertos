# stm32l152ret6-FreeRTOS
This is a port of FreeRTOS for the STM32L152RET6 board. 

## CONFIGURATION

The ARM toolchain for linux is required and can be downloaded from [this link](https://github.com/ARM-software/LLVM-embedded-toolchain-for-Arm/releases/tag/release-16.0.0). We highly recommend placing the config.cfg file of this repository into the bin directory of the toolchain, together with all the other cfg files.

For the compilation, configure the following parameters in the makefile:
- `ASPIS`: the path to the ASPIS directory
- `CONF`: the path to config.cfg
- `LLVM_BIN`: the path to the bin directory of the built llvm 
- `EXCLUDE_F`: the path to excluded_files.txt

## Compilation
The Makefile for the compilation takes a parameter `SCOPE` that defines the set of tasks that will be used in FreeRTOS as follows:

| SCOPE | Description                   |
|-------|-------------------------------|
| 1     | FreeRTOS with microbenchmarks |
| 2     | DES                           |
| 3     | MatMult                       |
| 4     | Lift                          |
| 5     | CRC                           |
| 6     | SHA                           |

### w/ ASPIS
You can compile FreeRTOS with ASPIS (all 9 combinations) with the following command, by specifying a scope according to the table above:

```bash
make out_all SCOPE=<scope>
```

### w/o ASPIS
The compilation without ASPIS can be done with the following command, specifying the scope as before:


```bash
make nopass SCOPE=<scope>
```

## Running
The OS can be run only on the target board, we did not try hardware emulation/simulation, hence we do not guarantee the success of the execution. 

If you have the board available, flash one of the compilation outputs with the IDE provided by ST (available [here](https://www.st.com/en/development-tools/stm32cubeide.html)). 
