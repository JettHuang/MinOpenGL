/* \brief
 *      openGL list commands.
 *
 * \author  jett huang
 * \date    2010-9-5
 *
 * \change log
 *
 */

#ifndef __GL_LISTCMD_H__
#define __GL_LISTCMD_H__


#include "../include/GL/gl.h"

/* maxinum count of params */
#define MAX_PARAM_COUNT		128

/* opcode enum */
typedef enum {
    GLACTIVETEXTURE = 0,
    GLALPHAFUNC,
    GLARRAYELEMENT,

    GLBEGIN,
    GLBINDTEXTURE,
    GLBITMAP,
    GLBLENDCOLOR,
    GLBLENDEQUATION,
    GLBLENDEQUATIONSEPARATE,
    GLBLENDFUNC,
    GLBLENDFUNCSEPARATE,

    GLCALLLIST,
    GLCALLLISTS,
    GLCLEAR,
    GLCLEARCOLOR,
    GLCLEARDEPTH,
    GLCLEARSTENCIL,
    GLCLIPPLANE,
    GLCOLOR,
    GLCOLORMASK,
    GLCOLORMATERIAL,
    GLCOLORPOINTER,
    GLCULLFACE,

    GLDELETELISTS,
    GLDELETETEXTURES,
    GLDEPTHFUNC,
    GLDEPTHMASK,
    GLDISABLE,
    GLDISABLECLIENTSTATE,
    GLDRAWARRAYS,
    GLDRAWELEMENTS,
    GLDRAWPIXELS,
    GLDRAWRANGEELEMENTS,

	GLEDGEFLAG,
    GLEDGEFLAGPOINTER,
    GLENABLE,
    GLENABLECLIENTSTATE,
    GLEND,
    GLENDLIST,
    GLENDQUERY,
    
	GLFEEDBACKBUFFER,
    GLFINISH,
    GLFLUSH,
    GLFOG,
    GLFOGCOORD,
    GLFOGCOORDPOINTER,
    GLFRONTFACE,
    GLFRUSTUM,

    GLGENLISTS,
    GLGENTEXTURES,
    GLGETERROR,

	GLHINT,

    GLINITNAMES,
    GLISENABLED,
    GLISLIST,
    GLISTEXTURE,

	GLLIGHT,
    GLLIGHTMODEL,
    GLLINESTIPPLE,
    GLLINEWIDTH,
    GLLISTBASE,
    GLLOADIDENTITY,
    GLLOADMATRIX,
    GLLOADNAME,
    GLLOADTRANSPOSEMATRIX,
    GLLOGICOP,

    GLMATERIAL,
    GLMATRIXMODE,
    GLMULTMATRIX,
    GLMULTTRANSPOSEMATRIX,
    GLMULTIDRAWARRAYS,
    GLMULTIDRAWELEMENTS,
    GLMULTITEXCOORD,

	GLNEWLIST,
    GLNORMAL,
    GLNORMALPOINTER,

	GLORTHO,

    GLPIXELMAP,
    GLPIXELSTORE,
    GLPIXELZOOM,
    GLPOINTPARAMETER,
    GLPOINTSIZE,
    GLPOLYGONMODE,
    GLPOLYGONOFFSET,
    GLPOLYGONSTIPPLE,
    GLPOPMATRIX,
    GLPOPNAME,
    GLPUSHMATRIX,
    GLPUSHNAME,

	GLRASTERPOS,
    GLREADBUFFER,
    GLREADPIXELS,
    GLRECT,
    GLRENDERMODE,
    GLROTATE,

    GLSCALE,
    GLSCISSOR,
    GLSELECTBUFFER,
    GLSHADEMODEL,
    GLSTENCILFUNC,
    GLSTENCILFUNCSEPARATE,
    GLSTENCILMASK,
    GLSTENCILMASKSEPARATE,
    GLSTENCILOP,
    GLSTENCILOPSEPARATE,

	GLTEXCOORD,
    GLTEXCOORDPOINTER,
    GLTEXENV,
    GLTEXGEN,
    GLTEXIMAGE2D,
    GLTEXPARAMETER,
    GLTRANSLATE,

    GLVERTEX,
    GLVERTEXPOINTER,
    GLVIEWPORT,

	GLWINDOWPOS
}OP_CODE;

/* param struct */
typedef union {
    GLboolean   b;
    GLenum      e;
	GLint       i;
	GLfloat     f;
	GLuint      ui;
	GLvoid      *p;
}OpParam;

/* command struct */
typedef struct {
    OP_CODE _opcode;
	OpParam _params[MAX_PARAM_COUNT];
}OpCommand, *PtrOpCommand;

/* \brief
 *		execute the command, this function is the interface of operation library.
 * \param 
 *		cmd		(in)	pointer to op-command
 * \ret
 *		return 0 if succeed, otherwise return none-zero.
 */
int execute_command(PtrOpCommand cmd);

#endif /* __GL_LISTCMD_H__ */