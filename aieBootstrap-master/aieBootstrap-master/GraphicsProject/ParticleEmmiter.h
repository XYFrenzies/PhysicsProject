#pragma once
#include <glm/glm.hpp>
//This file contains the emission of particles with their position, colour and more changing over time.
//Created by Benjamin McDonald on the 19th March 2021
//Last accessed: 1st April 2021
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
		float a_endSize,  glm::vec4& a_startColour,  glm::vec4& a_endColour);//Initialises the particles when the project begins
	void Emit();//When it is first draw to the scene.
	void Update(float a_deltaTime, const glm::vec3 a_cameraPVM, glm::vec4 a_startColour, 
		glm::vec4 a_endColour, float a_startSize, float a_endSize);//Updates the variables per frame.
	void Draw();//Draws the emission.
	glm::vec3 GetPosition() { return m_position; }
	glm::vec4 GetStartColour() { return m_startColour; }
	glm::vec4 GetEndColour() { return m_endColour; }
protected:
	Particle* m_particles;//A pointer in memory for the amount of particles in scene.
	unsigned int m_firstDead;//Checks for the first particle in the list that is not in the scene.
	unsigned int m_maxParticles;//The amount of particles that can be in the scene at once.

	unsigned int m_vao, m_vbo, m_ibo;//This is in regards to the gl buffers and sending the information to the gpu
	ParticleVertex* m_vertexData;//A pointer in memory for the amount of vertices in the scene.

	glm::vec3 m_position;//Position of the particles
	glm::vec4 m_startColour;//Starting colour of the particles
	glm::vec4 m_endColour;//Ending colour of the particles
	float m_emitTimer;//How long until the next particle occurs.
	float m_emitRate; //How fast the emission will occur.
	float m_lifespanMin;//Whats the minimum life that the particles can live for.
	float m_lifespanMax;//Whats the maximum amount of life that the particles live for.
	float m_velocityMin;//Minimum velocity over time.
	float m_velocityMax;//Maximum velocity over time.
	float m_startSize;//The beginning scale
	float m_endSize;//The ending scale.

};

