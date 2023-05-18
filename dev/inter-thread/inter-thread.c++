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
	uint32_t id;
};

inter_thread_message::inter_thread_message(const Packet& p) {
	s = p.data();
	id = p.id();
}

inter_thread_message::inter_thread_message(const string& s)
: s(s) {}

Packet inter_thread_message::pack() {
	return Packet(id, s);
}

/*---------------------------------------------------------------------------*/

stringstream pipe;

void send_loop() {
	while (true) {
		inter_thread_message m(random_words(5));
		cout << "sending: " << m.s << '\n';
		pipe.str("");
		pipe.clear();
		pipe << m.pack();
		this_thread::sleep_for(1s);
	}
}

void recv_loop() {
	while (true) {
		Packet p;
		printf("Pipe data: %s\n", pipe.str().c_str());
		pipe >> p;
		inter_thread_message m(p);

		if (!pipe) {
			cout << "read failed, retrying...\n";
			this_thread::sleep_for(.5s);
		} else {
			cout << "received: " << m.s << '\n';
		}
	}
}

int main() {
	stringstream test;
	Packet p;
	test >> p;

	cout << "starting threads...\n";

	/*** CHANGE THESE LINES                               ***/
	thread tx(send_loop);
	thread rx(recv_loop);
	tx.join();
	rx.join();
	/***                    IF YOU'RE RUNNING ON THE PICO ***/

	return 0;
}
