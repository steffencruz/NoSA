#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CC = g++
#CFLAGS  = -g -Wall

# flags configured by CMake
ifeq (macos,macos)
  LIB_FLAGS = -larmadillo -framework Accelerate
else
  LIB_FLAGS = -larmadillo
endif

## As the Armadillo library uses recursive templates, compilation times depend on the level of optimisation:
##
## -O0: quick compilation, but the resulting program will be slow
## -O1: good trade-off between compilation time and execution speed
## -O2: produces programs which have almost all possible speedups, but compilation takes longer
## -O3: enables auto vectorisation when using gcc
OPT = -O2

#EXTRA_OPT = -fwhole-program
## Uncomment the above line if you're compiling all source files into one program in a single hit

#DEBUG = -DARMA_EXTRA_DEBUG
## Uncomment the above line to enable low-level debugging.
## Lots of debugging information will be printed when a compiled program is run.
## Please enable this option when reporting bugs.

#FINAL = -DARMA_NO_DEBUG
## Uncomment the above line to disable Armadillo's checks.
## Not recommended unless your code has been first thoroughly tested!

CXXFLAGS = $(DEBUG) $(FINAL) $(OPT) $(EXTRA_OPT)

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
TARGET = NoSA
all: $(TARGET)

# To create the executable file count we need the object files
# ConnectedSystem.o and main..o
$(TARGET):  ConnectedSystem.o main.o
	$(CC) $(CXXFLAGS) -o $(TARGET) ConnectedSystem.o main.o $(LIB_FLAGS)

# To create the object file ConnectedSystem.o, we need the source
# files ConnectedSystem.cxx and ConnectedSystem.h
ConnectedSystem.o:  ConnectedSystem.h ConnectedSystem.cxx
	$(CC) $(CXXFLAGS) -c ConnectedSystem.cxx 

# To create the object file main.o, we need the source files
# main.cxx and ConnectedSystem.h
main.o:  main.cxx ConnectedSystem.h 
	$(CC) $(CXXFLAGS) -c main.cxx


# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) $(TARGET) *.o *~  
