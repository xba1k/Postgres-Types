//#include "headers/codecfactory.h"
#include "headers/deltautil.h"

#include "headers/common.h"
#include "headers/codecs.h"
#include "headers/vsencoding.h"
#include "headers/util.h"
#include "headers/simple16.h"
#include "headers/simple9.h"
#include "headers/simple8b.h"
#include "headers/newpfor.h"
#include "headers/simdnewpfor.h"
#include "headers/optpfor.h"
#include "headers/simdoptpfor.h"
#include "headers/fastpfor.h"
#include "headers/simdfastpfor.h"
#include "headers/variablebyte.h"
#include "headers/compositecodec.h"
#include "headers/blockpacking.h"
#include "headers/pfor.h"
#include "headers/simdpfor.h"
#include "headers/pfor2008.h"
#include "headers/VarIntG8IU.h"
#include "headers/simdbinarypacking.h"
#include "headers/snappydelta.h"

using namespace FastPFor;

#include "intlist.h"

//#define CODEC (new CompositeCodec<FastPFor, VariableByte> ()); // doesn't compile
//#define CODEC (new CompositeCodec<SIMDFastPFor, VariableByte> ()); // crashes
//#define CODEC ( new JustCopy()); # good
//#define CODEC (new CompositeCodec<FastBinaryPacking<8> , VariableByte>); // good 286 => 162
//#define CODEC (new CompositeCodec<FastBinaryPacking<16> , VariableByte>); // good 286 => 157
//#define CODEC (new CompositeCodec<FastBinaryPacking<32> , VariableByte>); // good 286 => 163
//#define CODEC (new vsencoding::VSEncodingBlocks(1U << 16)); // good 286 => 174
//#define CODEC (new CompositeCodec<SimplePFor<> , VariableByte> ()); // good 286 => 165
//#define CODEC (new CompositeCodec<PFor, VariableByte> ()); // good 286 => 180
//#define CODEC (new CompositeCodec<SIMDPFor, VariableByte> ()); // good 286 => 180
//#define CODEC (new CompositeCodec<PFor2008, VariableByte> ()); // good 286 => 181
//#define CODEC (new CompositeCodec<SIMDNewPFor<4, Simple16<false>> , VariableByte> ()); // good 286 => 184
//new CompositeCodec<NewPFor<4, Simple16<false>> , VariableByte> ());
//new CompositeCodec<OPTPFor<4, Simple16<false> > , VariableByte> ());
#define CODEC (new CompositeCodec<SIMDOPTPFor<4, Simple16<false> > , VariableByte> ()); // good 286 => 151
//#define CODEC (new Simple8b<true> ()); // good 286 => 159
//#define CODEC (new VarIntG8IU ()); // good 286 => 174
//new JustSnappy ());
//new CompositeCodec<SIMDBinaryPacking, VariableByte> ();
//new JustCopy();

#undef DEBUG
#define MINCOMPSIZE 5

size_t uncompress(uint32_t *datain, size_t length, uint32_t *buffer, size_t buffer_size);
static int cmpint(const void *p1, const void *p2);
int parse_int_list(char *source_data, int data_length, unsigned int **dest_buf);
char *make_int_list(unsigned int *source_data, int len);

static int cmpint(const void *p1, const void *p2) {

    unsigned int a = *((unsigned int*)p1);
    unsigned int b = *((unsigned int*)p2);

    if(a < b)
	return -1;
    if(a > b)
	return 1;
    return 0;

}

int parse_int_list(char *source_data, int data_length, uint32_t **dest_buf) {

    char buf[20];
    char *p, *p2;
    int i = 0;
    int memsiz = 4;

    for(p = source_data; p <= source_data+data_length; p++) {
	if(!(*p >= '0' && *p <= '9')) {
	    memsiz += 4;
	}
    }

    if(*dest_buf == NULL)
	*dest_buf = (uint32_t*)palloc(memsiz);

    for(p = source_data, p2 = buf, buf[0] = 0; p <= source_data+data_length; p++) {
    
	if(*p >= '0' && *p <= '9') {
	    *p2 = *p;
	    p2++;
	} else if(p2 != buf) {
	    *(p2) = '\0';
	    (*dest_buf)[i++] = atoi(buf);
	    p2 = buf;
	    buf[0] = '\0';
	}

    }
    
    return i;

}

char *make_int_list(uint32_t *source_data, int len) {

    char *result = (char*)palloc(11*len);
    int i, l = 0;
    
    for(i = 0; i < len; i++) {
	l+= sprintf(result+l, "%u,", source_data[i]);
    }

    *(result+l-1) = '\0';
    return result;
}


extern "C" {

PG_MODULE_MAGIC;

}

extern "C" {

PG_FUNCTION_INFO_V1(intlist_in);

}

Datum intlist_in(PG_FUNCTION_ARGS) {

    char	*s = PG_GETARG_CSTRING(0);
    bytea    	*result;

    uint32_t *parsed_data = NULL;
    int data_length = strlen(s);
    int parsed_size = 0;

    
    if(data_length>0)
	parsed_size = parse_int_list(s, data_length, &parsed_data);

#ifdef DEBUG
    elog(NOTICE, "input list has %u words", parsed_size);
#endif

    if(parsed_size > 0) {
    
	if(parsed_size > 1) {    
	    qsort(parsed_data, parsed_size, 4, cmpint);
	}

	size_t result_size = parsed_size;
	result = (bytea*)palloc(result_size * sizeof(uint32_t) + VARHDRSZ + 4);
	uint32_t *buf = (uint32_t*)VARDATA(result);
	buf[0] = htonl(parsed_size);

#ifdef DEBUG
	elog(NOTICE, "stored parsed_size = %u", parsed_size);
#endif

	if(parsed_size > MINCOMPSIZE) {

	    try {

		IntegerCODEC *codec = CODEC;
    	        //Delta::deltaSIMD(parsed_data, parsed_size);
    		Delta::delta(parsed_data, parsed_size);
    		codec->encodeArray(parsed_data, parsed_size, &(buf[1]), result_size);
    		delete codec;

	    } catch(std::exception& e) {
    		std::cout << e.what() << std::endl;
    		result_size = 0;
	    }
	    
	    SET_VARSIZE(result, result_size*4 + VARHDRSZ + 4);
	    
	} else {
	
	    unsigned char *cbuf = (unsigned char*)buf;
	
	    cbuf[0] = 0xff;
	    memcpy(cbuf+1, parsed_data, parsed_size * 4);
	    SET_VARSIZE(result, parsed_size*4 + VARHDRSZ + 1);
	 
	}

#ifdef DEBUG
	elog(NOTICE, "compressed size is %lu words", result_size);
#endif

    } else {
	result = (bytea*)palloc(VARHDRSZ);
	SET_VARSIZE(result, VARHDRSZ);
    }
    
    PG_RETURN_BYTEA_P(result);

}

extern "C" {

PG_FUNCTION_INFO_V1(intlist_out);

}

Datum intlist_out(PG_FUNCTION_ARGS) {

	bytea	*vlena = PG_GETARG_BYTEA_PP(0);
	char	*result;
	uint32_t *uncompressed_data = NULL;
	size_t compressed_size = 0;
	size_t uncompressed_size = 0;
	uint32_t *buf = (uint32_t*)VARDATA_ANY(vlena);
	
#ifdef DEBUG
	elog(NOTICE, "VARSIZE_ANY_EXHDR(vlena) = %u", VARSIZE_ANY_EXHDR(vlena));
#endif 
	
	if(VARSIZE_ANY_EXHDR(vlena) > 0) {
	    if(*((unsigned char*)buf) == 0xff) {
		uncompressed_size = (VARSIZE_ANY_EXHDR(vlena) - 1) / 4;
	    } else {
		uncompressed_size = ntohl(buf[0]);
		compressed_size = (VARSIZE_ANY_EXHDR(vlena) - 4) / 4;
	    }
	}

#ifdef DEBUG	
	elog(NOTICE, "compressed size is %lu words", compressed_size);
	elog(NOTICE, "uncompressed size is %lu words", uncompressed_size);
#endif

	if(compressed_size > 0) {
	
	    uint32_t *compressed_data = (uint32_t*)palloc(compressed_size * 4);
	    memcpy(compressed_data, buf+1, compressed_size * 4);
	    uncompressed_data = (uint32_t*)palloc(uncompressed_size* 4);

	    try {

    		IntegerCODEC *codec = CODEC;
    		codec->decodeArray(compressed_data, compressed_size, uncompressed_data, uncompressed_size);
    		//Delta::inverseDeltaSIMD(uncompressed_data, uncompressed_size);
    		Delta::inverseDelta(uncompressed_data, uncompressed_size);
    		delete codec;

	    } catch(std::exception& e) {
    		std::cout << e.what() << std::endl;
    		uncompressed_size = 0;
	    }
#ifdef DEBUG
	    elog(NOTICE, "uncompressed size is %lu words", uncompressed_size);
#endif
	    
	    if(uncompressed_size > 0) {
	        result = make_int_list(uncompressed_data, uncompressed_size);
	    } else {
	        result = pstrdup("");
	    }
	    
	} else 
	
	if(uncompressed_size > 0) {
	    unsigned char *cbuf = (unsigned char*)buf;
	    result = make_int_list((uint32_t*)(cbuf+1), uncompressed_size);
	} else {
	    result = pstrdup("");
	}
	
	PG_RETURN_CSTRING(result);

}
