PWD=`pwd`

export LD_LIBRARY_PATH=$PWD:$LD_LIBRARY_PATH

chmod +x ./r-type_server
./r-type_server
