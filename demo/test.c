/*
 * Copyright (c) 1993-1999, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  lines.c
 *  This program demonstrates geometric primitives and
 *  their attributes.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/GL/gl.h"
#include "../include/GL/mygl.h"


void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
#if 0
    glBegin(GL_POINT);
        glColor3f (1.0, 0.0, 0.0); glVertex2f (10.f, 10.f);
        glColor3f (1.0, 0.0, 0.0); glVertex2f (400.f, 10.f);
    glEnd();
#endif

#if 0
   glBegin(GL_LINES);
       /* dely > delx */
       glColor3f (1.0, 0.0, 0.0); glVertex2f (-10.f, 10.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (50.f, 100.f);
#if 0      
       glColor3f (1.0, 0.0, 0.0); glVertex2f (30.f, 10.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (10.f, 100.f);

       /* delx > dely */
       glColor3f (1.0, 0.0, 0.0); glVertex2f (10.f, 10.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (100.f, 30.f);

       glColor3f (1.0, 0.0, 0.0); glVertex2f (100.f, 10.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (10.f, 30.f);

       /* delx = 0 */
       glColor3f (1.0, 0.0, 0.0); glVertex2f (100.f, 100.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (100.f, 30.f);

       /* dely = 0 */
       glColor3f (1.0, 0.0, 0.0); glVertex2f (30.f, 100.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (100.f, 100.f);

       /* v0 = v1 */
       glColor3f (1.0, 0.0, 0.0); glVertex2f (100.f, 100.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (100.f, 100.f);
#endif
   glEnd();
#endif

#if 0
    glBegin(GL_TRIANGLES);
       /* CW */
#if 0
       glColor3f (1.0, 0.0, 0.0); glVertex2f (10.f, 10.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (30.f, 100.f);
       glColor3f (1.0, 0.0, 0.0); glVertex2f (30.f, 10.f);
#endif

       /* CCW */
       glColor3f (1.0, 0.0, 0.0); glVertex2f (-50.f, 50.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (130.f, 50.f);
       glColor3f (0.0, 0.0, 1.0); glVertex2f (130.f, 400.f);

    glEnd();
#endif

#if 0
    glBegin(GL_TRIANGLE_FAN);
       /* CCW */
       glColor3f (1.0, 0.0, 0.0); glVertex2f (10.f, 10.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (130.f, 10.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (180.f, 110.f); 
       glColor3f (0.0, 0.0, 1.0); glVertex2f (30.f, 100.f);
       glColor3f (0.0, 0.0, 1.0); glVertex2f (20.f, 150.f);
    glEnd();
    glBegin(GL_TRIANGLES);
       /* CCW */
       glColor3f (1.0, 0.0, 0.0); glVertex2f (100.1f, 100.f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (200.f, 168.2f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (-50.f, 299.9f);

       glColor3f (0.0, 1.0, 0.0); glVertex2f (10.f, 299.9f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (200.f, 168.2f);
       glColor3f (0.0, 1.0, 0.0); glVertex2f (300.f, 400.2f);
    glEnd();
#endif

#if 0
    glBegin(GL_QUADS);
        glColor3f (1.0, 0.0, 0.0); glVertex2f (10.f, 10.f);
        glColor3f (0.0, 1.0, 0.0); glVertex2f (130.f, 10.f);
        glColor3f (0.0, 1.0, 0.0); glVertex2f (180.f, 110.f);
        glColor3f (0.0, 0.0, 1.0); glVertex2f (30.f, 100.f);

    glEnd();
#endif

#if 1
    glBegin(GL_QUAD_STRIP);
        glColor3f (1.0, 0.0, 0.0); glVertex2f (10.f, 10.f);
        glColor3f (0.0, 1.0, 0.0); glVertex2f (130.f, 10.f);
        glColor3f (0.0, 0.0, 1.0); glVertex2f (30.f, 100.f);
        glColor3f (0.0, 1.0, 0.0); glVertex2f (180.f, 110.f);
        glColor3f (0.0, 0.0, 1.0); glVertex2f (50.f, 180.f);
        glColor3f (0.0, 0.0, 1.0); glVertex2f (180.f, 280.f);
    glEnd();
#endif

   glFlush ();
   myGLswapbuffer();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (0.0, (GLdouble) w, 0.0, (GLdouble) h, -1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
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
   myGLcreateWindow (argv[0]);
   myGLmainLoop();
   myGLexit();
   return 0;
}
