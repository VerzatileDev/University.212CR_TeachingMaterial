# Session 4 - Texture

#### Table of Contents
1. [SOIL Library](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%204#SOIL-Library)
2. [Add Texture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%204#Add-Texture)
3. [Add Sky](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%204#Add-Sky)
4. [Homework](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%204#Homework)

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

---- Add texture codes in fragment Shader ----

Open fragmentShader.glsl. You can drag fragmentShader.glsl into Visual Studio and edit it. 
Add following definition in fragmentShader.glsl (add before main(void) function).

```C++
uniform sampler2D grassTex;
```
Replace fieldTexColor color calculation with texture color in main function.

```C++
fieldTexColor = texture(grassTex, texCoordsExport);
```

---- compile and run ----

Final result should look like this. 

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%204/Readme%20Pictures/Texture1.JPG)


## Add Sky

In this section, you will be asked to add a Sky plane with sky texture.

---- Add Sky plane ----

Insert into global variable definition area.

```C++
static Vertex skyVertices[4] =  
{
	{vec4(100.0, 0.0, -70.0, 1.0), vec2(1.0, 0.0)},
	{vec4(100.0, 120.0, -70.0, 1.0), vec2(1.0, 1.0)},
	{vec4(-100.0, 0.0, -70.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-100.0, 120.0, -70.0, 1.0), vec2(0.0, 1.0)}
}; 
```

---- Add Sky vertices data ----
> Add following codes in void setup(void) function (after comments "// Sky vertex data here")

```C++
   glBindVertexArray(vao[SKY]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SKY_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), 0);  //layout(location=2) in vec4 skyCoords;
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), (void*)(sizeof(skyVertices[0].coords))); //layout(location=3) in vec2 skyTexCoords;
   glEnableVertexAttribArray(1);
```

---- Add drawing codes ----

in void drawScene(void) function.

```C++
   glUniform1ui(objectLoc, SKY);  //if (object == SKY)
   glBindVertexArray(vao[SKY]);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
```

---- compile and run ----

You will see a blue sky.

### Add Sky Texture

Using similar step as field texture. Add texture to Sky plane

---- Add Texture loc variable ----
> Add following codes after 

```C++
 skyTexLoc, 
```

Add sky texture loading and setup code

```C++
   // Bind Sky image.
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   
   ////// You need to complete remaining codes //
```

---- Add texture codes in fragment Shader ----

You need to add codes into fragment shader


## Homework


* Change sky texture map when a key is pressed.

> Implement additional codes for loading night sky texture (nightSky.bmp) in Textures folder of the base project.
Also send it the shader when a key is pressed. So, the sky texture can be changed when a key is pressed.



