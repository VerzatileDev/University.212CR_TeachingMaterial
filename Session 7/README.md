# Session 7 - Advanced Topics One

#### Table of Contents
1. [Skybox](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%207#Skybox)
2. [Look around camera](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%207#Look-around-camera)
3. [Your own project](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%207#Your-own-project)

Welcome to Week 7! 

> In this week, we will learn how to create a Skybox and create a look-around camera.


## Skybox

A skybox is a panoramic view representing a sky or any other scenery. It is a simple way to add realism to a game with minimal performance cost.

A skybox is generated from a cube. Each face of the cube contains a texture representing a visible view (up, down, front, back, left, right) of the scenery.
An example of skybox texture images is shown here:

![Tex1 picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%207/Readme%20Pictures/Skybox.png)

To implement a skybox is quite simple. We simply unwrap a cube into its UV Map. Apply a texture to each face of the cube and render the cube in the middle of the scene.

### Loading a skybox

* Download wood texture (wood.png) from week 6 folder and put it in the Textures folder.
* Add woodTexLoc variable after skyTexLoc

```C++
 woodTexLoc,
```

* In TexNames[] array (setup() function), add wood.png into namelist


```C++
 "Textures/wood.png",
```

* change 

```C++
 glGenTextures(2, texture);
```

to 

```C++
 glGenTextures(3, texture);
```

* Also change texture[2];  to texture[3]; 

* Add texture initalization codes

```C++
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, texture[2]);

   data = SOIL_load_image(TexNames[2].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   SOIL_free_image_data(data);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   woodTexLoc = glGetUniformLocation(programId, "woodTex");
   glUniform1i(woodTexLoc, 2); //send texture to shader
```

### Add texture drawing in fragment shader

* Add woodTex definition

```C++
uniform sampler2D woodTex;
```

* Add Wood texture color calculation codes

```C++
woodTexColor = texture(woodTex, texCoordsExport);
```

* Finally, test the texture out. Comments all lighting calculation codes and add woodTexColor only.

```C++
   if (object == HOVER) {
    colorsOut = woodTexColor;
    //normal = normalize(normalExport);
    //lightDirection = normalize(vec3(light0.coords));
    //fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
    //colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
   }
```

* It should look like this

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





