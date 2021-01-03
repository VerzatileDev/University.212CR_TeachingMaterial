# Session 5 - Import 3D Models

#### Table of Contents
1. [What is OBJ file](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#What-is-OBJ-file)
2. [Create Models](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#Create-Models)
3. [Import OBJ file](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#Import-OBJ-file)
4. [Class design](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#Class-design)
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
You need to complete the CreateObject() function to add importing codes for OBJ file.

There are already two testing OBJ files in week 5 folder. Please download track.obj and hover.obj and put them in the project folder.

* Modify Model.h 

We need change Model class so that it can input a OBJ file name, load it and draw it.
First  

```C++
 std::vector<Vertex> mesh = loadOBJ(objFile); 
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


## Class design

As you have learned object design principle in last few weeks, it is the time that you should think about designing c++ classes for modern OpenGL game engine.
Here some example of C++ classes are given. The first one is the abstract class for game object.

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%204/Readme%20Pictures/Texture1.JPG)

Header file

```C++
#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "SphereCollider.h" //link to your physics (Sphere collider)

class GameObject
{
protected:
	
public:
	GameObject();
	GameObject(glm::vec3 pos);
	~GameObject();

	glm::vec3 position;
	
	//link to your Sphere collider
	SphereCollider* collider;
	SphereCollider* GetCollider();
	void AttachCollider(SphereCollider* attachingCollider);

    //OpenGL drawing function
	virtual void Draw() = 0;
	
	///physics update function
	virtual void Update(float);
};

```

C++ file

```C++
#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::GameObject(glm::vec3 pos)
{
	position = pos;
}

GameObject::~GameObject()
{
}

SphereCollider* GameObject::GetCollider()
{
	return collider;
}

void GameObject::AttachCollider(SphereCollider* attachingCollider)
{
	collider = attachingCollider;
}

void GameObject::Update(float deltaTime)
{
}

```

Second example is a stationary object: Rock.

Header file

```C++
#pragma once
#include "GameObject.h"
#include "SphereCollider.h"
#include "objReader.h"  // OBJ file reader 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

class Rock : public GameObject
{
private:
	
public:
	Rock(glm::vec3 pos);
	~Rock();

	SphereCollider* collider;
	SphereCollider* GetCollider();

	void Draw();
	void Update(float, glm::vec3 offset);
};
 
```

C++ file

```C++
#include "Rock.h"

Rock::Rock(glm::vec3 pos) : GameObject(pos)
{
	position = pos;
	collider = new SphereCollider(4, glm::vec3(position.x, position.y, position.z));
	AttachCollider(collider);
}

Rock::~Rock()
{
}

SphereCollider* Rock::GetCollider()
{
	return collider;
}

void Rock::Draw()
{
}

void Rock::Update(float deltaTime, glm::vec3 offset)
{
	collider->Update(deltaTime, position, offset);
}
```

The final example is a moving object which is controlled by the user

Header file 


```C++
#pragma once
#include "GameObject.h"
#include "SphereCollider.h"
#include "objReader.h"
#include "Rock.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

class Hovercraft : public GameObject
{
private:
	float mass;

	glm::vec3 acceleration;
	glm::vec3 velocity;

public:
	Hovercraft(float mass, glm::vec3 pos);
	~Hovercraft();

	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 1.0f);
	float accelerationOrientation = 0;
	float angle;
	float drag = 0.8f;
	float terminalVelocity = 10;
	float turnSpeed = 0.0f;
	float propellorForce = 2.0f;

	SphereCollider* collider;
	SphereCollider* GetCollider();

	void Draw();
	void Update(float, glm::vec3 offset);
	void Collide(Rock collidedObject);
};
```

## Homework


* Change sky texture map when a key is pressed.

> Implement additional codes for loading night sky texture (nightSky.bmp) in Textures folder of the base project.
Also send it the shader when a key is pressed. So, the sky texture can be changed when a key is pressed.



