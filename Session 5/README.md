# Session 5 - Import 3D Models

#### Table of Contents
1. [What is OBJ file](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#What-is-OBJ-file)
2. [Create Models](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#Create-Models)
3. [Add Sky](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#Add-Sky)
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


## Base Project

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


## Class design

As you have learned object design principle in last few weeks, it is the time that you should think about designing c++ classes for modern OpenGL game engine.
Here some example of C++ classes are given. The first one is the abstract class for game object.

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



