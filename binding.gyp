{
  'targets': [
    {
      # have to specify 'liblib' here since gyp will remove the first one :\
      'target_name': 'tesseract_bindings',
      'sources': [
        'src/tesseract_bindings.cc',
        'src/tesseract_baseapi.cc',
        'src/leptonica_pix.cc'
      ],
      'conditions': [
        ['OS=="win"', {
          # no Windows support yet...
        }, {
          'libraries': [
            '-llept', '-ltesseract'
          ],
          'include_dirs': [
            '/usr/include/leptonica', '/usr/include/tesseract'
          ]
        }]
      ]
    }
  ]
}