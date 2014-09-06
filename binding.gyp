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
            '/usr/local/Cellar/leptonica/1.71/include/leptonica', '/usr/local/Cellar/tesseract/3.02.02/include/tesseract'
          ]
        }]
      ]
    }
  ]
}
