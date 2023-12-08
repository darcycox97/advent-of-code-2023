#include "print.h"

namespace util
{
    bool verbose = false;

    void set_verbose(bool v)
    {
        verbose = v;
    }

    bool get_verbose()
    {
        return verbose;
    }
}