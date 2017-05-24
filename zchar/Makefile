MODULE_big = zchar
OBJS = zchar.o
PG_CPPFLAGS = -I$(libpq_srcdir)
LDFLAGS_SL += -lz

EXTENSION = zchar
DATA = zchar--1.0.sql

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/zchar
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
