MODULE_big = pg_server
EXTENSION = pg_server     # the extersion's name
DATA = pg_server--0.0.1.sql    # script file to install
OBJS = pg_server.o
# REGRESS = get_sum_test      # the test script file

# for posgres build
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)