#include <grok.h>
#include <string.h>

void set_encoder_params(grk_cparameters* params, const bool lossless, const uint32_t channels) {
  params->cp_disto_alloc = 1;
  params->cod_format = 1;
  params->subsampling_dx = 1;
  params->subsampling_dy = 1;
  strncpy(params->outfile, "test.jp2", sizeof(params->outfile)-1);

  if (lossless) {
    params->tcp_numlayers = 1;
    params->tcp_rates[0] = 0;
  } else {
    params->tcp_numlayers = 5;
    params->tcp_rates[0] = 1920;
    params->tcp_rates[1] = 480;
    params->tcp_rates[2] = 120;
    params->tcp_rates[3] = 30;
    params->tcp_rates[4] = 10;
    params->irreversible = 1;
    if (channels >= 3) {
      params->tcp_mct = 1;
    }
  }
}
