#include <message/message.hpp>


/*constructor destructor exchange_management*/
message::exchange_management::exchange_management() : size(0) {}
message::exchange_management::~exchange_management() {};


/*constructor destructor receive_management*/
message::receive_management::receive_management() : exchange_management(),
step(NOTHING_DONE), is_there_cl(NO), is_there_tf(NO), msg(""){}

message::receive_management::~receive_management(){}


/*constructor destructor send_management*/
message::send_management::send_management() : exchange_management(), step(0){}
message::send_management::~send_management(){}