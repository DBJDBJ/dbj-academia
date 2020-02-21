#pragma once

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
            data = allocate_data(other_.data);
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

    };
} // dbj