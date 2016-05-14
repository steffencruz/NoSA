#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CC = g++
CFLAGS  = -g -Wall

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention

TARGET = NoSA
all: $(TARGET)

# To create the executable file count we need the object files
# ConnectedSystem.o and main..o
NoSA:  ConnectedSystem.o main.o
	$(CC) $(CFLAGS) -o $(TARGET) ConnectedSystem.o main.o

# To create the object file ConnectedSystem.o, we need the source
# files ConnectedSystem.cxx and ConnectedSystem.h
ConnectedSystem.o:  ConnectedSystem.h ConnectedSystem.cxx
	$(CC) $(CFLAGS) -c ConnectedSystem.cxx

# To create the object file main.o, we need the source files
# main.cxx and ConnectedSystem.h
main.o:  main.cxx ConnectedSystem.h 
	$(CC) $(CFLAGS) -c main.cxx


# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) $(TARGET) *.o *~  
