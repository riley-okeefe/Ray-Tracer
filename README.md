# Ray-Tracer
Basic implementation of a ray tracer completed using fundamentals learned in a Computer Graphics course taken at Saint Mary's University and taught by Dr. Jiju Poovvancheri.
## Resulting Image:
  ![finalResult](https://github.com/riley-okeefe/Ray-Tracer/assets/97004064/b0e57fca-380e-4caf-b730-79e3f167eac4)

# Introduction
This project uses fundamentals learned from class material as well using external resources to implement a simple backward ray tracer. This project follows the core requirements of writing a ray tracer that renders a sphere from a fixed point of view, models the perspective camera, shades the sphere using the Blinn-Phong illumination model, and includes a single point light source. Additional features such as anti-aliasing and texture mapping using bilinear interpolation have also been added.

# Features Added

## Camera Class
The camera class which is stored in the camera.h file has a function called ‘generateRay’ which receives a pixel position and returns the ray direction for a given pixel position. A function called ‘setPos’ and ‘getPos’ are used to get the value of the camera position for use in the ‘generateRay’ calculation.

## Image Plane Class
The image plane class which is stored in the imageplane.h file has functions called ‘setImage’, ‘setXCol’, and ‘setYRow’ for setting the image size, setting the x columns and setting the y rows respectively. Also included is a function for generating the pixel position which has 3 different variations for specific uses at certain points in the code. To elaborate there is a variation for generating the pixel position for the sphere, a variation for generating the pixel position of the 2 triangles used for the surface, and a variation for generating the pixel position of the subpixel colors for the sphere. The formulas for calculating the pixel position have been modified to accommodate anti-aliasing and were constructed using the slides from class on anti-aliasing (Poovvancheri, 2023). 

## Sphere Class
The sphere class which is stored in the sphere.h file has functions for getting and setting the sphere radius and sphere center called ‘setRadius’, ‘getRadius’, ’setCenter’,’getCenter’ respectively. Also included is a function called ‘intersectRay’ which checks for the ray sphere intersection using the quadratic equation, where A is the dot product of the ray direction, B is 2 * the dot product of the ray direction and Origin - center vector value, and C is the dot product of the origin - center value minus the sphere radius squared. The function then checks if the discriminant is less than zero (no intersection) and returns negative one if the value is less than zero, otherwise it calculates the t value which is the smallest value of t1 and t2.
The intersect ray has another variation which is used for the shadow calculation and differs only in the origin - center value, which replaces the origin with the light source. 

## Triangle Class
### triangle.h
The triangle.h file was obtained from the starter code given in lab 3 located on Github (Poovvancheri, 2023). This file includes the triangle class and declares functions for computing barycentric coordinates, computing the normal and for getting the texture coordinates.
### triangle.cpp
The triangle.cpp file was obtained from the starter code given in lab 3 located on Github (Poovvancheri, 2023). Additional content added to this file, includes code for computing the barycentric coordinates in the ‘Barycentric’ function and code for getting the texture coordinates in the ‘getTextCoord’ function. The code for the ‘Barycentric’ function and ‘getTextCoord’ was constructed using the calculations shown in the textbook (Shirley & Marschner, 2009).

## Texture Class
### texture.h
The texture.h file was obtained from the starter code given in lab 4 located on Github (Poovvancheri, 2023). This file includes the texture class and declares functions for normal texture lookup using a simple round off as well as a function function for bilinear interpolation texture lookup.
### texture.cpp
The texture.cpp file was obtained from the starter code given in lab 4 on Github (Poovvancheri, 2023). Additional content added to this code includes the calculation for uv0 and uv1 value, the parameters for interpolation au and av, and code for finding the interpolated pixel color. These additions were constructed using the course slides for texture mapping (Poovvancheri, 2023).

## Image Class
### image.h
The code for the Image.h file was obtained from the starter code for lab 4 located on Github (Poovvancheri, 2023). This file declares functions for displaying and saving the image and no additional changes were made to the code.
### image.cpp
The code for the Image.cpp file was obtained from the starter code for lab 4 located on Github (Poovvancheri, 2023). This file defines the functions for displaying and saving the image. No additional changes were made to this code.

## Lodepng Class
### lodepng.h
The code for lodepng.h was obtained from the starter code for lab 4 located on Github (Poovvancheri, 2023). This file declares functions for loading the png image. No additional changes were made to this file.
### lodepng.cpp
The code for lodepng.cpp was obtained from the starter code for lab 4 located on Github (Poovvancheri, 2023). This file defines functions for loading the png image. No additional changes were made to this file.

## Main.cpp
The code for main.cpp was obtained from the starter code for lab 2 located on Github (Poovvancheri, 2023). There are many additions to main.cpp which are described in more detail below:
### Camera Position
A camera position of type Vec3 with x, y, and z values of (0.0, 0.0, 0.0).
### Sphere Object and Characteristics
A sphere object is added by first declaring a sphere object of type Sphere from sphere.h, then the sphere position with x, y, and z values of (0.0, -0.5, -2.0) is defined, followed by defining the sphere radius which is set to a value of 1.0.
### Triangles for Floor
A list of type Triangle is added which contains two triangles with their respective coordinates and texture coordinates:
	list<Triangle> tri_list
 {
		Triangle(Vec3(-7, -1, -1), Vec3(7, -1, -1), Vec3(-7, -1, -7), Vec2(0, 0), Vec2(1, 0), Vec2(0, 1)),
			Triangle(Vec3(7, -1, -1), Vec3(7, -1, -7), Vec3(-7, -1, -7), Vec2(1, 0), Vec2(1, 1), Vec2(0, 1)) 
   };
### Reflection Coefficients
The reflection coefficients for ambient diffuse and specular are defined:
	Vec3 k_a = Vec3(0.2f, 0.2f, 0.2f);
	Vec3 k_d = Vec3(0.3f, 0.8f, 1.0f);
	Vec3 k_s = Vec3(1.0f, 1.0f, 1.0f);
### Single Point Light Source
A single point light source is added with x, y, and z values of (2.0, -4.0, 3.0).
### Ambient Diffuse and Specular Light Colors
The ambient, diffuse and specular light colors are added:
  Vec3 lightAmbient = Vec3(0.5f, 0.5f, 0.5f); //gray light
  Vec3 lightDiffuse = Vec3(1.0f, 1.0f, 1.0f); // white
  Vec3 lightSpecular = Vec3(1.0f, 1.0f, 1.0f); //white
### Origin Point
The origin is specified as the camera point.
### Textures
Textures for the floor, sphere and background are added as well as code to load the textures from the appropriate folder:
Texture tex = Texture();
Texture sphereTex = Texture();
Texture backTex = Texture();
//Load texture
tex.loadTexture("Textures/water.png");
sphereTex.loadTexture("Textures/earth1.png");
backTex.loadTexture("Textures/galaxy.png");
### Ray Construction
Inside the for loops ray construction is completed by calculating the pixel position at a given i and j and then using this pixel position the sphere direction is calculated. Next the sphere object calls the sphere class function ‘intersectRay’, which builds the coefficients of the quadratic equation along with computing the discriminant and t value. If the discriminant is greater than zero the calculation of the intersection point and a check to see if the t value is greater than 0 is computed. If the discriminant is greater than zero the ray does not intersect with the sphere.
### Blinn-Phong Shading
If the t value is greater than 0 the normal, light vector, diffuse term, half vector, and specular term are computed. Following is the definition of the sphere color with a variable called ‘objectColor’ having a value of (0.1, 0.3, 1.0). Finally the final sphere color is computed using the ambient, diffuse, specular, and object color values. If t is less than zero the sphere the i and j values at the given position are set to the color black because the camera would now be inside the sphere.
### Floor of the Scene
This portion of the code is computed if the initial check for the discriminant is less than 0. The code first starts off by recalculating the pixel position so that the surface can be flipped up right using a rotation value of 2. The direction of the triangle ray is calculated and then a for loop to iterate over the triangles is added. In this for loop code is added from the starter code given in lab 4 located on Github (Poovvancheri, 2023). The code added is for getting the normal, t value, triangle intersection, barycentric coordinates and a check to see if the point lies within the triangle. If the point is within the triangle the code then checks for the ray shadow intersection for each of the primitives in the scene, if the primitive is in the shadow then the surface is colored black and if it is not in the shadow the surface is textured using bilinear interpolation
### Shadows
In the code for the floor of the scene, if the point lies within the triangles otherwise known as the floor, then the ray shadow intersection is checked for each primitive (sphere and triangle). For the sphere the shadow checking is computed by using the shadow direction (lightsource -sphereIntersection) and the light source as the origin in a function called ‘raySphereIntersect’. This function is similar to the ‘intersectRay’ function used earlier in the code, but instead uses the shadow direction for the ray direction and the light source as the origin point. If ‘raySphereIntersect’ returns true then the point is in the shadow and the surface is shaded black. For the triangle the shadow checking is completed by again using the using the shadow direction (lightsource -sphereIntersection) and the light source as the origin, but this time using these values in a function called ‘rayTriangleIntersect’ along with three other parameters being the three triangle vertices. This function uses the Moller-Trumbore algorithm implementation found from a websource (Colasp, 2019). This algorithm for checking if a point is in a triangle or not was used to check if the shadow points are on either triangle. If rayTriangleIntersect returns true then the point is in the shadow and the surface is later shaded black.

## Additonal Features Added
### Anti-aliasing
The code adds anti aliasing by adding the following:
- An additional two for loops to the code 
- For the sphere shading the code calculates the subpixel color due to each component (ambient, diffuse, specular)
- Adding of the subpixel color to the overall color for averaging at the end
- Averaging the overall color by dividing the color by (h_samples *v_samples)
- Setting the image i and j value to the respective averaged color
### Texture Mapping
The code uses texture mapping for the floor and background, as well having the ability to add texture to the sphere by commenting/uncommenting a couple lines of code.

## References
- Shirley S & Black T & Hollasch S.(2023).Ray tracing in one weekend: 4.4.1 the ray class, 6.6.3 an abstraction for hittable objects. Retrieved from [source](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- Poovvancheri J.(2023).Introduction to computer graphics: antialiasing.
- Poovvancheri J.(2023).GraphicsLab: sampling. Retrieved from [source](https://github.com/jijup/GraphicsLab/tree/main/sampling)
- Shirley P & Marschner S. (2009). Fundamentals of computer graphics: 2.7.2 3D triangles (3rd ed., pp. 48-49).A K Peters.
- Poovvancheri J. (2023).GraphicsLab: texturing. Retrieved from [source](https://github.com/jijup/GraphicsLab/tree/main/texturing)
- Poovvancheri J.(2023).Introduction to computer graphics: texture mapping.
- Poovvancheri J.(2023).GraphicsLab: raytrace. Retrieved from [source](https://github.com/jijup/GraphicsLab/tree/main/raytrace)
- Colasp J.(2019). Moller-Trumbore algorithm. Retrieved from [source](https://github.com/scratchapixel/website/blob/main/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection.md)
