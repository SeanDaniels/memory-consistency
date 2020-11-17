##
# Project Title
#
# @file
# @version 0.1
CC = g++
CCFLAGS = -g -Wall
DEPS = main.h
OBJ = main.o
TARGET = sim_program2

%.o: %.cpp $(DEPS)
		$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
		$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm *.o
	rm $(TARGET)
