/* \brief
 *      glcontext descripts the state of the OpenGL-machine.
 *
 * \author  jett huang
 * \date    2010-9-8
 *
 * \change log
 *      1. add texture 2010-11-10
 *
 */

#ifndef __GL_CONTEXT_H__
#define __GL_CONTEXT_H__

#include "../include/GL/gl.h"
#include "glmath.h"


#define MAX_LIGHTS      8
#define MAX_TEXUNITS    1
#define MAX_STACK_SIZE  64
#define MAX_VERTEX_BUFFER   32
#define DEPTH           65535

typedef Vector4     Color4;

/* GLLightSource structure */
typedef struct tagGLLightSource {
    Color4      _ambient;
    Color4      _diffuse;
    Color4      _specular;
    Vector4     _position;
 /* Vector4     _halfVector; */
    Vector3     _spotDirection;
    float       _spotExponent;
    float       _spotCutoff;    /* range: [0.0, 90.0], 180.0 */
    float       _spotCosCutoff; /* range: [1.0,  0.0], -1.0  */
    float       _constantAttenuation;
    float       _linearAttenuation;
    float       _quadraticAttenuation;
}GLLightSource, *PtrGLLightSource;

/* GLLightModel structure */
typedef struct tagGLLightModel {
    Color4      _ambient;
}GLLightModel, *PtrGLLightModel;

/* GLMaterialParameters structure */
typedef struct tagMaterialParameters {
    Color4      _emission;
    Color4      _ambient;
    Color4      _diffuse;
    Color4      _specular;
    float       _shininess;
}MaterialParameters, *PtrMaterialParameters;

/* CLAMP the value */
#define CLAMP(value, min, max)  ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#define MIN(a, b)   ((a) < (b) ? (a) : (b))

/* vertex structure, this will be send to Primitive Assembly Machine */
#define VERTEX_CLIP_TOP         0x0001
#define VERTEX_CLIP_BOTTOM      0x0002
#define VERTEX_CLIP_LEFT        0x0004
#define VERTEX_CLIP_RIGHT       0x0008
#define VERTEX_CLIP_FRONT       0x0010
#define VERTEX_CLIP_BACK        0x0020

typedef struct tagGLVertex {
    GLboolean   _edge_flag;
    Vector4     _texcoords[MAX_TEXUNITS]; /* TODO: is it multiply by Coord-Matrix */
    Vector4     _front_color;    /* color */
    Vector4     _back_color;
    Vector3     _normal;

    /* coordinate */
    Vector4     _eye_coord; /* coordinate in the eye coordinate */
    Vector4     _pro_coord; /* coordinate in the normal volumne */
    Vector4     _win_coord; /* coordinate in the window coordinate, _win_coord._v[3] is 1/z of vertex */
    int         _clip_code; /* clip code by the volume */
}GLVertex, *PtrGLVertex;

typedef struct tagGLViewport {
    GLint       _x, _y;
    GLint       _width, _height;

    /* depth rangle */
    GLfloat     _zNear, _zFar;

    /* internal used */
    GLfloat     _xScale, _xCenter;
    GLfloat     _yScale, _yCenter;
    GLfloat     _zScale, _zCenter;
}GLViewport, *PtrGLViewport;

typedef struct tagGLFragment {
    GLint       _winX, _winY;   /* x,y frame buffer */
    GLushort    _winZ;          /* depth */
    Vector4     _texcoords[MAX_TEXUNITS];
    Vector4     _color;
}GLFragment, *PtrGLFragment;

/* GLContext structure */
typedef struct tagGLContext {
/************************
 * Vertex Operation
 ***********************/
    /* vertex attribute */
    GLboolean   _vertex_edge_flag;
    Color4      _vertex_color;
    Vector3     _vertex_normal;
    Vector4     _vertex_texcoord[MAX_TEXUNITS];

    /* lighting model && lighting */
    /* GLenum      _light_model_color_ctrl; */
    GLboolean   _light_model_local_viewer;
    GLboolean   _light_model_two_side;
    GLboolean   _lighting_on;
    GLboolean   _lights_on[MAX_LIGHTS];
    GLLightSource   _lights_source[MAX_LIGHTS];
    GLLightModel    _light_model;

    /* meterial && mode */
    GLboolean   _color_material_on;
    GLenum      _color_material_face;
    GLenum      _color_material_mode;
    MaterialParameters  _front_material;
    MaterialParameters  _back_material;

    /* vertex transform */
    GLboolean   _normalize_on;
    Matrix4     _matrix_stacks[3][MAX_STACK_SIZE]; /* 0: ModelView, 1: Projection, 2: Color */
    GLint       _matrix_top[3];                    /* the top of matrix */
    Matrix4     _tex_matrix_stacks[MAX_TEXUNITS][MAX_STACK_SIZE];
    GLint       _tex_matrix_top[MAX_TEXUNITS]; /* the top of tex-matrix */
    GLenum      _current_matrix;               /* current active matrix */
    Matrix4     _modelview_inv_transpose;      /* used by normal */

    /* vertexe array client */
#if 0
    GLvoid     *_array_color;
    GLvoid     *_array_normal;
    GLvoid     *_array_vertex;
    GLvoid     *_array_texcoord[MAX_TEXUNITS];
#endif
/***************************************
 * Primitive Assembly && Rasterlization
 **************************************/
    /* cull mode */
    GLenum      _cull_mode;
    GLenum      _fontface_mode;
    GLboolean   _cull_face_on;

    /* primitive */
    GLenum      _primitive_type;
    GLVertex    _vertex_buf[MAX_VERTEX_BUFFER];  /* 用于组装的点 */
    int         _vertex_count;

    GLVertex    _vertex_clip_temp[MAX_VERTEX_BUFFER]; /* 在多变形裁剪时, 使用之作为临时的缓存 */
    PtrGLVertex _next_clip_temp;

    /* viewport && buffer 
     *     | +y
     *     |
     *     |
     *     |
     *     O-------------> +x
     */
    GLViewport _viewport;

/************************
 * Fragment Operaion
 ***********************/
    /* texture */
    GLenum      _active_tex_unit; /* unit selected by user */

#if 0
    GLboolean   _fog_on;
    GLboolean   _blend_on;
#endif

/*************************
 * Frame Buffer Operation
 ************************/
#if 0
    /* enable / disable */
    GLboolean   _alpha_test_on;
    GLboolean   _stencil_test_on;
#endif

    /* depth function */
    GLboolean   _depth_test_on;
    GLenum      _depth_func;

    /* clear color & depth */
    GLbyte      _clear_r, _clear_g, _clear_b, _clear_a;
    GLushort    _clear_depth;

    /* frame buffers */
    GLint       _buf_width, _buf_height;
    GLint       _buf_color_align;
    GLbyte      *_buf_color;    /* 4bytes align */
    GLushort    *_buf_depth;    /* 2bytes align */

}GLContext, *PtrGLContext;

/* \brief
 *      initialize the GLContext
 * \ret
 *      return 0 if succeed, otherwise return none-zero.
 */
int glContext_init(PtrGLContext pContext);

/* \brief
 *      unInitialize the GLContext
 */
void glContext_uninit(PtrGLContext pContext);

/* \brief
 *      initialize the GLLightSource
 * \ret
 *      return 0 if succeed, otherwise return none-zero.
 */
int glLightSource_init(PtrGLLightSource pLightSrc);

/* \brief
 *      uninitialze the GLLightSource
 */
void glLightSource_uninit(PtrGLLightSource pLightSrc);

/* \brief
 *      initialize the GLLightModel
 * \ret
 *      return 0 if succeed, otherwise return none-zero.
 */
int glLightModel_init(PtrGLLightModel pLightModel);

/* \brief
 *      uninitialize the GLLightModel
 */
void glLightModel_uninit(PtrGLLightModel pLightModel);

/* \brief
 *      initialize the MaterialParameters
 * \ret
 *      return 0 if succeed, otherwise return none-zero.
 */
int glMaterialParameters_init(PtrMaterialParameters pMaterial);

/* \brief
 *      uninitialize the MaterialParameters
 */
void glMaterialParameters_uninit(PtrMaterialParameters pMaterial);

#endif /* __GL_CONTEXT_H__ */