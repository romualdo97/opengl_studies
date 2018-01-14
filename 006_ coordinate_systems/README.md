# Coordinate systems

![enter image description here](https://i.imgur.com/tmW1SKY.png)

**Model coordinates:**
**Local/model coordinates** are the coordinates of your object relative to its local origin; they're the coordinates your object begins in.

**Model matrix**
The next step is to transform the **local coordinates to world-space** coordinates which are coordinates in respect of a larger world. These coordinates are relative to a global origin of the world, together with many other objects also placed relative to the world's origin.

**View matrix**
Next we transform the world coordinates to **view-space** coordinates in such a way that each coordinate is as **seen from the camera or viewer's point of view**.

**Projection matrix**
After the coordinates are in view space we want to project them to clip coordinates. Clip coordinates are processed to the -1.0 and 1.0 range and determine which vertices will end up on the screen.
Note that as default opengl discards every vertex that is outside the NDC range, so is no necesary multiply them by the projection matrix for achieve this, however the projection matrix is considered the last vertex tranformation in a traditional vertex transformation pipeline and for this reason is possible to consider that the projection matrix determines which vertices will end up on the screen.
The true job of the projection matrix is to transform object vertices in a such way that rasterization stage can project vertices into the 2D surface known as viewport. but the result of the projection matrix is no a 2D vertex, this is a 3D vector (remember originally was a 4D vector).

> Read more at 
> - [Implementing transformation matrices in Unity using CPU by catlikecoding](http://catlikecoding.com/unity/tutorials/rendering/part-1/)
> - [In-depth explanation of projection matrices by scratchpixel 2.0](https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/projection-matrix-introduction)

# Model Matrices
We can consider three essential **model transformation matrices**, these are **scale**, **rotation** and **translation** matrices.

From a linear algebra stand point we know that **Vector spaces** are represented with a set of **basis vectors** and matrices are **vector sets**, so, we can use matrices for represent **vector spaces**. This way we can represent the following vector space using the matrix **S**.

![-](https://i.imgur.com/5Ha45dY.png)

> Continue reading in: **Appendix 1 - Additional Model Transformation Matrices Interpretation**

**About Homogeneous Coordinates: **
Why vertex shader output is a `vec4` and not a `vec3` since is supposed to be a vertex position.
Can we make any sense of that fourth coordinate? Does it represent anything useful? We know for **catlikecoding intro to matrix transformations** that when fourth component is 1 we are able to repositioning the points. When its value is 0, the offset (of the translation) would be ignored, but scaling and rotation would still happen.

Something that can be scaled and rotated, but not moved. That is **not a point**, that is a vector. **A direction**.

![enter image description here](https://i.imgur.com/QM4gIIN.png)

![enter image description here](https://i.imgur.com/KVSMYk9.png)

So what happens when the fourth coordinate gets a value other than 0 or 1? Well, it shouldn't. Or actually, it should make no difference. We are now working with homogeneous coordinates. The idea is that each point in space can be represented by an infinite amount of coordinate sets. The most straightforward form uses 1 as the fourth coordinate. All other alternatives can be found by multiplying the whole set with an arbitrary number.

![enter image description here](https://i.imgur.com/lWkV0kE.png)

So to get the Euclidean point – the actual 3D point – you divide each coordinate by the fourth one, which is then discarded.

![enter image description here](https://i.imgur.com/YCRjZDi.png)

Of course that doesn't work when the fourth coordinate is 0. Such points are defined to be infinitely far away. That's why they behave as directions.

**About rotation matrices:** 
![enter image description here](https://i.imgur.com/BlVbQEx.png)
![enter image description here](https://i.imgur.com/E3t2XFr.png)
> Read more at 
> - [codinglabs matrix chain transformation article](http://www.codinglabs.net/article_world_view_projection_matrix.aspx)
> - [Solarian Programer blog explanation](https://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/)

# Projection Matrices
So far, we've been transforming points from one positions in 3D to another position in 3D space. But how do those points end up drawn on a 2D display? This requires a transformation from 3D to 2D space. We can create a transformation matrix for that!

**Orthographic Camera:**
The most straightforward way to go from 3D to 2D is to simply discard one dimension. That will collapse 3D space into a flat plane. This plane acts like a canvas, used to render the scene. Let's just drop the Z dimension and see what happens.
![enter image description here](https://i.imgur.com/uP6TrK7.png)

Indeed, our grid becomes 2D. You can still scale, rotate, and reposition everything, but it gets projected onto the XY plane afterwards. This is a rudimentary orthographic camera projection.

> original model
![enter image description here](https://i.imgur.com/Oud1Yky.png)

-

> ortographic projection of model along plane XY 
![enter image description here](https://i.imgur.com/et0y0yA.png)

-

> scheme ilustrating ortographic projection
![enter image description here](https://i.imgur.com/MTUzzDZ.png)

**Perspective camera:**
An orthographic camera is nice, but doesn't show the world as we see it. We need a perspective camera for that. Due to perspective, things that are further away appear smaller to us. We can reproduce this effect by scaling points based on their distance from the camera.

Let's just divide everything by the Z coordinate. Can we do that with a matrix multiplication? Yes, by changing the bottom row of an identity matrix to 
`[0,0,1,0]`. That will make the fourth coordinate of the result equal to the original Z coordinate. Converting from homogeneous to Euclidean coordinates then takes care of the desired division.

![enter image description here](https://i.imgur.com/Is69Vnu.png)


> perspective projection along plane xy
> ![enter image description here](https://i.imgur.com/Fma1dWV.png)

-

> scheme ilustrating perspective projection
> ![enter image description here](https://i.imgur.com/amo2pdc.png)

The distance between the origin and the projection plane also influences the projection. It acts like the focal length of a camera. The larger you make it, the smaller your field of view will be. Right now we're using a focal length of 1, which produces a 90° field of view. We can make that configurable.

As a larger focal length means we're zooming in, this effectively increases the scale of our final points, so we can support it that way. As we're collapsing the Z dimension, that one doesn't need to be scaled.

![enter image description here](https://i.imgur.com/eaSScRM.png)

We now have a very simple perspective camera. If we were to fully mimic camera projection, we would also have to deal with the near and far plane. That would require projecting into a cube instead of a plane, so depth information is retained. Then there is the view aspect ratio to worry about. In the case of Unity, the camera looks in the negative Z direction, which requires negating some numbers. You could incorporate all that into the projection matrix.



# Utils

**Mesh:** High-level abstraction (could be a C++ class that interprets info and pass it from CPU to GPU using the graphic API) that describes 3d model vertex positions, EBO for index drawing and other useful vertex info as colors, normals, tangents, binormals, etc..

**About index ordering in EBO:** Which side a triangle is visible from is determined by the orientation of its vertex indices. By default, if they are arranged in a `clockwise direction` the triangle is considered to be `forward-facing` and visible. Counter-clockwise triangles are discarded so we don't need to spend time rendering the insides of objects, which are typically not meant to be seen anyway.

![enter image description here](https://i.imgur.com/VixPnZ6.png)

The previous concept is a particular theme of a more general concept known as **face culling**

**Face culling:** Triangle primitives after all transformation steps have a particular facing. This is defined by the order of the three vertices that make up the triangle, as well as their apparent order on-screen. Triangles can be discarded based on their apparent facing, a process known as Face Culling.

**---- Winding order**
The order of the vertices in a triangle, when combined with their visual orientation, can be used to determine whether the triangle is being seen from the "front" or the "back" side.

This is determined by the winding order of the triangle. Given an ordering of the triangle's three vertices, a triangle can appear to have a clockwise winding or counter-clockwise winding. Clockwise means that the three vertices, in order, rotate clockwise around the triangle's center. Counter-clockwise means that the three vertices, in order, rotate counter-clockwise around the triangle's center.

Which side is considered the "front" side is controlled by this function:

	void glFrontFace(GLenum mode​);

This is global state. mode​ may be `GL_CW` or `GL_CCW`, which mean clockwise or counter-clockwise is front, respectively. On a freshly created OpenGL Context, the default front face is `GL_CCW`.

The `Fragment Shader` gets a built-in input value saying whether the fragment was generated by the triangle's front face (this will always be true for non-triangular primitives).

![enter image description here](https://i.imgur.com/69FEojO.png)
[enter link description here](Read%20more%20in%20official%20docs)

**About tangent space:** Normal maps are defined in tangent space. This is a 3D space that flows around the surface of an object. This approach allows us to apply the same normal map in different places and orientations.

The surface normal represents upward in this space, but which way is right? That's defined by the tangent. Ideally, the angle between these two vectors is 90°. The cross product of them yields the third direction needed to define 3D space. In reality the angle is often not 90° but the results are still good enough.

**Unity case:**
So a tangent is a 3D vector, but Unity actually uses a 4D vector. Its fourth component is always either −1 or 1, which is used to control the direction of the third tangent space dimension – either forward or backward. This facilitates mirroring of normal maps, which is often used in 3D models of things with bilateral symmetry, like people. The way Unity's shaders perform this calculation requires us to use −1.
