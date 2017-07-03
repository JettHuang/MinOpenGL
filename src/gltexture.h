/* \brief
 *      texture sampler. surpport 2D texture.
 *
 * \author  jett huang
 * \date    2010-11-10
 *
 * \history:
 *
 */

#ifndef __GL_TEXTURE_H__
#define __GL_TEXTURE_H__

#include "../include/GL/gl.h"
#include "glcontext.h"

/* \brief
 *      initialize the texture manager
 */
GLvoid tex_initialize();

/* \brief
 *      allocate texture object
 */
GLvoid tex_GenTextures(GLsizei n, GLuint *textures);

/* \brief
 *      delete texture object
 */
GLvoid tex_DeleteTexture(GLsizei n, const GLuint *textures);

/* \brief
 *      bind a texture object as current 
 */
GLvoid tex_BindTexture(GLenum target, GLuint texture);

/* \brief
 *      set the attributes of a texture object
 */
GLvoid tex_TexParameteri(GLenum target, GLenum pname, GLint param);

GLvoid tex_TexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, 
                        GLenum type, const GLvoid *data);


/* \brief
 *      set texture environment parameters
 */
GLvoid tex_TexEnvi(GLenum target, GLenum pname, GLint param);

/* select active texture unit */
GLvoid tex_ActiveTexture( GLenum texture); 

/* \brief
 *      Enable texture
 */
GLvoid tex_TexEnable(GLenum target, GLboolean on);

/* =============================================================================================
 * SHADE A FRAGMENT
 *
 * ===========================================================================================*/

GLvoid tex_shade_fragment(PtrGLFragment frag);

#endif /* __GL_TEXTURE_H__ */
