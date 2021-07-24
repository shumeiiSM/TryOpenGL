#pragma once

#include <GL/glew.h>

/* Function called will be different for each compiler */
#define ASSERT(x) if(!(x)) __debugbreak(); // compiler intrinsic means specific to MSBC functions instead of clang/gcc
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);