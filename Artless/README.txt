Here are the controls to test out the certain features.

Controls (Physics):
W: Adding force to player in the positive y axis
S: Adding force to player in the negative y axis
D: Adding force to player in the positive x axis
A: Adding force to player in the positive x axis
Space: Jumping
Tab: To toggle step mode
1: To toggle RigidBody2D
2: To toggle Draw Collider2D
0: Step next cycle

Controls (General):
Left Control: Toggle between physics and batch rendering showcase
9: To toggle active status of Editor, used to hide all the panel
ECS: To exit the application

Physics:
When you startup the project, you will be able to use the physics control to move the player character

Batch Rendering:
After using the left control key, you will be able to see about 3000 entities rotating on the spot. 

Build Settings

Debug 

When building the project in debug mode, there will be no optimization of the code at all. 
Warning Level will be set to 4. This build will be the slowest among the two other build. 
This build is to be used for the development of the engine and will be used to debug the codes. 

Development 

Like debug mode, Warning Level will also be set to 4. A low-level optimization of the code 
will be applied to this build. This build will be used for alpha testing where optimization 
is wanted but we still want to be able to catch some errors. 

Distribution 

This build is mainly for the distribution of our game engine. Codes are compiled in the most 
optimized way possible, very prone to breaking if there are underlying bugs in the build. 
Hence this is only used at the end when distributing to our “customers”.  