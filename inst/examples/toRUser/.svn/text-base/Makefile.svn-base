# The Makefile

# Get the root stuff
rootincs := $(shell root-config --cflags)
rootlibs := $(shell root-config --glibs )

CXX := $(shell R CMD config CXX)
CXXFLAGS := $(shell R CMD config CXXFLAGS)
CPPFLAGS := $(shell R CMD config CPPFLAGS) 
CPICFLAGS := $(shell R CMD config CPICFLAGS)  
RINCLUDES := $(shell R CMD config --cppflags)
SHLIB_CXXLD := $(shell R CMD config SHLIB_CXXLD)
SHLIB_LDFLAGS := $(shell R CMD config SHLIB_LDFLAGS)
LDFLAGS := $(shell R CMD config LDFLAGS)
RLIBS := $(shell R CMD config --ldflags)

# These two lines are specific to D0 CAF trees
CAFINCLUDES := -I../include
CAFLIBS := -L/rooms/coralreef/lyon/cafPlace/caf_p18.03/lib

# You need this - adapt for your RootTreeToR area.
RTTRINCLUDES := -I/home/lyon/Rlib/RootTreeToR/include

# You need this
CXXFLAGS += $(rootincs) $(CPPFLAGS) $(CPICFLAGS) $(RINCLUDES) $(CAFINCLUDES) $(RTTRINCLUDES)

# Make the Missing ET Example
metR.so: met.o
	 $(SHLIB_CXXLD) $(SHLIB_LDFLAGS) $(LDFLAGS) $(RLIBS) -o metR.so met.o  /home/lyon/Rlib/RootTreeToR/libs/RootTreeToR.so $(CAFLIBS) -lcafe -ltmb_tree -lmet_util $(rootlibs)

met.o: met.cpp met.h
		    $(CXX) $(CXXFLAGS) -c $<        


# Make the tracks in cone example
trksInConeR.so: trksInCone.o
	 $(SHLIB_CXXLD) $(SHLIB_LDFLAGS) $(LDFLAGS) $(RLIBS) -o trksInConeR.so trksInCone.o  /home/lyon/Rlib/RootTreeToR/libs/RootTreeToR.so $(CAFLIBS) -lcafe -ltmb_tree -lmet_util $(rootlibs)

trksInCone.o: trksInCone.cpp trksInCone.h
		    $(CXX) $(CXXFLAGS) -c $<


# Make the MC Tree example
mcTreeR.so: mcTree.o
	 $(SHLIB_CXXLD) $(SHLIB_LDFLAGS) $(LDFLAGS) $(RLIBS) -o mcTreeR.so mcTree.o  /home/lyon/Rlib/RootTreeToR/libs/RootTreeToR.so $(CAFLIBS) -lcafe -ltmb_tree -lmet_util $(rootlibs)

mcTree.o: mcTree.cpp mcTree.h
		    $(CXX) $(CXXFLAGS) -c $<


clean:
	rm -f *.so
	rm -f *.o
