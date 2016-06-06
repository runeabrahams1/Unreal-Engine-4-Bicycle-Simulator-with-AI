# Unreal Engine 4 Bicycle Simulator
This is a bicycle simulator for road scenarios. IT includes a cyclist model, which can be controlled by an Arduino or a keyboard.
The project also includes an AI for cyclists, cars and pedestrains. A simple demo is set up by default.
The cars and cyclists use the default vehicle model in UE4.

The files in this project includes everything needed to compile and run the simulator in Unreal Engine 4.11.2.
It might work in other versions, but this is not tested.

See the ArduinoCode branch for some pictures of how the Arduino can be used to control the cyclist. THis also includes the code for the Arduino.

The simulator and the AI is made as part of my Master's Thesis in Game Technology at the Norwegian University of Science and Technology.
Included with the project is a PDF of the Thesis.

A short video demo of the cyclist model can be found at https://youtu.be/06Libu_58Ho, while a playlist with tests of the AI can be found at https://www.youtube.com/playlist?list=PLyQ2lVssCiGuufULltdKYA5pcY2pwBqiy.


To get the code:

1. Fork the project
2. Clone your new fork 
3. Install Unreal 4 (and possible Visual Studio 2015)
4. Launch the BikeV3.uproject file. This should compile the project.
5. It is a UE4 C++ project. Generating Visual Studio files will help with debugging if the code can't compile.
