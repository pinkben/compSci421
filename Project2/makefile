CC = gcc
CFLAGS = -Wall -g #Enable all compiler warnings, native OS debug info
OBJECTS = shell.o shellHistory.o shellCommandList.o

shell : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $?

shellHistory.o : shellHistory.h
shellCommandList.o : shellCommandList.h

.PHONY : clean #Ensures no file named "clean" to break recipe
clean:
	rm -f shell $(OBJECTS)