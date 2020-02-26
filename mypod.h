#pragma once

#include "common.h"

namespace dbj::mypod {

	/// 'capsule' is a Synonym  for POD
		struct capsule final {
			// for is_trivially_constructible
			// member initialization must not be used
			bool data;
		}; // capsule
	
	// POD is type for which both resolve to true
	static_assert(std::is_standard_layout<capsule>{} );
	static_assert(std::is_trivial<capsule>{} );
	/// is trivial is these two 
	/// static_assert(std::is_trivially_copyable<capsule>{}, "capsule not a pod");
	/// static_assert(std::is_trivially_constructible<capsule>{}, "capsule not a pod");
	
	inline capsule create(bool data_ = false) noexcept {	return {data_};	}
	inline capsule * create_ptr ( bool data_ = false ) noexcept { return new capsule{ data_ }; }

	inline void move_value() {
		capsule specimen = create();
		capsule rezult = std::move(specimen);
		_ASSERTE(specimen.data == rezult.data);
	}

	inline void move_pointer() {
		capsule * specimen = create_ptr();
		capsule * rezult = std::move(specimen);

		_ASSERTE( specimen == rezult );
		// do not erase twice
		delete specimen;
	}

	inline void swap_value() {
		capsule left  = create(false);
		capsule right = create(true);
		std::swap(left,right);
	}

	inline void swap_pointer() {
		capsule * left  = create_ptr(false);
		capsule * right = create_ptr(true);
		std::iter_swap(left, right);

		delete left;
		delete right;
	}

	inline void testing() {
		move_value();
		move_pointer();
		swap_value();
		swap_pointer();
	}

} // dbj::mypod
