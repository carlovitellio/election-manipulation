# The directory where the Makefile resides
ROOT=/home/carlo/election-manipulation

mkBoostInc?=/usr/include/boost/
BOOST_DIR=$(mkBoostInc)

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

## If you are using intel compilers
#ifeq ($(findstring icc, $(CXX)),icc)
#inteldir=$(dir $(CXX))
## Change it for the location of your compiler!
#OTHER_LIB_DIR+=/usr/local/opt/intel/cc/9.0/lib
#endif




#  DEFINES: Possible -DXX statements
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
#
ifdef RELEASE
DEBUG=no
else
DEBUG=yes
endif

ifeq ($(DEBUG),yes)
  OPTFLAGS=-g
# If debugging we use the local dynamic libraries and avoid ldconfig -d
# or setting LD_LIBRARY_PATH
  LDFLAGS=-Wl,-rpath=. -Wl,-rpath=$(PACS_ROOT)/lib#
else
  # Since aa 18-19 I have decided to indicate the libraries even if DEBUG=no.
  # Remember however that normally release code does not have hardwired in the
  # executables the directories where the loader looks for libraries.
  # You take the responsibility of installing them in the right place.
  # But here I want to avoid students messing around with LD_LIBRARY_PATH
  # or ldconfig.
  LDFLAGS=-Wl,-rpath=. -Wl,-rpath=$(PACS_ROOT)/lib#
  OPTFLAGS=-O3 -funroll-loops #maybe you want -O3
  DEFINES+=-DNDEBUG
endif
# Add verbosity if needed
ifeq ($(VERBOSE),yes)
  VERBOSITY=-DVERBOSE
endif

#
# Flags for standard
#

STANDARD=c++17

export STDFLAGS=-std=$(STANDARD)
##
#  END USER MODIFIABLE PART
##

export INC_DIR=$(ROOT)/include

export CC
export CXX
#
# Location of
# Doxygen configuration file
#
export DOXYFILE=$(PACS_ROOT)/DoxyfileCommon
#
# Compose options
#
# collect all includes
# ALL_INC_DIR= $(INC_DIR) $(BOOST_DIR)
ALL_INC_DIR= $(INC_DIR)
includes := $(foreach inc,$(ALL_INC_DIR),-I$(inc) )
# where to look for include files:
# first the current directory, then the directory ./include
# and then the possible other specifyed in OTHER_INC_DIR
INCLS  = -I. -I./include $(includes) -isystem $(BOOST_DIR)
# collect all library dirs
ALL_LIB_DIR=$(LIB_DIR)
LIBD := $(foreach lib,$(ALL_LIB_DIR),-L$(lib) )
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
