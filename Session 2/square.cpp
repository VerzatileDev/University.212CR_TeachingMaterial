///////////////////////////////////
// square.cpp
//
// OpenGL program to draw a square.
// 
///////////////////////////////////

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 
#endif

/*
		---- EXERCISE 2 OVERVIEW ----
		FOR THIS EXERCISE AS WE HAVE 2 OBJECTS (THE SQUARE FROM EXECRISE 1 AND A TRIANGLE),
		WE NEED TO USE VERTEX ARRAY OBJECTS (VAOs) FOR THIS.
		THIS ALLOWS US TO SET THE BUFFERS FOR EACH ONCE AND THEN SWITCH BETWEEN THEM WHEN DRAWING
		BY SELECTING THE VAO FOR THAT OBJECT.
		
		WE ONLY NEED TO CREATE A VAO (AS LEGACY HAS THESE IN).
		THIS MEANS NO SHADERS NEED TO BE USED - THIS WILL COME LATER IN EXERCISE 3.

		FOR A VAO, WE WILL CREATE IT, DO THE VBO PARTS NEEDED FOR VERTEX/OBJECT CREATION IN THE SETUP FUNCTION
		AND THEN SWITCH BETWEEN THESE IN THE DRAWING FUNCTION.

		---- WHAT IS A VAO ----

		A VERTEX ARRAY OBJECT OR VAO REMEMBERS ALL VBOS AND MEMORY LAYOUTS OF THEM. IT ALLOWS SWITCHING BETWEEN DIFFERENT VERTEX DATA AND VERTEX FORMATS VIA BINDING A VAO.


		FLOWCHART FOR VAO USAGE IN THIS EXAMPLE:

		---- SETUP FUNCTION ----

		GENERATE VERTEX BUFFER OBJECT NAMES FOR EACH OBJECT (2 OVERALL - 1 FOR EACH)						(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml)
		GENERATE BUFFER OBJECT NAMES FOR EACH OBJECT (2 OVERALL - 1 FOR EACH)								(https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glGenBuffers.xml)
		
		BIND THE FIRST VAO FOR THE FIRST OBJECT																(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml)
			(IN THIS CASE) SQUARE

		DO THE VBO THINGS FROM EXERICSE 1 FOR THE SQUARE

		BIND THE SECOND VAO FOR THE SECOND OBJECT															(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml)
			(IN THIS CASE) TRIANGLE

		DO THE VBO THINGS FROM EXERICSE 1 FOR THE TRIANGLE


		---- DRAW FUNCTION ----

		BIND THE FIRST VAO FOR THE FIRST OBJECT																(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml)
			(IN THIS CASE) SQUARE

		DRAW A TRIANGLE STRIP OF 4 VERTICES EACH FRAME														(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml)
			BASED ON THE BUFFER IN VAO 0 AND THE DATA WE PLACED INTO IT			

		BIND THE SECOND VAO FOR THE SECOND OBJECT															(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml)
			(IN THIS CASE) TRIANGLE

		DRAW A TRIANGLE OF 3 VERTICES EACH FRAME															(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml)
			BASED ON THE BUFFER IN VAO 1 AND THE DATA WE PLACED INTO IT
*/

/*
	MODERN OPENGL - STORE VERTICES ON GPU

	SQUARE => 4 vertices =

	4 positions ( 3 floats each )
	4 colours	( 3 floats each )

	TRIANGLE => 3 vertices =

	3 positions ( 3 floats each )
	3 colours	( 3 floats each )
*/

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

/// MODERN OPENGL - WILL HOLD 2 BUFFER OBJECT NAMES (SQUARE, TRIANGLE)
unsigned int buffer[2];
/// MODERN OPENGL - WILL HOLD 2  (SQUARE, TRIANGLE)
unsigned int vao[2]; // Array of VAO ids.

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   /// MODERN OPENGL - BIND THE FIRST VAO FOR THE FIRST OBJECT
   glBindVertexArray(vao[0]);
   /// MODERN OPENGL - DRAW A GL_TRIANGLE_STRIP USING THE 4 VERTICES IN THE VBO
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   
   /// MODERN OPENGL - BIND THE FIRST VAO FOR THE FIRST OBJECT
   glBindVertexArray(vao[1]);
   /// MODERN OPENGL - DRAW A GL_TRIANGLE_STRIP USING THE 4 VERTICES IN THE VBO
   glDrawArrays(GL_TRIANGLES, 0, 3);

   /*
			LEGACY OPENGL - CANNOT USE ANYMORE
   
		   glColor3f(0.0, 0.0, 0.0);

		   glBegin(GL_POLYGON);
			  glVertex3f(20.0, 20.0, 0.0);
			  glVertex3f(80.0, 20.0, 0.0);
			  glVertex3f(80.0, 80.0, 0.0);
			  glVertex3f(20.0, 80.0, 0.0);
		   glEnd();
 
   */

   glFlush(); 
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   /// MODERN OPENGL -GENERATE VERTEX BUFFER OBJECT NAMES FOR EACH OBJECT(2 OVERALL - 1 FOR EACH)
   glGenVertexArrays(2, vao); // Generate VAO ids
   
   /// MODERN OPENGL -GENERATE BUFFER OBJECT NAMES FOR EACH OBJECT(2 OVERALL - 1 FOR EACH)
   glGenBuffers(2, buffer); // Generate buffer ids.

   /// MODERN OPENGL - BIND THE FIRST VAO FOR THE FIRST OBJECT
   glBindVertexArray(vao[0]);

   glEnableClientState(GL_VERTEX_ARRAY);// Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_COLOR_ARRAY);

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

   glBindVertexArray(vao[1]);

   glEnableClientState(GL_VERTEX_ARRAY);// Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_COLOR_ARRAY);

   glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);   // Bind vertex buffer 
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1) + sizeof(colours1), NULL, GL_STATIC_DRAW); // and reserve space																					
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices1), vertices1);  // Copy vertex coordinates data into first half of vertex buffer.
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices1), sizeof(colours1), colours1); // Copy vertex color data into second half of vertex buffer.							
   glVertexPointer(3, GL_FLOAT, 0, 0);  // Specify vertex and color pointers to the start of the respective data.
   glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertices1)));
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("square.cpp");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}