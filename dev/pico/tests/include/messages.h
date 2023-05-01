#include "serialize.h"
#include <tuple>
#include "wireless_comms.h"

using namespace std;

class Message{
    public:
        Message();
        char* send();
        bool receive();
        int32_t id;
};

class MotorControlMsg:Message{};
class LedStateMsg:Message{};
class 
