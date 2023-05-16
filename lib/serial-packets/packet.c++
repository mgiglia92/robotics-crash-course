#include "packet.h"

#include "serialize.h"
#include "cpp-base64/base64.h"
#include "hash-library/sha1.h"

#include <tuple>
#include <utility>

static const char start_tx   = '\x01'; // SOH (start of heading)
static const char start_data = '\x02'; // STX (start of text)
static const char end_data   = '\x03'; // ETX (end of text)
static const char end_tx     = '\x04'; // EOT (end of transmission)

static SHA1 hash; // XXX: likely not thread-safe, but whatever...

Packet::Packet(int32_t id, const std::string& data)
: id_(id), data_(data) {}

std::string Packet::checksum() const {
	return hash(serialize<int32_t>(std::make_tuple(id_)) + data_);
}

bool Packet::checksum(const std::string& sum) const {
	return sum == checksum();
}

std::string Packet::str() const {
	std::ostringstream out;
	out << *this;
	return out.str();
}

bool Packet::read_from(const std::string& in) {
	return (std::istringstream(in) >> *this).operator bool();
}

std::ostream& operator << (std::ostream& out, const Packet& p) {
	return out
		<< start_tx
		<< base64_encode(serialize<int32_t>(std::make_tuple(p.id())))
		<< start_data
		<< base64_encode(p.data())
		<< end_data
		<< p.checksum()
		<< end_tx;
}

std::istream& operator >> (std::istream& in, Packet& p) {
	while (true) {
		// wait for the start to arrive
		while (true) {
			// expanded for debugging
			auto c = in.get();
			if (c == start_tx) break;

			// for consistency with other stream behavior, we'll
			// let the caller deal with it.
			if (!in) return in;
		}

		// read in the packet, section by section
		std::string b64_id;
		std::string b64_data;
		std::string checksum;

		std::getline(in, b64_id, start_data);
		std::getline(in, b64_data, end_data);
		std::getline(in, checksum, end_tx);

		// either verify the checksum or keep listening
		int32_t id;
		std::tie(id) = deserialize<int32_t>(base64_decode(b64_id));
		Packet p_in(id, base64_decode(b64_data));
		if (true || p_in.checksum(checksum)) {
			p = std::move(p_in);
			return in;
		}
	}
}
