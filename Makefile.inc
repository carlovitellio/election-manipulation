# The directory where the Makefile resides
ROOT=/home/carlo/election-manipulation

## Mpi directories for library and include files
##
#MPI_LIBDIR=${mkOpenmpiLib}
#MPI_INCDIR=${mkOpenmpiInc}

## The location of the multithresing building block library and header files
## necessary for the parallel version of the algorithms.
##
#mkTbbLib?=/usr/lib/x86_64-linux-gnu/
#mkTbbInc?=/usr/include


#
# The C++ compiler (taken from an environmental variable)

mkCxxCompiler?=g++
CXX = ${mkCxxCompiler}


LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)

# Example
# -DNDEBUG disactivates asserts
DEFINES=

#  OPTIMIZATION
# Warning flags: Level of warning
# -Wall main warnings activated
# -Wextra some extra warnings activated
# -Wsuggest-override  as the name says
# -Wnon-virtual-dtor Warns if a class with virtual meths oda does not have virt dstructors
# -Weffc++ Wanrs about violation of the coding style in Scott Mayer's Effective C++

WARNFLAGS=-Wall -Wextra -Wsuggest-override -Wnon-virtual-dtor


# Optimization flags : May be selected by using the variable RELEASE
#
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

ifdef RELEASE
DEBUG=no
else
DEBUG=yes
endif

ifeq ($(DEBUG),yes)
  OPTFLAGS=-O0 -g
else
  OPTFLAGS=-O3 -funroll-loops
  DEFINES+=-DNDEBUG
endif


export STDFLAGS=-std=c++17

export CC
export CXX

export DOXYFILE=$(ROOT)/Doxyfile

# Where the Boost library resides
mkBoostInc=/usr/local/boost_1_77_0
BOOST_DIR=$(mkBoostInc)

INCLS  = -I. -I./include -I$(BOOST_DIR)

#
# Set make macros
#
export CPPFLAGS=$(INCLS) $(DEFINES) $(VERBOSITY)
export CXXFLAGS=$(OPTFLAGS) $(STDFLAGS) $(WARNFLAGS)
#
# For C Programs
#
export CCFLAGS=$(CXXFLAGS)
#
# name of file with dependencies
#
#header files
export LDFLAGS+=$(OPTFLAGS)
#export LDLIBS=$(LIBRARIES)
export DEPEND=make.dep
# to avoid nasty warnings
export ARFLAGS=rsU
# Rule to generate pdf from latex files
%.pdf:%.tex
	pdflatex $<
