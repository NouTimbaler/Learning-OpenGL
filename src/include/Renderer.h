#pragma once

#include <GL/glew.h>


#define GLCALL(x) ClearErrors(); x; CheckErrors(#x, __FILE__, __LINE__);

void ClearErrors();

void CheckErrors(const char* func, const char* file, int line);
