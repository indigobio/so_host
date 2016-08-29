#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <dlfcn.h>
#include "comm.h"
#include "so_host.h"

using std::string;
using std::cerr;
using std::endl;
using std::vector;
using std::map;

void *open_so(const char *path);
string read_packet_str(vector<char>& buf);


class ConnectionClosedException: public std::exception
{
};

auto make_lookup_func(void *so_handle, map<string, so_host_api>& func_lookup) {
  return [so_handle, &func_lookup](string name) {
    if (func_lookup.count(name) == 0)
      func_lookup[name] = (so_host_api)dlsym(so_handle, name.c_str());
    return func_lookup[name];
  };
}

int main(int argc, char **argv) {
  void *so_handle = open_so(argv[1]);
  map<string, so_host_api> func_lookup;
  auto lookup_func = make_lookup_func(so_handle, func_lookup);
  vector<char> buf;

  try {
    while (true) {
      string func_name = read_packet_str(buf);
      string request = read_packet_str(buf);
      so_host_api func = lookup_func(func_name);
      func(NULL, request.c_str(), request.size(),
           [](void *context, const char *data, uint32_t data_len) {
             write_packet(data, data_len);
           });
    }
  } catch (ConnectionClosedException& e) {
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
