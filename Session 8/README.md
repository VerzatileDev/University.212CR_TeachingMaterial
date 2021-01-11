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

### Try to add a skybox into your project (Optional)

* Now, you can try to add the skybox into your own project. Following instructions were just for guidance (probably not able to solve every problem you have)

* add skybox texture map into your texture folder.  The list is

```C++
"Textures/SkyboxRight.jpg",
"Textures/SkyboxLeft.jpg",
"Textures/SkyboxTop.jpg",
"Textures/SkyboxBottom.jpg",
"Textures/SkyboxFront.jpg",
"Textures/SkyboxBack.jpg"
```

* Copy Both Skybox.cpp and Skybox.h into your project folder

* Copy Both SkyboxFragmentShader.glsl and SkyboxVertexShader.glsl into your project.
Both shaders are stand-alone shader, you do not need to add them into your main shaders.

* Add Skybox initialization codes into your main.cpp. Please refer to example project main.cpp to find out where to put them.

```C++
Skybox skybox;

skybox.InitialiseCubeMap();	
skybox.InitialiseSkybox();
skybox.CreateShader("SkyboxVertexShader.glsl", "SkyboxFragmentShader.glsl");
skybox.SetViewMatrix(modelViewMat);	


skybox.Bind();
skybox.Draw();

	
```

* Also, you need to be careful with textureID in Skybox class, which gives you the information which textureID has been used.
So, you should use skybox texture as the first textureID and add your other textures after it.

## Blending

In this section, you will learn how to Add a camera which can be controlled by key pressing to look around the scene.


### Basic theory. 

To look around the scene we have to change the cameraForward vector based on the input of the keyboard. However, changing the direction vector is a little complicated and requires some trigonometry. 
If you do not understand the trigonometry, don't worry, you can just skip to the code sections and paste them in your code; you can always come back later if you want to know more.. 


* Euler angles 

Euler angles are 3 values that can represent any rotation in 3D, defined by Leonhard Euler somewhere in the 1700s. 
There are 3 Euler angles: pitch, yaw and roll. The following image gives them a visual meaning:

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%207/Readme%20Pictures/Euler.png)

The pitch is the angle that depicts how much we're looking up or down as seen in the first image. 
The second image shows the yaw value which represents the magnitude we're looking to the left or to the right. 
The roll represents how much we roll as mostly used in space-flight cameras. 
Each of the Euler angles are represented by a single value and with the combination of all 3 of them we can calculate any rotation vector in 3D.

For our camera system we only care about the yaw and pitch values so we won't discuss the roll value here. 
Given a pitch and a yaw value we can convert them into a 3D vector that represents a new direction vector. 
The process of converting yaw and pitch values to a direction vector requires a bit of trigonometry.


```C++
cameraForward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
cameraForward.y = sin(glm::radians(pitch));
cameraForward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
```

### Implmentation codes.

* Add Yaw and Pitch definitions (global variables)

```C++
float cameraYaw = 90;
float cameraPitch;
```

* Create UpdateCamera function based trigonometry

```C++
void UpdateCamera()
{
	if (cameraPitch < -89)
	{
		cameraPitch = -89;
	}
	if (cameraPitch > 89)
	{
		cameraPitch = 89;
	}
	glm::vec3 eye = glm::vec3(0,0,0);
	eye.x = glm::cos(glm::radians(cameraPitch)) * -glm::cos(glm::radians(cameraYaw));
	eye.y = glm::sin(glm::radians(cameraPitch));
	eye.z = glm::cos(glm::radians(cameraPitch)) * glm::sin(glm::radians(cameraYaw));

	cameraForward = glm::normalize(eye);

	glm::mat4 modelViewMat = glm::lookAt(cameraForward, glm::vec3(0), glm::vec3(0, 1, 0));
	modelViewMat = glm::translate(modelViewMat, cameraPosition);
	skybox.SetViewMatrix(modelViewMat);
}
```

* Add Keyboard handling codes

w move forward. s move backword. a move left. d move right. e move up. q move down.


```C++
void KeyInputCallback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	{
		cameraPosition += cameraForward;
		UpdateCamera();
	}break;
	case 's':
	{
		cameraPosition += -cameraForward;
		UpdateCamera();
	}break;
	case 'a':
	{
		cameraPosition += -glm::normalize(glm::cross(cameraForward, glm::vec3(0, 1, 0)));
		UpdateCamera();
	}break;
	case 'd':
	{
		cameraPosition += glm::normalize(cross(cameraForward, glm::vec3(0, 1, 0)));
		UpdateCamera();
	}break;
	case 'e':
	{
		cameraPosition += -glm::vec3(0, 1, 0);
		UpdateCamera();
	}break;
	case 'q':
	{
		cameraPosition += glm::vec3(0, 1, 0);
		UpdateCamera();
	}break;
	case 27:
	{
		exit(0);
	}break;
	}
}
```

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




