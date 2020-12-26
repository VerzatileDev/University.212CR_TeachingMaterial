# Session 3 - Creat objects, Add lighting and Set up Camera

#### Table of Contents
1. [Create objects](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#create-objects)
2. [Add lighting](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Add-lights)
3. [Add a simple animation](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Add-a-simple-animation)
4. [Set up Camera](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Set-up-Camera)
5. [Homework](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/tree/master/Session%203#Homework)

Welcome to Week 3! 

> By following these sessions, we will learn how to create geomtric objects, add lights into the scene and set up a camera using Modern OpenGL. 


## Create objects

There is no auxiliary functions available in Modern OpenGL to create simple geometric objects such as sphere, square, cube and etc.
So, we have to create the objects from scratch, which involves creating vertices, triangle indices and normal vectors (for lighting effects).

---- Download the base project ----
> Please download CreateSphereClass.zip and unzip it. The OpenGL library has been sep up so it can be compiled. 
After running the program, you only see a grey plane (field) and nothing else. We will add a sphere into the scene.

![Base picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%203/Readme%20Pictures/BaseProject.JPG)

---- Add Sphere class ----
> Right click the project in Solution Explorer. choose "Add" -> "class..". The Add class dialog will pop up. Type "Sphere" as class name.

![Class picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%203/Readme%20Pictures/AddClass.JPG)

Add following header files and namespace.
```C++
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"

using namespace glm;
```

Add following private data variables and functions into the sphere class.

```C++
private:
   vec3 Position;
   VertexWtihNormal *sphereVerticesNor;  //Sphere vertices data with normals
   unsigned int *sphereIndices;          //Sphere triangle indices    

   int stacks; // nunber of segments
   int slices; // number of segments
   float radius;

   void CreateSpherewithNormal();
```

Add following public functions into the sphere class.

```C++
public:
	Sphere();
	~Sphere();

	void SetPosition(vec3 newPos); 
	vec3 GetPosition(void);
	VertexWtihNormal * GetVerData(int &);
	unsigned int * GetTriData(int &);
```

Both GetVerData and GetTriData allow the external function to extract data from the Sphere class.


Add header files into Sphere.cpp file

```C++
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
```

Add constructor and destructor functions

```C++
Sphere::Sphere()
{
	stacks = 10; //number of segments
	slices = 10; //number of segments
	radius = 6.0f;
	Position = vec3(0);

	sphereVerticesNor = (VertexWtihNormal *)malloc(sizeof(VertexWtihNormal) * 121); //total number of vertices = (stacks+1)*(slices +1)
	sphereIndices = (unsigned int *)malloc(sizeof(unsigned int) * 660);
	CreateSpherewithNormal();
}

Sphere::~Sphere()
{
	free(sphereVerticesNor);
	free(sphereIndices);
}
```

Add data create function.


```C++
void Sphere::CreateSpherewithNormal(void)
{
	int count;
	count = 0;
	for (int i = 0; i <= stacks; ++i) {

		GLfloat V = i / (float)stacks;
		GLfloat phi = V * glm::pi <float>();

		// Loop Through Slices
		for (int j = 0; j <= slices; ++j) {

			GLfloat U = j / (float)slices;
			GLfloat theta = U * (glm::pi <float>() * 2);

			// Calc The Vertex Positions
			GLfloat x = cosf(theta) * sinf(phi);
			GLfloat y = cosf(phi);
			GLfloat z = sinf(theta) * sinf(phi);

			sphereVerticesNor[count].coords = vec4(x * radius, y * radius + 6.0, z * radius, 1.0);
			sphereVerticesNor[count].normals = vec3(x, y, z); ///Sphere normals

			count++;
		}
	}

	count = 0;
	// Calc The Index Positions
	for (int i = 0; i < slices * stacks + slices; ++i) {

		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i + slices;
		count++;

		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + 1;
		count++;
	}

	count = 0;
}

```

Finish with some external access functions

```C++
VertexWtihNormal * Sphere::GetVerData(int &verNum)
{
	verNum = 121;
	return sphereVerticesNor;
}

unsigned int * Sphere::GetTriData(int &triNum)
{
	triNum = 660;
	return sphereIndices;
}

void Sphere::SetPosition(vec3 newPos)
{
	Position = newPos;
}

vec3 Sphere::GetPosition(void)
{
	return Position;
}
```


---- Set up drawing routine  ----

Add sphere header file

```C++
#include "sphere.h"
```

Add sphere global variables																	

Add following declare codes for sphere object and its data (vertices, normals and triangle indices)

```C++
static VertexWtihNormal *sphereVerticesNor = NULL;
static unsigned int *sphereIndices = NULL;
static Sphere testSphere;
```
BIND THE BUFFER OBJECT

> In the void setup(void) function, add following lines after  "//Sphere vertex data here" comments 
```C++
   // Obtain sphere data
   int verCount, triCount;
   sphereVerticesNor = testSphere.GetVerData(verCount);
   sphereIndices = testSphere.GetTriData(triCount);
   
   //Binding VAO and VBO
   glBindVertexArray(vao[SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWtihNormal)*verCount, sphereVerticesNor, GL_STATIC_DRAW);  ///please note the change
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*triCount, sphereIndices, GL_STATIC_DRAW); ///please note the change
   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), 0);  //layout(location=4) in vec4 fieldCoords;
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), (GLvoid*)sizeof(sphereVerticesNor[0].normals));
   glEnableVertexAttribArray(3);
```

---- Add drawing codes  ----
> In the void drawScene(void) function, add following lines after  "// Draw sphere" comments 
```C++
   int triCount;
   sphereIndices = testSphere.GetTriData(triCount);

   glUniform1ui(objectLoc, SPHERE);  //if (object == SPHERE)
   glBindVertexArray(vao[SPHERE]);
   glDrawElements(GL_TRIANGLE_STRIP, triCount, GL_UNSIGNED_INT, sphereIndices);  ///use index array to control triangle drawing
```

---- compile and change codes  ----

Now, you can run the code. You will see a green sphere in the middle of sphere. It does not look like sphere because there is no lighting for it.
 
## Add lights

We are going to add ambient and diffuse lighting effects which illuminate the sphere and make it look realistic.

---- Add lighting and material structure definitions ----

You add them anywhere in global variable definition area (before void setup(void) or before any function definition)

```C++
struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};
```

---- Add lighting and material value definitions ----
> Add following codes after lighting and material struture definitions

```C++
static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);
// Front and back material properties.
static const Material sphereFandB =
{
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};

static const Light light0 =
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 0.0, 0.0)
};
```

---- Send data to the shader ----
> Add following codes in void setup(void) before comments "// Create VAOs and VBOs... "

```C++
   //codes for OpenGL lighting
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1,&sphereFandB.ambRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1,&sphereFandB.difRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1,&sphereFandB.specRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1,&sphereFandB.emitCols[0]);
   glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), sphereFandB.shininess);

   glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

   glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1,&light0.ambCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1,&light0.difCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1,&light0.specCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);
```

---- Add drawing codes in fragment Shader ----

Open fragmentShader.glsl. You can drag fragmentShader.glsl into Visual Studio and edit it. 
Add the same structure definition codes into fragmentShader.glsl (after in "vec3 normalExport;")

```C++
struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};

struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};
```
Add the lighting and material definition codes into fragmentShader.glsl (after structure definition codes)

```C++
uniform Light light0;
uniform vec4 globAmb;
uniform Material sphereFandB;
```
Add temp variables for lighting calculation. (add after "out vec4 colorsOut;")

```C++
vec3 normal, lightDirection;
vec4 fAndBDif;
```

Finally, add lighting calculation codes for ambient and diffuse effects. (add inside if statement "if (object == SPHERE)" )
Replace "colorsOut =  vec4(0.0,1.0,0.0, 1.0);  " (we do not need fixed color anymore.)

```C++
	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
    colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0); 
```

Please note that you do not need compile the C++ project when change shader codes. You just change shader codes. Save it and run it.

Final result should look like this. You can reference to "CreateSphere_finish.cpp" if you think you have made some mistakes. 

![Base picture](https://github.coventry.ac.uk/ac7020/212CR_TeachingMaterial/blob/master/Session%203/Readme%20Pictures/LightEffect.JPG)

## Add a simple animation
 
> We can create a simple animation which let the sphere move long the Z axis.


* Step 1: Add a global variable for Z Co-ordinates of the ball 

```C++
static float zVal = 0; // Z Co-ordinates of the ball.
```

* Step 2: Add modelview matrix before draw the sphere

In void drawScene(void) function, add sphere translation codes before "glUniform1ui(objectLoc, SPHERE); "
		
```C++
   modelViewMat = mat4(1.0);
   modelViewMat = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0, 10.0, 0.0), vec3(0.0, 1.0, 0.0)); //apply the same modelview as other objects
   modelViewMat = translate(modelViewMat, testSphere.GetPosition()); //apply Sphere Position and modify the modelview matrix
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));  //send modelview matrix to the shader
```

* Step 3: Modifiy void animate() function

Add following codes before "glutPostRedisplay();" in animate() function. It changes zVal by -0.2 in every frame.

```C++
	zVal = zVal -0.2;
	if (zVal < -25.0) zVal = 0.0;
	testSphere.SetPosition(vec3(0, 0, zVal)); //modify sphere's position
```

glutIdleFunc(animate); has been added into main function, which enables animation.

* Step 4: compile and run the program.



## Set up Camera
 
> We will set up a moving camera by pressing arrow keys. It allow moving camera up and down.


* Step 1: Add camera position global variable

```C++
static float d = 0.0; //Camera position
```

* Step 2: Insert position into modelview matrix

In void drawScene(void), change lookAt function input parameters
		
from 
```C++
modelViewMat = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
```

to 
```C++
modelViewMat = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0 + d, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
```

We need to change both lookAt() functions in drawScene(void) function.


* Step 3: Add codes into void specialKeyInput(int key, int x, int y)

Add codes to change d value 

```C++
   if (key == GLUT_KEY_LEFT) 
   {
	   if (d > -50.0) d -= 0.1;
   }
   if (key == GLUT_KEY_RIGHT) 
   {
	   if (d < 15.0) d += 0.1;
   }
```

* Step 4: compile and run

Use left and right arrow keys to move the camera


## Homework


* Create A cube from scratch using modern OpenGL

> There is tutorial. The link is  http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/



