# ofxNDI3

Send receive video frames over the network. Based on work by @thomasgeissl

## Description

ofxNDI is an openframeworks addon to receive and send NDI streams. It extends the ofVideoGrabber class.

## Tested On

Currently only linux and osx libs are added and tested. Theoretically it should work for windows too.

## Troubelshooting

You might need to add the following run script to your build phases in xcode or fix the addon_config.mk and PR.

# Copy libndi.dylib
cp $OF_PATH/addons/ofxNDI/libs/libndi/lib/osx/libndi.dylib $TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/

cd $TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/

install_name_tool -change @rpath/libndi.3.dylib @executable_path/libndi.3.dylib $PRODUCT_NAME

## License

This project is released under MIT license, please note that dependencies might be released differently.

Copyright (c) 2018 Thomas Geissl & Fabian Morón Zirfas

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
