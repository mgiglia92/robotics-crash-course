/**
 * packet.h -- contains interfaces for sending & receiving checksummed packets
 *
 * Copyright (c) 2023  Catherine Van West <catherine.vanwest@cooper.edu>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>

class Packet {
public:
	Packet() = default;
	Packet(int32_t id, const std::string& data);

	/*
	 * checksum() -- get packet checksum or verify the given checksum is
	 * the correct one for the packet
	 */
	std::string checksum() const;
	bool checksum(const std::string&) const;

	/*
	 * str(), read_from() -- identical to the stream operators below, but
	 * work with strings. read_from() will return false if the operation
	 * fails (i.e., the string does not contain a valid packet).
	 */
	std::string str() const;
	bool read_from(const std::string&);

	inline int32_t id() const { return id_; }
	inline const std::string& data() const { return data_; }

private:
	int32_t id_;
	std::string data_;
};

/*
 * operator << () -- write an outbound packet to the given stream
 */
std::ostream& operator << (std::ostream&, const Packet&);

/*
 * operator << () -- extract an inbound packet from the given stream
 *
 * note: this will block until a valid (well-formatted, checksum-verified)
 * packet arrives!
 */
std::istream& operator >> (std::istream&, Packet&);

#endif
