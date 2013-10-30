
You will find three libraries and a small test program in this directory. The next assignment will only contain the program that you will have to extend, drop it in this folder right next to the libs.
The three libs are: glloadgen, glfw and glm (see below). glm is not used in the first assignment.

We can only give support for Linux on a computer with a modern graphics card. Modern here means capable of at least OpenGL 3.2. But that doesn't mean, that you can't solve the assignment on other systems! We try to make the assignment as compatible as possible and even test them as much as possible on Linux, MacOS X and sometimes even Windows. But the huge number of OS and GPU combinations is too large to test everything, also: we don't even have all combinations in our lab for testing! 
If your computers OS is not supported and/or your graphics card is not capable of modern OpenGL you can get a lab account from us. In case you need an account please request one early and note that we can not guarantee a free PC all the time.

If you have problems, please tell us what OS you're using, what GPU is installed, what drivers you use and what OpenGL version you are running.


Linux & MacOS X:
----------------
* MacOS X users might need to install XCode including the commandline tools and cmake
* Linux users need gcc (or clang) and developer tools from their distribution, denpending on the distribution you will need:
  * cmake
  * libglew-dev glew-utils # while GLEW itself isn't needed, this way all needed packages for OpenGL get installed :-D
  * libxrandr-dev
  * libxi-dev
  * libxxf86vm-dev


Compile the assignment:
* enter the assignmentXY directory and run 'make'
* run the application 'assignment' from your command prompt

Build your own solution:
* rename the assignment.cpp to assignment_MATRICULATION_NUMBER_OF_ONE_GROUP_MEMBER.cpp.
* edit just assignment_MATRICULATION_NUMBER_OF_ONE_MEMBER.cpp - DON'T edit any other file without speaking to us (don't edit the main.cc file)!
* always add all names and matriculation numbers of all team members in your source file as an output in the function printStudents().
* compile your solution with 'make GROUP=assignment_MATRICULATION_NUMBER_OF_ONE_MEMBER' -> the binary will be called assignment_MATRICULATION_NUMBER_OF_ONE_MEMBER.


Windows:
--------
* Figure out how to build the assignments.
* Document your steps in the L2P forum for the next Windows user.
