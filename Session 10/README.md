# Session 10 - Toon Shader


Welcome to Week 10! 

> In this week, we will learn how to create a Toon Shader.

## Toon Shader

Toon shading is probably the simplest non-photorealistic shader we can write. 
It uses very few colors, usually tones, hence it changes abruptly from tone to tone, yet it provides a sense of 3D to the model. 
The following image shows what we’re trying to achieve..

 ![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%2010/Readme%20Pictures/Example.JPG)

The tones in the teapot above are selected based on diffuse light calculation.

So if we have a normal that is close to the light’s direction, 
then we’ll use the brightest tone. As the angle between the normal and the light’s direction increases darker tones will be used.
 In other words, the cosine of the angle provides an intensity for the tone.


### Implementation

* Download the base project (waterEx.zip). Always to Compile option to "x64".  Open waterEx.cpp

There is a water OBJ file already in the project folder and it has been imported into the project.

You need to add water texture and replace the existing one (sky texture) on the water plane.

* There is a water.bmp in the Textures folder. Following the previous intructions to add it into the project.


```C++
y = sin(x+t) - cos(z+t);
```

* Add water texture into fragment shader and assign it to water mesh

It should look this

 ![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%2010/Readme%20Pictures/Example.JPG)


### Modify water mesh in vertex shader

* Add a time variable (global) in waterEx.cpp

```C++
static float waveTime = 0;
```

* In drawscene function, update the waveTime value and send it to the shader

```C++
   // Water settings
   waveTime += 0.004f;
   glUniform1f(glGetUniformLocation(programId, "waveTime"), waveTime);
```

* Add waveTime in vertex shader

```C++
uniform float waveTime;
```

* Update water mesh Y coordinates

```C++
    if (object == WATER)
    {
      coords = vec4(objCoords, 1.0f);
      coords.y += 1.5f * (sin(coords.x + waveTime) + cos(coords.z + waveTime)) + 10.0f;
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
    }
```


* Compile and run

 ![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%2010/Readme%20Pictures/Example.JPG)








