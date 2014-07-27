#################################################
# linux makefile for cpp uwsgi module kickstart
#################################################

#################################################
# compiler
#################################################

CC = g++
VAL=/usr/bin/valgrind

# debug mode

DEBUG = -g
REENTRANT = -D_REENTRANT

# release mode changes DEBUG flags to release

release : override DEBUG = -O3

#################################################
# include and dependency information
#################################################

# include dependencies
JSONCPP = $(shell pkg-config --cflags jsoncpp)
HIREDIS = $(shell pkg-config --cflags hiredis)

# includes
INCLUDE = -I./include $(JSONCPP) $(HIREDIS)

#options
OPTIONS = -std=c++11 -fpic -Wno-write-strings -pthread 

# compile flags
CPPFLAGS = -Wall $(DEBUG) $(REENTRANT) $(INCLUDE) $(OPTIONS)
TESTFLAGS = -g

# library dependencies
JSONCPPLIBS = $(shell pkg-config --libs jsoncpp)
APXSLIBS = $(shell apxs -q LDFLAGS)
HIREDISLIBS = $(shell pkg-config --libs hiredis)
# library flags
DEPS = $(JSONCPPLIBS) $(APXSLIBS) $(HIREDISLIBS) -lpthread 
LIBS =  $(DEPS) -shared -lcurl
TEST_LIBS = -lgtest $(DEPS) -lapr-1 -laprutil-1 

#################################################
# source and build path information
#################################################

# uwsgi header
UWSGI  = /usr/bin/uwsgi
UWSGIH = include/uwsgi.h

# the apache uwsgi cpp plugin
PLUGIN = ./cpp_plugin.so
PLUGIN_SRC = ./src
PLUGIN_BUILD = ./build

# the apache module
MOD = ./test_mod.so
MOD_SRC = ./mod
MOD_BUILD = ./mod/build

# the unit test executable
BIN = ./test_plugin
TEST_SRC = ./t
TEST_BUILD = ./t/build

# Objects for the static library
PLUGIN_SRCFILESABS = $(shell ls $(PLUGIN_SRC)/*.cpp)
PLUGIN_SRCFILES =  $(notdir $(PLUGIN_SRCFILESABS))
PLUGIN_OBJFILES = $(PLUGIN_SRCFILES:%.cpp=$(PLUGIN_BUILD)/%.o)

# Objects for the apache module
MOD_SRCFILESABS = $(shell ls $(MOD_SRC)/*.cpp)
MOD_SRCFILES =  $(notdir $(MOD_SRCFILESABS))
MOD_OBJFILES = $(MOD_SRCFILES:%.cpp=$(MOD_BUILD)/%.o)

# Objects for the test executable
TEST_SRCFILESABS = $(shell ls $(TEST_SRC)/*.cpp)
TEST_SRCFILES =  $(notdir $(TEST_SRCFILESABS))
TEST_OBJFILES = $(TEST_SRCFILES:%.cpp=$(TEST_BUILD)/%.o)

#################################################
# usually no need to change anything below
#################################################

.SUFFIXES: .cpp

#################################################
# rule to compile all (default rule)
#################################################

all: $(UWSGIH) $(MOD) 

#################################################
# actual build rules
#################################################

$(UWSGIH):
	$(UWSGI) --dot-h > $(UWSGIH)

#################################################
# rules to compile .o files from .cpp source
#################################################

$(PLUGIN_BUILD)/%.o: $(PLUGIN_SRC)/%.cpp 
	$(CC) -c $^ -o $@ $(CPPFLAGS)

$(TEST_BUILD)/%.o: $(TEST_SRC)/%.cpp 
	$(CC) -c $^ -o $@ $(CPPFLAGS) $(TESTFLAGS)

$(MOD_BUILD)/%.o: $(MOD_SRC)/%.cpp 
	$(CC) -c $^ -o $@ $(CPPFLAGS) 
	


#################################################
# rule to compile the artifacts from .o files
#################################################

$(PLUGIN): $(UWSGIH) $(PLUGIN_OBJFILES)
	$(CC) $(PLUGIN_OBJFILES) $(LIBS) -o $(PLUGIN) 

$(MOD): $(PLUGIN) $(MOD_OBJFILES) 
	$(CC) $(MOD_OBJFILES) $(LIBS) -o $(MOD) 


$(BIN): $(MOD) $(TEST_OBJFILES) 
	$(CC) $(TEST_OBJFILES) $(TEST_LIBS) -o $(BIN) 
		


#################################################
# make clean
#################################################

clean:
	-rm -f $(BIN)	
	-rm -f $(MOD)	
	-rm -f $(TEST_BUILD)/*.o
	-rm -f $(MOD_BUILD)/*.o
	-find -name "*~" -exec rm {} \;

proper: clean
	-rm -f $(UWSGIH)
	-rm -f $(PLUGIN)
	-rm -f $(PLUGIN_BUILD)/*.o
	
#################################################
# make test runs the unit tests
#################################################
	
test: $(BIN)
	./$(BIN)

#################################################
# valgrind special targets. run the unit tests
# with valgrind to watch out for memory leaks.
#################################################

verify: $(BIN)
	$(VAL)  $(BIN)

debug: $(BIN)
	$(VAL) --leak-check=full --show-leak-kinds=all  $(UWSGI) --plugin cpp --cpp-dir=$(shell pwd)/ --plugin http --http=127.0.0.1:3032 --http-modifier1=250
	
	
#################################################
# release rule 
#################################################

release: all
	
run : $(MOD)
	$(UWSGI) --plugin-dir=$(shell pwd)/ --plugin cpp --cpp-dir=$(shell pwd)/ --plugin http --http=127.0.0.1:3032 --http-modifier1=250
	
