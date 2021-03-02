#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
		: m_Count(count)
{
		// index buffer generation
		GLCALL(glGenBuffers(1, &m_RendererID));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		// put index in buffer                      make sure unsigned int == GLuint
		GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
		GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind() const
{
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
