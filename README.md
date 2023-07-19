# How to use

## running on seng440 linux machine
- `ssh seng440.ece.uvic.ca`
- cd into repository / clone repository if haven't already
- `./compile.sh`
- `lftp -u user3,q6coHjd7P @arm`
- `put cc.exe`
- `exit`
- `telnet arm`
- `user3`
- `q6coHjd7P`
- If you haven't already, create this file structure: 
  - input
    - put all input images here
  - output
    - luma
    - cb
    - cr
    - YCC to RGB
- `chmod +x cc.exe`
- `./cc.exe <input_img>.bmp`
- If you get a strange error, try deleting everything from the device and try again
- You can also compile with the arm emulator `qemu-arm main.exe` directly on the machine, but I've noticed differences
in what works on both.

## To get files from the arm device
- `lftp -u user3,q6coHjd7P @arm`
- `get <file>`
- `exit`
- `exit`
- `scp <username>@seng440.ece.uvic.ca:/home/<username>/SENG440/colorspace_converter/parrot100.bmp .`