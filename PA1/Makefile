CC	=  gcc
CX	=  g++

CFLAGS       = -Wall -g
CXXFLAGS     = -Wall -g
CXXFLAGS    += -std=c++0x

# flags for boost
CXXFLAGS       += -I boost

# Src files
BASE_CXXSRCS      = cubelist.cc cube.cc

BOOLCALC_CXXSRCS   = bool_calc.cc 
BOOLCALC_CXXSRCS  += $(BASE_CXXSRCS)
BOOLCALC_OBJS      = $(BOOLCALC_CXXSRCS:.cc=.o);

URCCOMP_CXXSRCS    = urc.cc 
URCCOMP_CXXSRCS   += $(BASE_CXXSRCS)
URCCOMP_OBJS       = $(URCCOMP_CXXSRCS:.cc=.o);

#tell make that "clean" is not a file name!
.PHONY: clean all urc bool_calc

all: bool_calc urc

### Build rules for programs
bool_calc       : $(BOOLCALC_OBJS)
	            $(CXX) -g -o $@ $^ $(LDFLAGS)

urc             : $(URCCOMP_OBJS)
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


