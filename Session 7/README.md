# Session 7 - Advanced Topics One

#### Table of Contents
1. [Skybox](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%207#Skybox)
2. [Look around camera](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%207#Look-around-camera)
3. [Your own project](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%207#Your-own-project)

Welcome to Week 7! 

> In this week, we will learn how to create a Skybox and create a look-around camera.


## Skybox

Since a skybox is by itself just a cubemap, loading a skybox isn't too different from what we've seen at the start of this chapter. To load the skybox we're going to use the following function that accepts a vector of 6 texture locations:

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%207/Readme%20Pictures/Skybox.png)

To implement a skybox is quite simple. We simply unwrap a cube into its UV Map. Apply a texture to each face of the cube and render the cube in the middle of the scene.

### Loading a skybox

* Download the base project. Open Skybox.cpp

* Since a skybox is by itself just a cubemap, loading a skybox is to accept a vector of 6 texture locations.

A cubemap is a texture like any other texture, so to create one we generate a texture and bind it to the proper texture target 
before we do any further texture operations. This time binding it to GL_TEXTURE_CUBE_MAP:

Because a cubemap contains 6 textures, one for each face, we have to call glTexImage2D six times with their parameters set. 
We have to set the texture target parameter to match a specific face of the cubemap, telling OpenGL which side of the cubemap we're creating a texture for. 
This means we have to call glTexImage2D once for each face of the cubemap.

Since we have 6 faces OpenGL gives us 6 special texture targets for targeting a face of the cubemap:

Texture target	Orientation
* GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
* GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
* GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
* GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
* GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back
* GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front

Like many of OpenGL's enums, their behind-the-scenes int value is linearly incremented, 
so if we were to have an array or vector of texture locations we could loop over them by starting with GL_TEXTURE_CUBE_MAP_POSITIVE_X and
 incrementing the enum by 1 each iteration, effectively looping through all the texture targets:

```C++
for (unsigned int i = 0; i < 6; i++)
 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); 
```

Because a cubemap is a texture like any other texture, we will also specify its wrapping and filtering methods

```C++
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
```
Don't be scared by the GL_TEXTURE_WRAP_R, this simply sets the wrapping method for the texture's R coordinate 
which corresponds to the texture's 3rd dimension (like z for positions). We set the wrapping method to GL_CLAMP_TO_EDGE 
since texture coordinates that are exactly between two faces may not hit an exact face (due to some hardware limitations) so 
by using GL_CLAMP_TO_EDGE OpenGL always returns their edge values whenever we sample between faces.

* Add following codes to Skybox::InitialiseCubeMap().  

```C++
void Skybox::InitialiseCubeMap()
{
	std::string skyboxTextures[] = 
	{	
		"Textures/SkyboxRight.jpg",
		"Textures/SkyboxLeft.jpg",
		"Textures/SkyboxTop.jpg",
		"Textures/SkyboxBottom.jpg",
		"Textures/SkyboxFront.jpg",
		"Textures/SkyboxBack.jpg"
	};
	glUseProgram(programId);
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0 + textureID);
	myTextureIDs[0] = textureID;
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height;
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char *data = SOIL_load_image(skyboxTextures[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		SOIL_free_image_data(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);

	++textureID;
}
```


### Redner Skybox

* Rendering the skybox is easy now that we have a cubemap texture, 
we simply bind the cubemap texture and the skybox sampler is automatically filled with the skybox cubemap.
 To draw the skybox we're going to draw it as the first object in the scene and disable depth writing. 
 This way the skybox will always be drawn at the background of all the other objects 
 since the unit cube is most likely smaller than the rest of the scene.


```C++
void Skybox::Draw()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(programId);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	int pos =glGetUniformLocation(programId, "skyboxTexture");
	glUniform1i(glGetUniformLocation(programId, "skyboxTexture"), myTextureIDs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}
```

* Finally it should look like this

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%206/Readme%20Pictures/Texture.JPG)

### combine diffuse light with texture

To improve the appearance of hovercraft, we need to add diffuse lighting effects on it. 
 
* We only need to combine texture color with diffuse light color. 
We only need multiple texture color with diffuse color in fragment shader

```C++
   if (object == HOVER) {
    normal = normalize(normalExport);
    lightDirection = normalize(vec3(light0.coords));
    fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
    colorsOut =  woodTexColor*vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
   }
```

* Finally, it should look like this

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%206/Readme%20Pictures/TextureLight.JPG)


## Look around camera

In this section, you will learn how to animate the object using glutIdleFunc function.


### How glutIdleFunc animation works. 

glutIdleFunc sets the global idle callback to be func so a GLUT program can perform background processing tasks or continuous
 animation when window system events are not being received. If enabled, the idle callback is continuously called when events are not being received. 

In this way, if the position of the object was changed over the time, it will create animation of the moving object.

* First, create a global variable for the position 

```C++
static float xVal = 0; // X Co-ordinates of the hover. 
```

* In animate function, change codes to add the position of the target object

```C++
void animate() 
{
	zVal = zVal - 0.2;
	xVal += 0.1;
	if (zVal < -25.0) zVal = 0.0;
	if (xVal > 12.0) xVal = -12.0;
	testSphere.SetPosition(vec3(0, 0, zVal)); //modify sphere's position
	Hover.SetPosition(vec3(xVal, 0, 0));
	// refresh screen 
	glutPostRedisplay();
}
```

* Finally, in Model class c++ file, add position update for modelview matrix in updateModelMatrix() function

```C++
void Model::updateModelMatrix(unsigned int modelViewMatLoc,float d,float scale,float ZPos)
{
	ModelMatrix = mat4(1.0);
	ModelMatrix = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0 + d, 10.0, 0.0), vec3(0.0, 1.0, 0.0)); //camera matrix, apply first
	ModelMatrix = glm::scale(ModelMatrix, vec3(scale, scale, scale));  //scale down the model
	ModelMatrix = glm::translate(ModelMatrix, vec3(0.0f, 0.0f, ZPos));
	ModelMatrix = glm::translate(ModelMatrix, GetPosition());
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix));  //send modelview matrix to the shader
}
```

## Your own project

Now, you should be able to work on your own model 

For example, 

* you can create a car model in 3DS max with normal vectors and texture coordinates.

* Import it into your own project

* Import your own texture map

* Add texture initialization code and sent it to the shader

* combine diffuse lighting effect with texture color

* Add some animations





