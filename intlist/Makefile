HEADERS = ./headers/simdpfor.h ./headers/simdfastpfor.h ./headers/simdbinarypacking.h ./headers/bitpackinghelpers.h ./headers/common.h ./headers/memutil.h ./headers/pfor.h ./headers/pfor2008.h ./headers/bitpackingunaligned.h ./headers/bitpackingaligned.h ./headers/blockpacking.h  ./headers/codecfactory.h ./headers/packingvectors.h ./headers/compositecodec.h ./headers/cpubenchmark.h  ./headers/maropuparser.h ./headers/bitpacking.h  ./headers/util.h ./headers/simple9.h ./headers/simple8b.h ./headers/simple16.h ./headers/optpfor.h ./headers/newpfor.h ./headers/vsencoding.h ./headers/mersenne.h  ./headers/ztimer.h ./headers/codecs.h ./headers/synthetic.h ./headers/fastpfor.h ./headers/variablebyte.h ./headers/stringutil.h ./headers/entropy.h ./headers/VarIntG8IU.h ./headers/deltautil.h
COMMONBINARIES= bitpacking.o bitpackingaligned.o bitpackingunaligned.o simdbitpacking.o  simdunalignedbitpacking.o

MODULE_big = intlist
OBJS = intlist.o $(COMMONBINARIES)

CXXFLAGS = -fPIC -O0 -Wall -Wextra -Wno-unused-parameter -pedantic -mssse3 -pedantic -std=c++0x -DNDEBUG=1 -ggdb
PG_CPPFLAGS = -I$(libpq_srcdir) $(CXXFLAGS)

LDFLAGS_SL += -lstdc++ -lsnappy
#-lFastPFor


EXTENSION = intlist
DATA = intlist--1.0.sql

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/intlist
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif

bitpacking.o: ./headers/bitpacking.h ./src/bitpacking.cpp
	$(CXX) $(CXXFLAGS) -c ./src/bitpacking.cpp -Iheaders

bitpackingunaligned.o: ./headers/bitpacking.h ./src/bitpackingunaligned.cpp
	$(CXX) $(CXXFLAGS) -c ./src/bitpackingunaligned.cpp -Iheaders

bitpackingaligned.o: ./headers/bitpacking.h ./src/bitpackingaligned.cpp
	$(CXX) $(CXXFLAGS) -c ./src/bitpackingaligned.cpp -Iheaders

simdbitpacking.o: ./headers/common.h ./headers/simdbitpacking.h ./src/simdbitpacking.cpp
	$(CXX) $(CXXFLAGS) -c ./src/simdbitpacking.cpp -Iheaders

simdunalignedbitpacking.o: ./headers/common.h ./headers/usimdbitpacking.h ./src/simdunalignedbitpacking.cpp
	$(CXX) $(CXXFLAGS) -c ./src/simdunalignedbitpacking.cpp -Iheaders

horizontalbitpacking.o: ./headers/common.h ./headers/horizontalbitpacking.h ./src/horizontalbitpacking.cpp
	$(CXX) $(CXXFLAGS) -msse4.1 -c ./src/horizontalbitpacking.cpp -Iheaders
