/* \brief
 *      mygl extern for win32.
 *
 * \date 2010-11-06
 *
 */

#ifndef __MYGL_H__
#define __MYGL_H__

#include "gl.h"

typedef void (*PTRRESHAPE)(int w, int h);
typedef void (*PTRKEYBOARD)(unsigned char key, int x, int y);
typedef void (*PTRDISPLAY)(void);
typedef void (*PTRIDLE)(void);

/* \brief
 *      ³õÊ¼»¯myGL
 *
 */
GLboolean myGLinit();

/* \brief
 *      ÍË³ömyGL
 *
 */
GLvoid myGLexit();


/* \brief
 *      resize frame buffer
 */
GLvoid myGLresize(GLuint w, GLuint h);

/* \brief
 *      swap buffer
 *
 */
GLvoid myGLswapbuffer();

/*\brief
 *      require redisplay
 */
GLvoid myGLpostRedisplay();

/* \brief
 *      set window size
 */
GLvoid myGLinitWindowSize(GLushort w, GLushort h);

GLvoid myGLinitWindowPosition(GLushort x, GLushort y);

/* \brief
 *      create window
 */
GLvoid myGLcreateWindow(GLubyte *name);

GLvoid myGLsetWindowCaption(GLubyte *name);

/* \brief
 *      set reshape callback function
 */
GLvoid myGLreshapeFunc(PTRRESHAPE reshape);

/* keyboard callback */
GLvoid myGLkeyboardFunc(PTRKEYBOARD keyboard);

GLvoid myGLdisplayFunc(PTRDISPLAY display);

GLvoid myGLidleFunc(PTRIDLE idle);

GLvoid myGLmainLoop();

#endif /* __MYGL_H__ */
