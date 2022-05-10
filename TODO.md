
# Personal stuff 

## TODO

### ImGui
* Maintaining Aspect Ratio and Scale Independent of Window Size : https://stackoverflow.com/questions/22735111/

* Add multi viewports : https://github.com/ocornut/imgui/wiki/Multi-Viewports

### Flock

* Flock 
    - quadtree: 
        The deepest node in the quadtree is by default 50. 
        It avoids having an infinite loop when a number of point (> max number per node) are at exactly the same position (e.g. at initialization if all boids start in (0,0) position).

    - quadtree query (can be added as an option):
        If the number of neighbouring boids is greater than a certain value, we only apply the behavior rules to a small portion of it. For this extract, we randomly select some boids among those returned by the query function.

### General

* Math
    - PI value:
        issue with M_PI and cmath : https://stackoverflow.com/questions/6563810/m-pi-works-with-math-h-but-not-with-cmath-in-visual-studio

    - Math constant : 
        Issue with #define _USE_MATH_DEFINES to get M_PI, need to put this macro definition in every header that include RandomNumber.hpp. Need to create my own math declaration constants.

## Perspectives

### Usefull resources if octree
https://stackoverflow.com/questions/32412107/quadtree-find-neighbor
https://www.reddit.com/r/VoxelGameDev/comments/ontjdf/how_is_sdf_stored_in_a_octree/




