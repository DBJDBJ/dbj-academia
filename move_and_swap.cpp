#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <utility>
// #include <algorithm>
#include "C:\\Program Files (x86)\\Visual Leak Detector\\include\\vld.h"
#include "std_lib_types.h"
#include "mytype.h"
using namespace std;
#ifdef __GNUC__
#include <cstdlib>
#include <cxxabi.h>
#else
#include <typeinfo>
#endif // __GNUC__
template<typename T>
static const char * nametype (T && t) {
    #ifdef __GNUC__
int     status{};
char   *realname{};
realname = abi::__cxa_demangle(
    typeid(T).name(), 0, 0, &status
);
    static string retval;
    retval = realname ;
    free(realname);
    return retval.c_str();
    #else
    return typeid(T).name();
    #endif
}

///-------------------------------------------------------------------
namespace dbj {

template<typename T>
void exchange(T & a, T & b) {
    T t = a;
    a = b ;
    b = t ;
}
//
// really exchage -- without moving
template <class T_>
void exchangex(T_& left_, T_& right_) 
{ 
    T_ temp_ = static_cast<T_&&>(left_);
    left_    = static_cast<T_&&>(right_);
    right_   = static_cast<T_&&>(temp_);
}
//
#define dbj_assign std::exchange

// FUNCTION TEMPLATE swap nicked from msvc std space
    template <class Ty, size_t S>
    void swap(Ty (&left)[S], Ty (&right)[S]) noexcept 
    {
        if (&left != &right) {
            Ty* first1 = left;
            Ty* Last1  = first1 + Size;
            Ty* first2 = right;
            for (; first1 != Last1; ++first1, ++first2) {
                std::iter_swap(first1, first2);
            }
        }
    }
} // dbj
///----------------------------------------------------
using namespace dbj;
//
auto natives_exchange()
    {
        using str = std::string;
        auto left = str("LEFT"), right = str("RIGHT") ;
        auto left_previous = dbj_assign(left,right);
        auto right_previous = dbj_assign(right, left_previous ) ;
        /* */
         dbj::exchange(left,right);
         /* */
        dbj::exchangex(left,right);
        // fool the debugger
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
    using std::swap;
  // swap( Mytype &, Mytype &)  
  // is UD and swaps only their data
    swap(x,y);
  // but std::swap swaps them completely
    std::swap(x,y);
  // the key point of WG21 contention is
  // is y usable or not at this point?
  // using MSVC STL it is not (2020 FEB 21)        
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
    // iter_swap will be calling Mytype swap
    std::iter_swap(xp, yp);
    /// xp and yp are NOT changed
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
#include <typeinfo>
#include <array>
#include <vector>

#define SWAP(A,B) do { \
printf("\n\nswaping instances of type: %s\n", nametype(A) );\
printf("\nBefore std::swap A: (%p)\"%s\"\tB: (%p)\"%s\"",A,A,B,B);\
std::swap(A, B);\
printf("\nBefore std::swap A: (%p)\"%s\"\tB: (%p)\"%s\"",A,A,B,B);\
} while(0)

void array_swap() {
{{
      char A[] { "ABC" };
      char B[] { "CBA"  };
      SWAP( A, B );
              // fool the debugger
        bool dumy{}; if (dumy) return;
}
    using std::swap ;
    std::vector A = {"Abra Ca!"}; std::vector B = {"Dabra ko babra?"};
    printf("\n\nswaping instances of type: %s\n", nametype(A) );
    //printf("\nBefore std::swap A: (%p)\"%s\"\tB: (%p)\"%s\"",&A,A.data(),&B,B.data());
    std::swap(A, B);
    //printf("\nBefore std::swap A: (%p)\"%s\"\tB: (%p)\"%s\"",&A,A.data(),&B,B.data());
            // fool the debugger
        bool dumy{}; if (dumy) return;

}
}
///----------------------------------------------------
int main(int, char**) 
{
array_swap();
natives_exchange();
native_moving();
std_move_shenanigans(); 
udt_value_swapping(); 
udt_pointer_swapping();
udt_iterator_swapping();
    return 42;
 }
///----------------------------------------------------
