// texture

#include <stdio.h>
#include <stdlib.h>

#include "../include/GL/gl.h"
#include "../include/GL/glu.h"
#include "../include/GL/mygl.h"
#include "../include/GL/util.h"
#include "Texture.h"
#include "Tga.h"	

GLfloat	xrot;				// X Rotation ( NEW )
GLfloat	yrot;				// Y Rotation ( NEW )
GLfloat	zrot;				// Z Rotation ( NEW )
Texture texture[2];												// Storage For 2 Textures ( NEW )

int LoadGLTextures()											// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;											// Status Indicator
    int loop;

	// Load The Bitmap, Check For Errors.
	if (LoadTGA(&texture[0], "Data/Uncompressed.tga") &&
		LoadTGA(&texture[1], "Data/Compressed.tga"))
	{
		Status=TRUE;											// Set The Status To TRUE

		for (loop=0; loop<2; loop++)						// Loop Through Both Textures
		{
			// Typical Texture Generation Using Data From The TGA ( CHANGE )
			glGenTextures(1, &texture[loop].texID);				// Create The Texture ( CHANGE )
			glBindTexture(GL_TEXTURE_2D, texture[loop].texID);
            glTexImage2D(GL_TEXTURE_2D, 0, texture[loop].type, 
                texture[loop].width, texture[loop].height, 0, texture[loop].type, GL_UNSIGNED_BYTE, texture[loop].imageData);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			if (texture[loop].imageData)						// If Texture Image Exists ( CHANGE )
			{
				free(texture[loop].imageData);					// Free The Texture Image Memory ( CHANGE )
			}
		}
	}
	return Status;												// Return The Status
}


void init(void)
{
    if (!LoadGLTextures()) exit(0);

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	glTranslatef(0.0f,0.0f,-5.0f);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);

    glBindTexture(GL_TEXTURE_2D, texture[1].texID);

	glBegin(GL_QUADS);
		// Front Face
        glColor3f(1.f, 0.f, 0.f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
        glColor3f(0.3f, 0.8f, 0.f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
        glColor3f(0.4f, 0.4f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
        glColor3f(0.5f, 1.f, 0.f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
        glColor3f(0.3f, 0.7f, 0.2f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
        glColor3f(0.f, 0.5f, 1.f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();

	glFlush();
    myGLswapbuffer();

	xrot+=0.3f;
	yrot+=0.2f;
	zrot+=0.4f;
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.f, (GLfloat)w/(GLfloat)h, 1.f, 100.f);
    glMatrixMode(GL_MODELVIEW);
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