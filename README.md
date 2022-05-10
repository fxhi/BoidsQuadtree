# BoidsQuadtree

BoidsQuadtree is a small apllication written in C++ with OpenGL and ImGUI. The idea relies on the Boids algorithm developed by Craig Renolds wich simulates the flocking behavious of birds.

The birds' behaviour is governed by rules based on the behaviour of neighboring birds. In order to only check the neighboring birds and not all the birds in the simulation, a quadtree structure is implemented for more efficiency. 

<p align="center">
    <img width="600" src="https://github.com/fxhi/BoidsQuadtree-CPP/blob/master/BoidsQuadtree.png" alt="Example">
</p>

## Information

This application as only been tested on Windows 10. The FPS was pretty low, one of the reason is I only have an integrated graphics card.

## References

https://learnopengl.com/

https://www.labri.fr/perso/nrougier/from-python-to-numpy/

http://www.kfish.org/boids/pseudocode.html

http://www.red3d.com/cwr/

https://scipython.com/blog/quadtrees-2-implementation-in-python/

