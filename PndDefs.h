#pragma once
#include <stdint.h>   // for UINT32_MAX, etc.
#include <cfloat>     // for DBL_MAX, etc.
#include <string>
#include <cmath>

typedef uint8_t BOOL;

typedef float flt32;
typedef double flt64;

typedef float* flt32ptr;
typedef float& flt32ref;
typedef double* flt64ptr;
typedef double& flt64ref;

typedef bool* boolptr;
typedef bool& boolref;

typedef char* charptr;
typedef char& charref;
typedef const char* c_charptr;
typedef const char& c_charref;

typedef void* voidptr;

typedef std::string* stringptr;
typedef const std::string* c_stringptr;
typedef std::string& stringref;
typedef const std::string& c_stringref;

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

typedef int8_t* int8ptr;
typedef uint8_t* uint8ptr;
typedef int16_t* int16ptr;
typedef uint16_t* uint16ptr;
typedef int32_t* int32ptr;
typedef uint32_t* uint32ptr;
typedef int64_t* int64ptr;
typedef uint64_t* uint64ptr;

typedef int8_t& int8ref;
typedef uint8_t& uint8ref;
typedef int16_t& int16ref;
typedef uint16_t& uint16ref;
typedef int32_t& int32ref;
typedef uint32_t& uint32ref;
typedef int64_t& int64ref;
typedef uint64_t& uint64ref;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef NULL
#define NULL                0
#endif

#define MAXSIGFIGS  15

//#define INT8MAX std::numeric_limits<int8_t>::max()
//#define UINT8MAX std::numeric_limits<uint8_t>::max()
//#define INT16MAX std::numeric_limits<int16_t>::max()
//#define UINT16MAX std::numeric_limits<uint16_t>::max()
//#define INT32MAX std::numeric_limits<int32_t>::max()
//#define UINT32MAX std::numeric_limits<uint32_t>::max()
//#define INT64MAX std::numeric_limits<int64_t>::max()
//#define UINT64MAX std::numeric_limits<uint64_t>::max()

