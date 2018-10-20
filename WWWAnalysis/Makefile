# Simple makefile

EXE=doAnalysis

SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)
HEADERS=$(SOURCES:.cc=.h)

CC         = g++
CXX        = g++
CXXFLAGS   = -g -O2 -Wall -fPIC -Wshadow -Woverloaded-virtual
LD         = g++
LDFLAGS    = -g -O2
SOFLAGS    = -g -shared
CXXFLAGS   = -g -O2 -Wall -fPIC -Wshadow -Woverloaded-virtual
LDFLAGS    = -g -O2
ROOTLIBS   = $(shell root-config --libs)
ROOTCFLAGS = $(shell root-config --cflags)
CXXFLAGS  += $(ROOTCFLAGS)
CFLAGS     = $(ROOTCFLAGS) -Wall -Wno-unused-function -g -O2 -fPIC -fno-var-tracking
EXTRAFLAGS = -fPIC -ITMultiDrawTreePlayer -Wunused-variable -lTMVA -lEG -lGenVector -lXMLIO -lMLP -lTreePlayer

.PHONY: check-env

$(EXE): $(OBJECTS) wwwtree.o check-env
	$(LD) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) $(ROOTLIBS) $(EXTRAFLAGS) -o $@

%.o: %.cc
	$(CC) $(CFLAGS) $< -c

wwwtree.cc:
	echo remaking tree class
	sh rooutil/makeclass.sh -f -x  t wwwtree tas www

clean:
	rm -f *.o $(EXE)

check-env:
ifndef TEMPLATE_TREE_PATH
  $(error TEMPLATE_TREE_PATH is not set. Please set via '>>> export TEMPLATE_TREE_PATH=/path/to/template.root)'
endif
