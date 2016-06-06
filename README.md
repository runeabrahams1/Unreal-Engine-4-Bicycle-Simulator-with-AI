# Unreal Engine 4 Bicycle Simulator
This is a bicycle simulator for road scenarios. It includes a cyclist model, which can be controlled by an Arduino or a keyboard.
The project also includes AI for cyclists, cars and pedestrains. A simple demo is set up by default.
The cars and cyclists use the default vehicle model in UE4.

The files in this project includes everything needed to compile and run the simulator in Unreal Engine 4.11.2.
It might work in other versions, but this is not tested.

See the ArduinoCode branch for some pictures of how the Arduino can be used to control the cyclist. This also includes the code for the Arduino.

The simulator was made by me as part of a cource-project the Norwegian University of Science and Technology. The AI is made as part of my Master's Thesis in Game Technology. A PDF of the thesis is included with the project. 

A short video demo of the cyclist model can be found at https://youtu.be/06Libu_58Ho, while a playlist with tests of the AI can be found at https://www.youtube.com/playlist?list=PLyQ2lVssCiGuufULltdKYA5pcY2pwBqiy.


To get the code:

1. I recommend to just download it as a zip. It will probably go much faster due the the size (~150MB).

2. Install Unreal 4 (and possible Visual Studio 2015).

3. Launch the BikeV3.uproject file. This should compile the project.

4. It is a UE4 C++ project. Generating Visual Studio files will help with debugging if the code can't compile.
