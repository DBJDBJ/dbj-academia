#include <cstdio>
#include <cstdlib>
#include <cassert>
// #include <utility>
#include <algorithm>
#include <include/vld.h>

// using namespace std;
namespace dbj {
    struct S;

    ///----------------------------------------------------
    // synopsis for std::move
    template<typename T>
    inline T&& move(T&& obj)
    {
        return (T&&)obj;        // return obj as an rvalue
    }
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
    template<typename T>
    inline void swap(T* a, T* b) {
        T t = move(*a);
        *a = move(*b);
        *b = move(t);
    }

    template<typename T>
    inline void swap(T& a, T& b) {
        T t = move(a);
        a = move(b);
        b = move(t);
    }
    ///----------------------------------------------------
    struct S final {
        // WARNING: this defines the oder of initialization
        // in the constructor initializer list
        // not the oder in the list itself!
        char * data{};
        size_t size{};

        S() noexcept  : data(nullptr), size(0)
        {
            printf("\n%5sconstructed by default S() nullptr", "");
        }

        // WARNING: never forget the initializer list
        explicit S(const char * str_ ) noexcept :
            data(allocate_data(str_)),
            size(strlen(str_))
        {
            printf("\n%5sconstructed S() data: '%s'", "", data);
        }
        ~S() {
            printf("\n%5sdestructed ~S() data: '%s'", "", data);
            if (data) { free_data(data), data = nullptr; size = 0; }
        }

        // WARNING: it is important
        // to initialize the members, before anything else
        S(S&& other) noexcept 
            : size(move(other.size)),
            data(move(other.data))
        {
            printf("\n%5sconstructed %s ( S && S{}) ,  data: '%s'", "", __func__, data);
            // CRITICAL: this is signal to the destructor of the other
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
        /// WARNING: copy assignment operator is
        // required by the swap's bellow
        constexpr S& operator=(const S&) = default;

        friend void swap(S* a, S* b) {
            printf("\nswap( S * a, S * b)");
            swap(a->data, b->data);
            swap( a->size, b->size );
        }

        friend void swap(S& a, S& b) {
            printf("\n swap( S & a, S & b)");
            swap(a.data, b.data);
            swap(a.size, b.size);
        }

    };
} // dbj
///----------------------------------------------------
dbj::S return_s(dbj::S s_)
{
    return s_; // move 
}
///----------------------------------------------------

///----------------------------------------------------
int main(int, char**) {
    {
        dbj::S x = return_s(dbj::S("ABC"));
        assert(3 == x.size);
    }
    {
        dbj::S x("White"), y("Black");
        swap(x, y);
        //assert(0xF == x.size);
    }
    {
        auto xp = new dbj::S("Red"), 
             yp = new dbj::S("Green");
        swap(xp, yp);
        //assert(0xF == x.size);
    }
    return 42;
 }