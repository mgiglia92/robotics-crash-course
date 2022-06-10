#TODO: variable shebang???\
# How for each os?
#!/usr/bin/env sh

#TODO: Add documentation to let students know to make a system variable RCC with the directory they installed the repository to
cd $RCC
cd python_code

echo "Removing .venv"
rm -r .venv



# Activate venv to install, depends on windows or ubuntu
echo "Activating venv"
in_dir=".venv/Scripts/"
lin_dir=".venv/bin/"

if [ -d $win_dir ];
then
    echo "Windows" 
    echo "Making .venv"
    python -m venv .venv
    source .venv/Scripts/activate
elif [ -d $lin_dir ];
then 
    echo "Linux"
    echo "Making .venv"
    python3 -m venv .venv
    source .venv/bin/activate
else 
    echo "Error, venv activate script not found"
    exit
fi

cd $RCC/python_code
echo "Installing required libraries"
pip install -r requirements.txt
echo "Install completed"