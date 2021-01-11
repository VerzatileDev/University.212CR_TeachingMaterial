# Session 8 - Advanced Topics Two

#### Table of Contents
1. [Instancing](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%208#Instancing)
2. [Blending](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%208#Blending)
3. [Billboards](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%208#Billboards)
4. [Home work](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%208#Home-work)

Welcome to Week 8! 

> In this week, we will learn how to add instances of an object, blending the object and add billboards into the scene.

## Instancing

Say you have a scene where you're drawing a lot of models where most of these models contain the same set of vertex data, 
but with different world transformations. Think of a scene filled with grass leaves: each grass leave is a small model that consists of only a few triangles. 
You'll probably want to draw quite a few of them and your scene may end up with thousands or maybe tens of thousands of grass leaves 
that you need to render each frame. Because each leaf is only a few triangles, the leaf is rendered almost instantly. 
However, the thousands of render calls you'll have to make will drastically reduce performance.

If we were to actually render such a large amount of objects it will look a bit like this in code:

```C++
for(unsigned int i = 0; i < amount_of_models_to_draw; i++)
{
    DoSomePreparations(); // bind VAO, bind textures, set uniforms etc.
    glDrawArrays(GL_TRIANGLES, 0, amount_of_vertices);
}
```

When drawing many instances of your model like this you'll quickly reach a performance bottleneck because of the many draw calls. 
Compared to rendering the actual vertices, telling the GPU to render your vertex data with functions like glDrawArrays or 
glDrawElements eats up quite some performance since OpenGL must make necessary preparations before it can draw your vertex data 
(like telling the GPU which buffer to read data from, where to find vertex attributes and all this over the relatively slow CPU to GPU bus).
 So even though rendering your vertices is super fast, giving your GPU the commands to render them isn't.
 
It would be much more convenient if we could send data over to the GPU once, and then tell OpenGL to draw multiple objects using this data with a single drawing call. 
That is instancing.

To render using instancing many times,

* glDrawArrays -> glDrawArraysInstanced
* glDrawElements -> glDrawElementsInstanced 

These instanced versions of the classic rendering functions take an extra parameter called the instance count
 that sets the number of instances we want to render. We sent all the required data to the GPU once, 
 and then tell the GPU how it should draw all these instances with a single call. 
 The GPU then renders all these instances without having to continually communicate with the CPU.

### Implementation

* Download the base project (CreateSphereClass.zip). Always to Compile option to "x64".  Open CreateSphere.cpp

We just need to modify the drawing function to draw 10 spheres instead one.

Replace

```C++
glDrawElements(GL_TRIANGLE_STRIP, triCount, GL_UNSIGNED_INT, sphereIndices);
```

with 

```C++
glDrawElementsInstanced(GL_TRIANGLE_STRIP, 660, GL_UNSIGNED_INT, sphereIndices, 10);
```


* Modify Vertex shader codes so that we can see 10 sphere.

Now, all 10 sphere is in the same position. So, you do not see any difference at all. Modify position  

Replace
```C++
   if (object == SPHERE)
   {
      coords = sphereCoords;
      normalExport = sphereNormals;
   }
```

with

```C++
   if (object == SPHERE)
   {
      coords = vec4(sphereCoords.x+gl_InstanceID*3,sphereCoords.y,sphereCoords.z,sphereCoords.w);
      normalExport = sphereNormals;
   }
```

Here gl_InstanceID is a built-in variable inside shader which represent the id of instance object. So, it is range from 0 to 9. 
We, move the x coordinate by gl_InstanceID*3 for each instance.


* Finally it should look like this (Always to Compile option to "x64" )

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%208/Readme%20Pictures/Instance.JPG)

### Add instancing into your own project

* Select a decorated object in your scene such as a rock object. Modify the drawing codes.

* In the vertex shader, change their position. Ideally, along certain pattern such as line, circle and etc.


## Blending

Blending in OpenGL is commonly known as the technique to implement transparency within objects. 
Transparency is all about objects (or parts of them) not having a solid color, 
but having a combination of colors from the object itself and any other object behind it with varying intensity.

The textures we've used so far all consisted of 3 color components: red, green and blue, 
but some textures also have an embedded alpha channel that contains an alpha value per texel (such as PNG image file).

We are going to use a transparent image (trees.png) as the texture map and use it as decorated objects.

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%208/Readme%20Pictures/trees.png)


* Step 1: Add a rectangle plane into your secne 

Use the project in instancing (set the number of instance to 1), open CreateSphere.cpp. Add Both VAO and VBO for new object: Tree.

```C++
static enum object {FIELD, SKY,SPHERE, TREE}; // VAO ids.
static enum buffer {FIELD_VERTICES, SKY_VERTICES,SPHERE_VERTICES, SPHERE_INDICES, TREE_VERTICES}; // VBO ids.
```

* Increase VAO and VBO array size

```C++
   buffer[5], ///add one more object
   vao[4], ///add one more object
   
   glGenVertexArrays(4, vao); ///add one more object
   glGenBuffers(3, buffer);  ///add one more object
```

* Create ID for VBO

```C++
   glGenBuffers(1, &buffer[TREE_VERTICES]);
```

* Binding VAO and VBO

```C++
   glBindVertexArray(vao[TREE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[TREE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(treeVertices), treeVertices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(treeVertices[0]), 0);  //layout(location=2) in vec4 skyCoords;
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(treeVertices[0]), (void*)(sizeof(treeVertices[0].coords))); //layout(location=3) in vec2 skyTexCoords;
   glEnableVertexAttribArray(1);
```

* Draw tree

```C++
   // Draw Tree.
   glUniform1ui(objectLoc, TREE);  //if (object == TREE)
   glBindVertexArray(vao[TREE]);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
```

* Add codes for vertex shader

```C++
#define TREE 3

   if (object == TREE)
   {
      coords = Coords;
      texCoordsExport = TexCoords;
   }
```

* Add testing codes for fragment shader

For now, we just use sky texture map for testing

```C++
#define TREE 3

 if (object == TREE) colorsOut = skyTexColor;
```

* compile and run

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%208/Readme%20Pictures/StepOne.JPG)

## Billboards

It is an advanced topic (optional). 

For example, 

* Add skybox into your project (optional)

* Add look around camera into your project. You have to modify the model class to allow input modelview matrix from camera setup.
change void Model::updateModelMatrix(unsigned int modelViewMatLoc,float d,float scale,float ZPos) function (both input parameters and codes)

* Add mouse control camera codes (optional, advanced level)
Tutorials: https://learnopengl.com/Getting-started/Camera


## Home work

Now, you should be able to work on your own project 

For example, 

* Add skybox into your project (optional)

* Add look around camera into your project. You have to modify the model class to allow input modelview matrix from camera setup.
change void Model::updateModelMatrix(unsigned int modelViewMatLoc,float d,float scale,float ZPos) function (both input parameters and codes)

* Add mouse control camera codes (optional, advanced level)
Tutorials: https://learnopengl.com/Getting-started/Camera




