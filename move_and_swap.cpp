#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <utility>
// #include <algorithm>
#include "C:\\Program Files (x86)\\Visual Leak Detector\\include\\vld.h"

#include "std_lib_types.h"
#include "mytype.h"
// using namespace std;


///----------------------------------------------------
using namespace dbj;

template<typename T>
void dbj_exchange(T & a, T & b) {
    T t = a;
    a = b ;
    b = t ;
}
auto natives_exchange()
    {
        auto left = "LEFT", right = "RIGHT" ;
        /*
        right is unchanged
        left == right
        previous is left previous
        auto left_previous = std::exchange(left,right);

        ditto one more call is required
        auto right_previous = std::exchange(right, left_previous ) ;
        */
         dbj_exchange(left,right);
        bool dumy{}; if (dumy) return;
    }

auto native_moving () {
    char * str_ = allocate_data("TEST");
        using std::move;
            char * pointer_ = move(str_);
    free_data( str_ );
}
auto udt_moving () {
    Mytype * x = new Mytype("White"); 
    using std::move ;
    // pointer move    
    Mytype * xp_move_target = move(x);
    // value move
    Mytype xv_move_target = move(*x);
    delete x; 
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
    /// xp and yp are changed
    /// just the values they are pointing to
    delete xp; delete yp;
}

void pause ( bool cls = true ) {
    system("@echo.");
    system("@pause");
    if (cls) system("@cls");
    system("@echo.");
}
///----------------------------------------------------
int main(int, char**) 
{
natives_exchange();
native_moving();
std_move_shenanigans(); 
udt_value_swapping(); 
udt_pointer_swapping();
udt_iterator_swapping();
    return 42;
 }
///----------------------------------------------------
