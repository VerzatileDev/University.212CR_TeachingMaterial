# Session 2 - Creat objects, Add lighting and Set up Camera

#### Table of Contents
1. [Create objects](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#create-objects)
2. [Add lighting](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Add-lights)
3. [Set up Camera](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Set-up-Camera)
4. [Homework](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Homework)

Welcome to Week 3! 

> By following these sessions, we will learn how to create geomtric objects, add lights into the scene and set up a camera using Modern OpenGL. 


## Create objects

There is no auxiliary functions available in Modern OpenGL to create simple geometric objects such as sphere, square, cube and etc.
So, we have to create the objects from scratch, which involves creating vertices, triangle indices and normal vectors (for lighting effects).

---- Download the base project ----
> Please download CreateSphereClass.zip and unzip it. The OpenGL library has been sep up so it can be compiled. 
After running the program, you only see a grey plane (field) and nothing else. We will add a sphere into the scene.
![Base picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%203/Readme%20Pictures/BaseProject.JPG)

---- Set up Data ----
> Add following data codes just before void drawScene(void) function in square.cpp file

```C++
float vertices[] = 
{
	20.0, 20.0, 0.0,
	80.0, 20.0, 0.0,
	20.0, 80.0, 0.0,
	80.0, 80.0, 0.0
};

float colours[] = 
{
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0
};
```

This will set up vertex data for the square. The vertices array is the coordinates of four corners of the square. 
The colours array is the colour of four corners. The colour is in the format of RGB (red, green, blue). 
The current value is black (0.0,0.0,0.0).

---- Set up drawing routine  ----

Generate a (1) BUFFER Object name																	

add following declare line just before void drawScene(void) function in square.cpp file

```C++
/// MODERN OPENGL - WILL HOLD 1 BUFFER OBJECT NAME
unsigned int buffer[1];
```
BIND THE BUFFER OBJECT

In the void setup(void) function, add following lines after  glClearColor(1.0, 1.0, 1.0, 0.0);

```C++
   /// MODERN OPENGL - GENERATE A(1) BUFFER OBJECT NAME
   glGenBuffers(1, buffer); // Generate buffer ids.
   
   /// MODERN OPENGL - BIND THE BUFFER OBJECT NAME - IT TELL OPENGL THIS IS THE BUFFER THE BELOW COMMANDS WILL WORK ON
   glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);   // Bind vertex buffer 
   /// MODERN OPENGL - CREATE AND INITIALIZE INTO THE BUFFER'S DATA STORE (BUFFER TYPE, SIZE, DATA GOING IN, USAGE)	
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colours), NULL, GL_STATIC_DRAW); // and reserve space
   
   /// MODERN OPENGL - 	(IN THIS CASE) TELL OPENGL THAT PART(FIRST HALF) OF THE BUFFER(BUFFERSUBDATA) IS FOR VERTICES
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);  // Copy vertex coordinates data into first half of vertex buffer.

   /// MODERN OPENGL - 		(IN THIS CASE) TELL OPENGL THAT PART(SECOND HALF) OF THE BUFFER(BUFFERSUBDATA) IS FOR COLOURS
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colours), colours); // Copy vertex color data into second half of vertex buffer.
   
   /// MODERN OPENGL - 		(IN THIS CASE) TELL OPENGL HOW MANY THINGS MAKE UP A VERTEX POSITION AND WHERE THIS STARTS
   glVertexPointer(3, GL_FLOAT, 0, 0);  // Specify vertex and color pointers to the start of the respective data.
   /// MODERN OPENGL - 			(IN THIS CASE) TELL OPENGL HOW MANY THINGS MAKE UP A VERTEX COLOUR AND WHERE THIS STARTS
   glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices)));
   
   /// MODERN OPENGL - (IN THIS CASE) TELL OPENGL TO ENABLE THE USE OF 2 CAPABILITIES, - VERTEX ARRAY - AND - COLOR ARRAY -
   glEnableClientState(GL_VERTEX_ARRAY);// Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_COLOR_ARRAY);
```
---- Add drawing codes  ----
> In the void drawScene(void) function, add following lines after  glClear(GL_COLOR_BUFFER_BIT);
```C++
   /// MODERN OPENGL - DRAW A GL_TRIANGLE_STRIP USING THE 4 VERTICES IN THE VBO
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
```

---- Test and change codes  ----

Now, you can run the code. You will see a black square in the middle of the window. You can change color data to change to your preferred color.
Or, you can change the coordinates of four corners of the square.
 
## Add lights

The main purpose of VAO is used for identifying each data object if more than one objects will be drawn in the scene.
  
FOR THIS EXERCISE AS WE HAVE 2 OBJECTS (THE SQUARE FROM EXECRISE 1 AND A TRIANGLE),
WE NEED TO USE VERTEX ARRAY OBJECTS (VAOs) FOR THIS.
THIS ALLOWS US TO SET THE BUFFERS FOR EACH ONCE AND THEN SWITCH BETWEEN THEM WHEN DRAWING
BY SELECTING THE VAO FOR THAT OBJECT.
		
WE ONLY NEED TO CREATE A VAO (AS LEGACY HAS THESE IN).
THIS MEANS NO SHADERS NEED TO BE USED - THIS WILL COME LATER IN EXERCISE 3.

FOR A VAO, WE WILL CREATE IT, DO THE VBO PARTS NEEDED FOR VERTEX/OBJECT CREATION IN THE SETUP FUNCTION
AND THEN SWITCH BETWEEN THESE IN THE DRAWING FUNCTION.

---- WHAT IS A VAO ----
> A VERTEX ARRAY OBJECT OR VAO REMEMBERS ALL VBOS AND MEMORY LAYOUTS OF THEM. IT ALLOWS SWITCHING BETWEEN DIFFERENT VERTEX DATA AND VERTEX FORMATS VIA BINDING A VAO.

---- Use the previous Projecrt from Exercise 1 ----
> Use the previous Projecrt from Exercise 1. Use following instructions to add codes or replace codes.

---- Set up Data ----
> Add following triange data right after the square data.

```C++
float vertices1[] =
{
	30.0, 30.0, 1.0,
	70.0, 30.0, 1.0,
	30.0, 70.0, 1.0

};

float colours1[] =
{
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0
};
```

---- Set up drawing routine  ----

Replace (we now have two objects instead one object)

```C++
/// MODERN OPENGL - WILL HOLD 1 BUFFER OBJECT NAME
unsigned int buffer[1];
```

with

```C++
/// MODERN OPENGL - WILL HOLD 2 BUFFER OBJECT NAMES (SQUARE, TRIANGLE)
unsigned int buffer[2];
```

Add VAO array after buffer[2] definition.

```C++
/// MODERN OPENGL - WILL HOLD 2  (SQUARE, TRIANGLE)
unsigned int vao[2]; // Array of VAO ids.
```

BIND THE BUFFER OBJECT

In the void setup(void) function, REPLACE

```C++
   /// MODERN OPENGL - GENERATE A(1) BUFFER OBJECT NAME
   glGenBuffers(1, buffer); // Generate buffer ids.
```

with (we have two objects. So, both vao and buffer have two objects)

```C++
   /// MODERN OPENGL -GENERATE VERTEX BUFFER OBJECT NAMES FOR EACH OBJECT(2 OVERALL - 1 FOR EACH)
   glGenVertexArrays(2, vao); // Generate VAO ids
   
   /// MODERN OPENGL -GENERATE BUFFER OBJECT NAMES FOR EACH OBJECT(2 OVERALL - 1 FOR EACH)
   glGenBuffers(2, buffer); // Generate buffer ids.
```

After glGenBuffers(2, buffer), REPLACE remaining codes ( The diference is to add glBindVertexArray(vao[0]);)


```C++
   /// MODERN OPENGL - BIND THE BUFFER OBJECT NAME - AKA TELL OPENGL THIS IS THE BUFFER THE BELOW COMMANDS WILL WORK ON
   glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);   // Bind vertex buffer 
   /// MODERN OPENGL - CREATE AND INITIALIZE INTO THE BUFFER'S DATA STORE (BUFFER TYPE, SIZE, DATA GOING IN, USAGE)	
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colours), NULL, GL_STATIC_DRAW); // and reserve space

  
   /// MODERN OPENGL - 	(IN THIS CASE) TELL OPENGL THAT PART(FIRST HALF) OF THE BUFFER(BUFFERSUBDATA) IS FOR VERTICES
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);  // Copy vertex coordinates data into first half of vertex buffer.

   /// MODERN OPENGL - 		(IN THIS CASE) TELL OPENGL THAT PART(SECOND HALF) OF THE BUFFER(BUFFERSUBDATA) IS FOR COLOURS
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colours), colours); // Copy vertex color data into second half of vertex buffer.

   /// MODERN OPENGL - 		(IN THIS CASE) TELL OPENGL HOW MANY THINGS MAKE UP A VERTEX POSITION AND WHERE THIS STARTS
   glVertexPointer(3, GL_FLOAT, 0, 0);  // Specify vertex and color pointers to the start of the respective data.
   /// MODERN OPENGL - 			(IN THIS CASE) TELL OPENGL HOW MANY THINGS MAKE UP A VERTEX COLOUR AND WHERE THIS STARTS
   glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices)));
   
   /// MODERN OPENGL - (IN THIS CASE) TELL OPENGL TO ENABLE THE USE OF 2 CAPABILITIES, - VERTEX ARRAY - AND - COLOR ARRAY -
   glEnableClientState(GL_VERTEX_ARRAY);// Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_COLOR_ARRAY);
```

with following codes for binding first object

```C++
   /// MODERN OPENGL - BIND THE FIRST VAO FOR THE FIRST OBJECT
   glBindVertexArray(vao[0]);

   /// MODERN OPENGL - BIND THE BUFFER OBJECT NAME - AKA TELL OPENGL THIS IS THE BUFFER THE BELOW COMMANDS WILL WORK ON
   glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);   // Bind vertex buffer 
   /// MODERN OPENGL - CREATE AND INITIALIZE INTO THE BUFFER'S DATA STORE (BUFFER TYPE, SIZE, DATA GOING IN, USAGE)	
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colours), NULL, GL_STATIC_DRAW); // and reserve space
  
   /// MODERN OPENGL - 	(IN THIS CASE) TELL OPENGL THAT PART(FIRST HALF) OF THE BUFFER(BUFFERSUBDATA) IS FOR VERTICES
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);  // Copy vertex coordinates data into first half of vertex buffer.

   /// MODERN OPENGL - 		(IN THIS CASE) TELL OPENGL THAT PART(SECOND HALF) OF THE BUFFER(BUFFERSUBDATA) IS FOR COLOURS
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colours), colours); // Copy vertex color data into second half of vertex buffer.

   /// MODERN OPENGL - 		(IN THIS CASE) TELL OPENGL HOW MANY THINGS MAKE UP A VERTEX POSITION AND WHERE THIS STARTS
   glVertexPointer(3, GL_FLOAT, 0, 0);  // Specify vertex and color pointers to the start of the respective data.
   /// MODERN OPENGL - 			(IN THIS CASE) TELL OPENGL HOW MANY THINGS MAKE UP A VERTEX COLOUR AND WHERE THIS STARTS
   glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices)));
   
   /// MODERN OPENGL - (IN THIS CASE) TELL OPENGL TO ENABLE THE USE OF 2 CAPABILITIES, - VERTEX ARRAY - AND - COLOR ARRAY -
   glEnableClientState(GL_VERTEX_ARRAY);// Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_COLOR_ARRAY);
```

Then add codes for binding second second

```C++
   glBindVertexArray(vao[1]);

   glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);   // Bind vertex buffer 
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1) + sizeof(colours1), NULL, GL_STATIC_DRAW); // and reserve space																					
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices1), vertices1);  // Copy vertex coordinates data into first half of vertex buffer.
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices1), sizeof(colours1), colours1); // Copy vertex color data into second half of vertex buffer.							
   glVertexPointer(3, GL_FLOAT, 0, 0);  // Specify vertex and color pointers to the start of the respective data.
   glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices1)));
   
   glEnableClientState(GL_VERTEX_ARRAY);// Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_COLOR_ARRAY);
```

---- Add drawing codes  ----

In the void drawScene(void) function, REPLACE

```C++
   /// MODERN OPENGL - DRAW A GL_TRIANGLE_STRIP USING THE 4 VERTICES IN THE VBO
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
```

with 

```C++
   /// MODERN OPENGL - BIND THE FIRST VAO FOR THE FIRST OBJECT
   glBindVertexArray(vao[0]);
   /// MODERN OPENGL - DRAW A GL_TRIANGLE_STRIP USING THE 4 VERTICES IN THE VBO
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   
   /// MODERN OPENGL - BIND THE SECOND VAO FOR THE SECOND OBJECT
   glBindVertexArray(vao[1]);
   /// MODERN OPENGL - DRAW A GL_TRIANGLE_STRIP USING THE 3 VERTICES IN THE VBO
   glDrawArrays(GL_TRIANGLES, 0, 3);
```

---- Test and change codes  ----

Now, you can run the code. You can change color data to change to your preferred color.
Or, you can change the coordinates of the triangle.

## Set up Camera
 
> You can use the finished codes in exercise 2 (VAO) or you can use square.cpp in week 2.

FOR THIS EXERCISE WE WILL FULLY CONVERT THE LEGACY PROGRAM INTO A MODERN ONE BY USING SHADERS ALONG WITH VBOs AND VAOs.

WE WILL NEED TO ALWAYS DEFINE AT LEAST 2 SHADERS
 
> THE VERTEX SHADER - WHICH RUNS PER VERTEX AND GIVES THE POSITION OF THE VERTEX

> THE FRAGMENT SHADER - WHICH RUNS PER PIXEL (WHICH CAN HAVE MULTIPLE FRAGMENTS IN IT) AND GIVES THE COLOUR OF THE PIXEL

* WHAT IS A SHADER

  A SHADER IS A MINI PROGRAM DEFINING A STYLE OF RENDERING ON THE GPU AND IS HIGHLY PARALLELISED.

* Step 1: Add header files

```C++
# include <fstream> ///FOR SHADER READING
# include <glm/glm.hpp> ///FOR MAT4 USAGE
# include <glm/gtc/type_ptr.hpp> ///FOR MAT4 VALUE PTR USAGE
# include <vector> ///FOR SHADER COMPILE TEST
# include <iostream> ///FOR SHADER COMPILE TEST
```

* Step 2: Set up gobal variable

Gobal variables are the codes after header files and before function definitions.

Replace the data definition 
		
```C++
float vertices[] = 
{
	20.0, 20.0, 0.0,
	80.0, 20.0, 0.0,
	20.0, 80.0, 0.0,
	80.0, 80.0, 0.0
};

float colours[] = 
{
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0
};return 0;

float vertices1[] =
{
	30.0, 30.0, 1.0,
	70.0, 30.0, 1.0,
	30.0, 70.0, 1.0

};

float colours1[] =
{
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 1.0
};
```

with (Here, we use a structure to combine vertex and color data together).

```C++
struct Vertex
{
	float coords[4];
	float colours[4];
};

///MODERN OPENGL - THESE ARE THE SAME AS EXERCISE 2 - JUST STORED DIFFERENTLY AND BASED OFF THE VERTEX CLASS NOW
Vertex squareVertices[] =
{
	{ { 20.0, 20.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { 80.0, 20.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { 20.0, 80.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { 80.0, 80.0, 0.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } }
};

Vertex triangleVertices[] =
{

	{ { 30.0, 30.0, 1.0, 1.0 },{ 1.0, 0.0, 1.0, 1.0 } },
	{ { 70.0, 30.0, 1.0, 1.0 },{ 1.0, 0.0, 1.0, 1.0 } },
	{ { 30.0, 70.0, 1.0, 1.0 },{ 1.0, 0.0, 1.0, 1.0 } }
};

```


Add modelview and project matrix definition

Both modelview and project matrix are used to definition the camera.


```C++
///MODERN OPENGL - OUR MATRICES - MODELVIEW (COMBINED MODEL MATRIX AND VIEW MATRIX) AND PROJECTION MATRIX
///				 - WHAT THESE DO IS ON MOODLE AS A LINK THAT SHOWS A DIAGRAM - http://antongerdelan.net/opengl/raycasting.html
///				 - I HAVE USED GLM (A MATHS LIBRARY) TO DO MY MATRICES WHILE THE BOOK CODE USES A SELF DEFINED METHOD

glm::mat4 modelViewMat(1.0f);
glm::mat4 projMat(1.0f);
```

Add program location and other global variables

```C++
///MODERN OPENGL - THESE ARE USED IN THE SETUPO TO SET UP SHADERS AND SEND DATA TO THEM
unsigned int programId,		vertexShaderId,		fragmentShaderId,	modelViewMatLoc,	projMatLoc;
```

* Step 3: Add shader loading and checking functions

Add shader loading function and checking function after all global variables

```C++
// Function to read text file.
char* readTextFile(const char* aTextFile)
{
	FILE* filePointer;
	fopen_s(&filePointer,aTextFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

void shaderCompileTest(GLuint shader)
{ 
	GLint result = GL_FALSE; 
	int logLength; glGetShaderiv(shader, GL_COMPILE_STATUS, &result); 
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength); 
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1); 
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]); 
	std::cout << &vertShaderError[0] << std::endl; 
}
```

* Step 4: Add shader initialization codes

Add shader loading and compiling check codes in the void setup(void) (after glClearColor(1.0, 1.0, 1.0, 0.0); )


```C++
   // Create shader program executable.
   char* vertexShader = readTextFile("vertexShader.glsl");
   vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
   glCompileShader(vertexShaderId);
   std::cout << "::: VERTEX SHADER :::" << std::endl;
   shaderCompileTest(vertexShaderId);

   char* fragmentShader = readTextFile("fragmentShader.glsl");
   fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
   glCompileShader(fragmentShaderId);
   std::cout << "::: FRAGMENT SHADER :::" << std::endl;
   shaderCompileTest(fragmentShaderId);

   programId = glCreateProgram();
   glAttachShader(programId, vertexShaderId);
   glAttachShader(programId, fragmentShaderId);
   glLinkProgram(programId);
   glUseProgram(programId);
```

Remove glEnableClientState functions (no longer needed for shaders)

```C++
   glEnableClientState(GL_VERTEX_ARRAY);// Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_COLOR_ARRAY);
```

Replace 

```C++
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colours), NULL, GL_STATIC_DRAW); // and reserve space
```

with (as we combine vertices and colours into one structure array)

```C++
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
```

Replace 

```C++
	/// MODERN OPENGL - 	(IN THIS CASE) TELL OPENGL THAT PART(FIRST HALF) OF THE BUFFER(BUFFERSUBDATA) IS FOR VERTICES
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);  // Copy vertex coordinates data into first half of vertex buffer.

   /// MODERN OPENGL - 		(IN THIS CASE) TELL OPENGL THAT PART(SECOND HALF) OF THE BUFFER(BUFFERSUBDATA) IS FOR COLOURS
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colours), colours); // Copy vertex color data into second half of vertex buffer.

   /// MODERN OPENGL - 		(IN THIS CASE) TELL OPENGL HOW MANY THINGS MAKE UP A VERTEX POSITION AND WHERE THIS STARTS
   glVertexPointer(3, GL_FLOAT, 0, 0);  // Specify vertex and color pointers to the start of the respective data.
   /// MODERN OPENGL - 			(IN THIS CASE) TELL OPENGL HOW MANY THINGS MAKE UP A VERTEX COLOUR AND WHERE THIS STARTS
   glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices)));
```

with

```C++
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), (GLvoid*)sizeof(squareVertices[0].coords));
   glEnableVertexAttribArray(1);
```

Remove glEnableClientState functions (no longer needed for shaders)

```C++
   glEnableClientState(GL_VERTEX_ARRAY);// Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_COLOR_ARRAY);
```

Replace 

```C++
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1) + sizeof(colours1), NULL, GL_STATIC_DRAW); // and reserve space
```

with (as we combine vertices and colours into one structure array)

```C++
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
```

Replace 

```C++
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices1), vertices1);  // Copy vertex coordinates data into first half of vertex buffer.
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices1), sizeof(colours1), colours1); // Copy vertex color data into second half of vertex buffer.							
   glVertexPointer(3, GL_FLOAT, 0, 0);  // Specify vertex and color pointers to the start of the respective data.
   glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices1)));
```

with

```C++
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(triangleVertices[0]), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(triangleVertices[0]), (GLvoid*)sizeof(triangleVertices[0].coords));
   glEnableVertexAttribArray(1);
```

Add mdoelview and project matrix codes

```C++
  // Obtain projection matrix uniform location and set value.
  glm::mat4 projMat  =
	{
		0.02, 0.0,  0.0, -1.0,
		0.0,  0.02, 0.0, -1.0,
		0.0,  0.0, -1.0,  0.0,
		0.0,  0.0,  0.0,  1.0
	};
   projMatLoc = glGetUniformLocation(programId, "projMat");
   glUniformMatrix4fv(projMatLoc, 1, GL_TRUE, glm::value_ptr(projMat));
   ///////////////////////////////////////

   // Obtain modelview matrix uniform location and set value.
   glm::mat4 modelViewMat(1.0f);
   modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE, glm::value_ptr(modelViewMat));
   ///////////////////////////////////////
```

* Step 5: Add shader codes

Download both fragmentShader.glsl and vertexShader.glsl from week 2 in github. Put both files in your project directory.

* Step 6: Run the codes

Make sure both fragmentShader.glsl and vertexShader.glsl together with the executable file in the same folder or put shader files in the project folder.

* Step 7: Review the relationship between shaders and c++ program.

Take a look the diagram beblow. 
Please note that location in layout in vertex shader is linking to "n" in glVertexAttribPointer and glEnableVertexAttribArray.
the name of uniform variable in vertext shader has to be exact the same name as it in glGetUniformLocation.

![Shader picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%202/Readme%20Pictures/ShaderProg.JPG)

## Homework


* Review the codes inside vertex and fragment shaders

> Understand what vertex shader and frament shader do. Take a look at https://learnopengl.com/Getting-started/Shaders
Read shader tutorials. 



