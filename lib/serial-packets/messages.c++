#include "messages.h"
#include "serialize.h"
#include <sstream>

Test_Inbound::Test_Inbound(const Packet& p) {
	std::tie(
		field_1, field_2
	) = deserialize<
		float, float
	>(p.data());
}

Packet Test_Outbound::pack() {
	return Packet(
		Test_Outbound::id,
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

Twist::Twist(){

}

Twist::Twist(const Packet& p){
	std::tie(linear, angular) = deserialize<float, float>(p.data());
}

Packet Twist::pack(){
		return Packet(
		Twist::id,
		serialize<
			float, float
		>(std::make_tuple(
			linear,angular
		))
	);
	}

std::string Twist::repr()
{
	std::stringstream ss;
	ss << "Twist<linear: " << linear << " angular: " << angular << ">\n";
	return ss.str();
}