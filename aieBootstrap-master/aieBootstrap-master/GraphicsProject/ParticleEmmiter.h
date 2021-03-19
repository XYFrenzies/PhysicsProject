#pragma once
#include <glm/glm.hpp>

struct Particle 
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 colour;
	float size;
	float lifeTime;
	float lifeSpan;
};
struct ParticleVertex 
{
	glm::vec4 position;
	glm::vec4 colour;
};

class ParticleEmmiter
{
public:
	ParticleEmmiter();
	virtual ~ParticleEmmiter();

protected:
	Particle* m_particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;

	unsigned int m_vao, m_vbo, m_ibo;
	ParticleVertex* m_vertexData;

	glm::vec3 m_position;
	glm::vec4 m_startColour;
	glm::vec4 m_endColour;
	float m_emitTimer;
	float m_emitRate; 
	float m_lifespanMin;
	float m_lifespanMax;
	float m_velocityMin;
	float m_velocityMax;
	float m_startSize;
	float m_endSize;

};

