# Yape 2D
Yet Another Physics Engine (2D)

## Project Structure
Yape2D is a game agnostic engine and should be fairly straight forward to use it
in any game engine. All the code related to the physics engine is is inside the folder 
```yape2d/engine```. <br>
Inside ```source/playground``` you will find all the code dedicated to the examples
and usage cases of the engine. It uses the 
[PixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) to handle 
graphics and input from the keyboard/mouse.

## The Playground
The engine needs to be part of a project to be compile. In this case that is the Playground.
The Playground includes a graphics user interface which allows you to play and test
different features of the engine. To compile it you will need:
* C++ compiler

If you want to compile it for emscripten, you will also need:
* emscripten toolchain [emscripten.org](https://emscripten.org/)<br>
    * When you run ```./emsdk activate latest``` you might want to make the change permanent.
* [ninja](https://github.com/ninja-build/ninja/releases) generator

## Compilation
In both cases you can use the makefile included in the project. As you can see, there
is nothing special about it. 

In case you want to to compile using emscripten, from the build folder, run: <br>
* ```emcmake cmake -G Ninja ..```
* ```emmake ninja```

## Running emscripten version
Launch a web server from the folder where you have the html file that emscripten has generated:<br>
*  ```python -m http.server 8000```
Then open a web browser and go to  ```http://localhost:8000/[YOUR_HTML_FILE].html```

