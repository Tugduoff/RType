#!/usr/bin/env bash
# Fedora Compilation

docker build --file FedoraDockerfile -t fedora_compilation . --progress plain
docker container rm fedora_build -f > /dev/null
docker run --name fedora_build fedora_compilation

rm -f ./fedora_build.tar
docker cp fedora_build:/app/fedora_build.tar .


# Ubuntu Compilation

docker build --file DebianDockerfile -t debian_compilation . --progress plain
docker container rm debian_build -f > /dev/null
docker run --name debian_build debian_compilation

rm -f ./debian_build.tar
docker cp debian_build:/app/debian_build.tar .
