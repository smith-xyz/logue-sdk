/*
 * File: silent.cpp
 *
 * Naive silent oscillator test
 *
 */

#include "userosc.h"

void OSC_INIT(uint32_t platform, uint32_t api)
{
  (void)platform;
  (void)api;
}

void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{
  q31_t * __restrict y = (q31_t *)yn;
  const q31_t * y_e = y + frames;
  
  for (; y != y_e; ) {
    *(y++) = 0;
  }
}