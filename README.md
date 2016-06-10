# Unreal Engine 4 Bicycle Simulator
This is a bicycle simulator for road scenarios. It includes a cyclist model, which can be controlled by an Arduino or a keyboard.
The project also includes AI for cyclists, cars and pedestrains. A simple demo is set up by default.
The cars and cyclists use the default vehicle model in UE4.

The files in this project includes everything needed to compile and run the simulator in Unreal Engine 4.11.2.
It might work in other versions, but this is not tested.

See the ArduinoCode branch for some pictures of how the Arduino can be used to control the cyclist. This also includes the code for the Arduino.

The simulator was made by me as part of a cource-project the Norwegian University of Science and Technology. The AI is made as part of my Master's Thesis in Game Technology. A PDF of the thesis is included with the project. 

A short video demo of the cyclist model can be found at https://youtu.be/06Libu_58Ho, while a playlist with tests of the AI can be found at https://www.youtube.com/playlist?list=PLyQ2lVssCiGuufULltdKYA5pcY2pwBqiy.


Installing the solution

1. Download Unreal Engine from www.unrealengine.com.

2. Install Unreal Engine with version 4.11

3. Get a copy of the project’s code. Either from the included zip file or from GitHub.

4. Launch the BikeV3.uproject file. This will compile the project and open it in Unreal
Engine.


Using the solution

When the project opens it will be set up to run a demo of a traffic scenario. The scenario
includes a cyclist model, which will be controlled by a human player, and some other actors,
which will be controlled by AI agents. To run the scenario in the editor:

1. Run the project by using “Play”. This will play the game in the editor.

2. Control the cyclist by using “WASD” to steer and “Q” or “E” to change gears. The
mouse can be used to look around. “C” will change the camera to first person.

3. Click “T” to run the AI cyclist, “Y” to run the AI cars and “U” to run the AI
pedestrians.


The cars and pedestrians have one option. The option is where they should go when the AI is
running. To change this:

1. Click on the actor model that needs its goal changed.

2. Locate the option menu for the selected actor (a menu on the right side by default)

3. Set the goal in the “Goal” variable. A goal is a unique actor type that can be placed
and moved in the 3D environment.


The cyclist has two options. These are: where they should go when the AI is running and what
personality they have. The goal is modified in the same way as for the cars or pedestrians. To
modify the personality one can:

1. Click on the cyclist model that needs its personality changed.

2. Locate the option menu for the selected actor (a menu on the right side by default)

3. Select the personality from the dropdown list for the “Personality” variable.
