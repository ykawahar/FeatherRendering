# FeatherRendering
This is project work for COMP465.

## Author
Yuki and Hengrui 
Bret offers a lot of suggestions.

## Overview
This project was based on a 2002 graphics paper.

Yanyun Chen, Yingqing Xu, Baining Guo, and Heung-Yeung Shum. 2002. Modeling
and rendering of realistic feathers. ACM Transactions on Graphics 21, 3 (July 2002),
630–636. https://doi.org/10.1145/566654.566628

The L system was inspired by this paper. 

We were able to implement the L system however we were only able to implement the most basic version of it. We also added the angles to it so it looked more realisticl.

## Known bugs
The cylinder would not draw on the direction where the abstract vector was chosen namely the vector vec3(123,231,999).

vec3 w = glm::normalize(glm::cross( vec3(123, 231, 999), u)); 

The cylinder also has minor winding order issue mainly when you watch it from some perspective it appears that some cylinder are connected by a thin line.

## Building it
Make sure you have completed the tutorial on building a graphics project from Macalester comp465.

Then you would need to clone the project first.

After that using terminal if you are mac user, using cygwin if you are window user to change your directory to the project folder. After you found the folder, do 
- **mkdir build** 
- **cd build**
- **cmakegui ..**
- click configure
- click generate
- click open project

Run the project from either xcode or vscode. 


