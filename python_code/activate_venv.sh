if [ "$(uname)" == "Darwin" ]; then
    # Do something under Mac OS X platform 
    echo MACOS
    machine=MACOS     
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under GNU/Linux platform
    echo LINUX
    machine=LINUX
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    # Do something under 32 bits Windows NT platform
    echo WIN32
    machine=WIN32
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    # Do something under 64 bits Windows NT platform
    echo WIN64
    machine=WIN64
fi

echo "Activating venv"

cd $RCC
cd python_code

if machine==WIN64 || machine==WIN32;
then 
    source .venv/Scripts/activate
elif machine==MACOS || machine==LINUX;
then 
    source .venv/bin/activate
else     
    echo "Error, machine type not valid"
    exit
fi