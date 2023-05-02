#include "messages.h"

#include "serialize.h"

Test_Inbound::Test_Inbound(const Packet& p) {
	std::tie(
		field_1, field_2
	) = deserialize<
		float, float
	>(p.data());
}

Packet Test_Outbound::pack() {
	return Packet(
		377,
		serialize<
			float, float, float
		>(std::make_tuple(
			field_1, field_2, field_3
		))
	);
}

Position::Position(const Packet& p) {
	std::tie(
		x_trans, y_trans, z_trans,
		x_rot, y_rot, z_rot,
		w,
		frame_number
	) = deserialize<
		float, float, float,
		float, float, float,
		float,
		int32_t
	>(p.data());
}

Simple_Move::Simple_Move(const Packet& p) {
	std::tie(
		distance, curvature, speed
	) = deserialize<
		float, float, float
	>(p.data());
}

MotorControl::MotorControl(const Packet& p)
{
	std::tie(leftPwr,rightPwr) = deserialize<float, float>(p.data());
}