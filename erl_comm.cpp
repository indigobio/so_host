#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include "erl_comm.h"
#include <iostream>

using std::vector;

// read/write from/to file descriptors directly to avoid buffering
#define STDIN_FD 0
#define STDOUT_FD 1

// adapted from http://erlang.org/doc/tutorial/c_port.html

int read_packet(vector<char> &buf)
{
  char len_chars[4];
  if (read_exact(len_chars, 4) <= 0)
    return -1;
  unsigned int len = ntohl(*(unsigned int*)len_chars);
  buf.reserve(len);
  return read_exact(buf.data(), len);
}

int write_packet(const char *data, int len)
{
  int net_order_len = htonl(len);
  write_exact((char*)&net_order_len, 4);
  return write_exact(data, len);
}

int read_exact(char *buf, int len)
{
  int i, nread=0;

  do {
    if ((i = read(STDIN_FD, buf+nread, len-nread)) <= 0) // eof (0) or error (-1)
      return i;
    nread += i;
  } while (nread<len);

  return nread;
}

int write_exact(const char *buf, int len)
{
  int i, nwritten = 0;

  do {
    if ((i = write(STDOUT_FD, buf+nwritten, len-nwritten)) <= 0) // ??? (0) or error (-1)
      return i;
    nwritten += i;
  } while (nwritten<len);

  return nwritten;
}
