#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <utility>
// #include <algorithm>
#include <include/vld.h>

// using namespace std;
///----------------------------------------------------
namespace dbj {
    struct S;

    ///----------------------------------------------------
    extern "C" {
        inline char * allocate_data( const char * data_ ) {
            assert( data_ );
            char* p = _strdup( data_ );
            printf("\n%20sallocated ( %p )", "", p);
            return p;
        }

        inline void  free_data(char* ip_) {
            printf("\n%20sfreed ( %p )", "", ip_);
            free(ip_);
            ip_ = nullptr;
        }
    }
    ///----------------------------------------------------
    template <class _Ty, class>
    void swap(_Ty& _Left, _Ty& _Right) 
        // noexcept(is_nothrow_move_constructible_v<_Ty>&& is_nothrow_move_assignable_v<_Ty>) 
    {
        _Ty _Tmp = std:: move(_Left);
        _Left = std:: move(_Right);
        _Right = std:: move(_Tmp);
    }
    ///----------------------------------------------------
    // std::move and swaps are in here so that we can follow 
    // ltno them through the debugger session

    // synopsis for std::std::move
    //template<typename T>
    //inline T&& std::move(T&& obj)
    //{
    //    return (T&&)obj; // return obj as an rvalue
    //}
    /// pointer swapping
    /// actualy swaps just the 
    /// values pointers are
    /// pointing to
    //template<typename T>
    //inline void swap(T* a, T* b) {
    //    T * t = std::move(a);
    //    a = std::move(b);
    //    b = std::move(t);
    //}

    //template<typename T>
    //inline void swap(T& a, T& b) {
    //    T t = std::move(a);
    //    a = std::move(b);
    //    b = std::move(t);
    //}
    ///----------------------------------------------------
    /// there are better designs
    /// S is like it is for teaching purposes
    struct S final 
    {
        // POINT: order here rules the oder of initialization
        // in the constructor initializer list
        // not the oder in the list itself!
        char * data{};
        size_t size{};

        S() noexcept  : 
            size(0),            // initialized first
            data(nullptr)       // initialized second
        {
            printf("\n%5sconstructed by default S() data:nullptr", "");
        }

        // POINT: never forget the ctor initializer list
        explicit S(const char * str_ ) noexcept :
            data(allocate_data(str_)),
            size(strlen(str_))
        {
            printf("\n%5sconstructed with S(const char *) data: '%s'", "", data);
        }

        ~S() {
            printf("\n%5sdestructed ~S() with data: '%s'", "", data);
            if (data) { free_data(data), data = nullptr; size = 0; }
        }

        // POINT: it is important
        // to initialize the members, before anything else
        S(S&& other) noexcept 
            : size(std::move(other.size)),
            data(std::move(other.data))
        {
            printf("\n%5sconstructed with S( S && S{}) ,  data: '%s'", "", data);
            // CRITICAL: this is signal to the destructor 
            // of the other object
            // NOT to attempt freeing the moved from data
            other.data = nullptr;
        }
        /* */
        S(const S& other_) noexcept
            : size(other_.size),
            data(allocate_data(other_.data))
        {
            printf("\n%5sconstructed  S(const S& other_), data: '%s'", "", data);
        }
        /// POINT: copy assignment operator is
        /// required by the swap's
        constexpr S& operator=(const S&) = default;

        //friend void swap(S* a, S* b) {
        //    printf("\nswap( S * a, S * b)");
        //    swap(a->data, b->data);
        //    swap( a->size, b->size );
        //}

        //friend void swap(S& a, S& b) {
        //    printf("\n swap( S & a, S & b)");
        //    swap(a.data, b.data);
        //    swap(a.size, b.size);
        //}

    };
} // dbj

using namespace dbj;
///----------------------------------------------------
/// value argument
/// return by value
S return_s(S s_)
{
    return s_; // moving 
}
auto t1()
{
    S x = return_s(S("ABC"));
    assert(3 == x.size);
}
auto t2()
{
    S x("White"), y("Black");
    std::swap(x, y);
    //assert(0xF == x.size);
}
auto t3()
{
    /// POINT: swap implementation assures
    /// xp and yp are not changed
    /// just the values they are pointing to
    auto xp = new S("Red"), yp = new S("Green");

    S* xp_before = xp;
    S* yp_before = yp;

    std::swap(xp, yp);

    auto same_x = xp_before == xp;
    auto same_y = yp_before == yp;

    delete xp; delete yp;
}

///----------------------------------------------------
int main(int, char**) {
    t3();
    return 42;
 }
///----------------------------------------------------
