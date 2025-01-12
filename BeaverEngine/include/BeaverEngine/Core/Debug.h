#pragma once
#include <cstdio>
#include "BeaverEngine/Core/std.h"
namespace bv
{
#ifdef DEBUG

#define DEBUG_PRINT_STRING(str) printf(str);printf("\n")
#define DEBUG_PRINT_INT(str) printf(std::to_string(str).c_str());printf("\n")

#else

#define DEBUG_PRINT_STRING(str) ()
#define DEBUG_PRINT_INT(str) ()

#endif // DEBUG
}
