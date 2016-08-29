#include <string>
#include "../so_host.h"

using std::string;

extern "C" {
  void echo(void *context, const char *data, uint32_t data_len, so_host_consumer consumer);
}

void echo(void *context, const char *data, uint32_t data_len, so_host_consumer consumer) {
  string msg(data, data_len);
  string response("you said: " + msg);
  consumer(context, response.data(), response.size());
}
