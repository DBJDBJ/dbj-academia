
#include "common.h"
#include "mypod.h"
#include "not_pod.h"

///-----------------------------------------
auto intrinsic_value_moving () {
    char chr_ = "TEST"[0]; // chr_ == 'T'
    // rezulting value is exactly the same as
    // moved from value
    // move has returned char &&
    char char_move_target_ = DBJ_STL::move(chr_);
     _ASSERTE(char_move_target_ == chr_);
}
auto native_pointer_moving () {
    char * str_ = dbj::allocate_data("TEST");
    // rezulting pointer is exactly the same as
    // moved from pointer
    // move has returned char * &&
    char * pointer_ = DBJ_STL::move(str_);
    _ASSERTE(pointer_ == str_);
    dbj::free_data( str_ );
}

///----------------------------------------------------
int main(int, char**) 
{
    dbj::mypod::testing();
    dbj::not_pod::testing();
    
// dbj::udt::testing();
//
//native_pointer_moving();
//intrinsic_value_moving();
//
//dbj::array_swap();
//dbj::natives_exchange();
//std_move_shenanigans();
//dbj::pause();

    return 42;
 }
///----------------------------------------------------
