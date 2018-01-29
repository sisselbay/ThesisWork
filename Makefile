SHELL = /bin/sh

# A few variables used in this Makefile:
ROOTCXXFLAGS :=  $(shell root-config --cflags)
DICTCXXFLAGS := -I$(HEPMC2_DIR)/include -I$(FASTJET3)/include

LDFLAGS := $(shell root-config --ldflags --glibs) \
	-L$(HEPMC2_DIR)/lib -L$(FASTJET3)/lib -lHepMC -lfastjet

# Rule to build hepmc_analysis
hepmc_analysis: hepmc_analysis.cc
	$(CXX) $(ROOTCXXFLAGS) $(DICTCXXFLAGS) $@.cc MyUtils.cc -o $@.exe $(LDFLAGS)

# Clean up
clean:
	rm -f hepmc_analysis.exe
	

