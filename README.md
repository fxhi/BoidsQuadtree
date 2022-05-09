### TODO
Maintaining Aspect Ratio and Scale Independent of Window Size : maintaining-aspect-ratio-and-scale-independent-of-window-size-with-freeglut

Flock 
    - quadtree  : the deepthest node of the quadtree is by default 50. It avoid to have infinit loop when a number of point (> max number per node)
are exactly at the same position (e.g. a initialization if all boids start in (0,0) position).
    - quadtree query, may be not usefull, can be added as an option: if the number of neighbouring boids is greater than a certain value, only applies the behavior rules to 
                        a small porition of the neighbouring boids. For this extract select randomly some boids in those return by the query function.

issue with M_PI and cmath : https://stackoverflow.com/questions/6563810/m-pi-works-with-math-h-but-not-with-cmath-in-visual-studio

 - Math constant : issue with #define _USE_MATH_DEFINES to get M_PI, need to put this macro definition in every header that include RandomNumber.hpp. Need to create my own math declaration constants.

N.B: (keep this project simple)



TODO:
Imgui:
Add multi viewports : https://github.com/ocornut/imgui/wiki/Multi-Viewports

### References

OpenGL part thank to : learnopengl.com

https://stackoverflow.com/questions/22735111/

### Usefull resources if octree
https://stackoverflow.com/questions/32412107/quadtree-find-neighbor
https://www.reddit.com/r/VoxelGameDev/comments/ontjdf/how_is_sdf_stored_in_a_octree/

