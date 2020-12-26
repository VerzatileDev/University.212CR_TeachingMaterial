# Session 4 - Texture

#### Table of Contents
1. [SOIL Library](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%204#SOIL-Library)
2. [Add Texture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Add-Texture)
3. [Add Sky](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Add-Sky)
4. [Homework](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Homework)

Welcome to Week 4! 

> By following these sessions, we will learn how to load texture and send texture to shader.


## SOIL Library

SOIL is a tiny C library used primarily for uploading textures into OpenGL.
It is based on `stb_image` version 1.16. It has been extended to load TGA and DDS files, and
to perform common functions needed in loading OpenGL textures. SOIL can
also be used to save and load images in a variety of formats.

https://github.com/littlstar/soil

### Features

* No external dependencies
* Tiny
* Cross platform (Windows, \*nix, Mac OS X)
* Public Domain
* Can load an image file directly into a 2D OpenGL texture
* Can generate a new texture handle, or reuse one specified
* Can automatically rescale the image to the next largest power-of-two size
* Can automatically create MIPmaps
* Can scale (not simply clamp) the RGB values into the "safe range" for NTSC displays (16 to 235, as recommended [here][1])
* Can multiply alpha on load (for more correct blending / compositing)
* Can flip the image vertically
* Can compress and upload any image as DXT1 or DXT5 (if `EXT_texture_compression_s3tc` is available), using an internal (very fast!) compressor
* Can convert the RGB to YCoCg color space (useful with DXT5 compression: see [this link][2] from NVIDIA)
* Will automatically downsize a texture if it is larger than `GL_MAX_TEXTURE_SIZE`
* Can directly upload DDS files (DXT1/3/5/uncompressed/cubemap, with or without MIPmaps). Note: directly uploading the compressed DDS image will disable the other options (no flipping, no pre-multiplying alpha, no rescaling, no creation of MIPmaps, no auto-downsizing)
* Can load rectangluar textures for GUI elements or splash screens (requires `GL_ARB/EXT/NV_texture_rectangle`)
* Can decompress images from RAM (e.g. via [PhysicsFS][3] or similar) into an OpenGL texture (same features as regular 2D textures, above)
* Can load cube maps directly into an OpenGL texture (same features as regular 2D textures, above)
* Can take six image files directly into an OpenGL cube map texture
* Can take a single image file where `width = 6 * height` (or vice versa), split it into an OpenGL cube map texture

### Readable Image Formats

* BMP - non-1bpp, non-RLE (from `stb_image` documentation)
* PNG - non-interlaced (from `stb_image` documentation)
* JPG - JPEG baseline (from `stb_image` documentation)
* TGA - greyscale or RGB or RGBA or indexed, uncompressed or RLE
* DDS - DXT1/2/3/4/5, uncompressed, cubemaps (can't read 3D DDS files yet)
* PSD - (from `stb_image` documentation)
* HDR - converted to LDR, unless loaded with *HDR* functions (RGBE or RGBdivA or RGBdivA2)

### SOIL library struture in Base project
* header files - They are located in include/soil folder.
* lib files - one lib file: soil.lib is located in include folder. It is compiled with x64 option so it can be only used with x64 configuration.
 
 
## Add Texture

Download based project: CreateSphereClassTex.zip from week 4 folder. unzip it. Open CreateSphere.cpp for editing.

---- Add header file ----

```C++
#include "soil/SOIL.h"
```

---- Add Texture loc variable ----
> Add following codes after "objectLoc," in Global variable definition area

```C++
 grassTexLoc,  ///for grass texture 
```

---- Add texture loading and setup codes ----
> Add following codes in the end of void setup(void) function.

```C++
   // Load the images.
   std::string TexNames[] = {
		"Textures/grass.bmp",
		"Textures/sky.bmp",
   };
   
   // Create texture ids.
   glGenTextures(2, texture);

   int width, height;
   unsigned char* data;

   // Bind grass image.
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture[0]);

   //load image data using SOIL library
   data = SOIL_load_image(TexNames[0].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   SOIL_free_image_data(data);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   grassTexLoc = glGetUniformLocation(programId, "grassTex");
   glUniform1i(grassTexLoc, 0); //send texture to shader
```

---- Add drawing codes in fragment Shader ----

Open fragmentShader.glsl. You can drag fragmentShader.glsl into Visual Studio and edit it. 
Add the same structure definition codes into fragmentShader.glsl (after in "vec3 normalExport;")

```C++
struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};

struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};
```
Add the lighting and material definition codes into fragmentShader.glsl (after structure definition codes)

```C++
uniform Light light0;
uniform vec4 globAmb;
uniform Material sphereFandB;
```
Add temp variables for lighting calculation. (add after "out vec4 colorsOut;")

```C++
vec3 normal, lightDirection;
vec4 fAndBDif;
```

Finally, add lighting calculation codes for ambient and diffuse effects. (add inside if statement "if (object == SPHERE)" )
Replace "colorsOut =  vec4(0.0,1.0,0.0, 1.0);  " (we do not need fixed color anymore.)

```C++
	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
    colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0); 
```

Please note that you do not need compile the C++ project when change shader codes. You just change shader codes. Save it and run it.

Final result should look like this. You can reference to "CreateSphere_finish.cpp" if you think you have made some mistakes. 

![Base picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%203/Readme%20Pictures/LightEffect.JPG)

## Add a simple animation
 
> We can create a simple animation which let the sphere move long the Z axis.


* Step 1: Add a global variable for Z Co-ordinates of the ball 

```C++
static float zVal = 0; // Z Co-ordinates of the ball.
```

* Step 2: Add modelview matrix before draw the sphere

In void drawScene(void) function, add sphere translation codes before "glUniform1ui(objectLoc, SPHERE); "
		
```C++
   modelViewMat = mat4(1.0);
   modelViewMat = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0, 10.0, 0.0), vec3(0.0, 1.0, 0.0)); //apply the same modelview as other objects
   modelViewMat = translate(modelViewMat, testSphere.GetPosition()); //apply Sphere Position and modify the modelview matrix
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));  //send modelview matrix to the shader
```

* Step 3: Modifiy void animate() function

Add following codes before "glutPostRedisplay();" in animate() function. It changes zVal by -0.2 in every frame.

```C++
	zVal = zVal -0.2;
	if (zVal < -25.0) zVal = 0.0;
	testSphere.SetPosition(vec3(0, 0, zVal)); //modify sphere's position
```

glutIdleFunc(animate); has been added into main function, which enables animation.

* Step 4: compile and run the program.



## Set up Camera
 
> We will set up a moving camera by pressing arrow keys. It allow moving camera up and down.


* Step 1: Add camera position global variable

```C++
static float d = 0.0; //Camera position
```

* Step 2: Insert position into modelview matrix

In void drawScene(void), change lookAt function input parameters
		
from 
```C++
modelViewMat = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
```

to 
```C++
modelViewMat = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0 + d, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
```

We need to change both lookAt() functions in drawScene(void) function.


* Step 3: Add codes into void specialKeyInput(int key, int x, int y)

Add codes to change d value 

```C++
   if (key == GLUT_KEY_LEFT) 
   {
	   if (d > -50.0) d -= 0.1;
   }
   if (key == GLUT_KEY_RIGHT) 
   {
	   if (d < 15.0) d += 0.1;
   }
```

* Step 4: compile and run

Use left and right arrow keys to move the camera


## Homework


* Create A cube from scratch using modern OpenGL

> There is tutorial. The link is  http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/



