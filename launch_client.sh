PWD=`pwd`

export LD_LIBRARY_PATH=$PWD:$LD_LIBRARY_PATH

chmod +x ./r-type_client
./r-type_client
