#pragma once
///----------------------------------------------------
#include "common.h"
///----------------------------------------------------
namespace dbj
{

	///----------------------------------------------------
	auto natives_exchange()
	{
		using str = DBJ_STL::string;
		auto left = str("LEFT"), right = str("RIGHT");
		auto left_previous = dbj_assign(left, right);
		auto right_previous = dbj_assign(right, left_previous);
		/* */
		dbj::exchange(left, right);
		/* */
		dbj::exchangex(left, right);
		// fool the debugger
		bool dumy{}; if (dumy) return;
	}

#define SWAP(A,B) do { \
printf("\n\nswaping instances of type: %s\n", nametype(A) );\
printf("\nBefore DBJ_STL::swap A: (%p)\"%s\"\tB: (%p)\"%s\"",A,A,B,B);\
DBJ_STL::swap(A, B);\
printf("\nAfter DBJ_STL::swap A: (%p)\"%s\"\tB: (%p)\"%s\"",A,A,B,B);\
} while(0)

	void array_swap()
	{
		{
			char A[]{ "ABC" };
			char B[]{ "CBA" };
			SWAP(A, B);
			// fool the debugger
			bool dumy{}; if (dumy) return;
		}
		using DBJ_STL::swap;
		DBJ_STL::array<char, 4> A{ 'A','B','C', char(0) }, B{ 'C','B','A', char(0) };
		printf("\n\nswaping instances of type: %s\n", nametype(A));
		printf("\nBefore DBJ_STL::swap A: (%p)\"%s\"\tB: (%p)\"%s\"", &A, A.data(), &B, B.data());
		DBJ_STL::swap(A, B);
		printf("\nAfter DBJ_STL::swap A: (%p)\"%s\"\tB: (%p)\"%s\"", &A, A.data(), &B, B.data());
		// fool the debugger
		bool dumy{}; if (dumy) return;
	}

} // dbj

#undef SWAP