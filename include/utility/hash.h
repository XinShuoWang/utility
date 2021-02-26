//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_HASH_H_
#define UTILITY_INCLUDE_UTILITY_HASH_H_

#define XXH_INLINE_ALL
#include "xxhash.h"

class Hash {
 private:
  inline static int DecodeFixed32(const char *ptr, uint32_t &result){
    const uint8_t *const buffer = reinterpret_cast<const uint8_t *>(ptr);
    // Recent clang and gcc optimize this to a single mov / ldr instruction.
    result = (static_cast<uint32_t>(buffer[0])) |
        (static_cast<uint32_t>(buffer[1]) << 8) |
        (static_cast<uint32_t>(buffer[2]) << 16) |
        (static_cast<uint32_t>(buffer[3]) << 24);
    return 0;
  }

 public:
  inline static uint32_t Murmur(const char *data, size_t n, uint32_t seed){
    // Similar to murmur hash
    const uint32_t m = 0xc6a4a793;
    const uint32_t r = 24;
    const char *limit = data + n;
    uint32_t h = seed ^ (n * m);
    // Pick up four bytes at a time
    while (data + 4 <= limit) {
      uint32_t w;
      DecodeFixed32(data, w);
      data += 4;
      h += w;
      h *= m;
      h ^= (h >> 16);
    }
    // Pick up remaining bytes
    if (limit - data == 3) {
      h += static_cast<uint8_t>(data[2]) << 16;
      h += static_cast<uint8_t>(data[1]) << 8;
      h += static_cast<uint8_t>(data[0]);
      h *= m;
      h ^= (h >> r);
    } else if (limit - data == 2) {
      h += static_cast<uint8_t>(data[1]) << 8;
      h += static_cast<uint8_t>(data[0]);
      h *= m;
      h ^= (h >> r);
    } else if (limit - data == 1) {
      h += static_cast<uint8_t>(data[0]);
      h *= m;
      h ^= (h >> r);
    }
    return h;
  }
  inline static uint32_t XXHash(const char *data, size_t n, uint32_t seed){
    return XXH32(data, n, seed);
  }

  inline static uint32_t XXHash(const char *data, size_t n){
    return XXH32(data, n, 9);
  }

  inline static uint32_t FastHash(const char* data, uint32_t n){
    return (*reinterpret_cast<uint32_t*>(const_cast<char*>(data)));
  }
};

#endif //UTILITY_INCLUDE_UTILITY_HASH_H_
