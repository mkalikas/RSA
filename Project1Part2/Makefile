# Mention default target.
all:

# Implicit rule to compile C++ files.  Modify to your taste.
%.o: %.cc %.cpp
	g++ -std=c++14

# Components of the library.
library-objects = \
	BigUnsigned.o \
	BigInteger.o \
	BigIntegerAlgorithms.o \
	BigUnsignedInABase.o \
	BigIntegerUtils.o \
	sha256.o \

library-headers = \
	NumberlikeArray.hh \
	BigUnsigned.hh \
	BigInteger.hh \
	BigIntegerAlgorithms.hh \
	BigUnsignedInABase.hh \
	BigIntegerLibrary.hh \
	sha256.h \

# To ``make the library'', make all its objects using the implicit rule.
library: $(library-objects)

# Conservatively assume that all the objects depend on all the headers.
$(library-objects): $(library-headers)

# The rules below build a program that uses the library.  They are preset to
# build ``main'' from ``main.cc''.

# Components of the program.
program = main
program-objects = main.o

# Conservatively assume all the program source files depend on all the library headers. 
$(program-objects) : $(library-headers)

# How to link the program.  The implicit rule covers individual objects.
$(program) : $(program-objects) $(library-objects)
	g++ $^ -o $@

# Delete all generated files we know about.
clean :
	rm -f $(library-objects) $(testsuite-cleanfiles) $(program-objects) $(program)

# Come back and define default target.
all : library $(program)
