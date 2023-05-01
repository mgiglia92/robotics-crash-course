/**
 * messages.h -- contains message classes
 *
 * Copyright (c) 2023  Catherine Van West <catherine.vanwest@cooper.edu>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MESSAGES_H
#define MESSAGES_H

#include "packet.h"

#include <cstdint>

struct Test_Inbound {
	Test_Inbound(const Packet&);

	float field_1;
	float field_2;
};

struct Test_Outbound {
	Packet pack();

	float field_1;
	float field_2;
	float field_3;
};

struct Position {
	static constexpr const int32_t id = 93;

	Position(const Packet&);

	float x_trans, y_trans, z_trans;
	float x_rot, y_rot, z_rot;
	float w;
	int32_t frame_number;
};

struct Simple_Move {
	static constexpr const int32_t id = 66;

	Simple_Move(const Packet&);

	float curvature;
	float distance;
	float speed;
};

struct Stop {
	static constexpr const int32_t id = 666;
};

struct MotorControl {
	static constexpr const int32_t id = 420;
	MotorControl(const Packet&);

	float leftPwr;
	float rightPwr;
};

#endif
