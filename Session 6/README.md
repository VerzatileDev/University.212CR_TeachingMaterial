# Session 6 - Put All Together

#### Table of Contents
1. [Combine Light and Texture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%206#Combine-Light-and-Texture)
2. [Animation](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%206#Animation)
3. [Animation using shader](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%206#Animation-using-shader)
4. [Work on your own model](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%206#Work-on-your-own-model)
4. [Homework](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%205#Homework)

Welcome to Week 6! 

> In this week, we will learn how to put everything learn in last few week together and create a interactive scene with animations.


## Combine Light and Texture

We are going to add texture and lighting effects to the hovercraft model.


### Add texture

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


## Animation

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


## Animation using shader

In this section, you will learn how to use a uniform variable in vextex shader to create animation

You can pass a variable to shader to change the position of an object (instead of going through modelview matrix).

* Add

```C++
static float yVal = 0; // Y Co-ordinates of the track. 
```

* Change yVale in animation function

Add yValLoc for accessing yPos in the shader

```C++
static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   objectLoc,
   yValLoc, //added
   grassTexLoc,  
   skyTexLoc,
   woodTexLoc,
   buffer[6], 
   vao[5], 
   texture[3];  
```

* Add codes in animation function to change yVale

```C++
	yVal += 0.1;
	if (yVal > 12.0) yVal = 0.0;
```

* Send value to shader (in animation function)

```C++
	//set yVal to vertex shader
	yValLoc = glGetUniformLocation(programId, "yPos");  //uniform uint object;
	glUniform1f(yValLoc, yVal);
```

* Add yPos into vertex shder (vertexShader.glsl)

Add

```C++
uniform float yPos;
```

* Add track position update codes

```C++
    if (object == TRACK)
    {
      coords = vec4(objCoords, 1.0f);
      coords.y = coords.y+yPos; //added
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
    }
```

* Compile and run


## Work on your own model

Now, you should be able to work on your own model 

For example, 

* you can create a car model in 3DS max with normal vector and texture coordinates.

* Import it into your own project

* Import your own texture map

* Add texture initialization code and sent it to the shader

* combine diffuse lighting effect with texture color

* Add some animation


## Homework


* Create complex motion animation, for example, move the object around the track of a circle.

You should update the position of object around the track of a circle. The circle equation is

x = R*cos(angle);
y = R*sin(angle);

Where R is the radius of circle. The origin  of circle in vec3(0,0,0);
You suggest that your compute an array of position along the track of a circle in the setup function
 so that you do not have to compute them during the runtime. 




