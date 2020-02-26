#pragma once
// #define DBJ_EASTL

#ifdef DBJ_EASTL
#include <EASTL/type_traits.h>
#include <EASTL/array.h>
#include <EASTL/vector.h>
#include <EASTL/string.h>
#include <EASTL/utility.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vcruntime_typeinfo.h>
#define DBJ_STL eastl
#else
#include <typeinfo>
#include <array>
#include <vector>
#include <string>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <utility>
#define DBJ_STL std
// #include <algorithm>
// #include <include/vld.h>
#endif

#ifdef __GNUC__
#include <cstdlib>
#include <cxxabi.h>
#endif // __GNUC__


#define dbj_assign DBJ_STL::exchange

    ///----------------------------------------------------
namespace dbj {
    ///----------------------------------------------------
    template<typename T>
    inline  const char* nametype(T&& t) {
#ifdef __GNUC__
        int     status{};
        char* realname{};
        realname = abi::__cxa_demangle(
            typeid(T).name(), 0, 0, &status
        );
        static string retval;
        retval = realname;
        free(realname);
        return retval.c_str();
#else
        return typeid(T).name();
#endif
    }
    ///----------------------------------------------------
    template<typename T>
    inline void exchange(T& a, T& b) {
        T t = a;
        a = b;
        b = t;
    }
    //
    // really exchage -- without moving
    template <class T_>
    inline void exchangex(T_& left_, T_& right_)
    {
        T_ temp_ = static_cast<T_&&>(left_);
        left_ = static_cast<T_&&>(right_);
        right_ = static_cast<T_&&>(temp_);
    }
    //


    // FUNCTION TEMPLATE swap nicked from msvc std space
    template <class Ty, size_t S>
    inline void swap(Ty(&left)[S], Ty(&right)[S]) noexcept
    {
        if (&left != &right) {
            Ty* first1 = left;
            Ty* Last1 = first1 + S;
            Ty* first2 = right;
            for (; first1 != Last1; ++first1, ++first2) {
                DBJ_STL::iter_swap(first1, first2);
            }
        }
    }

    ///-------------------------------------------------------
    void pause(const char* prompt = "", bool pause_ = false, bool cls = false) {
        printf("\n%s", "");
        printf("\n%s", "====================================================================================");
        printf("\n%s", prompt);
        if (pause_) system("@pause");
        if (cls) system("@cls");
        printf("\n%s", "");
    }

    ///----------------------------------------------------
    extern "C" {
        inline char* allocate_data(const char* data_) {
            assert(data_);
            char* p = _strdup(data_);
            printf("\n%20s_strdup(\"%s\")", "", p);
            return p;
        }

        inline void  free_data(char* ip_) {
            printf("\n%20sfree(\"%s\")", "", ip_);
            free(ip_);
        }
    }
    ///----------------------------------------------------
} // dbj
    ///----------------------------------------------------
