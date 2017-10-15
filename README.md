# GUI for CCExtractor
## Developed Under Google Summer of Code 2017

This repository contains CCExtractor's code from [official CCExtractor repository](https://github.com/CCExtractor/ccextractor) along with the GUI code (written during GSoC 2017 period.

### Note: The project is merged to [official repository](http://github.com/CCExtractor/ccextractor)
Link to pull request : https://github.com/CCExtractor/ccextractor/pull/794


## Compiling CCExtractor (with GUI)

### Linux
   
1. Make sure all the dependencies are met.</br>
 * Build GLEW from source, instructions [here](http://glew.sourceforge.net/build.html)
 * Build GLFW from source, instructions [here](http://www.glfw.org/docs/latest/compile.html)
    
2. Compiling

    
```
cd ccextractor/linux
./autogen.sh
./configure --with-gui
make

# test your build
./ccextractorGUI
```


### MacOS

1. Make sure all the dependencies are met. They can be installed via Homebrew as

```
brew install glfw
brew install glew
```

2. Compiling

```
cd ccextractor/mac
./autogen.sh
./configure --with-gui
make
```
### Windows
Requirement: Visual Studi 2015 or above.
Just open `windows/ccextractor.sln` with visual studio and build the solution.