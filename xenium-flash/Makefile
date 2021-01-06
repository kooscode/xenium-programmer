APP_FLASH=xenium-flash
APP_JTAG=xenium-jtag
# PLATFORM=PI
PLATFORM=OMEGA2
SRC_MAIN=src
OUT_DIR=bin

#extra paths
FLASH=$(SRC_MAIN)/flash
JTAG=$(SRC_MAIN)/jtag
OMEGA=$(SRC_MAIN)/omega2-gpio

CXX=g++
#G++ options
# To Make Debug Build set: -g
# To Debug with GDB run: gdb -ex=r --args APP ARG1 ARG2
CFLAGS=-Wall -ansi
CPP_STD=c++17

#include paths
I1=$(SRC_MAIN)
I2=$(FLASH)
I3=$(JTAG)
ifeq ($(PLATFORM),OMEGA2)
I4=$(OMEGA)
endif

#sources files
S1=$(FLASH)/BitBus.cpp
S2=$(FLASH)/Flash.cpp
S3=$(JTAG)/IOBase.cpp
S4=$(JTAG)/Jtag.cpp
S5=$(JTAG)/JedecFile.cpp
S6=$(JTAG)/ProgAlgXC95X.cpp

ifeq ($(PLATFORM),OMEGA2)
S7=$(OMEGA)/Module.cpp
S8=$(OMEGA)/FastGpioOmega2.cpp
S9=$(JTAG)/IOOmega2.cpp
S10=$(FLASH)/BitBusOmega2.cpp
else
S7=$(JTAG)/IOWiringPi.cpp
S8=$(FLASH)/BitBusPI.cpp
endif

#lib paths
LP1=
LP2=
LP3=
LP4=

#linked libs
L1=
L2=
L3=
L4=
ifeq ($(PLATFORM),OMEGA2)
L5=
else
L5=wiringPi 
endif

#pkg-configs
PKG1=
PKG2=
PKG3= 
PKG4= 

#defines
D1=$(PLATFORM)
D2=
D3=
D4=

#expand all options
INC=$(I1) $(I2) $(I3) $(I4)
INCLUDES=$(foreach d, $(INC), -I$d)
SOURCES=$(S1) $(S2) $(S3) $(S4) $(S5) $(S6) $(S7) $(S8) $(S9) $(10)
LIB_PATH=$(LP1) $(LP2) $(LP3) $(LP4)
LIB_PATHS=$(foreach d, $(LIB_PATH), -L$d)
LIB=$(L1) $(L2) $(L3) $(L4) $(L5)
LIBS=$(foreach d, $(LIB), -l$d)
PKGS=$(PKG1) $(PKG2) $(PKG3) $(PKG4)
ifneq ($(strip $(PKGS)),)
PKG_INCS=$(shell pkg-config --cflags $(PKGS))
PKG_LIBS=$(shell pkg-config --libs $(PKGS))
endif
DEF=$(D1) $(D2) $(D3) $(D4)
DEFS=$(foreach d, $(DEF), -D$d)

#compile
MAKE_FLASH = $(CXX) $(CFLAGS) -std=$(CPP_STD) $(DEFS) $(SRC_MAIN)/$(APP_FLASH).cpp $(SOURCES) -o $(OUT_DIR)/$(APP_FLASH) $(INCLUDES) $(PKG_INCS) $(LIB_PATHS) $(LIBS) $(PKG_LIBS)
MAKE_JTAG = $(CXX) $(CFLAGS) -std=$(CPP_STD) $(DEFS) $(SRC_MAIN)/$(APP_JTAG).cpp $(SOURCES) -o $(OUT_DIR)/$(APP_JTAG) $(INCLUDES) $(PKG_INCS) $(LIB_PATHS) $(LIBS) $(PKG_LIBS)

all:
	test -d bin || mkdir -p bin
	$(MAKE_JTAG)
	$(MAKE_FLASH)

$(APP_FLASH): $(SRC_MAIN)/$(APP_FLASH).cpp 
	test -d bin || mkdir -p bin
	$(MAKE_FLASH)
	
$(APP_JTAG): $(SRC_MAIN)/$(APP_JTAG).cpp 
	test -d bin || mkdir -p bin
	$(MAKE_JTAG)
	
clean:
	rm bin -fr
	
