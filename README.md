so_host
=======

Exposes shared object functions in a language-neutral way. The functions must
conform to the following `pb_api` interface.

```cpp
typedef void (*pb_api)(void *context,
                       const char *data,
                       uint32_t data_len,
                       result_consumer consumer);

typedef void (*result_consumer)(void *context,
                                const char *data,
                                uint32_t data_len);
```

Notably,

- The function receives
  - an opaque `context`
  - a `data` buffer containing the input, e.g., a serialized protobuf message
  - a `result_consumer` function to receive the output
  
- the consumer receives
  - the original `context`
  - a `data` buffer containing the output (also a protobuf serialized message)
  
The callback arrangement eliminates the need for caller/callee coordination on memory
management. The caller doesn't have to concern itself with allocating, freeing,
or sizing buffers. The consumer is expected to immediately read/decode the
buffer. Once the consumer returns, the callee frees the buffer.

_Note: Since so_host is written in C++, control over memory management is not an
issue. However, the same libraries hosted by so_host may be called directly by
managed languages like Java, in which case the callback design is advantageous._
  
The context allows the consumer to associate the result with the original
request (in case the request isn't lexically available).

The use of encoded messages separates marshaling from domain logic concerns.
`so_host` can host any version of any library as long as the exported functions
conform to the `pb_api` interface.


Usage
-----

so_host is intended to be spawned by a client library.

```
so_host /usr/lib/libfoo.so
```

The client communicates with the executable over stdin/stdout using
length-prefix framing. Length-prefix framing is a simple, ubiquitous way to
delimit data in a stream. Each frame is prefixed by a 32-bit, unsigned,
big-endian length.

A function call looks like:

**stdin**: `<len><function_name><len><data>`

**stdout**: `<len><response>`

The function name is a string suitable to be passed to
[`dlsym`](http://linux.die.net/man/3/dlsym).


Testing
-------

The repository contains a trivial `libecho` library containing an `echo`
function that is used by `test.sh` and can be used in client tests.
