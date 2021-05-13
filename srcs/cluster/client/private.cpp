#include "client.hpp"

void        client::reset_time() // reset the time
{
    gettimeofday(&time, NULL);
}