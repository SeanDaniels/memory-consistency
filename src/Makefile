##
# Project Title
#
# @file
# @version 0.1
CC = gcc
CCFLAGS = -g -Wall
DEPS = main.h
OBJ = main.o

%.o: %.cpp $(DEPS)
		$(CC) $(CFLAGS) -c -o $@ $<

sim_program2: $(OBJ)
		$(CC) $(CCFLAGS) -o $@ $^

# end
# $(OBJ):
# CC = gcc
# CFLAGS = -Wall
# DEPS = primes.h
# OBJ = go.o primes.o

# %.o: %.c $(DEPS)
#         $(CC) $(CFLAGS) -c -o $@ $<

# go: $(OBJ)
#         gcc $(CFLAGS) -o $@ $^
