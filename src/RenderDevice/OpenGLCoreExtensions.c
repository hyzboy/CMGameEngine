#include<hgl/graph/GL/glew.h>
#include<GLFW/glfw3.h>
#include<malloc.h>
#include<string.h>

//供glew使用，请在glew.c中删除原本的glewGetExtension函数
//旧的glGetString(GL_EXTENSIONS)方式会被报告为错误，但GLEW 1.5.8未改用新式处理方案，所以暂且如此

static int opengl_core_ext_number = 0;
static char **opengl_core_ext_string = 0;

GLboolean glewGetExtension(const char *name)
{
    int i;

    if (opengl_core_ext_number == 0)return(GL_FALSE);

    for (i = 0; i < opengl_core_ext_number; i++)
        if (!strcmp(opengl_core_ext_string[i], name))
            return(GL_TRUE);

    return(GL_FALSE);
}

void InitOpenGLCoreExtensions()
{
    int i;

    PFNGLGETSTRINGIPROC getfunc;

    getfunc = (PFNGLGETSTRINGIPROC)glfwGetProcAddress((const GLubyte*)"glGetStringi");        //此函数opengl 3.0才有

    glGetIntegerv(GL_NUM_EXTENSIONS, &opengl_core_ext_number);

    opengl_core_ext_string = malloc(opengl_core_ext_number*sizeof(char *));

    for (i = 0; i < opengl_core_ext_number; i++)
        opengl_core_ext_string[i] = (char *)getfunc(GL_EXTENSIONS, i);
}

void ClearOpenGLCoreExtension()
{
    if (opengl_core_ext_string)
    {
        free(opengl_core_ext_string);

        opengl_core_ext_string = 0;
    }

    opengl_core_ext_number = 0;
}
