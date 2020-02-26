#pragma once
#include "common.h"
namespace {

///----------------------------------------------------
// follow with debugger
// see the DBJ_STL::move effect
inline auto std_move_shenanigans()
{ 
    // value
    {
    DBJ_STL::string x = DBJ_STL::string("White"); 
    // clears the string
    DBJ_STL::string move_target = DBJ_STL::move(x);
    /* fool the debugger */bool dumy{}; if (dumy) return;
    }
    // pointer
    {
    DBJ_STL::string * x = new DBJ_STL::string("White"); 
    DBJ_STL::string * move_target = DBJ_STL::move(x);
    /* fool the debugger */bool dumy{}; if (dumy) return;
    delete x ;
    }
    // pointer derefing
    {
    DBJ_STL::string * x = new DBJ_STL::string("White"); 
    // clears the string
    DBJ_STL::string   move_target = DBJ_STL::move(*x);
    /* fool the debugger */bool dumy{}; if (dumy) return;
    delete x ;
    }
}
} // ns