#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <dlfcn.h>
#include <iostream>
#include <vector>
#include <functional>
#include <termios.h>
#include <map>
#include "erl_comm.h"

using std::map;
using std::string;
using std::vector;
using std::function;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;

typedef void (*result_consumer)(void *context,
                                const char *data,
                                uint32_t data_len);

typedef void (*pb_api)(void *context,
                       const char *data,
                       uint32_t data_len,
                       result_consumer consumer);

void *open_so(const char *path);
string read_packet_str(vector<char>& buf);


class ConnectionClosedException: public std::exception
{
};

auto make_lookup_func(void *so_handle, map<string, pb_api>& func_lookup) {
  return [so_handle, &func_lookup](string name) {
    if (func_lookup.count(name) == 0)
      func_lookup[name] = (pb_api)dlsym(so_handle, name.c_str());
    return func_lookup[name];
  };
}

int main(int argc, char **argv) {
  void *so_handle = open_so(argv[1]);
  map<string, pb_api> func_lookup;
  auto lookup_func = make_lookup_func(so_handle, func_lookup);
  vector<char> buf;

  try {
    while (true) {
      string func_name = read_packet_str(buf);
      // cerr << "Function: " << func_name << endl;
      string request = read_packet_str(buf);
      pb_api func = lookup_func(func_name);
      func(NULL, request.c_str(), request.size(),
           [](void *context, const char *data, uint32_t data_len) {
             write_packet(data, data_len);
           });
    }
  } catch (ConnectionClosedException& e) {
    // cerr << "Connection closed." << endl;
    // exit
  }
}

string read_packet_str(vector<char>& buf) {
  int nread = read_packet(buf);
  if (nread <= 0)
    throw ConnectionClosedException();
  return string(buf.data(), nread);
}

void *open_so(const char* path) {
  void *compute_handle = dlopen(path, RTLD_LAZY);
  if (compute_handle == NULL) {
    cerr << "failed to dlopen " << path << "  Error: " << dlerror() << endl;
    throw "dlopen failed";
  }
  return compute_handle;
}
