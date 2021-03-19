#include "ParticleEmmiter.h"
#include <gl_core_4_4.h>

ParticleEmmiter::ParticleEmmiter():
	m_particles(nullptr), m_firstDead(0), m_maxParticles(0),
	m_position(0,0,0), m_vao(0), m_vbo(0), m_ibo(0), m_vertexData(nullptr)
{
}

ParticleEmmiter::~ParticleEmmiter()
{
	delete[] m_particles;
	delete[] m_vertexData;
	glDeleteVertexArrays(1, &m_vao);

}
