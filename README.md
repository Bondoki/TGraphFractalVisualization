# T-Graph Fractal Visualization
Rendering of the T-Graph fractal aka Tree fractal aka T-fractal with functionality f=3, for different generations G, see the paper by S. Havlin and H. Weissman, ["Mapping between hopping on hierarchical
 structures and diffusion on a family of fractals"](https://iopscience.iop.org/article/10.1088/0305-4470/19/16/013), J. Phys. A: Math. Gen. (1986), 19, L1021-L1026.
 This fractal has the fractal dimension of df=ln(f=3)/ln(2)=1.5849625 with a total number of nodes N=f^G+1.
 
![TGraphFractal](https://user-images.githubusercontent.com/16195016/104137386-81a4d900-539c-11eb-9e74-67f57ee9d5eb.gif)
 
## Installation
Just do for standard compilation (tested with gcc 8.2.1):

````sh
    # generates the application
    g++ -o TGraphFractalVisualization TGraphFractalVisualization.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
````

## Usage
Simply use the mouse to move, zoom, and pan. Use the arrow keys:
&#11014; : next generation
&#11015; : previous generation
&#11013; : rotate angle by -5°
&#10145; : rotate angle by +5°
R : reset view to angle = 0°
ESC : quit


## References and License
The program acknowledge the libraries and refer to the appropriate licenses
* [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) by [Javidx9](https://github.com/OneLoneCoder)
* [olcPGEX_AdditionalColours](https://github.com/matt-hayward/olcPGEX_AdditionalColours) by [matt-hayward](https://github.com/matt-hayward)

## Acknowledgment
Inspired by the [Pan&Zoom example](https://youtu.be/ZQ8qtAizis4) of [Javidx9](https://github.com/OneLoneCoder) gave the idea for this.
