#include "Errors.h"

#include <iostream>

void ClearErrors()
{
		while (glGetError() != GL_NO_ERROR);
}

void CheckErrors(const char* func, const char* file, int line)
{
		while (GLenum error = glGetError())
		{
				std::cout << "[OpenGL Error]: " << error << ": " <<
						func << " " << file << ":" << line << std::endl;
		}
}
