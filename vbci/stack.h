#pragma once

#include "ident.h"

#include <array>
#include <cstdint>
#include <vector>

namespace vbci
{
  struct Stack
  {
    struct Idx
    {
      size_t chunk;
      size_t offset;
    };

  private:
    static constexpr size_t ChunkSize = 1024;
    using Chunk = std::array<uint8_t, ChunkSize>;
    std::vector<Chunk> chunks;
    Idx top;

  public:
    Idx save();
    void restore(const Idx& idx);
    void* alloc(size_t size);
    Array* array(Location frame_id, uint32_t type_id, size_t size);
  };
}
