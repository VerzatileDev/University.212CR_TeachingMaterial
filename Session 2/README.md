# Session 2 - VBO, VAO and Shaders

#### Table of Contents
1. [Create VBO](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%202#create-vbo)
2. [Create VAO](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%202#create-vao)
3. [Convert to Modern OpenGL shader codes](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%202#Convert-to-Modern-OpenGL-shader-codes)
4. [Homework](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%202#Homework)

Welcome to Week 2! 

> By following these sessions, we will learn how to create VERTEX BUFFER OBJECT (VBO), VERTEX ARRAY OBJECTS (VAO) and Shader codes, which are used to draw objects in OpenGL. 


## Create VBO

FOR THIS EXERCISE WE ONLY NEED TO CREATE AN VERTEX BUFFER OBJECT (VBO) (AS LEGACY HAS THESE IN). 
THIS MEANS NO SHADERS NEED TO BE USED - THIS WILL COME LATER IN EXERCISE 3 (Shader).

FOR A VBO, WE NEED TO PUT THE VERTEX DATA INTO A BUFFER AND THEN SEND THIS TO THE GPU.
THIS WILL BE DIFFERENT LATER WHEN WE COMBINE CO-ORDS AND COLOUR INTO ONE VERTEX STRUCT, 
BUT WE WANT TO EASE YOU INTO MODERN OPENGL WITH THIS WEEKS LAB TASKS.

---- WHAT IS A VBO ----
> A VERTEX BUFFER OBJECT OR VBO IS AN ARRAY OF DATA THAT IS COPIED TO THE GPU BEFORE DRAWING AND RAN ON THE GPU

---- Download the Base Projecrt ----
> Please download ModernOpenGLSimple.zip and unzip it. The OpenGL library has been sep up so it can be compiled.

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
 
## Create VAO

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

## Convert to Modern OpenGL shader codes
 
> You can use the finished codes in exercise 2 (VAO) or you can use square.cpp in week 2.

FOR THIS EXERCISE WE WILL FULLY CONVERT THE LEGACY PROGRAM INTO A MODERN ONE BY USING SHADERS ALONG WITH VBOs AND VAOs.

WE WILL NEED TO ALWAYS DEFINE AT LEAST 2 SHADERS
 
> THE VERTEX SHADER - WHICH RUNS PER VERTEX AND GIVES THE POSITION OF THE VERTEX

> THE FRAGMENT SHADER - WHICH RUNS PER PIXEL (WHICH CAN HAVE MULTIPLE FRAGMENTS IN IT) AND GIVES THE COLOUR OF THE PIXEL

```C++
#include <iostream>

int main(int argc, char** argv)
{
	std::cout << "Hello world!" << std::endl;
	return 0;
}
```



## Homework


* Make sure you understand how GitHub works. Make sure to _git clone_ your work onto your home computer / laptop so you can work on the project outside of lecture / lab hours.<br/>This will make sure you understand how Git works.

> Make sure to _git pull_ when you are on the other machine when you do changes so you have the latest files as your local copy!

You need to understand how OpenGL works.

* Can you tell which part is responsible for drawing what? 
* Can you tell what each glut function is for? If you are unsure, look them up on OpenGL deocumentation or ask for help.
* Why does idle() makes a call to glutPostRedisplay()?
* How to enable and disable animation using keyboard input

