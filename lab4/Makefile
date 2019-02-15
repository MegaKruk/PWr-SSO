# compiler 
CC = g++

# flags to pass compiler
CFLAGS = -ggdb -Wall
CFLAGS+ = -g

# name for executable
EXE = lab4 

# space-separated list of header files
HDRS = 

# space-separated list of libraries
LIBS = -lrt 

# space-separated list of source files
SRCS = lab4.c 

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)


# default target
$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies 
$(OBJS): $(HDRS) Makefile

# cleaning
clean:
	rm -f core $(EXE) *.o
