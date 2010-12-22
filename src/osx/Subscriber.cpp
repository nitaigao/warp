#include "Subscriber.h"

std::string Subscriber::receive()
{
  sleep(1);
  return "Some Computer";
}
