/**
 * serial_types.impl.h -- implementations for commonly-used types
 *
 * Copyright (c) 2023  Catherine Van West <catherine.vanwest@cooper.edu>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SERIAL_TYPES_IMPL_H
#define SERIAL_TYPES_IMPL_H

#include <cstddef>
#include <cstdint>

/* all serializations must implement the two functions shown here -- namely
 * - insert(), for inserting items into the serialization, and
 * - extract(), for extracting items from the serialization.
 * these can perform their operations (almost) however they want, so long as
 * both sides are consistent and the operations don't interfere with other
 * ones!
 */

/* XXX: this thing is... well, terrible, but whatever */
template<typename to_t, typename from_t>
to_t interpret_as(from_t value) {
	union {
		from_t from;
		to_t to;
	} converter;
	converter.from = value;
	return converter.to;
}

template<>
struct serialization<char> {
	static void insert(std::ostream& out, char c) {
		out.put(c);
	}

	static char extract(std::istream& in) {
		return static_cast<char>(in.get());
	}
};

template<>
struct serialization<int32_t> {
	static_assert(sizeof(int32_t) == 4*sizeof(char), "oh god why");

	static void insert(std::ostream& out, int32_t i) {
		char to_write[] = {
			static_cast<char>(i >> 24),
			static_cast<char>(i >> 16),
			static_cast<char>(i >> 8),
			static_cast<char>(i)
		};
		out.write(to_write, sizeof(to_write));
	}

	static int32_t extract(std::istream& in) {
		char read[4];
		in.read(read, sizeof(read));
		return (read[0] << 24)
		     + (read[1] << 16)
		     + (read[2] << 8)
		     +  read[3];
	}
};

template<>
struct serialization<float> {
	static_assert(sizeof(float) == 4*sizeof(char), "problems!");

	static void insert(std::ostream& out, float f) {
		// XXX: change this if IEEE changes their standard!
		serialization<int32_t>::insert(
			out, interpret_as<int32_t>(f)
		);
	}

	static float extract(std::istream& in) {
		// XXX: see above
		return interpret_as<float>(
			serialization<int32_t>::extract(in)
		);
	}
};

#endif
