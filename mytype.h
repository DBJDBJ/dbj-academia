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
            //
            // CRITICAL! this is a bug
            // other and this might be the same object!
            //
            // : data(std::move(other.data))
        {
            if ( this == & other )
                     return ;

            std::swap( data, other.data ) ;
            //
            // CRITICAL!
            //  this is signal to the destructor 
            // of the other object
            // NOT to attempt freeing the moved from data
            other.data = nullptr;
            printf("\n%5sconstructed with Mytype( Mytype && Mytype{}) ,  data: '%s'", "", data);
        }
        /* */
        Mytype(const Mytype& other_) noexcept
            // BUG! same as above : data(allocate_data(other_.data))
        {
            if ( this == & other_ )
                     return ;
            data = allocate_data(other_.data);
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