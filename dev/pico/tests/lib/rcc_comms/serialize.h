/**
 * serialize.h -- contains interfaces for serializing/deserializing objects.
 * (Note that these *are* actually necessary, as we're working across
 * compilers!)
 *
 * Copyright (c) 2023  Catherine Van West <catherine.vanwest@cooper.edu>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <string>
#include <istream>
#include <tuple>

/*
 * serialization{} -- base serialization type
 *
 * See the implementation header for more details on implementing these for
 * user-specified types.
 */
template <typename working_t>
struct serialization {};

/*
 * serialize() -- takes a std::tuple<...> and unifies it into a binary packet
 *
 * (According to the implemented serializations, that is.) Either a
 * string-format packet or a stream works here -- pass (and ask for) whatever's
 * most convenient. (They're both stream-oriented behind the scenes.)
 */
template <typename... fields>
void serialize(std::ostream&, const std::tuple<fields...>&);

template <typename... fields>
std::string serialize(const std::tuple<fields...>&);

/*
 * deserialize() -- takes a binary packet and splits it into fields
 * 
 * (See serialize() for comments on the interface.)
 */
template <typename... fields>
std::tuple<fields...> deserialize(std::istream&);

template <typename... fields>
std::tuple<fields...> deserialize(const std::string&);

#include "serialize.impl.h"
#include "serial_types.impl.h"

#endif
