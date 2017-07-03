// texture

#include <stdio.h>
#include <stdlib.h>

#include "../include/GL/gl.h"
#include "../include/GL/glu.h"
#include "../include/GL/mygl.h"
#include "../include/GL/util.h"


//´´½¨ÎÆÀíÍ¼Ïñ
#define checkImageWidth		64
#define checkImageHeight	64
static GLubyte checkImage[checkImageWidth][checkImageHeight][4];

static GLuint texName[2], texNow;
static GLfloat spin;


void makeCheckImage(void)
{
	int i, j, c;

	for (i=0; i<checkImageHeight; i++)
	{
		for (j=0; j<checkImageWidth; j++)
		{
			c = (((i&0x08)==0)^((j&0x08)==0)) * 255;
            checkImage[i][j][0] = (GLubyte)(c ? 255 : 0);
			checkImage[i][j][1] = (GLubyte)0;
            checkImage[i][j][2] = (GLubyte)(c ? 0 : 255);
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}


void init(void)
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(2, texName);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkImage);
    
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkImage);

    texNow = texName[0];
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texNow);
    glColor3f(1.f, 0.f, 0.f);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glTranslatef(0.f, 0.f, -3.6f);
    glRotatef(spin, 0.f, 1.f, 0.f);
#if 1
	glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f); glVertex3f(-2.f, -1.f, 0.f);
		glTexCoord2f(0.f, 2.50); glVertex3f(-2.f, 1.f, 0.f);
		glTexCoord2f(2.5f, 2.5f); glVertex3f(0.f, 1.f, 0.f);
		glTexCoord2f(2.5f, 0.f); glVertex3f(0.f, -1.f, 0.f);
   
		glTexCoord2f(0.f, 0.f);
		glVertex3f(1.f, -1.f, 0.f);
		glTexCoord2f(0.f, 1.5f);
		glVertex3f(1.f, 1.f, 0.f);
		glTexCoord2f(1.5f, 1.5f);
		glVertex3f(2.41421, 1.f, -1.41421);
		glTexCoord2f(1.5f, 0.f);
		glVertex3f(2.41421, -1.f, -1.41421);

	glEnd();
#endif

#if 0
	glBegin(GL_TRIANGLES);
		glTexCoord2f(0.f, 0.f); glVertex3f(-2.f, -1.f, 0.f);
		glTexCoord2f(3.5f, 0.0); glVertex3f( 2.f, -1.f, 0.f);
		glTexCoord2f(3.5f, 3.5f); glVertex3f( 2.f, -0.5f,-1.f);
	glEnd();
#endif

#if 0
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.f, 0.f); glVertex3f(-1.f, 0.f, 0.f);
		glTexCoord2f(1.f, 0.0); glVertex3f( 1.f, 0.f, 0.f);
		glTexCoord2f(0.5f, 0.5f); glVertex3f(0.f, 1.f, 0.f);
    glEnd();
#endif

#if 0
    glBegin(GL_TRIANGLES);
		glTexCoord2f(0.f, 0.f); glVertex3f(-2.f, -1.f, 0.f);
		glTexCoord2f(2.f, 2.f); glVertex3f(0.f, 2.f, 0.f);
		glTexCoord2f(2.f, 0.f); glVertex3f(0.f, 0.5f, 0.f);
   
		glTexCoord2f(0.f, 0.f); glVertex3f(1.f, -1.f, 0.f);
        glTexCoord2f(0.f, 2.f); glVertex3f(1.f, 1.f, 0.f);
        glTexCoord2f(2.f, 2.f); glVertex3f(2.41421, 1.f, -1.41421);

        glTexCoord2f(0.f, 0.f); glVertex3f(1.f, -1.f, 0.f);
		glTexCoord2f(2.f, 2.f); glVertex3f(2.41421, 1.f, -1.41421);
		glTexCoord2f(2.f, 0.f); glVertex3f(2.41421, -1.f, -1.41421);
    glEnd();
#endif

#if 0
    glBegin(GL_LINES);
        glTexCoord2f(0.55f, 0.f); glVertex3f(0.f, -1.f, 0.f);
		glTexCoord2f(0.55f, 1.0); glVertex3f(0.f,  1.f, 0.f);
    glEnd();
#endif

	glFlush();
	glDisable(GL_TEXTURE_2D);
    myGLswapbuffer();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.f, (GLfloat)w/(GLfloat)h, 1.f, 30.f);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
    case ' ':
        texNow = (texNow == texName[0]) ? texName[1] : texName[0];
        myGLpostRedisplay();
        break;
   }
}

void idle(void)
{
    GLbyte szbuf[64];

    begin_performance();
    display();
    sprintf(szbuf, "FPS: %f", end_performance(NULL));
    myGLsetWindowCaption(szbuf);
}

int main(int argc, char** argv)
{
    myGLinit();
    init();
    myGLinitWindowSize (250, 250); 
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