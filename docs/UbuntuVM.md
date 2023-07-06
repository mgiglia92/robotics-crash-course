# **Setting up your Ubuntu Virtual Machine**

## **Configuring VirtualBox**
+ The first step in setting up your virtual machine is to download VirtualBox. It can be found here https://www.virtualbox.org/wiki/Downloads.

+ Download the VirtualBox 7.0.8 platform packages that pertain to your machine (most likely Windows hosts). VirtualBox allows us to utilize parts of our computer and direct them towards emulating a seperate PC. If a computer generally has 8 processors for instance, VirtualBox can utilize 4 of the 8 to create a "seperate" PC, that when combined with memory (RAM), and storage (HDD or SSD space) can immitate a seperate computer.

+ Next we need to download the operating system we'd like to place on the VM (virtual machine). For RCC we'll be using Ubuntu 22.0.4. The .iso (image of data) can be found here https://releases.ubuntu.com/jammy/.

+ Once these two are downloaded, launch the VirtualBox application. Here, you'll want to create a new virtual machine. Name it as you wish, and select a folder to place the VM. Select the **.iso** you just downloaded and continue

+ Next, create a username and password (of course, make these memorable!). You can also add a hostname and domain name. Be sure to check the **Guest Additions** box, as this will allow you to copy and paste from your native machine onto the VM.

+ You'll now need to allocate resources to your Virtual Machine. Generally speaking, **at least 2GB (2000MB) of RAM and 4 CPU cores** are advised if you want the VM to run smoothly. Allocate as many as are reasonable for your hardware, just don't put too much.

+ Then you will create a "partition" for your Operating System to run on, **preallocating** the full-size would help with performance but isn't necessary (don't do this unless you have plenty of space to spare). Generally **aim for 20GB of space** or more.

+ With that all done, you should be ready to finish up the set-up for your virtual machine, but there's one last thing you might want to change. Now that you've created a virtual machine, you can add video memory to it. That can be found in the main **VM manager screen**, to the right. The default is **16MB**, increase it to about half of what you have capacity for.

Now you're ready to launch the VM!

---

## **Getting Ubuntu to Work**
+ When you launch the VM, it'll automatically start the process of installing Ubuntu, once it finishes sign in.

+ Unfortunately, the setup process is not done yet. What follows is likely the most intimidating aspect of this install, but don't be scared! We now need to restart the VM. This is because when you first install the VM, the user you created doesn't have sudo permissions. In Linux, **sudo** basically means "admin", and without being an admin you're very limited in what you can do. *For those of you who may want proof, open up the terminal and type*:

```
sudo -v
```

+ To fix this, you need to **restart the VM (within Ubuntu, not the machine itself)**. **Once you see the VirtualBox logo, press down the shift button** to access advanced options for Ubuntu (Don't worry if you don't time it properly the first time, you can try as many times as you like!).

+ Select the **Advanced options for Ubuntu** line by using the **arrow keys** to move and pressing enter to select.

+ Now select the second option, **Ubuntu with Linux (....) (recovery mode)**

+ You'll now be hit with a **GUI** (graphical user interface) with a lot of options. Move down towards **root** and hit enter.

+ A terminal will open up at the bottom of the VM. Within this terminal, you must first enter your password, **the password you type won't appear on the bottom, but just write it out and hit enter**, and then type the following commands:

```
mount -o rw,remount /
usermod -aG sudo <your_username>
exit
```

+ Once that's done, hit enter, and you should boot into Ubuntu with sudo permissions!


That's it, you're done! Thanks for following through the tutorial.


*Suggestion: Personally, I like to use my VM on scaled mode so I can fit it in my screen easily, but you can adjust the resolution to whatever you like in the view section. Just don't make it as large as your native resolution so that you can fit the entirety of the VM on your display!*
