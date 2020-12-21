///////////////////////////////////
// square.cpp
//
// OpenGL program to draw a square.
// 
// Coventry Univ.
///////////////////////////////////

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#pragma comment(lib, "glew32d.lib") 
#endif

/*
		---- EXERCISE 1 OVERVIEW ----
		FOR THIS EXERCISE WE ONLY NEED TO CREATE AN VERTEX BUFFER OBJECT (VBO) (AS LEGACY HAS THESE IN).
		THIS MEANS NO SHADERS NEED TO BE USED - THIS WILL COME LATER IN EXERCISE 3.

		FOR A VBO, WE NEED TO PUT THE VERTEX DATA INTO A BUFFER AND THEN SEND THIS TO THE GPU.
		THIS WILL BE DIFFERENT LATER WHEN WE COMBINE CO-ORDS AND COLOUR INTO ONE VERTEX STRUCT, 
		BUT WE WANT TO EASE YOU INTO MODERN OPENGL WITH THIS WEEKS LAB TASKS.

		---- WHAT IS A VBO ----
		A VERTEX BUFFER OBJECT OR VBO IS AN ARRAY OF DATA THAT IS COPIED TO THE GPU BEFORE DRAWING AND RAN ON THE GPU

		FLOWCHART FOR VBO USAGE IN THIS EXAMPLE:

		---- SETUP FUNCTION ----
		GENERATE A (1) BUFFER OBJECT NAME																	(https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glGenBuffers.xml)
		(IN THIS CASE) TELL OPENGL TO ENABLE THE USE OF 2 CAPABILITIES, -VERTEX ARRAY- AND -COLOR ARRAY-	(https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glEnableClientState.xml)
			THESE ALLOW US TO USE GLDRAWARRAYS LATER

		BIND THE BUFFER OBJECT NAME - AKA TELL OPENGL THIS IS THE BUFFER THE BELOW COMMANDS WILL WORK ON	(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml)
		CREATE AND INITIALIZE INTO THE BUFFER'S DATA STORE (BUFFER TYPE, SIZE, DATA GOING IN, USAGE)		(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml)
			(IN THIS CASE) NO DATA GOES IN STRAIGHT AWAY BECAUSE OF THE BELOW SPLITTING OF DATA HENCE THE null

		(IN THIS CASE) TELL OPENGL THAT PART (FIRST HALF) OF THE BUFFER (BUFFERSUBDATA) IS FOR VERTICES		(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferSubData.xhtml)
			AND PLACE THE VERTEX DATA IN THAT HALF
		(IN THIS CASE) TELL OPENGL THAT PART (SECOND HALF) OF THE BUFFER (BUFFERSUBDATA) IS FOR COLOURS		(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferSubData.xhtml)
			AND PLACE THE COLOUR DATA IN THAT HALF

		(IN THIS CASE) TELL OPENGL HOW MANY THINGS MAKE UP A VERTEX POSITION AND WHERE THIS STARTS			(https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glVertexPointer.xml)
		(IN THIS CASE) TELL OPENGL HOW MANY THINGS MAKE UP A VERTEX COLOUR AND WHERE THIS STARTS			(https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glColorPointer.xml)
			TAKING INTO ACCOUNT THAT COLOURS ARE IN THE SECOND HALF OF THE BUFFER

		---- DRAW FUNCTION ----
		IN DRAW SCENE, DRAW A TRIANGLE STRIP OF 4 VERTICES EACH FRAME										(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml)
			BASED ON THE ABOVE BUFFER AND THE DATA WE PLACED INTO IT			
		
		---- WHY DO WE NEED TO DO THIS? ----
		THE BUFFER WILL LOOK LIKE THIS WHEN FILLED WITH DATA (WITHOUT THE SPACES - BUT SO WE CAN SEE THIS)
		[ xyz xyz xyz xyz rgb rgb rgb rgb ]

		WE THEN TELL OPENGL WHAT THAT DATA MEANS AS WE (AS HUMANS) SEE HOW IT FITS BUT ALL OPENGL SEES ARE NUMBERS ALL TOGETHER SUCH AS

		[20.0,20.0,0.0,80.0,20.0,0.0,20.0,80.0,0.0,80.0,80.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]

		WITH glVertexPointer WE TELL OPENGL THAT A VERTEX POSITION IS 3 FLOATS AND TO START FROM THE BEGINNING OF THE BUFFER
		WITH glColorPointer WE TELL OPENGL THAT A VERTEX COLOUR IS ALSO 3 FLOATS BUT TO START FROM AFTER THE VERTICES IN THE BUFFER (BY GIVING THE SIZE OF ALL THE VERTICES AS A STRIDE)

		WHEN WE DRAW OUR SHAPE WE TELL OPENGL THERE ARE 4 VERTICES SO IT KNOWS AFTER THE FIRST 4 LOTS OF 3 FLOATS THAT IT IS NO LONGER POSITIONS AND NOW IS ONTO COLOURS FOR THE NEXT 4 LOTS OF 3 FLOATS
		
		OPENGL THEN WOULD SEE THE DATA LIKE THIS

		[	20.0,20.0,0.0,	80.0,20.0,0.0,	20.0,80.0,0.0,	80.0,80.0,0.0,	0.0,0.0,0.0,	0.0,0.0,0.0,	0.0,0.0,0.0,	0.0,0.0,0.0]
			VERT 1 POS		VERT 2 POS		VERT 3 POS		VERT 4 POS		VERT 1 COLOUR	VERT 2 COLOUR	VERT 3 COLOUR	VERT 4 COLOUR
			3 FLOATS		3 FLOATS		3 FLOATS		3 FLOATS		3 FLOATS		3 FLOATS		3 FLOATS		3 FLOATS
			~~~~~~~~~~~~~~~~~~~~~~~~~~~~~VERTEX POSITION DATA ~~~~~~~~~~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~VERTEX COLOUR DATA ~~~~~~~~~~~~~
*/

/*
	MODERN OPENGL - STORE VERTICES ON GPU

	4 vertices =

	4 positions ( 3 floats each )
	4 colours	( 3 floats each )
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

/// MODERN OPENGL - WILL HOLD 1 BUFFER OBJECT NAME
unsigned int buffer[1];

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   /// MODERN OPENGL - DRAW A GL_TRIANGLE_STRIP USING THE 4 VERTICES IN THE VBO
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   
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

   /// MODERN OPENGL - GENERATE A(1) BUFFER OBJECT NAME
   glGenBuffers(1, buffer); // Generate buffer ids.

   /// MODERN OPENGL - (IN THIS CASE) TELL OPENGL TO ENABLE THE USE OF 2 CAPABILITIES, - VERTEX ARRAY - AND - COLOR ARRAY -
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