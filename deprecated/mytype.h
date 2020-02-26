#pragma once

///----------------------------------------------------
namespace dbj::udt {


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

        /// CONSTRUCTING
        Mytype() noexcept : data(nullptr)
        {
            printf("\n%5sconstructed by default Mytype() data:nullptr", "");
        }

        // POINT: never forget the ctor initializer list
        explicit Mytype(const char * str_ ) noexcept :
            data(allocate_data(str_))
        {
            printf("\n%5sconstructed with Mytype(const char *) data: '%s'", "", data);
        }

        /// DESTRUCTING
        ~Mytype() noexcept {
            printf("\n%5sdestructed ~Mytype() with data: '%s'", "", data);
            /// CRITICAL: see the moving where data = nullptr
            if (data) 
            { free_data(data); data = nullptr; }
        }

        /// MOVING
        /// CRITICAL! Also called from inside std::swap
        Mytype(Mytype&& other) noexcept 
           // this is ok but superfluous as member
           // init `char * data{}` will be called if this is not here
        : data(nullptr)
            //
            // CRITICAL! this is a bug
            // other and this might be the same object!
            //
            // : data(std::move(other.data))
        {
            if ( this == & other ) return ;

            std::swap( data, other.data ) ;
            //
            // CRITICAL! At this point using MSVC STL
            // other.data == nullptr
            // at least in debug build
            //
            // CRITICAL!
            //  this is signal to the destructor 
            // of the other object
            // NOT to attempt freeing the moved from data
            other.data = nullptr;
            printf("\n%5smove constructed from: '%s'", "", data);
        }
        constexpr Mytype& operator =(Mytype&& other) noexcept 
        {
            if ( this == & other ) return *this ;
            // this is pointer rewiring 
            std::swap( data, other.data ) ;
            //
            // CRITICAL!
            //  this is signal to the destructor 
            // of the other object
            // NOT to attempt freeing the moved from data
            other.data = nullptr;
            printf("\n%5smove assigned from: '%s'", "", data);
            return *this ;
        }

        /// COPYING
        Mytype(const Mytype& other_) noexcept
            // BUG! same as above : data(allocate_data(other_.data))
        {
            if ( this == & other_ ) return ;
            data = allocate_data(other_.data);
            printf("\n%5scopy constructed from: '%s'", "", data);
        }

        constexpr Mytype& operator=(const Mytype& other_ ) 
        {
              if ( this == &other_ ) return *this ;
            /// this is not a ctor! so we are likely to have 
              /// some data already in here
              if (data) {
                  free_data(data);
              }
              /// data is free'd above 
              /// but do we have anything to copy over?
              if (other_.data) {
                  data = allocate_data(other_.data);
              }
            printf("\n%5scopy assigned  from: '%s'", "", other_.data);
            return *this ;
        }

        // use swap to clearly show the intent
        // NOTE: std::iter_swap will call into here
        // just make sure ADL is respected
        //
        // Mytype *x, *y ;
        // ... make them somehow ...
        // std::iter_swap(x,y) ;
        // 
        friend void swap(Mytype& a, Mytype& b) noexcept {
           printf("\n swaping from: %s to: %s", b.data, a.data );
           std::swap(a.data, b.data);
        }

    }; // Mytype

    static_assert(std::is_destructible<Mytype>{});
    static_assert(std::is_default_constructible<Mytype>{});
    static_assert(std::is_copy_constructible<Mytype>{});
    static_assert(std::is_copy_assignable<Mytype>{});
    static_assert(std::is_move_constructible<Mytype>{});
    static_assert(std::is_move_assignable<Mytype>{});

    ///-----------------------------------------
    namespace {
        inline auto value_moving() {

            Mytype x = Mytype("White");
            // Mytype value move
            // xv_move_target move ctor is called
            // inside it
            // x.data is set to null
            // target data is relinked to what was x data
            Mytype xv_move_target = std::move(x);

            _ASSERTE(x.data == nullptr);
        }

        inline auto pointer_moving() {

            Mytype* x = new Mytype("White");
            // pointer move    
            Mytype* xp_move_target = std::move(x);
            // same pointers
            _ASSERTE(x == xp_move_target);

            delete x;
        }

        ///-----------------------------------------
        inline auto value_swapping()
        {
            using namespace dbj;
            Mytype x("White"), y("Black");

            // offer first the generic std::swap( T &, T& )
           // but than make compiler use your implementation
            using std::swap;
            // swap( Mytype &, Mytype &)  
            // is UD and swaps only their data
            swap(x, y);
            // but std::swap swaps them completely
            std::swap(x, y);
            // the key point of WG21 contention is
            // is y usable or not at this point?
            // using MSVC STL it is not (2020 FEB 21)        
        }
        ///-----------------------------------------
        inline auto pointer_swapping()
        {
            using namespace dbj;
            Mytype* xp = new Mytype("White"), * yp = new Mytype("Black");

            // effectively calling
            // swap( Mytype & , Mytype &  )  
            using std::swap;
            swap(*xp, *yp);

            delete xp, delete yp;
        }
        ///-----------------------------------------
        inline auto iterator_swapping()
        {
            using namespace dbj;
            Mytype* xp = new Mytype("Red");
            Mytype* yp = new Mytype("Green");
            // superfluous
            using std::iter_swap;
            // iter_swap will be calling Mytype swap
            std::iter_swap(xp->data, yp->data);
            std::iter_swap(xp, yp);
            /// xp and yp are NOT changed
            /// just the values they are pointing to
            delete xp; delete yp;
        }
    }
    inline void testing() {
        value_moving();
        pointer_moving();
        value_swapping();
        pointer_swapping();
        iterator_swapping();
    }
} // dbj::udt