# ccextractorGUI-nuklear
Graphical User Interface for CCExtractor (GSoC 2017)

## Note: Currently the GUI is fully functional and up to date with proposal for Macintosh and Linux only. Windows build will be ready as stated in GSoC proposal.


### Install Dependencies
1. Insall [GLEW](http://glew.sourceforge.net/) from source.
2. Instll [GLFW](http://www.glfw.org/) from source.
3. Depending upon your distribution you may or may not require X-Windowing-Server-devel packages. Most commonly required packages are: Xinerama, Xcursor, Xrandr, Xxf86vm which would be available in package manager repositories. 


### To compile
#### Navigate to `src` directory and run below command in terminal
    gcc *.c -DUNIX -lglfw3 -lX11 -lXinerama -lXcursor -lXi -lXrandr -lXxf86vm -lGLEW -lGL -lGLU -lm -ldl -lpthread

    additional -D flag to check library availability for burned subs extraction: -DENABLE_OCR

## OR

	1. Navigate to linux directory.
	2. ./autogen.sh
	3. ./configure
	4. make

	Note: autoconf scripts add required -D flags automatically according to the environment and installed libraries.
	If you want to check the GUI working under different situations than detected by the scripts. Use the manual compilation command given above.
