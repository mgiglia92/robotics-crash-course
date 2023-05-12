/**
 * Copyright (c) 2023  Catherine Van West <catherine.vanwest@cooper.edu>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "serialize/serialize.h"

#include "cpp-base64/base64.h"
#include "hash-library/sha1.h"
#include "pico/stdlib.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

SHA1 sha1;

/* stolen from
 * https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
 * and modified to suit my needs.
 */
vector<string> split(string s, string delimiter) {
	size_t pos_start = 0;
	size_t pos_end;
	size_t delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
		// don't bother with empty tokens
		if (pos_end != pos_start) {
			token = s.substr(pos_start, pos_end - pos_start);
			res.push_back(token);
		}
		pos_start = pos_end + delim_len;
	}

	res.push_back (s.substr (pos_start));
	return res;
}

void serial_console() {
	bool switch_modes = false;
	while (!switch_modes) {
		ostringstream command_buf;
		cout << "pico:> ";

		bool end = false;
		while (!end) {
			const int next = cin.get();
			const char c = static_cast<char>(next);

			switch (next) {
			case '\n':
			case '\r':
			case EOF:
				end = true;
				cout << '\n';
				break;
			default:
				command_buf << c;
				cout << c;
			}
		}

		const string command_str = command_buf.str();
		auto command = split(command_str, " ");
		const string name = command[0];

		if (name.compare("switch-modes") == 0) {
			cout << "all right; switching modes now...\n";
			switch_modes = true;
		} else if (name.compare("stop") == 0) {
			cout << "well, this will work eventually...\n";
		} else if (name.compare("set-gain") == 0) {
			if (command.size() != 3) {
				cerr << "usage: set-gain ( -p | -i | -d ) <gain>\n";
			} else {
				float gain = stof(command[2]);

				switch (command[1].at(1)) {
				case 'p':
				case 'i':
				case 'd':
					cout << "setting gain to " << gain << '\n';
					break;
				default:
					cerr << "not a valid gain?";
				}
			}
		} else if (name.compare("parse") == 0) {
			if (command.size() != 2) {
				cerr << "usage: parse <packet>\n";
			} else {
				char c1, c2;
				int32_t i1;
				float f1;

				std::tie(c1, c2, i1, f1) = deserialize<
					char, char, int32_t, float
				>(base64_decode(command[1]));

				string repacked = base64_encode(serialize<
					char, char, int32_t, float
				>(std::make_tuple(c1, c2, i1, f1)));

				cout << "parsed"
				     << ": c1='" << c1
				     << "' c2='" << c2
				     << "' i1='" << i1
				     << "' f1='" << f1 << "'\n";
				cout << "repacked: " << repacked << '\n';
			}
		} else if (name.length() > 0) {
			cerr << "aaauaaugh!\n";
		}
	}
}

static const char start_tx = '\x01'; // SOH (start of heading)
static const char end_tx = '\x04'; // SOH (start of heading)

void packet_receiver() {
	bool switch_modes = false;
	while (!switch_modes) {
		// wait for the start to arrive
		while (cin.get() != start_tx);

		// read in the whole packet at once
		string raw_packet;
		getline(cin, raw_packet, end_tx);

		// decode it (TODO: actually do this right)
		int32_t packet_id;
		float field_1, field_2, field_3;

		tie(packet_id, field_1, field_2)
			= deserialize<int32_t, float, float>(
				base64_decode(raw_packet)
			);

		// send an acknowledgement back
		string ack = base64_encode(
			serialize<int32_t, float, float, float>(
				make_tuple(
					377l,
					field_2,
					field_1 + field_2,
					exp(1.0f)
				)
			)
		);
		cout.put(start_tx);
		cout << ack;
		cout.put(end_tx);
	}
}

int main() {
	stdio_init_all();

	while (true) {
		serial_console();
		packet_receiver();
	}

	return 0;
}
