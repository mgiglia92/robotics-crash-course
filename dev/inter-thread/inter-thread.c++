/* A short test program for multi-core message passing.
 *
 * Copyright (C) 2023  Catherine Van West <cat@vanwestco.com>
 */
// SPDX-License-Identifier: GPL-3.0-or-later

#include "random-words.h"

#include "serial-packets/packet.h"
#include "serial-packets/serialize.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;
using namespace std::chrono_literals;

/*---------------------------------------------------------------------------*/

struct inter_thread_message {
	inter_thread_message() = default;
	inter_thread_message(const Packet&);
	inter_thread_message(const string& m);

	Packet pack();

	string s;
};

inter_thread_message::inter_thread_message(const Packet& p) {
	s = p.data();
}

inter_thread_message::inter_thread_message(const string& s)
: s(s) {}

Packet inter_thread_message::pack() {
	return Packet(99, s);
}

/*---------------------------------------------------------------------------*/

stringstream pipe;

void send_loop() {
	while (true) {
		inter_thread_message m(random_words(5));
		cout << m.s << '\n';
		this_thread::sleep_for(1s);
	}
}

void recv_loop() {
	while (true) {
		this_thread::sleep_for(1s);
	}
}

int main() {
	cout << "starting threads...\n";

	thread tx(send_loop);
	thread rx(recv_loop);
	tx.join();
	rx.join();

	return 0;
}
