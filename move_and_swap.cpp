#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <utility>
// #include <algorithm>
#include "C:\\Program Files (x86)\\Visual Leak Detector\\include\\vld.h"

// using namespace std;
///----------------------------------------------------
namespace dbj {
    struct Mytype;

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
            // ip_ = nullptr;
        }
    }
    ///----------------------------------------------------
    /// there are better designs
    /// Mytype is like it is for teaching purposes
    struct Mytype final 
    {
        using value_type = char ;
        // POINT: order here rules the oder of initialization
        // in the constructor initializer list
        // not the oder in the list itself!
        value_type * data{};

        Mytype() noexcept 
        {
            printf("\n%5sconstructed by default Mytype() data:nullptr", "");
        }

        // POINT: never forget the ctor initializer list
        explicit Mytype(const char * str_ ) noexcept :
            data(allocate_data(str_))
        {
            printf("\n%5sconstructed with Mytype(const char *) data: '%s'", "", data);
        }

        ~Mytype() noexcept {
            printf("\n%5sdestructed ~Mytype() with data: '%s'", "", data);
            /* if (data) */
            { free_data(data); /*data = nullptr;*/ }
        }

        // POINT: it is important
        // to initialize the members, before anything else
        Mytype(Mytype&& other) noexcept 
            : data(std::move(other.data))
        {
            printf("\n%5sconstructed with Mytype( Mytype && Mytype{}) ,  data: '%s'", "", data);
            // CRITICAL: this is signal to the destructor 
            // of the other object
            // NOT to attempt freeing the moved from data
            other.data = nullptr;
        }
        /* */
        Mytype(const Mytype& other_) noexcept
            : data(allocate_data(other_.data))
        {
            printf("\n%5sconstructed  Mytype(const Mytype& other_), data: '%s'", "", data);
        }
        /// POINT: copy assignment operator is
        /// required by the swap's
        constexpr Mytype& operator=(const Mytype&) = default;

        // use swap to clearly show the intent
        // swap is for swapping the values of this type
        // NOTE: std::iter_swap will call into here
        // just make sure ADL is respected
        friend void swap(Mytype& a, Mytype& b) noexcept {
           printf("\n swap( Mytype & a, Mytype & b)");
           std::swap(a.data, b.data);
        }

    };
} // dbj

///----------------------------------------------------
using namespace dbj;
///----------------------------------------------------
// follow with debugger
// see the std::move effect
auto move_shenanigans()
{
    Mytype * x = new Mytype("White"); 
    Mytype * y = new Mytype("Black");

    // pointer move    
    Mytype * xp_move_target = std::move(x);
    Mytype * yp_move_target = std::move(y);

    // value move
    Mytype xv_move_target = std::move(*x);
    Mytype yv_move_target = std::move(*y);


    delete x; delete y;
}
auto udt_value_swapping()
{
    Mytype x("White"), y("Black");

   // offer first the generic std::swap( T &, T& )
  // but than make compiler use your implementation
  // swap( Mytype &, Mytype &)  
    using std::swap;
    swap(x,y);
}
auto udt_pointer_swapping()
{
    Mytype * xp = new Mytype("White"), * yp = new Mytype("Black");

  // effectively calling
  // swap( Mytype & , Mytype &  )  
    using std::swap;
    swap( *xp, *yp );

    delete xp, delete yp;
}
/// -------------------------------------------------------------
auto udt_iterator_swapping()
{
    Mytype* xp = new Mytype("Red");
    Mytype* yp = new Mytype("Green");
    // superfluous
    using std::iter_swap ;
    // iter_swap is calling using Mytype swap
    std::iter_swap(xp, yp);
    /// xp and yp are not changed
    /// just the values they are pointing to
    delete xp; delete yp;
}

void pause () {
    system("@echo.");
    system("@pause");
    system("@cls");
    system("@echo.");
}
///----------------------------------------------------
int main(int, char**) {
   move_shenanigans(); pause();
   udt_value_swapping(); pause();
   udt_pointer_swapping();pause();
   udt_iterator_swapping();pause();
    return 42;
 }
///----------------------------------------------------
