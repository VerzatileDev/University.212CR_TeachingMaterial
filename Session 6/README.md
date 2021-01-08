# Session 6 - Put All Together

#### Table of Contents
1. [Combine Light and Texture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%206#Combine-Light-and-Texture)
2. [Animation](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%206#Animation)
3. [Animation using shader](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%206#Animation-using-shader)
4. [Interactive Camera](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%206#Interactive-Camera)
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


### combine diffuse light with texture

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
 

## Animation using shader

In this section, you will learn how to import your model in OBJ format into the OpenGL program.


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

## Interactive Camera

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


* Add your model into scene.

You should create a track 3D model in 3DS max or Blender and export it as OBJ file. Import it into your OpenGL program.

You should position the center of your track model in the orgin of coordinate system in 3DS max or Blender. 
That will cause less trouble to position it in your OpenGL program. Please adjust model scale it first and apply translation to position the model.




