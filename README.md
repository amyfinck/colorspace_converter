# How to use

## compiling on ugls linux machine
- `ssh ugls1.ece.uvic.ca` (or any of the ugls machines)
- type in uvic credentials
- `mkdir /tmp/tmp_dir && cd "$_"`
- clone this repo
- `chmod +x ./compile.sh`
- `./compile.sh`
- check output

## running on seng440 linux machine
- `ssh seng440.ece.uvic.ca`
- `lftp -u user3,q6coHjd7P @arm`
- `put main.exe`
- `exit`
- `telnet arm`
- `user1`
- `q6coHjd7P`
- `chmod +x main.exe`
- `./main.exe`
- If you get a strange error, try deleting everything from the device and try again
- You can also compile with the arm emulator `qemu-arm main.exe` directly on the machine, but I've noticed differences
in what works on both.