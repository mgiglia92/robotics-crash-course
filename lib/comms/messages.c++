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
		distance, curvature, velocity, trans_id
	) = deserialize<
		float, float, float, int32_t
	>(p.data());
}

Packet Move_Feedback::pack() {
	return Packet(
		Move_Feedback::id,
		serialize<
			float, float, float, int32_t
		>(std::make_tuple(
			distance, curvature, velocity, trans_id
		))
	);
}
