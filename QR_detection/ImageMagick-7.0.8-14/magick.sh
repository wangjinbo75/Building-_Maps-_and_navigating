#!/bin/sh
#
#  Copyright 1999-2018 ImageMagick Studio LLC, a non-profit organization
#  dedicated to making software imaging solutions freely available.
#
#  You may not use this file except in compliance with the License.  You may
#  obtain a copy of the License at
#
#    https://imagemagick.org/script/license.php
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#  Convenience script to verify the ImageMagick build before you install.  For
#  example:
#
#    magick.sh convert -size 640x480 gradient:black-yellow gradient.png

top_srcdir='/home/w/Building_Maps_and_navigating/QR_detection/ImageMagick-7.0.8-14'
top_builddir='/home/w/Building_Maps_and_navigating/QR_detection/ImageMagick-7.0.8-14'

MAGICK_CODER_MODULE_PATH='/home/w/Building_Maps_and_navigating/QR_detection/ImageMagick-7.0.8-14/coders'
MAGICK_CONFIGURE_SRC_PATH='/home/w/Building_Maps_and_navigating/QR_detection/ImageMagick-7.0.8-14/config'
MAGICK_CONFIGURE_BUILD_PATH='/home/w/Building_Maps_and_navigating/QR_detection/ImageMagick-7.0.8-14/config'
MAGICK_FILTER_MODULE_PATH='/home/w/Building_Maps_and_navigating/QR_detection/ImageMagick-7.0.8-14/filters'
DIRSEP=':'

PATH="${top_builddir}/utilities:${PATH}"

if test -n "$VERBOSE"
then
  echo "$@"
fi
env \
  LD_LIBRARY_PATH="${top_builddir}/MagickCore/.libs:${top_builddir}/MagickWand/.libs${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" \
  MAGICK_CODER_MODULE_PATH="${MAGICK_CODER_MODULE_PATH}" \
  MAGICK_CONFIGURE_PATH="${MAGICK_CONFIGURE_BUILD_PATH}${DIRSEP}${MAGICK_CONFIGURE_SRC_PATH}" \
  MAGICK_FILTER_MODULE_PATH="${MAGICK_FILTER_MODULE_PATH}" \
  PATH="${PATH}" \
  "$@"
