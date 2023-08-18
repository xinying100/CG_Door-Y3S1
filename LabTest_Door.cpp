/* Heng Chia Ying - CST2104280 */

#define _USE_MATH_DEFINES 
#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "getBMP.h"

// Globals.
static bool isMove = 0, isOpen = 0, isClose = 0;
static int self_angle = 0;
static float z = 0;
static const long font = (long)GLUT_BITMAP_HELVETICA_18;

// For texture
static const int numTexture = 1;
static unsigned int texture[numTexture];		// Array of texture indices.

// Vertex co-ordinate vectors.
static float vertices[] =
{
	-70.0, 10.0, 0.0,		// Bottom Left
	  0.0, 10.0, 0.0,		// Bottom Right
	  0.0, 135.0, 0.0,		// Upper Right
	-70.0, 135.0, 0.0		// Upper Left
};

static float textureCoordinates[] =
{
	1,0,	0,0,	0,1,	1,1
};

// Load image as a texture. 
void loadTextures()
{
	imageFile* image[numTexture];

	// Load the texture.
	image[0] = getBMP("Door.bmp");

	// Bind the specific image to respective texture[i]. 
	for (int i = 0; i < numTexture; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[i]->width, image[i]->height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, image[i]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void drawFrame()
{
	// Draw Frame
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.7569, 0.7569, 0.7569);
	glVertex3f(-70.0, 10.0, -20);   glVertex3f(-75.0, 5.0, -20);
	glVertex3f(-70.0, 135.0, -20);  glVertex3f(-75.0, 140.0, -20);
	glVertex3f(0.0, 135.0, -20);    glVertex3f(5.0, 140.0, -20);
	glVertex3f(0.0, 10.0, -20);     glVertex3f(5.0, 5.0, -20);
	glVertex3f(-70.0, 10.0, -20);   glVertex3f(-75.0, 5.0, -20);
	glEnd();
}

void drawDoor()
{
	glPushMatrix();
	glRotatef(self_angle, 0, 1, 0);

	// Draw door and apply texture.
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	for (int i = 0; i < 1000; i++)
	{
		z = 0.004 * i;
		for (int j = 0; j < 4; j++)  vertices[j * 3 + 2] = z;
		glBegin(GL_POLYGON);
		for (int i = 0; i < 5; ++i)  glArrayElement(i % 4);
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void writeBitmapString(void* font, const char* string)
{
	const char* c;							// String Pointer
	for (c = string; *c != '\0'; c++)		// Printing each char till the End Of String
		glutBitmapCharacter(font, *c);		// Not our function
}

void writeText()
{
	glColor3f(1,1,1);
	if (self_angle == 0 && isMove == 0)
	{
		glRasterPos3f(-10, 0.0, 0.0);
		writeBitmapString((void*)font, "Door is Close.");
	}
	else if (self_angle > 0 && isMove == 0)
	{
		glRasterPos3f(-10, 0.0, 0.0);
		writeBitmapString((void*)font, "Door is Open.");
	}
	else if (isOpen)
	{
		glRasterPos3f(-10, 0.0, 0.0);
		writeBitmapString((void*)font, "Door is Opening...");
	}
	else if (isClose)
	{
		glRasterPos3f(-10, 0.0, 0.0);
		writeBitmapString((void*)font, "Door is Closing...");
	}
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawFrame();
	drawDoor();
	writeText();
	glutSwapBuffers();
}

void setup(void)
{
	glClearColor(0.439, 0.133, 0.051, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Texture
	glGenTextures(numTexture, texture);			// Texture IDs
	loadTextures();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);

	// Specify locations for the co-ordinates and texture arrays.
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates);
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 100.0, -5.0, 145.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void animate(int val)
{
	if (isMove)
	{
		if (isOpen)
		{
			if (self_angle >= 180) { isOpen = 0; isMove = 0; }
			else self_angle += 2;
		}
		if (isClose)
		{
			if (self_angle <= 0)   { isClose = 0; isMove = 0; }
			else self_angle -= 2;
		}
		glutPostRedisplay();
		glutTimerFunc(40, animate, val);
	}
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'o':
		isOpen = 1;
		isClose = 0;
		if (isMove) isMove = 0;
		else { isMove = 1; animate(1); }
		break;
	case 'c':
		isClose = 1;
		isOpen = 0;
		if (isMove) isMove = 0;
		else { isMove = 1; animate(1); }
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press o to open the door. (Press again to stop)" << std::endl;
	std::cout << "Press c to close the door. (Press again to stop)" << std::endl;
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(1000, 700);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("CST2104280_Door.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	glutMainLoop();
}
