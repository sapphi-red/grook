#pragma once

#include <napi.h>
#include <grok.h>

void set_default_cmptparm(
  grk_image_cmptparm* cmptparm,
  const uint32_t width,
  const uint32_t height,
  const uint32_t channels
);

void set_image_data(
  grk_image* image,
  const uint8_t* data,
  const uint32_t width,
  const uint32_t height,
  const uint32_t channels
);
