# Session 4 - Add Texture and Texture animation

#### Table of Contents
1. [SOIL Library](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%204#SOIL-Library)
2. [Add Texture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Add-Texture)
3. [Add Sky](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Add-Sky)
4. [Homework](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Homework)

Welcome to Week 3! 

> By following these sessions, we will learn how to create geomtric objects, add lights into the scene and set up a camera using Modern OpenGL. 


## SOIL Library

SOIL is a tiny C library used primarily for uploading textures into OpenGL.
It is based on `stb_image` version 1.16. It has been extended to load TGA and DDS files, and
to perform common functions needed in loading OpenGL textures. SOIL can
also be used to save and load images in a variety of formats.

https://github.com/littlstar/soil

**Features

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
 
## Add lights

We are going to add ambient and diffuse lighting effects which illuminate the sphere and make it look realistic.

---- Add lighting and material structure definitions ----

You add them anywhere in global variable definition area (before void setup(void) or before any function definition)

```C++
struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};
```

---- Add lighting and material value definitions ----
> Add following codes after lighting and material struture definitions

```C++
static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);
// Front and back material properties.
static const Material sphereFandB =
{
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};

static const Light light0 =
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 0.0, 0.0)
};
```

---- Send data to the shader ----
> Add following codes in void setup(void) before comments "// Create VAOs and VBOs... "

```C++
   //codes for OpenGL lighting
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1,&sphereFandB.ambRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1,&sphereFandB.difRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1,&sphereFandB.specRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1,&sphereFandB.emitCols[0]);
   glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), sphereFandB.shininess);

   glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

   glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1,&light0.ambCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1,&light0.difCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1,&light0.specCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);
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



