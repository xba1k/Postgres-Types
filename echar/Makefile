MODULE_big = echar
OBJS = echar.o
PG_CPPFLAGS = -I$(libpq_srcdir)
LDFLAGS_SL += -lssl -lcrypto

EXTENSION = echar
DATA = echar--1.0.sql

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/echar
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
