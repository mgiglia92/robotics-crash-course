# TODO: Variable shebang?
# How for each os?
#!/usr/bin/env sh
echo "Activating venv"

# TODO: Need to determine how .venv was created, windows directory, python_code/.venv/Scripts/activate
# For ubuntu directory is, python_code/.venv/bin/activate
cd $RCC
cd python_code
win_dir=".venv/Scripts/"
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