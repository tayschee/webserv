#include "message/exchange_management.hpp"

//use to be more readable
typedef message::exchange_management exchange_management;


/*parent class to receive and send content with child */

//constructor destructor
exchange_management::exchange_management(){}
exchange_management::exchange_management(const exchange_management &x){ (void)x; }
exchange_management::exchange_management::exchange_management(const internal_exchange *x) { (void)x; }
exchange_management::~exchange_management(){}

//operator =
exchange_management &exchange_management::operator=(const exchange_management &x){ (void)x; return *this; }



/*internal class not useful for parent but will be neccessary to child*/

//class constructor destructor
exchange_management::internal_exchange::internal_exchange() : msg() {}
exchange_management::internal_exchange::internal_exchange(const std::string &msg) : msg(msg) {}
exchange_management::internal_exchange::internal_exchange(const internal_exchange &x) : msg(x.msg){}
exchange_management::internal_exchange::~internal_exchange(){}

//operator=
exchange_management::internal_exchange	&
exchange_management::internal_exchange::operator=(const internal_exchange &x){ msg = x.msg; return *this; }

