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
	void Initalise(unsigned int a_maxParticles,
		unsigned int a_emitRate, float a_lifetimeMin, float a_lifetimeMax,
		float a_velocityMin, float a_velocityMax, float a_startSize,
		float a_endSize,  glm::vec4& a_startColour,  glm::vec4& a_endColour);
	void Emit();
	void Update(float a_deltaTime, const glm::vec3 a_cameraPVM, glm::vec4 a_startColour, 
		glm::vec4 a_endColour, float a_startSize, float a_endSize);
	void Draw();
	glm::vec3 GetPosition() { return m_position; }
	glm::vec4 GetStartColour() { return m_startColour; }
	glm::vec4 GetEndColour() { return m_endColour; }
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

