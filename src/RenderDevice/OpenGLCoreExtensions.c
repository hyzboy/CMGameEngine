#include<GL/glew.h>
#include<malloc.h>
#include<string.h>

//供glew使用，请在glew.c中删除原本的glewGetExtension函数
//旧的glGetString(GL_EXTENSIONS)方式会被报告为错误，但GLEW 1.5.8未改用新式处理方案，所以暂且如此

/*
 * Define glewGetProcAddress.
 */
#if defined(_WIN32)
#include<wglew.h>
#  define glewGetProcAddress(name) wglGetProcAddress((LPCSTR)name)
#else
#  include <GL/glxew.h>
#  if defined(__APPLE__)
#    define glewGetProcAddress(name) NSGLGetProcAddress(name)
#  else
#    if defined(__sgi) || defined(__sun)
#      define glewGetProcAddress(name) dlGetProcAddress(name)
#    else /* __linux */
#      define glewGetProcAddress(name) (*glXGetProcAddressARB)(name)
#    endif
#  endif
#endif

static int opengl_core_ext_number=0;
static char **opengl_core_ext_string=0;

void InitOpenGLCoreExtensions()
{
	int i;

	PFNGLGETSTRINGIPROC getfunc;

	getfunc=(PFNGLGETSTRINGIPROC)glewGetProcAddress((const GLubyte*)"glGetStringi");		//此函数opengl 3.0才有

	glGetIntegerv(GL_NUM_EXTENSIONS,&opengl_core_ext_number);

	opengl_core_ext_string=malloc(opengl_core_ext_number*sizeof(char *));

	for(i=0;i<opengl_core_ext_number;i++)
		opengl_core_ext_string[i]=(char *)getfunc(GL_EXTENSIONS,i);
}

void ClearOpenGLCoreExtension()
{
	if(opengl_core_ext_string)
	{
		free(opengl_core_ext_string);

		opengl_core_ext_string=0;
	}

	opengl_core_ext_number=0;
}
/*
GLboolean glewGetExtension (const char *name)
{
	int i;

	if(opengl_core_ext_number==0)return(GL_FALSE);

	for(i=0;i<opengl_core_ext_number;i++)
		if(!strcmp(opengl_core_ext_string[i],name))
			return(GL_TRUE);

	return(GL_FALSE);
}
*/
