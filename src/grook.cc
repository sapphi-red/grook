#include <napi.h>
#include <grok.h>

#include <iostream>

#include "grok_image.h"
#include "grok_codec_params.h"


static void error_callback(const char *msg, void *client_data)
{
    (void)client_data;
    std::cout << "error:" << msg << std::endl;
}
static void warning_callback(const char *msg, void *client_data)
{
    (void)client_data;
    std::cout << "warn:" << msg << std::endl;
}
static void info_callback(const char *msg, void *client_data)
{
    (void)client_data;
    std::cout << "info:" << msg << std::endl;
}


void ToJP2(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 5) {
    Napi::TypeError::New(env, "toJP2: 5 argument is required")
        .ThrowAsJavaScriptException();
    return;
  }
  if (!info[0].IsBuffer()) {
    Napi::TypeError::New(env, "toJP2: imageBuffer must be buffer")
        .ThrowAsJavaScriptException();
    return;
  }
  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "toJP2: width must be number")
        .ThrowAsJavaScriptException();
    return;
  }
  if (!info[2].IsNumber()) {
    Napi::TypeError::New(env, "toJP2: height must be number")
        .ThrowAsJavaScriptException();
    return;
  }
  if (!info[3].IsNumber()) {
    Napi::TypeError::New(env, "toJP2: channels must be number")
        .ThrowAsJavaScriptException();
    return;
  }
  if (!info[4].IsBoolean()) {
    Napi::TypeError::New(env, "toJP2: lossless must be boolean")
        .ThrowAsJavaScriptException();
    return;
  }

  const auto input_image_buffer = info[0].As<Napi::Buffer<uint8_t>>();
  printf("%u %u\n", input_image_buffer.Data(), input_image_buffer.Length());
  const auto data = static_cast<const uint8_t*>(input_image_buffer.Data());
  const auto width = info[1].ToNumber().Uint32Value();
  const auto height = info[2].ToNumber().Uint32Value();
  const auto channels = info[3].ToNumber().Uint32Value();
  const auto lossless = info[4].ToBoolean().Value();

  if (channels > 5) {
    Napi::TypeError::New(env, "toJP2: channels must be less than 5")
        .ThrowAsJavaScriptException();
    return;
  }

  grk_image_cmptparm cmptparm[5];
  set_default_cmptparm(cmptparm, width, height, channels);

  printf("cmptparm\n");

  grk_image* input_image = grk_image_create(channels, cmptparm, GRK_CLRSPC_SRGB);
  printf("image create\n");
  if (input_image == NULL) {
    Napi::TypeError::New(env, "toJP2: Failed to get image [grk_image_create]")
      .ThrowAsJavaScriptException();
  }
  printf("before image set\n");
  set_image_data(input_image, data, width, height, channels);
  printf("after image set\n");

  grk_codec* codec = grk_create_compress(GRK_CODEC_JP2);
  printf("after codec\n");

  grk_cparameters params;
  grk_set_default_encoder_parameters(&params);
  printf("after default encoder params\n");
  set_encoder_params(&params, lossless, channels);
  printf("after encoder params\n");

  grk_set_info_handler(codec, info_callback, nullptr);
  grk_set_warning_handler(codec, warning_callback, nullptr);
	grk_set_error_handler(codec, error_callback, nullptr);

  bool success = grk_setup_encoder(codec, &params, input_image);
  if (!success) {
    Napi::TypeError::New(env, "toJP2: concoder setup error")
        .ThrowAsJavaScriptException();
    // destructs
    grk_destroy_codec(codec);
    grk_image_destroy(input_image);
    return;
  }
  printf("after encoder\n");

  grk_stream* output_stream = grk_stream_create_default_file_stream(
    "D:\\test.jp2",
    false
  );
  printf("after output stream\n");
  if (!output_stream) {
    Napi::TypeError::New(env, "toJP2: output file error")
        .ThrowAsJavaScriptException();
    // destructs
    grk_stream_destroy(output_stream);
    grk_destroy_codec(codec);
    grk_image_destroy(input_image);
    return;
  }

  success = grk_start_compress(codec, input_image, output_stream);
  if (!success) {
    Napi::TypeError::New(env, "toJP2: compress error")
        .ThrowAsJavaScriptException();
    // destructs
    grk_stream_destroy(output_stream);
    grk_destroy_codec(codec);
    grk_image_destroy(input_image);
    return;
  }
  printf("after compress start\n");

  //success = grk_encode(codec, output_stream);
  printf("after encode start\n");
  success = grk_end_decompress(codec, output_stream);
  printf("after compress end\n");

  // destructs
  grk_stream_destroy(output_stream);
  grk_destroy_codec(codec);
  grk_image_destroy(input_image);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "toJp2"),
              Napi::Function::New(env, ToJP2));
  return exports;
}

NODE_API_MODULE(grok, Init)
