# **Setting up your Pico Development Enviornment**

The first step is to open up your terminal. This can be done by searching for it by pressing the windows key and typing in the search bar or pressing `CTRL+ALT+t`

Within your terminal type the following commands:
```bash
sudo apt update

sudo apt install -y build-essential

sudo apt install git

sudo apt install screen

sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib
```

Now clone the Pico SDK and setup your environment variables

```bash
sudo git clone https://github.com/raspberrypi/pico-sdk.git /opt/pico-sdk

sudo git -C /opt/pico-sdk/ submodule update --init

echo 'export PICO_SDK_PATH=/opt/pico-sdk' | sudo tee -a /etc/profile.d/pico-sdk.sh

source /etc/profile.d/pico-sdk.sh
```

Lastly, clone the repo:

```bash
sudo git clone https://github.com/robotics-crash-course/rcc-pico.git

git submodule update --init --recursive
```

Your repo should be ready to go!

To check if CMake and Make are working, go to `/dev/pico` and create the build directory by typing:
```bash
mkdir build && cd build
cmake ..
```
If this runs without errors, CMake is working properly.

After that, try typing:
```bash
make -j12
```

If that works, you've now compiled a few C++ programs which you can upload to the Pico!