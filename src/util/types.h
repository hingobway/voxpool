#pragma once

// very important: number of active channels allocated in audio/graphics.
// must be <= the number of buses and params.
#define NUM_CHANNELS (8)

namespace types
{
  struct MeterVal
  {
    float pool;
    float level;
  };
}
