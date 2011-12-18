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

#define PTR_FREE(p)	if (p != NULL) \
				 free(p); \
			p = NULL;

#define PTR_DELETE(p)	if (p != NULL) \
				 delete p; \
			p = NULL;

#define PTR_INIT(p)	p = NULL;


#define TRUE  1
#define FALSE 0

#define SUCCESS 0
#define FAILURE 1

#endif

