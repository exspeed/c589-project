# c589-project

///////////////////////
// HOW TO BUILD     //
/////////////////////

Dependencies:

GLFW
If on Ubuntu you can run:
	sudo apt-get install libglfw3-dev libglfw3
Or download directly from: 
	https://www.opengl.org/discussion_boards/showthread.php/200108-Installing-glfw-on-Ubuntu-16-04


Makefile build commands:

make 
	Builds the project and creates directory for object files
make run
	Builds the project and runs the executable
make clean
	Deletes executable, object files and object directory

Note: This is designed for linux, however it may work on Mac OSX, while it is untested. For a more reliable version, download the xcode version.


==========================================================================
Barebones OpenGL Core Profile Boilerplate
   using the GLFW windowing system (http://www.glfw.org)

Loosely based on
 - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
 - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)

Author:  Sonny Chan, University of Calgary
Co-Authors:
		Jeremy Hart, University of Calgary
		John Hall, University of Calgary
Date:    December 2015
==========================================================================

