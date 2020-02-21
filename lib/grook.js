const path = require('path')
const sharp = require('sharp')
const { toJp2 } = require('bindings')('grook')

const inputFile = path.resolve(__dirname, '../test/test.jpg')
console.log('input', inputFile)

;(async () => {
  const { data, info } = await sharp(inputFile)
    .raw()
    .toBuffer({ resolveWithObject: true })

  toJp2(data, info.width, info.height, info.channels, true)

  console.log('output')
})()
