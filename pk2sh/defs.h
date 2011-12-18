#ifndef __DEFS__
#define __DEFS__

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include <iomanip>
using namespace std;
#include <boost/iostreams/filter/stdio.hpp>
namespace io = boost::iostreams;

#include "util.h"

#define foreach(name, obj, it) for(it name = obj.begin(), end##name = obj.end(); name!=end##name; ++name)
#define foreach_by(index, name, obj, it) for(it name = obj.get<index>().begin(), end##name = obj.get<index>().end(); name!=end##name; ++name)

#define PTR_FREE(p)	if (p != NULL) \
				 free(p); \
			p = NULL;

#define PTR_DELETE(p)	if (p != NULL) \
				 delete p; \
			p = NULL;

#define PTR_INIT(p)	p = NULL;


#define SET_FLAG(x, flag)	x |= flag
#define RESET_FLAG(x, flag)	x &= ~flag
#define HAS_FLAG(x, flag)	((x & flag) == flag)

#define TRUE  1
#define FALSE 0

#define SUCCESS 0
#define FAILURE 1

#endif

