#!/usr/bin/env bash

BUILD_DIR=./build
BINARY_DIR=./bin

SFML_SO_FILES=(
  "libsfml-audio.so.2.6.1"
  "libsfml-graphics.so.2.6.1"
  "libsfml-network.so.2.6.1"
  "libsfml-window.so.2.6.1"
  "libsfml-system.so.2.6.1"
)

SO_FILES=(
  "libX11.so.6"
  "libXrandr.so.2"
  "libXcursor.so.1"
  "libXrender.so.1"
  "libGL.so.1"
  "libudev.so.1"
  "libfreetype.so.6"
  "libpng16.so.16"
)


# Compile the R-Type project
echo "R-Type Compilation..."

mkdir -p $BUILD_DIR
cd $BUILD_DIR && cmake .. && cd ..
cmake --build $BUILD_DIR

if [ $? -ne 0 ]; then
  echo "Compilation failed"
  exit 84
fi

# Find all sfml .so files and copy them to the binary directory
find $BUILD_DIR -type f -name "*.so*" -exec cp {} "$BINARY_DIR" \;
echo "All SFML .so files have been copied to $BINARY_DIR"

for SFML_SO_FILE in "${SFML_SO_FILES[@]}"; do
  if [ -f "$BINARY_DIR/$SFML_SO_FILE" ]; then
    # Remove the trailing '.1' to create the link name
    LINK_NAME="${SFML_SO_FILE%.1}"
    echo "Creating symbolic link for $SFML_SO_FILE as $LINK_NAME"
    
    ln -sf "./$SFML_SO_FILE" "$BINARY_DIR/$LINK_NAME"
  else
    echo "$SFML_SO_FILE not found in $BINARY_DIR"
  fi
done
echo "Symbolic links of SFML .so created."


# apt-file update
# Find all sfml dependencies .so files and copy them to the binary directory
for SO_FILE in "${SO_FILES[@]}"; do
  # Check if the file exists in /usr/lib64
  # SO_PATH=`apt-file search $SO_FILE | sed -E ':a;N;$!ba;s/^[^ ]+ ([^ ]+).*/\1/'`
  SO_APTFILE_SEARCH=`apt-file search $SO_FILE | tr '\n' ' '`
  SO_PATH=`echo $SO_APTFILE_SEARCH | sed -E 's/^[^ ]+ ([^ ]+).*/\1/'`
  echo "apt-file search $SO_FILE : `echo $SO_APTFILE_SEARCH`"
  if [ -f $SO_PATH ]; then
    echo "Copying $SO_FILE to $BINARY_DIR"
    cp "$SO_PATH" "$BINARY_DIR"
  else
    echo "$SO_FILE not found"
    echo "SO_PATH : '$SO_PATH'"
  fi
done

cp ./launch_server.sh $BINARY_DIR
cp ./launch_client.sh $BINARY_DIR
chmod +x $BINARY_DIR/launch_server.sh
chmod +x $BINARY_DIR/launch_client.sh

echo "Creating archive ubuntu_build.tar which will contain the application"
tar -cf ubuntu_build.tar $BINARY_DIR

# libsfml-graphics.so.2.6
# libsfml-network.so.2.6
# libsfml-window.so.2.6
# libsfml-system.so.2.6
# libX11.so.6
# libXrandr.so.2
# libXcursor.so.1
# libGL.so.1
# libudev.so.1
# libfreetype.so.6
# libstdc++.so.6
