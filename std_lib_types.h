#pragma once
#include <string>
namespace {

///----------------------------------------------------
// follow with debugger
// see the std::move effect
inline auto std_move_shenanigans()
{ 
    std::string * x = new std::string("White"); 
    
    using std::move ;
    using std::iter_swap ;
    // pointer move 
    // is rewiring bu default   
    std::string * xp_move_target = move(x);
    // exchange allows
    // for null targets
    std::string * xv_exch_target{} ;
    std::exchange(xv_exch_target,x);

    delete x;
}
} // ns