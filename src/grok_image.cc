#include <napi.h>
#include <grok.h>

void set_default_cmptparm(
  grk_image_cmptparm* cmptparm,
  const uint32_t width,
  const uint32_t height,
  const uint32_t channels
) {
  for (uint32_t i = 0; i < channels; i++) {
    auto p = &cmptparm[i];
    p->dx = 1;
    p->dy = 1;
    p->w = width;
    p->h = height;
    p->x0 = 0;
    p->y0 = 0;
    p->prec = 8;
    p->sgnd = 0;
  }
}

void set_image_data(
  grk_image* image,
  const uint8_t* data,
  const uint32_t width,
  const uint32_t height,
  const uint32_t channels
) {
  image->x0 = 0;
  image->y0 = 0;
  image->x1 = width;
  image->y1 = height;
  const uint64_t pixels = static_cast<uint64_t>(width) * static_cast<uint64_t>(height);

  for (uint32_t i = 0; i < channels; i++) {
    printf("%i %u\n", image->comps[i].data, image->comps[i].w);
  }

  for (uint32_t i = 0; i < channels; i++) {
    std::copy(data + i * pixels, data + (i + 1) * pixels, image->comps[i].data);
    printf("%u %u %i\n", data + i * pixels, data + (i + 1) * pixels, image->comps[i].data);
  }
}
