#include <postgres.h>
#include <fmgr.h>
#include "executor/spi.h"
#include "funcapi.h"

#include "utils/builtins.h"
#include <utils/numeric.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "src/server.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif


void get_ports(int **ports, int *counter)
{
	//query SQL
	char query[] = "select port from http_server.server;";
	// result of the query : successful or not
	int query_result;


	// Execute the query itself
	query_result = SPI_execute(query, true, 0);
	if(query_result == SPI_OK_SELECT) {

        TupleDesc tupdesc = SPI_tuptable->tupdesc;
        SPITupleTable *tuptable = SPI_tuptable;

		char buf[8192] = {0};
		int j = 0;

		for (j = 0; j < tuptable->numvals; j++) {
			HeapTuple tuple = tuptable->vals[j];
			char *port = NULL;

			elog(WARNING, "number of values %ld", tuptable->numvals);

			port = SPI_getvalue(tuple, tupdesc, 1);
			*(*ports+j) = atoi(port);
			elog(WARNING, "EXECQ: %s", port);
		}

		*counter = tuptable->numvals;
		
	} else {
		// if not ok, result in error:
		// TODO use the error loggin of pg
		perror("In SPI_execute");

	}
	
	SPI_finish();
}

PG_FUNCTION_INFO_V1(start_http_server);

Datum start_http_server(PG_FUNCTION_ARGS)
{
	int port = PG_GETARG_INT32(0);
	int res = 0;
	elog(WARNING, "port = %d", port);
	res = start_server(port);
}

PG_FUNCTION_INFO_V1(start);

Datum start(PG_FUNCTION_ARGS) {

	// close(connfd);
	// FuncCallContext  *funcctx;
	// Datum result;

    FuncCallContext     *funcctx;
	int *ports;
	int counter = 0;

    if (SRF_IS_FIRSTCALL())
    {
        MemoryContext oldcontext;

        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        /* One-time setup code appears here: */

		
		// descriptor of the connection
		int spiconn;
		
		// Connect to de server programming interface of pg
		spiconn = SPI_connect();
		if (spiconn != SPI_OK_CONNECT) {
			// if not ok, result in error
			// TODO use the error loggin of pg
			perror("In SPI connect");
		}
		ports = SPI_palloc(16);

		get_ports(&ports, &counter);

        funcctx->max_calls = counter;

        // if returning composite
            // build TupleDesc, and perhaps AttInMetadata
        // endif returning composite
        // user code
        funcctx->user_fctx = ports;

        MemoryContextSwitchTo(oldcontext);
    }

    /* Each-time setup code appears here: */
    // user code
    funcctx = SRF_PERCALL_SETUP();
    ports = (int *)funcctx->user_fctx;

    /* this is just one way we might test whether we are done: */
    if (funcctx->call_cntr < funcctx->max_calls)
    {
        /* Here we want to return another item: */
        Datum        *values;
        HeapTuple    tuple;
        Datum        result;

		result = (Datum) ports[funcctx->call_cntr];
        // obtain result Datum
        SRF_RETURN_NEXT(funcctx, result);
    }
    else
    {
        /* Here we are done returning items, so just report that fact. */
        /* (Resist the temptation to put cleanup code here.) */
        SRF_RETURN_DONE(funcctx);
    }
}
