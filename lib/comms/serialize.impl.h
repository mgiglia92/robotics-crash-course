/**
 * serialize.impl.h -- implements the serialization/deserialization
 *
 * This file uses a small amount of black magic to present a pretty interface.
 * Apologies in advance if you have to work on it.
 *
 * Copyright (c) 2023  Catherine Van West <catherine.vanwest@cooper.edu>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SERIALIZE_IMPL_H
#define SERIALIZE_IMPL_H

#include <sstream>

/* these functions deal with creating a binary packet from a tuple. see the
 * notes in the next section for more of an overview of how these work (as i
 * wrote deserialize() first lol). */

template <typename tuple_t, std::size_t index>
void send_data(std::ostream&, const tuple_t&) {}

template <typename tuple_t,
          std::size_t index,
          typename next_type,
          typename... remaining>
void send_data(std::ostream& out, const tuple_t& packet) {
	serialization<next_type>::insert(out, std::get<index>(packet));
	send_data<tuple_t, index + 1, remaining...>(out, packet);
}

/*---------------------------------------------------------------------------*/

/* these functions deal with actually filling out the packet tuple -- they
 * iteratively consume one argument from the template list, attempt to extract
 * that type from the packet, and return how many bytes have been consumed so
 * far.
 */

/* base case -- no types left in the list, so do nothing and be done with it */
template <typename tuple_t, std::size_t index>
void fill_packet(std::istream&, tuple_t&) {}

/* induction case -- we have at least one type left to extract, so pull it and
 * keep going */
template <typename tuple_t,
          std::size_t index,
          typename next_type,
          typename... remaining>
void fill_packet(std::istream& in, tuple_t& packet) {
	/* extract the next thing, increment the tuple index by 1, and go to
	 * the next type */
	std::get<index>(packet) = serialization<next_type>::extract(in);
	fill_packet<tuple_t, index + 1, remaining...>(in, packet);
}

/*---------------------------------------------------------------------------*/

/* finally, we actually call everything from here. */

template <typename... fields>
void serialize(std::ostream& out, const std::tuple<fields...>& packet) {
	send_data<decltype(packet), 0, fields...>(out, packet);
}

template <typename... fields>
std::string serialize(const std::tuple<fields...>& packet) {
	std::ostringstream out;
	serialize<fields...>(out, packet);
	return out.str();
}

template <typename... fields>
std::tuple<fields...> deserialize(std::istream& in) {
	std::tuple<fields...> packet;
	fill_packet<decltype(packet), 0, fields...>(in, packet);
	return packet;
}

template <typename... fields>
std::tuple<fields...> deserialize(const std::string& bytes) {
	std::istringstream in(bytes);
	return deserialize<fields...>(in);
}

#endif
