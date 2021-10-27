
-include Makefile.inc

SRC_DIR = $(ROOT)/src
INC_DIR = $(ROOT)/include
OBJ_DIR = $(ROOT)/obj
LIB_DIR = $(ROOT)/lib

VERSION = 1
RELEASE = 0

# Library with factory
FACTORY_LIBNAME  = GraphFactory
FACTORY_LIBFILE  = lib$(FACTORY_LIBNAME).so
FACTORY_LIB      = $(LIB_DIR)/$(FACTORY_LIBFILE)

LDLIBS += -L$(LIB_DIR) -l$(FACTORY_LIBNAME)
LIBLIB  = -L$(LIB_DIR) -l$(FACTORY_LIBNAME)

FACTORY_LIB_SRCS := $(SRC_DIR)/GraphCreatorFactory.cpp
FACTORY_LIB_OBJS := $(FACTORY_LIB_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Library with Graph Creators using Boost methods
BOOST_LIBNAME   = BoostCreators
BOOST_LIBFILE   = lib$(BOOST_LIBNAME).so
BOOST_LIB       = $(LIB_DIR)/$(BOOST_LIBFILE)
BOOST_LIB_SRCS := $(SRC_DIR)/BoostCreators.cpp $(SRC_DIR)/GraphCreatorErdosRenyi.cpp \
										 $(SRC_DIR)/GraphCreatorPLOD.cpp $(SRC_DIR)/GraphCreatorRMAT.cpp \
										 $(SRC_DIR)/GraphCreatorSmallWorld.cpp
BOOST_LIB_OBJS := $(BOOST_LIB_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Library with Graph Creators with input files
INPUT_LIBNAME   = InputCreator
INPUT_LIBFILE   = lib$(INPUT_LIBNAME).so
INPUT_LIB       = $(LIB_DIR)/$(INPUT_LIBFILE)
INPUT_LIB_SRCS := $(SRC_DIR)/GraphCreatorInputFile.cpp
INPUT_LIB_OBJS := $(INPUT_LIB_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


LDLIBS += -L$(LIB_DIR) -l$(BOOST_LIBNAME) -l$(INPUT_LIBNAME)

LDFLAGS+=-Wl,-rpath,$(LIB_DIR)


# get all headers in the working directory
HEADERS = $(wildcard $(INC_DIR)/*.hpp)


# get all files for the executable
MAIN_SRCS = $(SRC_DIR)/Person.cpp $(SRC_DIR)/ManipulatorInfluence.cpp \
						$(SRC_DIR)/PerformanceEvaluator.cpp
# get the corresponding object file
MAIN_OBJS = $(MAIN_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

exe_sources = $(SRC_DIR)/main.cpp
EXEC = $(exe_sources:.cpp=)

# get all cpp files for the dependency file
SRCS=$(wildcard $(SRC_DIR)/*.cpp)

#========================== NEW THE DEFINITION OF THE TARGETS
.phony= all clean depend distclean doc dynamic exec help

.DEFAULT_GOAL = help

help:
	@echo "make help prints this"
	@echo "make dynamic makes all dynamic libraries"
	@echo "make exec compiles executable"
	@echo "make depend makes dependency file"
	@echo "make all runs make depend, dynamic and exec"
	@echo "make clean removes the executable and the object directory"
	@echo "make distclean removes dependency file, documentation, libraries "
	@echo "	and object directories and the executable"
	@echo "make doc create the documentation via Doxygen"
	@echo "macro DEBUG=no deactivates debugging"

all:
	$(MAKE) depend
	$(MAKE) dynamic
	$(MAKE) exec

exec: $(EXEC)

depend: $(DEPEND)

clean:
	$(RM) -f $(EXEC)
	$(RM) -r $(OBJ_DIR)

distclean:
	$(MAKE) clean
	$(RM) -f $(DEPEND)
	$(RM) -r ./doc
	$(RM) -r $(LIB_DIR)
	$(RM) *.out *.bak *~

doc:
	doxygen $(DOXYFILE)

dynamic: CXXFLAGS += -fPIC
dynamic: $(FACTORY_LIB_OBJS) $(BOOST_LIB_OBJS) $(INPUT_LIB_OBJS)
	@echo " "
	@echo "--- Building factory library ---"
	@mkdir -p $(LIB_DIR)
	@$(CXX) -shared -Wl,-soname,$(FACTORY_LIBFILE) $(FACTORY_LIB_OBJS) -o $(FACTORY_LIB)
	@echo "--- Building boost creators library ---"
	@$(CXX) -shared $(LDFLAGS) $(LIBLIB) -Wl,-soname,$(BOOST_LIBFILE) $(BOOST_LIB_OBJS) -o $(BOOST_LIB)
	@echo "--- Building input creator library ---"
	@$(CXX) -shared $(LDFLAGS) $(LIBLIB) -Wl,-soname,$(INPUT_LIBFILE) $(INPUT_LIB_OBJS) -o $(INPUT_LIB)
	@echo " "


$(FACTORY_LIB_OBJS): $(FACTORY_LIB_SRCS) $(HEADERS) | $(OBJ_DIR)
	@echo "--- Compiling $< related to the factory library ---"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BOOST_LIB_OBJS): $(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_DIR)
	@echo "--- Compiling $< related to the boost creators library ---"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(INPUT_LIB_OBJS): $(INPUT_LIB_SRCS) $(HEADERS) | $(OBJ_DIR)
	@echo "--- Compiling $< related to the input creator library ---"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(EXEC): $(MAIN_OBJS)

$(MAIN_OBJS): $(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "--- Compiling $< related to the main ---"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done
