# morphotree
A simple morphological tree filter prototyping library

This library implements morphological trees with minimal dependencies as possible. The main code of the library where the algorithms are implemented in C++. 
It also provides a interface in Python. 

## building

### C++

This library relies on <a href="https://conan.io/">Conan</a> for packages management and CMake as bulding tool. One may build the library by access the 
directory and creating the conan package using the following commands:

```shellscript
 $ mkdir build && cd build
 $ conan install ..
 $ cmake ..
 $ make
 ```
 
 The first line create a directory called build to store the binaries of the compiled code and enter in it. The second line generate the conan cmake file 
 to be included in your main CMakeLists.txt. The third line runs cmake and the fourth line builds the library and the executable "morphotreemain". This
 executable is just a example program showing simple a simple example of usage of the morphotree library.
 
 If you want to install morphotree library in your Conan system, we can run:
 
 ```shellscript
 $ conan create . morphotree/0.0.1@dennisjosesilva/dev
 ```
 
 By installing it in this way, you can use the library in your project using Conan by including it into the package list file (conanfile.txt):
 
 ```
 [requires]
 morphotree/0.0.1@dennisjosesilva/dev
 ```
 A example of usage can be found in src/main.cpp
 
 ### Python
 
 The morphotree library also provides a Python interface where the C++ implementation can be access and used in Python. You can install it in our enviroment 
 as a package. To do it, first, we have to install the C++ implementation in our Conan enviroment which can be done by the following commands:
 
 ```shellscript
 $ conan create . morphotree/0.0.1@dennisjosesilva/dev
 $ conan create . morphotree/0.0.1@dennisjosesilva/dev -omorphotree:shared=True
 ```
 
 The first command install morphotree library in your conan system and the second command compile it as a dynamic library (need to integrated with Python).
 Then, you can build the python extension by entering at "python" directory and using the setup.py by running the following command:
 
 ```shellscript
 $ python setup.py build
 ```
 
 It will generate a directory named "build" and a dinamyc library which provides the morphotree library in python (its name in python is morphotreepy).
