#!/bin/sh
echo "Configuring with meson:"
if ./build-meson.sh; then
  echo "Running ninja: "
  cd build
  ninja
  cd ..
fi
