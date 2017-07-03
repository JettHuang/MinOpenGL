//2009-09-05
//
//在场景中添加光照的步骤：
//1. 定义每个物体的每个顶点的法线向量，这些法线决定了物体相对于光源的方向；
//2. 创建和选择一个或多个光源，并设置它们的位置；
//3. 创建和选择一种光照模型(lighting model)，它定义了全局环境光的层次以及
//   观察点的有效位置（便于进行光照计算）；
//4. 定义场景中物体的材料属性.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/GL/gl.h"
#include "../include/GL/glu.h"
#include "../include/GL/mygl.h"

#ifndef M_PI
#define M_PI    3.1415926
#endif

GLUquadricObj *obj;
GLfloat spin = 0.f;


void init(void)
{
	GLfloat mat_specular[] = {0.f, 1.f, 1.f, 1.f};
	GLfloat mat_shininess = 50.f;
	GLfloat light_position[] = {1.f, 1.f, 1.f, 0.f};
    GLfloat light_position1[] = {-1.f, 1.f, 1.f, 1.f};
	GLfloat white_light[] = {0.f, 1.f, 0.f, 1.f};
	GLfloat lmodel_ambient[] = {0.1f, 0.1f, 0.1f, 1.f};

	glClearColor(0.f, 0.f, 0.f, 0.f);
#if 1
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
 	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
 	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.f);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHT1);
#endif
	glEnable(GL_DEPTH_TEST);
 //   glEnable(GL_CULL_FACE);

    obj = gluNewQuadric();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glTranslatef(0.f, 0.f, -3.f);
 //   glRotatef(spin, 0.f, 1.f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
	gluSphere(obj, 1.f, 20.f, 32.f);
	glFlush();
    myGLswapbuffer();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
/*
	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w, 1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5*(GLfloat)w/(GLfloat)h, 1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
*/
    gluPerspective(60.f, (GLfloat)w/(GLfloat)h, 1.f, 30.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

void idle(void)
{
    spin += 1.f;
    if (spin >= 360.f) spin = 0.f;
    myGLpostRedisplay();
}

int main(int argc, char** argv)
{
    myGLinit();
    init();
    myGLinitWindowSize (400, 400); 
    myGLinitWindowPosition (100, 100);
    myGLdisplayFunc(display); 
    myGLreshapeFunc(reshape);
    myGLkeyboardFunc(keyboard);
    myGLidleFunc(idle);
    myGLcreateWindow (argv[0]);
    myGLmainLoop();
    myGLexit();
    return 0;
}