APP_FLASH=xenium-flash
APP_JTAG=xenium-jtag
SRC_MAIN=src
OUT_DIR=bin

#extras
FLASH=$(SRC_MAIN)/flash
JTAG=$(SRC_MAIN)/jtag

#G++ options (-g = debug)
CXX=g++
CFLAGS=-g -Wall -ansi
CPP_STD=c++17

#extra include paths
I1=$(SRC_MAIN)
I2=$(FLASH)
I3=$(JTAG)
I4=
#include folders
INC=$(I1) $(I2) $(I3) $(I4)
INCLUDES=$(foreach d, $(INC), -I$d)

#additional sources files (add extras if needed)
S1= $(FLASH)/BitBus.cpp
S2= $(FLASH)/Flash.cpp
S3=
S4=$(JTAG)/IOBase.cpp
S5=$(JTAG)/IOWiringPi.cpp
S6=$(JTAG)/Jtag.cpp
S7=$(JTAG)/JedecFile.cpp
S8=$(JTAG)/ProgAlgXC95X.cpp
SOURCES=$(S1) $(S2) $(S3) $(S4) $(S5) $(S6) $(S7) $(S8)

#lib paths (add extras if needed)
LP1=
LP2=
LP3=
LP4=
LIB_PATH=$(LP1) $(LP2) $(LP3) $(LP4)
LIB_PATHS=$(foreach d, $(LIB_PATH), -L$d)

#link libs (add extras if needed)
L1=wiringPi
L2=
L3=
L4=
LIB=$(L1) $(L2) $(L3) $(L4)
LIBS=$(foreach d, $(LIB), -l$d)

#pkg-configs (add extras if needed)
PKG1=
PKG2=
PKG3= 
PKG4= 
PKGS=$(PKG1) $(PKG2) $(PKG3) $(PKG4)
ifneq ($(strip $(PKGS)),)
PKG_INCS=$(shell pkg-config --cflags $(PKGS))
PKG_LIBS=$(shell pkg-config --libs $(PKGS))
endif

#compile (-g = debug mode)

MAKE_FLASH = $(CXX) $(CFLAGS) -std=$(CPP_STD) $(SRC_MAIN)/$(APP_FLASH).cpp $(SOURCES) -o $(OUT_DIR)/$(APP_FLASH) $(INCLUDES) $(PKG_INCS) $(LIB_PATHS) $(LIBS) $(PKG_LIBS)
MAKE_JTAG = $(CXX) $(CFLAGS) -std=$(CPP_STD) $(SRC_MAIN)/$(APP_JTAG).cpp $(SOURCES) -o $(OUT_DIR)/$(APP_JTAG) $(INCLUDES) $(PKG_INCS) $(LIB_PATHS) $(LIBS) $(PKG_LIBS)

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
	
