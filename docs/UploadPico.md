# **Building, Uploading and Monitoring the Pico**
## **Building**
To upload code to the Pico you must pre-compile and build it using CMake and Make.

### **CMake and Make**
CMake gathers libraries and file contignencies that your C/C++ program needs to be able to operate. It links all of these files together and passes them off to Make, which compiles them in the form of a `.uf2` file.

To build a program you must first add the program to your CMakeLists.txt file which is located in the `/dev/pico` direcotry.

At the bottom of the file you will use the macro called `make_exec(arg1 arg2)` and enter your specific arguments

`arg1` should be the name you want your `.uf2` file to be.

`arg2` should be the location of your file relative to `/dev/pico`.

Now, you need to create a build directory where you will run the CMake command (the build directory should be in `/dev/pico/buld`). Type the following:
```bash
mkdir build && cd build
cmake ..
```
Next, run:
```bash
make -j12
```
There we go, you should now have .uf2 files for each of the `make_exec` files you specified in your `CMakeLists.txt` file!


---

The Raspberry Pi Pico W has three modes
+ RUN
    + Microcontroller is running the code copied to it.
+ BOOT
    + Microcontroller is accepting code from the user.
+ OFF
    + Microcontroller is off.

---
## **Uploading**
The process of uploading code to the Pico is as follows. It starts off in the OFF State, transitions to BOOT and then to RUN.

The only *complicated* aspect of this is putting the Pico into boot mode. To do so, you must hold down the while button near the USB port on the Pico. Keep this button pressed while you insert the USB cable into the Pico (assuming it's already plugged into your PC). Once it's plugged in, release the boot button and the Pico should be ready to accept `.uf2` files (the files that contain your programs).


### **Making this work in a VM**
---
If you're using a VM there will initially be some extra steps involved in recognizing the BOOT and RUN modes of the Pico.
+ Open up the settings of your VM and go to the USB section.
+ Plug the Pico in without pressing any buttons and hit the green USB button to the right of the settings menu. You should see **RPI-RP2**, click on it to add the device.
+ Now remove the Pico from the USB and replug it in BOOT mode. Repeat the steps for adding it as a device.
---

First off, we must check to see if the Pico has been recognized as a boot device. To do so, type in:
```
sudo blkid -o list | grep RPI-RP2
```
If an output along these lines appears, your pico is being recognized.
```
/dev/sdb1   vfat    RPI-RP2    /media/user/RPI-RP2  0000-0000
```
At this point a two things can happen, the process of uploading code to the Pico will be detailed for both circumstances below.

+ Everything goes smoothly and the Pico "mounts" to `/media/user/RPI-RP2`
    +  Enter the **build** direcotry which sould contain the made .uf2 files.
    + Type the following command
```
cp my_file.uf2 /media/user/RPI-RP2
```
That's it, you've now uploaded the program to the Pico and it'll automatically enter the run state.

**OR**
+ It doesn't mount to that location and you must create it yourself.
    + The first time this error occurs, you must create the mounting point. Do so by typing the following command.
```
sudo mkdir /mnt/pico
```

Remember, the above command is only done the first time you encounter this issue, from then out the mounting point has already been created and you only need to perform the instructions below.

+ Now that you've created the mounting point, you need to associate the pico's device that you found from the **blkid** command to this mounting point. Type:

```
sudo mount /dev/sd__ /mnt/pico
```

+ You're now ready to copy over the file to the pico. Type the following commands:

```
sudo cp my_file.uf2 /mnt/pico
sudo sync
```

Hopefully this won't happen too often, but you've now uploaded the program to the Pico and it'll automatically enter the run state.

---
## **Monitoring Output**
To monitor the output, we'll be using the screen application.

Whenever the pico is in the **run** mode, it'll be identifiable as a USB device called `ttyACM0`. To find this device type:

```
ls /dev/tty* | grep ACM0
```

If `ttyACM0` is present, you can type the following command to read the serial output from the Pico.

```
sudo screen /dev/ttyACM0 115200
```

The number 115200 represents the **baud rate**, which is how fast the Pico sends information over serial.

To exit screen you could do three things:
+ Remove the USB cable from the Pico
+ Press the reset button on the Pico
+ Press `CTRL+A`, then `d`