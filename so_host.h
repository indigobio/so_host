#pragma once

#include <stdint.h>

typedef void (*so_host_consumer)(void *context,
                                const char *data,
                                uint32_t data_len);

typedef void (*so_host_api)(void *context,
                            const char *data,
                            uint32_t data_len,
                            so_host_consumer consumer);
