#include <iostream>
#pragma once

namespace util
{
    void set_verbose(bool v);
    bool get_verbose();

    template <typename... TArgs>
    void verbose_print(const char *fmt, TArgs &&...args)
    {
        if (!get_verbose())
            return;

        if constexpr (sizeof...(args) == 0)
        {
            printf("%s", fmt);
        }
        else
        {
            printf(fmt, std::forward<TArgs>(args)...);
        }
    };

}