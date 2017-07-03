// texture

#include <stdio.h>
#include <stdlib.h>

#include "../include/GL/gl.h"
#include "../include/GL/glu.h"
#include "../include/GL/mygl.h"
#include "../include/GL/util.h"

#include "Texture.h"
#include "Tga.h"


float	spin;													// Spin Variable
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
{	if (!LoadGLTextures())										// Jump To Texture Loading Routine ( NEW )
	{
		exit(0);											// If Texture Didn't Load Return FALSE
	}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
}

void display(void)
{
    int loop;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	glTranslatef(0.0f,0.0f,-10.0f);								// Translate 20 Units Into The Screen

	spin+=0.05f;												// Increase Spin

	for (loop=0; loop<2; loop++)							// Loop Of 20
	{
		glPushMatrix();											// Push The Matrix
		glRotatef(spin+loop*18.0f,1.0f,0.0f,0.0f);				// Rotate On The X-Axis (Up - Down)
		glTranslatef(-2.0f,2.0f,0.0f);							// Translate 2 Units Left And 2 Up

		glBindTexture(GL_TEXTURE_2D, texture[0].texID);			// ( CHANGE )
		glBegin(GL_QUADS);										// Draw Our Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glEnd();												// Done Drawing The Quad
		glPopMatrix();											// Pop The Matrix

		glPushMatrix();											// Push The Matrix
		glTranslatef(2.0f,0.0f,0.0f);							// Translate 2 Units To The Right
		glRotatef(spin+loop*36.0f,0.0f,1.0f,0.0f);				// Rotate On The Y-Axis (Left - Right)
		glTranslatef(1.0f,0.0f,0.0f);							// Move One Unit Right

		glBindTexture(GL_TEXTURE_2D, texture[1].texID);			// ( CHANGE )
		glBegin(GL_QUADS);										// Draw Our Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glEnd();												// Done Drawing The Quad
		glPopMatrix();											// Pop The Matrix
	}
	glFlush();
    myGLswapbuffer();
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