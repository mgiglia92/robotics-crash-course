#TODO: variable shebang???
#!/usr/bin/env sh

#TODO: Add documentation to let students know to make a system variable RCC with the directory they installed the repository to
cd $RCC
cd python_code

echo "Removing .venv"
rm -r .venv

echo "Making .venv"
python3 -m venv .venv

# Activate venv to install, depends on windows or ubuntu
echo "Activating venv"w
in_dir=".venv/Scripts/"
lin_dir=".venv/bin/"

if [ -d $win_dir ];
then
    echo "Windows" 
    source .venv/Scripts/activate
elif [ -d $lin_dir ];
then 
    echo "Linux"
    source .venv/bin/activate
else 
    echo "Error, venv activate script not found"
fi

cd $RCC/python_code
where python3
echo "Installing required libraries"
pip install -r requirements.txt