#include "VertexBufferLayout.h"



VertexBufferLayout::VertexBufferLayout()
		: m_Stride(0)
{
}


const std::vector<VertexBufferElement>& VertexBufferLayout::GetElements() const
{
		return m_Elements;
}
unsigned int VertexBufferLayout::GetStride() const
{
		return m_Stride;
}
