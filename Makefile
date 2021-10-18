
-include Makefile.inc

SRC_DIR = src
INC_DIR = include

# get all files *.cpp
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# get the corresponding object file
OBJS = $(SRCS:.cpp=.o)
# get all headers in the working directory
HEADERS = $(wildcard $(INC_DIR)/*.hpp)
#
exe_sources = $(SRC_DIR)/main.cpp
EXEC = $(exe_sources:.cpp=)

#========================== NEW THE DEFINITION OF THE TARGETS
.phony= all clean distclean doc

.DEFAULT_GOAL = all

all: $(DEPEND) $(EXEC)

clean:
	$(RM) -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	$(RM) -f $(DEPEND)
	$(RM) -r ./doc
	$(RM) *.out *.bak *~

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(OBJS)

$(OBJS): $(SRCS)

install:
	cp *.hpp $(PACS_INC_DIR)

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)
