CC	=  gcc
CX	=  g++

CFLAGS       = -Wall -g
CXXFLAGS     = -Wall -g
#CXXFLAGS    += -std=c++0x

# flags for boost
CXXFLAGS       += -I boost

# Src files
QP_CXXSRCS   = netlist.cpp 
QP_CXXSRCS  += gate.cpp netpart.cpp solver.cpp
QP_OBJS      = $(QP_CXXSRCS:.cpp=.o);

#tell make that "clean" is not a file name!
.PHONY: clean all qp 

all: qp 

### Build rules for programs
qp              : $(QP_OBJS)
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
