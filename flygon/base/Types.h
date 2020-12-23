#ifndef FLYGON_BASE_TYPES
#define FLYGON_BASE_TYPES

#include <stdint.h>
#include <string.h>
#include <string>

namespace flygon
{

using std::string;

inline void memZero(void* p, size_t n)
{
    memset(p, 0, n);
}

}


#endif //FLYGON_BASE_TYPES