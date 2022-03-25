#include "pch.h"
#include "StackAllocator.h"


void StackAllocator::init(const size_t total_size)
{
	m_TotalSize = total_size;
	m_Offset = 0;
	m_StartPtr = malloc(m_TotalSize);
}

void* StackAllocator::allocate(const size_t size)
{
	const size_t last_offset = m_Offset;
	m_Offset += size;
	return (&m_StartPtr + last_offset);
}

void StackAllocator::reset()
{
	m_Offset = 0;
}

StackAllocator::~StackAllocator()
{
	reset();
	free(m_StartPtr);
}

