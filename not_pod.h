#pragma once
#pragma once

#include "common.h"

namespace dbj::not_pod {

	struct not_a_pod final {

		char* data{};

		~not_a_pod() {
			if (data) {
				dbj::free_data(data);
				data = nullptr;
			}
		}

		not_a_pod() = default;

		/// havig this provokes the need for 
		/// defaul constructor
		/// + the need for local swap imp.
		not_a_pod(not_a_pod && other) noexcept
		{
			/// pointer rewiring
			std::swap( data, other.data );
			/// signal to the other not to free the data
			other.data = nullptr;
		}

		friend void swap(not_a_pod & left, not_a_pod & right ) noexcept 
		{
			std::swap( left.data, right.data );
		}

	}; // not_a_pod

    ///
	static_assert(std::is_standard_layout<not_a_pod>{});
	/// static_assert(std::is_trivial<not_a_pod>{});
	/// is trivial is these two 
	/// NO
	/// static_assert(std::is_trivially_copyable<not_a_pod>{}, "not_a_pod not a pod");
	/// static_assert(std::is_trivially_constructible<not_a_pod>{}, "not_a_pod not a pod");

	inline not_a_pod create( char const * data_ = "TEST" ) noexcept 
	{ 
		not_a_pod retval_ ;
		retval_.data = dbj::allocate_data(data_);
		return retval_ ;
	}

	inline not_a_pod* create_ptr(char const* data_ = "TEST" ) noexcept
	{ return new not_a_pod{ create(data_) }; }

	inline void move_value() {
		not_a_pod specimen = create();
		/// calling move constructor with the
		/// result of std::movve
		not_a_pod rezult = std::move(specimen);
		/// both data pointers are now pointing to the same 
		/// heap allocated mem block
		/// that will be freed in 2 destructors 
		/// of two instances
		/// that will provoke an Access violation crash
		/// _ASSERTE(specimen.data == rezult.data);

		/// after move ctor is implemented specimen data is NULL
	}

	inline void move_pointer() {
		not_a_pod* specimen = create_ptr();
		not_a_pod* rezult = std::move(specimen);

		_ASSERTE(specimen == rezult);
		// do not erase twice
		delete specimen;
	}

	inline void swap_value() {
		not_a_pod left = create("LEFT");
		not_a_pod right = create("RIGHT");

		/// having move ctor
		/// provoked the need for default ctor
		/// and local swap
		using std::swap;
		swap(left, right);
	}

	inline void swap_pointer() {
		not_a_pod* left = create_ptr("LEFT");
		not_a_pod* right = create_ptr("RIGHT");
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

} // dbj::not_pod
