CC := gcc
CFLAGS := -Wall -O3
Libs = -lpthread
Target := client
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c,%.o,$(Source))
Modules += check_putin pack_message main
AllObjs := $(addsuffix /src/*.o,$(Modules))

