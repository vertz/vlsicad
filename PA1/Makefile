CC	     =  gcc
CXX      =  g++

CFLAGS       = -Wall -g
CXXFLAGS     = -Wall -g

# flags for boost
CXXFLAGS       += -I boost

# Src files for the urpcomp
URPCOMP_CXXSRCS  = cubelist.cc cube.cc
URPCOMP_OBJS     = $(URPCOMP_CXXSRCS:.cc=.o);

### Build rules for programs
urp             : $(URPCOMP_OBJS)
	            $(CXX) -g -o $@ $^ $(LDFLAGS)

# C compilation implicit rule 
%.o             : %.c 
	            $(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

# C++ compilation implicit rule
%.o             : %.cc
	            $(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o *~


