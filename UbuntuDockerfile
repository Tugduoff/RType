# Use an official Ubuntu as the base image
FROM ubuntu:20.04

# Set environment variables for non-interactive installs
ENV DEBIAN_FRONTEND=noninteractive

# Update the package list and install dependencies for CMake, build tools, and SFML
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libsfml-dev \
    libudev-dev \
    libopenal-dev \
    libvorbis-dev \
    libflac-dev \
    libxrandr-dev \
    libxcursor-dev \
    libfreetype-dev \
    libfreetype6-dev \
    libftgl2 libxft2 libxft-dev freetype2-doc freetype2-demos \
    && apt-get clean

# Create a working directory inside the container
WORKDIR /app

# Clone your project repository (replace with your own repo URL)
# For example, if your sources are in GitHub, you would use:
# RUN git clone https://github.com/username/project-name.git .
# But for this example, I'll assume the sources are in a local context
COPY . .

RUN rm ./build -rf

# Create a build directory and navigate to it
RUN mkdir -p build && cd build && cmake ..

RUN cmake --build ./build/ --target all

# Set the default command to run after the container starts
# (Replace 'r-type_server' with your executable name if necessary)
# CMD ["/app/bin/r-type_client"]
