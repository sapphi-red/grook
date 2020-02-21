{
  "targets": [
    {
      "target_name": "grook",
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "sources": [
        "src/grook.cc",
        "src/grok_image.cc",
        "src/grok_codec_params.cc"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        '<(module_root_dir)/vendor/include/grok-3.1'
      ],
      "link_settings": {
        "ldflags": [
            "-Wl,-z,defs"
        ]
      },
      'conditions': [
        ['OS == "win"', {
          'defines': [
            '_ALLOW_KEYWORD_MACROS',
            '_FILE_OFFSET_BITS=64'
          ],
          'libraries': [
            '<(module_root_dir)/vendor/lib/grok.lib'
          ]
        }, {
          'defines': [
            '_GLIBCXX_USE_CXX11_ABI=0'
          ],
          'libraries': [
            '<(module_root_dir)/vendor/lib/libgrok.so.3.1.0'
          ]
        }]
      ]
    }
  ],
  'cflags_cc': [
    '-std=c++0x',
    '-fexceptions',
    '-Wall',
    '-O3'
  ],
  'xcode_settings': {
    'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
    'CLANG_CXX_LIBRARY': 'libc++',
    'MACOSX_DEPLOYMENT_TARGET': '10.7',
    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
    'GCC_ENABLE_CPP_RTTI': 'YES',
    'OTHER_CPLUSPLUSFLAGS': [
      '-fexceptions',
      '-Wall',
      '-O3'
    ]
  },
  'configurations': {
    'Release': {
      'conditions': [
        ['OS == "win"', {
          'msvs_settings': {
            'VCCLCompilerTool': {
              'ExceptionHandling': 1,
              'RuntimeTypeInfo': 'true'
            }
          },
          'msvs_disabled_warnings': [
            4275
          ]
        }]
      ]
    }
  }
}
