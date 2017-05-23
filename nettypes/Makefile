MODULE_big = nettypes
OBJS = nettypes.o
PG_CPPFLAGS = -I$(libpq_srcdir)

EXTENSION = nettypes
DATA = nettypes--1.0.sql

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/nettypes
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
