#!/bin/bash

# Note that enum.pl should be in PATH for this script to work and
# JHBUILD_SOURCES should be defined to contain the path to the root of the
# jhbuild sources.  Also this script should reside in
# gstreamermm/gstrearmer/src.

if [ -z "$JHBUILD_SOURCES" -o ! -x "`which enum.pl`" ]; then
  echo -e "JHBUILD_SOURCES must contain the path to the jhbuild sources and \
enum.pl\nneeds to be executable and in PATH."
  exit 1;
fi

DIR=`dirname "$0"`

PREFIX="$JHBUILD_SOURCES/gstreamer"
enum.pl "$PREFIX"/gst/*.h "$PREFIX"/libs/gst/{base,controller,dataprotocol,\
net,check}/*.h "$PREFIX"/plugins/elements/*.h > "$DIR/gst_enums.defs"

PREFIX="$JHBUILD_SOURCES/gst-plugins-base"
enum.pl "$PREFIX"/gst-libs/gst/{app,audio,cdda,fft,floatcast,interfaces,netbuffer,\
riff,rtp,rtsp,sdp,tag,pbutils,video}/*.h "$PREFIX"/gst/*/*.h \
"$PREFIX"/ext/*/*.h >> "$DIR/gst_enums.defs"

#Patch generated file.
patch $DIR/gst_enums.defs $DIR/gst_enums.defs.patch
