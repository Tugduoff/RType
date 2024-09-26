docker build --file FedoraDockerfile -t fedora_compilation .
docker container rm fedora_build -f
docker run --name fedora_build fedora_compilation

rm -f ./fedora_build.tar

docker cp fedora_build:/app/fedora_build.tar .