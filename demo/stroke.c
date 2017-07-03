//定义一种笔画字体的多个显示列表
//2009-09-19
//

#include <string.h>
#include <stdlib.h>

#include "../include/GL/gl.h"
#include "../include/GL/glu.h"
#include "../include/GL/mygl.h"


#define PT		1
#define STROKE	2
#define END		3


typedef struct charpoint{
	GLfloat x, y;
	int type;
}CP;

CP Adata[] = {
	{0, 0, PT}, {0, 9, PT}, {1, 10, PT}, {4, 10, PT},
	{5, 9, PT}, {5, 0, STROKE}, {0, 5, PT}, {5, 5, END}
};

CP Edata[] = {
	{5, 0, PT}, {0, 0, PT}, {0, 10, PT}, {5, 10, STROKE},
	{0, 5, PT}, {4, 5, END}
};

CP Pdata[] = {
	{0, 0, PT}, {0, 10, PT}, {4, 10, PT}, {5, 9, PT}, {5, 6, PT},
	{4, 5, PT}, {0, 5, END}
};

CP Rdata[] = {
	{0, 0, PT}, {0, 10, PT}, {4, 10, PT}, {5, 9, PT}, {5, 6, PT},
	{4, 5, PT}, {0, 5, STROKE}, {3, 5, PT}, {5, 0, END}
};

CP Sdata[] = {
	{0, 1, PT}, {1, 0, PT}, {4, 0, PT}, {5, 1, PT}, {5, 4, PT},
	{4, 5, PT}, {1, 5, PT}, {0, 6, PT}, {0, 9, PT}, {1, 10, PT},
	{4, 10, PT}, {5, 9, END}
};


//使用线段画字符
static void drawLetter(CP *letter)
{
	glBegin(GL_LINE_STRIP);
	while (letter)
	{
		switch (letter->type) {
			case PT:
				glVertex2fv(&letter->x);
				break;
			
			case STROKE:
				glVertex2fv(&letter->x);
				glEnd();
				glBegin(GL_LINE_STRIP);
				break;

			case END:
				glVertex2fv(&letter->x);
				glEnd();
				glTranslatef(8.f, 0.f, 0.f);
				return;
		}
		letter++;
	}
}

//为6个字符分别创建一个显示列表
static void init(void)
{
	glShadeModel(GL_FLAT);
}

char *test1 = "A SPARE SERAPE APPEARS AS";
char *test2 = "APES PREPARE RARE PEPPERS";

static void printStrokedString(char *s)
{
    if (!s) return;

    while (*s) {
        switch (*s) {
        case 'A':   
            drawLetter(Adata); break;

        case 'S':
            drawLetter(Sdata); break;

        case 'P':
            drawLetter(Pdata); break;

        case 'E':
            drawLetter(Edata); break;

        case 'R':
            drawLetter(Rdata); break;

        case ' ':
            glTranslatef(8.f, 0.f, 0.f); break;

        default: break;
        }

        ++s;
    } /* end while(*s) */
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.f, 0.f, 0.f);
	glPushMatrix();
	glTranslatef(50.f, 150.f, 0.f);
	glScalef(2.f, 2.f, 2.f);
	printStrokedString(test1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50.f, 75.f, 0.f);
	glScalef(2.f, 2.f, 2.f);
	printStrokedString(test2);
	glPopMatrix();
	glFlush();
    myGLswapbuffer();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, (GLdouble)w, 0.f, (GLdouble)h, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	}
}

int main(int argc, char **argv)
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