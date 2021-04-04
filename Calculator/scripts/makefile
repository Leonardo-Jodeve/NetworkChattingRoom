CC := gcc
CFLAGS := -Wall -O3
Target := Calculator
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c,%.o,$(Source))
Modules += module main
AllObjs := $(addsuffix /src/*.o,$(Modules))

#Libs = -lpthread