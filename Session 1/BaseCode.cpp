#include <iostream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>

#pragma comment(lib, "glew32.lib") 
#endif

using namespace std;

double xx;

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glLoadIdentity();

	// Modeling transformations.
	glTranslatef(xx, 0.0, -5.0);

	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(25, 1, 0, 0);
	glRotatef(45, 0, 1, 0);

	glPushMatrix();                   // body
	glScalef(2, .5, 1);
	glutSolidCube(.5);
	glPopMatrix();
	glTranslatef(0, 0, .25);
	glPushMatrix();
	glTranslatef(-0.4, -0.2, 0);
	glutSolidTorus(0.05, .1, 8, 8);       // wheel
	glTranslatef(0.8, 0, 0);
	glutSolidTorus(0.05, 0.1, 8, 8);       // wheel
	glPopMatrix();
	glTranslatef(0, 0, -0.5);
	glPushMatrix();
	glTranslatef(-0.4, -0.2, 0);
	glutSolidTorus(0.05, 0.1, 8, 8);       // wheel
	glTranslatef(0.8, 0, 0);
	glutSolidTorus(0.05, 0.1, 8, 8);       // wheel
	glPopMatrix();


	glFlush();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void animate() {

	/* update state variables */
	xx += .001;

	/* refresh screen */
	glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	/* set up depth-buffering */
	glEnable(GL_DEPTH_TEST);

	/* turn on default lighting */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1, 4, 20);

	glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("box.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	//glutIdleFunc(animate);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	xx = 0.0;

	glutMainLoop();
}
