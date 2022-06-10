#TODO: Add documentation to let students know to make a system variable RCC with the directory they installed the repository to
if [ "$(uname)" == "Darwin" ]; then
    # Do something under Mac OS X platform 
    echo MACOS
    machine="MACOS"     
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under GNU/Linux platform
    echo LINUX
    machine="LINUX"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    # Do something under 32 bits Windows NT platform
    echo WIN32
    machine="WIN32"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    # Do something under 64 bits Windows NT platform
    echo WIN64
    machine="WIN64"
fi

echo machine_value$machine

cd $RCC
cd python_code

echo "Removing .venv"
rm -r .venv

echo "Making venv"


if [ $machine == "MACOS" ] || [ $machine == "LINUX" ];
then 
    python3 -m venv .venv
    source .venv/bin/activate
    echo "Linux/Mac, Installing required libraries"
    pip3 install wheel
    pip3 install -r requirements.txt
    echo "Install completed"
elif [ $machine == "WIN64" ] || [ $machine == "WIN32" ];
then 
    python -m venv .venv
    source .venv/Scripts/activate
    echo "Windows, Installing required libraries"
    pip install -r requirements.txt
    echo "Install completed"
else 
    echo "Error, machine type not valid"
fi

