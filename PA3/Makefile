CC	=  gcc
CX	=  g++

CFLAGS       = -Wall -g
CXXFLAGS     = -Wall -g
#CXXFLAGS    += -std=c++0x

# flags for boost
CXXFLAGS       += -I boost

# Src files
QP_CXXSRCS   = netlist.cpp netpart.cpp solver.cpp
QP_OBJS      = $(QP_CXXSRCS:.cc=.o);

#tell make that "clean" is not a file name!
.PHONY: clean all 3qp 

all: 3qp 

### Build rules for programs
3qp             : $(QP_OBJS)
	            $(CXX) -g -o $@ $^ $(LDFLAGS)

# C compilation implicit rule 
%.o             : %.c 
	            $(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

# C++ compilation implicit rule
%.o             : %.cc
	            $(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@



#Clean the build directory
clean: 
	rm -f *.o *~
