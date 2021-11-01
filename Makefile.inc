# The directory where the Makefile resides
ROOT=/home/carlo/election-manipulation

# Where the Boost library resides
mkBoostInc=/usr/local/boost_1_77_0

MUPARSER_INC_DIR = /home/carlo/pacs-examples/Examples/include
MUPARSER_LIB_DIR = /home/carlo/pacs-examples/Examples/lib

LDLIBS += -L$(MUPARSER_LIB_DIR) -lmuparser

# The C++ compiler
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
  LDFLAGS=-Wl,-rpath=$(MUPARSER_LIB_DIR)
else
  OPTFLAGS=-O3 -funroll-loops
  DEFINES+=-DNDEBUG
  LDFLAGS=-Wl,-rpath=$(MUPARSER_LIB_DIR)
endif


export STDFLAGS=-std=c++17

export CC
export CXX

export DOXYFILE=$(ROOT)/Doxyfile

BOOST_DIR=$(mkBoostInc)

INCLS  = -I. -I$(ROOT)/include -I$(BOOST_DIR) -I$(MUPARSER_INC_DIR)

export CPPFLAGS=$(INCLS) $(DEFINES) $(VERBOSITY)
export CXXFLAGS=$(OPTFLAGS) $(STDFLAGS) $(WARNFLAGS)

export CCFLAGS=$(CXXFLAGS)

export LDFLAGS+=$(OPTFLAGS)
export DEPEND=make.dep

export ARFLAGS=rsU
