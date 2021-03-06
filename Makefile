
-include Makefile.inc

SRC_DIR = $(ROOT)/src
INC_DIR = $(ROOT)/include
OBJ_DIR = $(ROOT)/obj
OBJ_LIB_DIR = $(OBJ_DIR)/lib
LIB_DIR = $(ROOT)/lib


# Library with factory
FACTORY_LIBNAME  = GraphFactory
FACTORY_LIBFILE  = lib$(FACTORY_LIBNAME).so
FACTORY_LIB      = $(LIB_DIR)/$(FACTORY_LIBFILE)

LDLIBS += -L$(LIB_DIR) -l$(FACTORY_LIBNAME)
LIBLIB  = -L$(LIB_DIR) -l$(FACTORY_LIBNAME)

# link libdl to be able to load dynamic libraries at run-time
LDLIBS += -ldl

GC_SRC_DIR = $(SRC_DIR)/GraphCreators

# Library with the Graph Factory
FACTORY_LIB_SRCS := $(GC_SRC_DIR)/GraphCreatorFactory.cpp
FACTORY_LIB_OBJS := $(FACTORY_LIB_SRCS:$(GC_SRC_DIR)/%.cpp=$(OBJ_LIB_DIR)/%.o)

# Library with Graph Creators with input files
INPUT_LIBNAME   = InputCreator
INPUT_LIBFILE   = lib$(INPUT_LIBNAME).so
INPUT_LIB       = $(LIB_DIR)/$(INPUT_LIBFILE)
INPUT_LIB_SRCS := $(GC_SRC_DIR)/GraphCreatorInputFile.cpp
INPUT_LIB_OBJS := $(INPUT_LIB_SRCS:$(GC_SRC_DIR)/%.cpp=$(OBJ_LIB_DIR)/%.o)

# Library with Graph Creators using Boost methods
BOOST_LIBNAME   = BoostCreators
BOOST_LIBFILE   = lib$(BOOST_LIBNAME).so
BOOST_LIB       = $(LIB_DIR)/$(BOOST_LIBFILE)
BOOST_LIB_SRCS := $(filter-out $(FACTORY_LIB_SRCS) $(INPUT_LIB_SRCS),$(wildcard $(GC_SRC_DIR)/*.cpp))
BOOST_LIB_OBJS := $(BOOST_LIB_SRCS:$(GC_SRC_DIR)/%.cpp=$(OBJ_LIB_DIR)/%.o)

# LDLIBS += -L$(LIB_DIR) -l$(BOOST_LIBNAME) -l$(INPUT_LIBNAME)

LDFLAGS+=-Wl,-rpath,$(LIB_DIR)


# get all headers in the include directory and its subdirectories
HEADERS = $(shell find $(INC_DIR) -name '*.hpp')

exe_sources = $(SRC_DIR)/main.cpp
EXEC = $(exe_sources:.cpp=)

# get all files for the executable
MAIN_SRCS = $(filter-out $(exe_sources),$(wildcard $(SRC_DIR)/*.cpp))

# get the corresponding object file
MAIN_OBJS = $(MAIN_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# get all cpp files for the dependency file
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')

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
	$(RM) -f $(DEPEND) ./index.html
	$(RM) -r $(LIB_DIR) ./doc
	$(RM) *.out *.bak *~

doc:
	doxygen $(DOXYFILE)
	ln -s doc/html/index.html ./index.html


dynamic: CXXFLAGS += -fPIC
dynamic: $(FACTORY_LIB_OBJS) $(INPUT_LIB_OBJS) $(BOOST_LIB_OBJS)
	@echo " "
	@echo "--- Building factory library ---"
	@mkdir -p $(LIB_DIR)
	@$(CXX) -shared -Wl,-soname,$(FACTORY_LIBFILE) $(FACTORY_LIB_OBJS) -o $(FACTORY_LIB)
	@echo "--- Building input creator library ---"
	@$(CXX) -shared $(LDFLAGS) $(LIBLIB) -Wl,-soname,$(INPUT_LIBFILE) $(INPUT_LIB_OBJS) -o $(INPUT_LIB)
	@echo "--- Building boost creators library ---"
	@$(CXX) -shared $(LDFLAGS) $(LIBLIB) -Wl,-soname,$(BOOST_LIBFILE) $(BOOST_LIB_OBJS) -o $(BOOST_LIB)
	@echo " "


$(FACTORY_LIB_OBJS): $(FACTORY_LIB_SRCS) $(HEADERS) | $(OBJ_LIB_DIR)
	@echo "--- Compiling $< requested by the factory library ---"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(INPUT_LIB_OBJS): $(INPUT_LIB_SRCS) $(HEADERS) | $(OBJ_LIB_DIR)
	@echo "--- Compiling $< requested by the input creator library ---"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BOOST_LIB_OBJS): $(OBJ_LIB_DIR)/%.o:$(GC_SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_LIB_DIR)
	@echo "--- Compiling $< requested by the boost creators library ---"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_LIB_DIR): $(OBJ_DIR)
	@mkdir -p $(OBJ_LIB_DIR)

$(EXEC): $(MAIN_OBJS)

$(MAIN_OBJS): $(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "--- Compiling $< related to the main ---"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done
