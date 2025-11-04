# Embedded Sensor Network in FreeRTOS with Built-in ML Model Prediction

# Compile
Git clone the FreeRTOS-Kernel into the folder `git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git` <br/>
Otherwise, change the include path in both `CMakeLists.txt` and `c_cpp_properties.json` (.vscode) to the path to the FreeRTOS-Kernel download <br/>
Run the following commands <br/>
```
mkdir build
cd build
cmake ..
make
```
And then run the executable `./embml` 
