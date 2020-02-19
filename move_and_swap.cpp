#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <utility>
// #include <algorithm>
//  #include <vld.h>

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
    template <class _Ty>
    _NODISCARD constexpr std::remove_reference_t<_Ty>&& 
    move(_Ty&& _Arg) 
    noexcept { // forward _Arg as movable
        return static_cast<std::remove_reference_t<_Ty>&&>(_Arg);
    }
    template <class _Ty, class>
    void swap(_Ty& _Left, _Ty& _Right) 
        noexcept(
            std::is_nothrow_move_constructible_v<_Ty> && 
            std::is_nothrow_move_assignable_v<_Ty>
        ) 
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
    /// Mytype is like it is for teaching purposes
    struct Mytype final 
    {
        using value_type = char ;
        // POINT: order here rules the oder of initialization
        // in the constructor initializer list
        // not the oder in the list itself!
        value_type * data{};
        size_t size{};

        Mytype() noexcept  : 
            size(0),            // initialized first
            data(nullptr)       // initialized second
        {
            printf("\n%5sconstructed by default Mytype() data:nullptr", "");
        }

        // POINT: never forget the ctor initializer list
        explicit Mytype(const char * str_ ) noexcept :
            data(allocate_data(str_)),
            size(strlen(str_))
        {
            printf("\n%5sconstructed with Mytype(const char *) data: '%s'", "", data);
        }

        ~Mytype() {
            printf("\n%5sdestructed ~Mytype() with data: '%s'", "", data);
            /* if (data) */
            { free_data(data); /*data = nullptr;*/ size = 0; }
        }

        // POINT: it is important
        // to initialize the members, before anything else
        Mytype(Mytype&& other) noexcept 
            : size(std::move(other.size)),
            data(std::move(other.data))
        {
            printf("\n%5sconstructed with Mytype( Mytype && Mytype{}) ,  data: '%s'", "", data);
            // CRITICAL: this is signal to the destructor 
            // of the other object
            // NOT to attempt freeing the moved from data
            other.data = nullptr;
        }
        /* */
        Mytype(const Mytype& other_) noexcept
            : size(other_.size),
            data(allocate_data(other_.data))
        {
            printf("\n%5sconstructed  Mytype(const Mytype& other_), data: '%s'", "", data);
        }
        /// POINT: copy assignment operator is
        /// required by the swap's
        constexpr Mytype& operator=(const Mytype&) = default;

        // use iter_swap to clearly show the intent
        // iter_swap is for swapping the pointers to the type
        friend void iter_swap(Mytype* a, Mytype* b) {
           printf("\nswap( Mytype * a, Mytype * b)");
           using std::swap;
           swap(a->data, b->data);
           swap( a->size, b->size );
        }

        // use swap to clearly show the intent
        // swap is for swapping the value of the type
        friend void swap(Mytype& a, Mytype& b) {
           printf("\n swap( Mytype & a, Mytype & b)");
           std::swap(a.data, b.data);
           std::swap(a.size, b.size);
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
  // but than make compiler use your implementation
  // effectively doing
  // swap( Mytype & * Mytype & * )  
    using std::swap;
    swap( *xp, *yp );

    delete xp, delete yp;
}
/// -------------------------------------------------------------
auto udt_iterator_swapping()
{
    /// POINT: iter_swap udt implementation 
    /// assures
    /// xp and yp are not changed
    /// just the values they are pointing to
    Mytype* xp = new Mytype("Red");
    Mytype* yp = new Mytype("Green");

    // superfluous
    using std::iter_swap ;
    // using Mytype swap
    std::iter_swap(xp, yp);

    delete xp; delete yp;
}

///----------------------------------------------------
int main(int, char**) {
    // move_shenanigans();
   udt_value_swapping();
   udt_pointer_swapping();
   udt_iterator_swapping();
    return 42;
 }
///----------------------------------------------------
