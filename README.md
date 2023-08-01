## running on seng440 linux machine
- `ssh seng440.ece.uvic.ca`
- cd into repository / clone this repository if you haven't already
- cd into src/
- `./compile.sh`
- `lftp -u user3,q6coHjd7P @arm`
- `put cc.exe`
- `put dir_structre.sh`
- `put parrot100.bmp`, or your input file
- `exit`
- `telnet arm`
- `user3`
- `q6coHjd7P`
- `./dir_structure.sh`
    * This script creates the directory structure neccicary for the program to run
    * If there is a problem with this code, create an `input` directory, as well as an `output` directory
    * inside the `output` directory, create a `rgb` directory
- `mv parrot100.bmp input`
  - The code checks for the image in the input directory, so it is essential that it is moved here
- `chmod +x cc.exe`
- `./cc.exe <input_img>.bmp`
- If you get an error, try deleting everything from the device and try again
- You can also compile with the arm emulator `qemu-arm cc.exe` directly on the machine, but this is not reccomended

## profiling
- instead of running `./compile.sh`, use `prof_compile.sh` 
- After running on arm device, use `lftp` to get gmon.out
- `gprof cc.exe gmon.out > profile.txt`

## To get files from the arm device
- `lftp -u user3,q6coHjd7P @arm`
- `get <file>`
- `exit`
- `exit`
- `scp <username>@seng440.ece.uvic.ca:/home/<username>/SENG440/colorspace_converter/parrot100.bmp .`

## To get an entire directory from the arm device
- mkdir <directory_name>; cd <directory_name>;
- `lftp -u user3,q6coHjd7P @arm`
- `cd output`
- `mirror`
- `exit`
- `exit`
- `scp -r <username>@seng440.ece.uvic.ca:/home/<username>/SENG440/colorspace_converter/<directory_name> .`