ftpMalware
====

A joke teller applications that embeds an FTP server in the background.

Features
--------

- Sets up FTP server when the application launches
- No logs from the FTP server are shown to the user
- Very funny jokes.


Build and install
------------------

1. Setting up development environment(If you only want to run the application, skip to "Install the Malware" steps)
  1. Follow [these instructions](http://3dbrew.org/wiki/Setting_up_Development_Environment) to set up development environment on your computer.
  2. Open command window(or terminal) on your computer.
  3. Download the newest “ctrulib” library from github.
    > git clone https://github.com/smealum/ctrulib.git
    
  4. Copy the “libctru” folder from ctrulib and place it in /opt/devkitpro (replace the previous version of libctru)
  5. Install libctru library
    > cd libctru/
    
    > make clean
    
    > make && make install
    
2. Install the Malware(If you already have the executables of the application, skip step 2-1 and 2-2)
  1. Download our malware from github
    > git clone https://github.com/ChengshiZhang/ftpMalware
    
  2. Compile the software
    > cd ftpMalware/

    > source ~/.bashrc

    > make
   
  3. Insert your Nintendo 3DS’ SD card and navigate to /3ds
  4. Create a folder in /3ds called “ftpMalware”
  5. Copy the executables of the malware “ftpMalware.3dsx” and “ftpMalware.smdh” and place them in /3ds/ftpMalware/ in your SD card.
  6. Eject the SD card and insert it into your Nintendo 3DS.
  7. Launch homebrew(Default setting: Hold L camera button, press power button, then release the L camera button when the top screen shows TV static white noise)
  8. Open the “Very Safe Joke Teller” application
  
3. Attacker Side
  1. Connect to the same network as the user’s Nintendo 3DS.
  2. Use IP scanner(or other IP scanning softwares) to scan the local area network.
  3. Find “Nintendo Entertainment Device” on the list of scanned devices.
  4. Scan whether port 5000 is open on this IP address.
  5. Input the IP address of the 3DS and port 5000 in Filezilla(or other FTP client softwares)
  6. Wait until the FTP connection is established.
  7. Attackers now have full userland access
