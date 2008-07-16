CC = gcc-4.3.1

CCFLAGS = -Wdisabled-optimization -Wno-pointer-sign -Wdeclaration-after-statement -std=gnu99 -I. -Wall -Wno-switch -Wpointer-arith -Wredundant-decls -O4 -ffast-math -fomit-frame-pointer -DPTW32_STATIC_LIB 

EXTRALIBS = -mwindows -s

OBJS = mloader.o mloader-rc.o

LIBS =

TARGET = mloader.exe

$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS) $(EXTRALIBS)

all:	$(TARGET)

%-rc.o: %.rc
	WINDRES -I. $< $@

clean:
	rm -f $(OBJS) $(TARGET)
