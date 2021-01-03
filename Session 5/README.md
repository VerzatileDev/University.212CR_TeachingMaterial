# Session 5 - Import 3D Models

#### Table of Contents
1. [What is OBJ file](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#What-is-OBJ-file)
2. [Create Models](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#Create-Models)
3. [Import OBJ file](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#Import-OBJ-file)
4. [Add another OBJ](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#Add-another-OBJ)
5. [Homework](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#Homework)

Welcome to Week 5! 

> In this week, we will learn how to use OBJ file to export from 3D modelling software and import into your OpenGL program.
Also, we will learn how to create a racing track in 3DS Max.


## What is OBJ file

An OBJ file is a standard 3D image format that can be exported and opened by various 3D image editing programs. 
It contains a three-dimensional object, which includes 3D coordinates, texture maps, polygonal faces, 
and other object information.

The OBJ format is considered to be a universal 3D model format since it is widely supported by 3D editing applications. 
The format is simple and text-based, which is one reason why many programs use it. 
However, the simple format structure may also lead to huge OBJ file sizes if they store large and complex 3D objects.

If you receive an OBJ file you can choose from a large number of free and commercial applications to open it such as 
Microsoft 3D Builder, Microsoft 3D viewer (Windows 10), 3DS Max, Blender, MeshLab and more. 


### File structure

* The following types of data may be included in an .obj file. In this
list, the keyword (in parentheses) follows the data type.

### Vertex data

*       geometric vertices (v)
*       texture vertices (vt)
*       vertex normals (vn)

Example

* v 34.954346 7.512550 -53.632324
* vt 1.000000 1.000000
* vn -0.0000 1.0000 0.0000


### Face elements

Faces are defined using lists of vertex, texture and normal indices in the format
 vertex_index/texture_index/normal_index for which each index starts at 1 and increases corresponding to the order
 in which the referenced element was defined. Polygons such as quadrilaterals can be 
 defined by using more than three indices.
 
 Each face can contain three or more vertices.
 
 Example
 
 * f v1 v2 v3 ....
 
 Optionally, texture coordinate indices can be used to specify texture coordinates when defining a face. 
 To add a texture coordinate index to a vertex index when defining a face, 
 one must put a slash immediately after the vertex index and then put the texture coordinate index.
 No spaces are permitted before or after the slash. A valid texture coordinate 
 index starts from 1 and matches the corresponding element in the previously defined list of texture coordinates.
 Each face can contain three or more elements.
 
 Example
 
 * f v1/vt1 v2/vt2 v3/vt3 ...
 
 
 Optionally, normal indices can be used to specify normal vectors for vertices when defining a face. 
 To add a normal index to a vertex index when defining a face, one must put a second slash after the texture
 coordinate index and then put the normal index. A valid normal index starts from 1 and matches the 
 corresponding element in the previously defined list of normals. Each face can contain three or more elements.
 
 Example
 
 * f v1//vn1 v2//vn2 v3//vn3 ...
 
## Create Models

Please download MakingRaceTrack3DSMax.pdf in week 5 folder. Following the tutorial to create your own racing track. 
You also can watch youtube video embedded in the PDF and following the video tutorial.

In the end, please export texture coordinates if you have applied texture to the object.

You are also welcome to use Blender to create the racing track if you prefer to. 


## Import OBJ file

In this section, you will learn how to import your model in OBJ format into the OpenGL program.


### Base Project

The base project (LoadObjClass.zip) is located in week 5 folder. Please download it and unzip it.

The codes handle the import of OBJ file is inside the header file : OBJloader.h.
This is a head file only library. It is a simplified version of Bly7 OBJ Loader library (https://github.com/Bly7/OBJ-Loader).
If you prefer to use the Bly7 OBJ loader, you can use Bly7 OBJ-Loader (https://github.com/Bly7/OBJ-Loader) 
which can material files.

But the following tutorial is designed for my OBJ loader. If you use Bly7 OBJ-Loader, you have to change some codes (for example, data structures)
  

### Add Import codes into Model Class

There is already a skeleton class for importing OBJ file. It is Model class. 
In Solution Explorer (MS Visual Studio), double click Model.cpp and open it. 
You need to modify both header and cpp files to add importing codes for OBJ file and draw OBJ model.

There are already two testing OBJ files in week 5 folder. Please download track.obj and hover.obj and put them in the project folder.

* Modify Model.h 

We need change Model class so that it can input a OBJ file name, load it and draw it.

First declare NumVert variable as private variable

```C++
 int NumVert; 
```

Change CreateObject input parameter so it can take filename as an input parameter

```C++
 void CreateObject(const char *);
```

Change constructor input parameter so it can take filename as an input parameter

```C++
 Model(const char*);
```

Change updateModelMatrix input parameter so it can take position and scale for individual model.

```C++
 void updateModelMatrix(unsigned int, float,float,float);
```

* Modify constructor  

Change constructor to following codes (allow it take input filename

```C++
Model::Model(const char* name)
{
	position = vec3(0);
	VAO = VBO = 0;
	VerticesData = NULL; 
	NumVert = 0;
	CreateObject(name);
}
```

* Complete CreateObject function (in Model.cpp)

```C++
void Model::CreateObject(const char *name)
{
	std::vector<VertexWithAll> mesh = loadOBJ(name); //Import OBJ file data into a vector array
	NumVert = mesh.size();

	this->VerticesData = new VertexWithAll[NumVert];
	for (size_t i = 0; i < NumVert; i++)
	{
		this->VerticesData[i] = mesh[i];
	}
}
```

* Complete setup function (in Model.cpp). 

```C++
void Model::Setup()
{
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWithAll) * NumVert, VerticesData, GL_STATIC_DRAW);  ///please note the change

   glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, position));
   glEnableVertexAttribArray(4);
   glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, normal));
   glEnableVertexAttribArray(5);

   glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, textcoord));
   glEnableVertexAttribArray(6);

}
```

* Complete updateModelMatrix function (in Model.cpp).

So, it takes scale and Z position parameter. But you add more or remove parameters to make it subitable for your class.

```C++
void Model::updateModelMatrix(unsigned int modelViewMatLoc,float d,float scale,float ZPos)
{
	ModelMatrix = mat4(1.0);
	ModelMatrix = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0 + d, 10.0, 0.0), vec3(0.0, 1.0, 0.0)); //camera matrix, apply first
	ModelMatrix = glm::scale(ModelMatrix, vec3(scale, scale, scale));  //scale down the model
	ModelMatrix = glm::translate(ModelMatrix, vec3(0.0f, 0.0f, ZPos));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix));  //send modelview matrix to the shader
}
```

* Complete Draw function (in Model.cpp).

```C++
void Model::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, NumVert);
}
```

### Use Model class in GameEngine (LoadObjClass.cpp)

* Add head file

```C++
#include "Model.h"
```

* Add object into VAO and VBO

```C++
static enum object {FIELD, SKY, SPHERE,TRACK}; // VAO ids.
static enum buffer {FIELD_VERTICES, SKY_VERTICES,SPHERE_VERTICES, SPHERE_INDICES, TRACK_VERTICES}; // VBO ids.
```

* change VAO and VBO array definition

```C++
   buffer[5], ///add one more object
   vao[4], ///add one more object
```

* Generate more VAO in setup function

```C++
glGenVertexArrays(4, vao); ///add one more object
```

* Binding TRACK VAO and VBO in setup function

```C++
   //Binding Track VAO and VBO
   glGenBuffers(1, &buffer[TRACK_VERTICES]); ///generate one more id for VBO
   Track.SetIDs(vao[TRACK], buffer[TRACK_VERTICES], 0);
   Track.Setup();
```

* Add drawing codes for the TRACK in drawScene function, add it after draw sphere

```C++
   // Draw Track
   Track.updateModelMatrix(modelViewMatLoc, d, 0.2f,-60.0f);
   glUniform1ui(objectLoc, TRACK);  //if (object == TRACK)
   Track.Draw();
```

### Modify Vertex shader

Open vertexShader.glsl in MS Visual Studio.

* Add Track object definition

```C++
#define TRACK 3
```

* Add import ports, please note objCoords is vec3 (NOT vec4)

```C++
layout(location=4) in vec3 objCoords;
layout(location=5) in vec3 objNormals;
layout(location=6) in vec2 objTexCoords;

```

* Add codes to process Track data

Please note that objCoords is vec3. It need to be converted into vec4

```C++
	if (object == TRACK)
    {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
    }
```

### Modify Fragment shader

Open fragmentShader.glsl in MS Visual Studio.

* Add Track object definition

```C++
#define TRACK 3
```

* Add Track coloring codes. For now, we just borrow Sky texture to color the track object.

```C++
if (object == TRACK) {
    colorsOut = skyTexColor;
   }
```

* Finally, compile it and run it. You will see a track object with Sky texture in the scene.

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%205/Readme%20Pictures/Track.JPG)

## Add another OBJ

You can easily add another OBJ object into scene. For testing purpose, you can use hover.obj as the second testing file.

You need to declare another instantce of Model class as 

```C++
static Model Hover("hover.obj");
```

You also need to add one more object into VAO and VBO arrays. Following instructions from last section.

You need binding hover object and draw it. You could use following parameter for ModelView matrix

```C++
   // Draw Hover
   Hover.updateModelMatrix(modelViewMatLoc, d, 1.5f, 0.0f);
   glUniform1ui(objectLoc, HOVER);  //if (object == HOVER)
   Hover.Draw();
```

Finally, you need to add codes to both Vertex and Fragement Shaders.

This time, you can use diffuse lighting effects instead of texture mapping in Fragment shader.

```C++
if (object == HOVER) {
    normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
    colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
   }
```

Final result

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%205/Readme%20Pictures/Hover.JPG)


## Homework


* Change sky texture map when a key is pressed.

> Implement additional codes for loading night sky texture (nightSky.bmp) in Textures folder of the base project.
Also send it the shader when a key is pressed. So, the sky texture can be changed when a key is pressed.



