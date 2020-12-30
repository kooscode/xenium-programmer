APP=xenium-flash
SRC_MAIN=src
OUT_DIR=bin

#G++ options (-g = debug)
CXX=g++
CFLAGS=-g -Wall -ansi
CPP_STD=c++17

#extra include paths
I1=
I2=
I3=
I4=
#include folders
INC=$(I1) $(I2) $(I3) $(I4)
INCLUDES=$(foreach d, $(INC), -I$d)

#additional sources files (add extras if needed)
S1= $(SRC_MAIN)/BitBus.cpp
S2= $(SRC_MAIN)/Flash.cpp
S3=
S4=
SOURCES=$(S1) $(S2) $(S3) $(S4)

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
$(APP): $(SRC_MAIN)/$(APP).cpp 
	test -d bin || mkdir -p bin
	$(CXX) $(CFLAGS) -std=$(CPP_STD) $(SRC_MAIN)/$(APP).cpp $(SOURCES) -o $(OUT_DIR)/$(APP) $(INCLUDES) $(PKG_INCS) $(LIB_PATHS) $(LIBS) $(PKG_LIBS)

clean:
	rm bin -fr
	
