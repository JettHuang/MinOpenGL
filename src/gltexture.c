/* \brief
 *      implement the texure manager
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "gltexture.h"


/* 
 *  TEXTURE OBJECT definition    
 */
typedef struct tagTexObj {
    GLint      _min_filter;    /* GL_NEAREST, GL_LINER */
    GLint      _mag_filter;
    GLint      _wrap_s;        /* GL_REPEAT, GL_CLAMP  */
    GLint      _wrap_t;
    /* image data information */
    GLint      _width, _height, _bpp;
    GLubyte    *_data;
}GLTexObj;

/*
 * TEXTURE UNITS
 */
typedef struct tagTexUnit {
    GLint       _tex_env_mode;   /* GL_MODULATE, GL_REPLACE */
#if 0
    GLboolean   _tex_1D_on;
    GLuint      _tex_1D_obj;
#endif

    /* GL_TEXTURE_2D */
    GLboolean   _tex_2D_on;
    GLuint      _tex_2D_obj;

#if 0
    GLboolean   _tex_3D_on;
    Guint       _tex_3D_obj;
#endif
}GLTexUnit;

/* 
 * TEXTURE OBJECTS POOL
 */
#define MAX_TEX_OBJS    128

typedef struct tagTexObjPool {
    GLTexObj    _objs[MAX_TEX_OBJS];
    GLboolean   _used[MAX_TEX_OBJS];
}GLTexObjPool;


static GLTexObjPool tex_pool;
static GLTexUnit    tex_units[MAX_TEXUNITS];
static GLuint       active_unit;

/* \brief
 *      initialize the texture manager
 */
GLvoid tex_initialize()
{
    GLint i;

    tex_pool._used[0] = GL_TRUE;
    for (i=1; i<MAX_TEX_OBJS; ++i) {
        tex_pool._used[i] = GL_FALSE;
    }

    for (i=0; i<MAX_TEXUNITS; ++i) {
        tex_units[i]._tex_env_mode = GL_MODULATE;
        tex_units[i]._tex_2D_on    = GL_FALSE;
        tex_units[i]._tex_2D_obj   = 0;
    }

    active_unit = 0; /* GL_TEXTURE0 */
}

/* allocate a object id */
static GLuint get_object_id()
{
    static GLint last = 0;
    GLint i;

    for (i=last+1; i<MAX_TEX_OBJS; i++, i = i % MAX_TEX_OBJS)
    {
        if (i == last) return 0;
        if (!tex_pool._used[i]) break;
    }
    last = i;
    return i;
}

static GLvoid tex_obj_init(GLTexObj *obj)
{
    obj->_min_filter = GL_NEAREST;
    obj->_mag_filter = GL_NEAREST;
    obj->_wrap_s = GL_REPEAT;
    obj->_wrap_t = GL_REPEAT;
    obj->_width  = 0;
    obj->_height = 0;
    obj->_bpp    = 3;
    obj->_data   = NULL;
}

static GLvoid tex_obj_uninit(GLTexObj *obj)
{
    if (obj->_data)  free(obj->_data);
    obj->_data = NULL;
}

static GLuint alloc_tex_obj()
{
    GLTexObj *obj = NULL;
    GLuint id;

    if (!(id = get_object_id())) return 0;
    tex_obj_init(&tex_pool._objs[id]);
    return id;
}

static GLvoid free_obj(GLuint id)
{
    if (id <= 0 || id >= MAX_TEX_OBJS) return;
    tex_obj_uninit(&tex_pool._objs[id]);
    tex_pool._used[id] = GL_FALSE;
}

/* \brief
 *      allocate texture object
 */
GLvoid tex_GenTextures(GLsizei n, GLuint *textures)
{
    GLuint id;
    GLint i;
    
    for (i=0; i<n; ++i) {
        textures[i] = 0;
    }

    for (i=0; i<n; ++i) {
        if (id = alloc_tex_obj())
            textures[i] = id;
        else
            return;
    }
}

/* \brief
 *      delete texture object
 */
GLvoid tex_DeleteTexture(GLsizei n, const GLuint *textures)
{
    GLint i;

    for (i=0; i<n; ++i) {
        free_obj(textures[i]);
    }
}

/* \brief
 *      bind a texture object as current 
 */
GLvoid tex_BindTexture(GLenum target, GLuint texture)
{
    if (target != GL_TEXTURE_2D) {
        printf("glBindTexture: invalid target\n");
        return;
    }
    if (active_unit >= MAX_TEXUNITS) return;
    tex_units[active_unit]._tex_2D_obj = texture;
}

/* \brief
 *      set the attributes of a texture object
 */
GLvoid tex_TexParameteri(GLenum target, GLenum pname, GLint param)
{
    GLTexObj *obj = NULL;
    GLuint id;

    id = tex_units[active_unit]._tex_2D_obj;
    if (id <= 0 || id >= MAX_TEX_OBJS) return;
    
    obj = &tex_pool._objs[id];
    if (!obj) return;
    
    switch (pname) {
    case GL_TEXTURE_MIN_FILTER:
        {
            if (param != GL_NEAREST && param != GL_LINEAR) {
                printf("glTexParameter, invalid param\n");
                return;
            }
            obj->_min_filter = param;
        }
        break;

    case GL_TEXTURE_MAG_FILTER:
        {
            if (param != GL_NEAREST && param != GL_LINEAR) {
                printf("glTexParameter, invalid param\n");
                return;
            }
            obj->_mag_filter = param;
        }
        break;

    case GL_TEXTURE_WRAP_S:
        {
            if (param != GL_REPEAT && param != GL_CLAMP) {
                printf("glTexParameter, invalid param\n");
                return;
            }
            obj->_wrap_s = param;
        }
        break;
    
    case GL_TEXTURE_WRAP_T:
        {
            if ((param != GL_REPEAT) && (param != GL_CLAMP))
            {
                printf("glTexParameter, invalid param\n");
                return;
            }
            obj->_wrap_t = param;
        }
        break;

    default:
        printf("glTexParameter, invalid pname\n");
        break;
    }
}

GLvoid tex_TexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, 
                        GLenum type, const GLvoid *data)
{
    GLTexObj *obj = NULL;
    GLbyte *image = NULL;
    GLuint id;
    GLint bpp;

    /* omit all not used parameter */
    if (format != GL_RGB && format != GL_RGBA) {
        printf("glTexImage2D, invalid format\n");
        return;
    }

    if (type != GL_UNSIGNED_BYTE) return;
    if (!data) {
        printf("glTexImage2D, data is null\n");
        return;
    }

    id = tex_units[active_unit]._tex_2D_obj;
    if (id <= 0 || id >= MAX_TEX_OBJS) return;
    
    obj = &tex_pool._objs[id];
    bpp = (format == GL_RGBA) ? 4 : 3;
    image = malloc(width * height * bpp);
    if (!image) return;

    memcpy(image, data, width * height * bpp);
    obj->_width = width;
    obj->_height= height;
    obj->_bpp   = bpp;
    if (obj->_data) free(obj->_data);
    obj->_data  = image;
}


/* \brief
 *      set texture environment parameters
 */
GLvoid tex_TexEnvi(GLenum target, GLenum pname, GLint param)
{
    if (target != GL_TEXTURE_ENV || pname != GL_TEXTURE_ENV_MODE)
    {
        printf("glTexEnvi, invalid target or pname\n");
        return;
    }

    if (param != GL_MODULATE && param != GL_REPLACE) {
        printf("glTexEnvi, invalid param");
        return;
    }

    tex_units[active_unit]._tex_env_mode = param;
}

/* select active texture unit */
GLvoid tex_ActiveTexture(GLenum texture)
{
    if (texture >= GL_TEXTURE0 && texture <= GL_TEXTURE7)
    {
        active_unit = texture - GL_TEXTURE0;
        return;
    }

    printf("glActiveTexture, invalid texture\n");
}

/* \brief
 *      Enable texture
 */
GLvoid tex_TexEnable(GLenum target, GLboolean on)
{
    if (target != GL_TEXTURE_2D) {
        printf("tex_TexEnable, invalid target\n");
        return;
    }

    tex_units[active_unit]._tex_2D_on = on;
}

/* =============================================================================================
 * SHADE A FRAGMENT
 *      双线性纹理采样
 * ===========================================================================================*/

static GLvoid two_linear_sample2D(GLTexObj *obj, GLfloat s, GLfloat t, Vector4 *rgba)
{
    GLbyte *image = NULL;
    GLfloat coordu, coordv, tmp;
    GLint u0, u1, v0, v1;
    GLfloat du, dv;
    GLint umask, vmask;

    if (!obj || !rgba) return;

    image = obj->_data;
    coordu= s * obj->_width;
    coordv= t * obj->_height;
    umask = obj->_width - 1;
    vmask = obj->_height - 1;

    switch (obj->_wrap_s)
    {
    case GL_REPEAT:
        coordu -= 0.5f;
        tmp= floorf(coordu);
        du = coordu - tmp;
        u0 = (GLint)tmp & umask;
        u1 = (u0 + 1) & umask;
        break;
    case GL_CLAMP:
        coordu = CLAMP(coordu, 0.f, obj->_width - 0.5f);
                coordu -= 0.5f;
        tmp= floorf(coordu);
        du = coordu - tmp;
        u0 = (GLint)tmp & vmask;
        u1 = (u0 + 1) & vmask;
        break;
    }

    switch (obj->_wrap_t)
    {
    case GL_REPEAT:
        coordv -= 0.5f;
        tmp= floorf(coordv);
        dv = coordv - tmp;
        v0 = (GLint)tmp & vmask;
        v1 = (v0 + 1) & vmask;
        break;
    case GL_CLAMP:
        coordv = CLAMP(coordv, 0.f, obj->_height-0.5f);
                coordv -= 0.5f;
        tmp= floorf(coordv);
        dv = coordv - tmp;
        v0 = (GLint)tmp & vmask;
        v1 = (v0 + 1) & vmask;
        break;
    }

    {
    /* v
       +-------+--------+
       |  3    |   2    |
       |_______|________|
       |  0    |   1    |
       |       |        |
       +-------+--------+  u
    */
        GLubyte *tex0, *tex1, *tex2, *tex3;
        GLfloat f0, f1, f2, f3;
        
        f0 = (1.f - du) * (1.f - dv); f1 = du * (1.f - dv);
        f2 = du * dv;                 f3 = (1.f - du) * dv;
        tex0 = image + (obj->_width * v0 + u0) * obj->_bpp;
        tex1 = image + (obj->_width * v0 + u1) * obj->_bpp;
        tex2 = image + (obj->_width * v1 + u1) * obj->_bpp;
        tex3 = image + (obj->_width * v1 + u0) * obj->_bpp;
        rgba->_v[0] = f0 * (*tex0 / 255.f) + f1 * (*tex1 / 255.f) + f2 * (*tex2 / 255.f) + f3 * (*tex3 / 255.f);
        rgba->_v[1] = f0 * (*(tex0+1) / 255.f) + f1 * (*(tex1+1) / 255.f) + f2 * (*(tex2+1) / 255.f) + f3 * (*(tex3+1) / 255.f);
        rgba->_v[2] = f0 * (*(tex0+2) / 255.f) + f1 * (*(tex1+2) / 255.f) + f2 * (*(tex2+2) / 255.f) + f3 * (*(tex3+2) / 255.f);
        if (obj->_bpp == 4)
            rgba->_v[3] = f0 * (*(tex0+3) / 255.f) + f1 * (*(tex1+3) / 255.f) + f2 * (*(tex2+3) / 255.f) + f3 * (*(tex3+3) / 255.f);
    }

}

GLvoid tex_shade_fragment(PtrGLFragment frag)
{
    Vector4 rgba = {0.f, 0.f, 0.f, 0.f};
    GLint i;
    GLuint id;
    GLfloat s, t;

    for (i=0; i<MAX_TEXUNITS; ++i)
    {
        if (!tex_units[i]._tex_2D_on) continue;
        
        s = frag->_texcoords[i]._v[0] / frag->_texcoords[i]._v[3];
        t = frag->_texcoords[i]._v[1] / frag->_texcoords[i]._v[3];
        id = tex_units[i]._tex_2D_obj;
        if (id != 0 && id < MAX_TEX_OBJS) {
            two_linear_sample2D(&tex_pool._objs[id], s, t, &rgba);
        }
        /* do replace or modulate */
        switch (tex_units[i]._tex_env_mode)
        {
        case GL_MODULATE:
            {
                frag->_color._v[0] *= rgba._v[0];
                frag->_color._v[1] *= rgba._v[1];
                frag->_color._v[2] *= rgba._v[2];
                frag->_color._v[3] *= rgba._v[3];
            }
            break;

        case GL_REPLACE:
            {
                frag->_color._v[0] = rgba._v[0];
                frag->_color._v[1] = rgba._v[1];
                frag->_color._v[2] = rgba._v[2];
                frag->_color._v[3] = rgba._v[3];
            }
            break;
        default:
            printf("ERROR: unkown tex_env_mode \n");
            break;
        }
    } /* end for i */
}
