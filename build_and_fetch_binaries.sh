# Fedora Compilation

docker build --file FedoraDockerfile -t fedora_compilation . --progress plain
docker container rm fedora_build -f > /dev/null
docker run --name fedora_build fedora_compilation

rm -f ./fedora_build.tar
docker cp fedora_build:/app/fedora_build.tar .


# Ubuntu Compilation

docker build --file UbuntuDockerfile -t ubuntu_compilation . --progress plain
docker container rm ubuntu_build -f > /dev/null
docker run --name ubuntu_build ubuntu_compilation

rm -f ./ubuntu_build.tar
docker cp ubuntu_build:/app/ubuntu_build.tar .
