#pragma once

#include <vector>

int read_packet(std::vector<char>& buf);

int write_packet(const char *data, int len);

int read_exact(char *data, int len);

int write_exact(const char *data, int len);
