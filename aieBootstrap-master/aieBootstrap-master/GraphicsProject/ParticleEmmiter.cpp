#include "ParticleEmmiter.h"
#include <gl_core_4_4.h>
#include <iostream>
#include <string>

ParticleEmmiter::ParticleEmmiter() :
	m_particles(nullptr), m_firstDead(0), m_maxParticles(0),
	m_position(0, 0, 0), m_vao(0), m_vbo(0), m_ibo(0), m_vertexData(nullptr), 
	m_emitRate(), m_emitTimer(), m_endColour(), m_endSize(), m_lifespanMax(), 
	m_lifespanMin(), m_startColour(), m_startSize(), m_velocityMax(), 
	m_velocityMin()
{
}

ParticleEmmiter::~ParticleEmmiter()
{
	delete[] m_particles;
	delete[] m_vertexData;
	glDeleteVertexArrays(1, &m_vao);

}

void ParticleEmmiter::Initalise(unsigned int a_maxParticles, 
	unsigned int a_emitRate, float a_lifetimeMin, float a_lifetimeMax, 
	float a_velocityMin, float a_velocityMax, float a_startSize, 
	float a_endSize,  glm::vec4& a_startColour, 
	 glm::vec4& a_endColour)
{
	// set up emit timers
	m_emitTimer = 0; 
	m_emitRate = 1.0f / a_emitRate;
	// store all variables passed in
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;
	m_lifespanMin = a_lifetimeMin;
	m_lifespanMax = a_lifetimeMax;
	m_maxParticles = a_maxParticles;
	// create particle array
	m_particles = new Particle[m_maxParticles];
	m_firstDead = 0;
	// create the array of vertices for the particles
	// 4 vertices per particle for a quad.
	// will be filled during update
	m_vertexData = new ParticleVertex[(unsigned __int64)m_maxParticles * 4];

	//a index buffer is needed for the particles to occur:
	unsigned int* indexData = new unsigned int[(unsigned __int64)m_maxParticles * 6];

	//This forloop assigned the values of each paricles triangle indicie. 
	//There are 6 indicies in the 2 triangles to make a quad.
	//We then fill them in as they are not needing to be changed.
	for (unsigned int i = 0; i < m_maxParticles; ++i) 
	{ 
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;
		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}
	// create opengl buffers
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, (unsigned __int64)m_maxParticles* 4 * sizeof(ParticleVertex),
		m_vertexData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned __int64)m_maxParticles* 6 * sizeof(unsigned int),
		indexData, GL_STATIC_DRAW);glEnableVertexAttribArray(0);
	// position
	glEnableVertexAttribArray(1); 
	// colour
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex),
		0);glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex),
		((char*)0) + 16);glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	delete[] indexData;
}

void ParticleEmmiter::Emit()
{
	//only emit if there is a dead particle to use.
	if(m_firstDead >= m_maxParticles)
		return;

	//bring the first dead particle back to the scene.
	Particle& particle = m_particles[m_firstDead++];

	//assign its starting position (original starting pos)
	particle.position = m_position;

	//randomise its lifespan (giving the effect of the particle movement.)
	particle.lifeTime = 0;
	particle.lifeSpan = (rand() / (float)RAND_MAX) *
		(m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	//set starting size and colour
	particle.colour = m_startColour;
	particle.size = m_startSize;

	// randomise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) * 
		(m_velocityMax - m_velocityMin) + m_velocityMin;
	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 -1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 -1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 -1;
	particle.velocity = glm::normalize(particle.velocity) * velocity;
}

void ParticleEmmiter::Update(float a_deltaTime, const glm::vec3 a_cameraPVM, glm::vec4 a_startColour, glm::vec4 a_endColour, 
	float a_startSize, float a_endSize)
{
	using glm::vec3; 
	using glm::vec4;
	//spawn particles
	// spawn particles
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_emitTimer += a_deltaTime;
	m_startSize = a_startSize;//Sets the size of the particles per frame.
	m_endSize = a_endSize;//Sets the size of the particles per frame.
	while(m_emitTimer > m_emitRate) 
	{
		Emit();
		m_emitTimer -= m_emitRate;
	}
	unsigned int quad= 0;
	// update particles and turn live particles into billboard quads
	for(unsigned int i= 0; i < m_firstDead; i++) 
	{
		Particle* particle = &m_particles[i];
		particle->lifeTime += a_deltaTime;
		if(particle->lifeTime >= particle->lifeSpan) 
		{
			// swap last alive with this one
			*particle= m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else{
			// move particle in particular direction
			particle->position += particle->velocity * a_deltaTime;
			// size particle over time
			particle->size= glm::mix(m_startSize, m_endSize, 
				particle->lifeTime / particle->lifeSpan);
			// colour particle over time.
			particle->colour= glm::mix(m_startColour, m_endColour, 
				particle->lifeTime / particle->lifeSpan);

			// make a quad the correct size and colour
			float halfSize = particle->size * 0.5f;
			m_vertexData[quad * 4 + 0].position = vec4(halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 0].colour = particle->colour;
			m_vertexData[quad * 4 + 1].position= vec4(-halfSize, halfSize, 0, 1);
			m_vertexData[quad * 4 + 1].colour= particle->colour;
			m_vertexData[quad * 4 + 2].position= vec4(-halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 2].colour= particle->colour;
			m_vertexData[quad * 4 + 3].position= vec4(halfSize, -halfSize, 0, 1);
			m_vertexData[quad * 4 + 3].colour= particle->colour;
			// create billboard transform
			//The location in world space for the camera to see the particles.
			vec3 zAxis= glm::normalize(vec3(a_cameraPVM) - particle->position);
			vec3 xAxis= glm::cross(vec3(0,1,0), zAxis); 
			vec3 yAxis= glm::cross(zAxis, xAxis);
			glm::mat4 billboard(
				vec4(xAxis, 0), 
				vec4(yAxis,0),
				vec4(zAxis,0),
				vec4(0,0,0,1));

			//Determines the position in world space for the vertex of the particles to be.
			//(Checks all 4 corners.)
			m_vertexData[quad * 4 + 0].position= billboard * 
				m_vertexData[quad* 4 + 0].position+ vec4(particle->position,0);
			m_vertexData[quad * 4 + 1].position= billboard *
				m_vertexData[quad * 4 + 1].position+vec4(particle->position,0);
			m_vertexData[quad * 4 + 2].position= billboard * 
				m_vertexData[quad * 4 + 2].position+ vec4(particle->position,0);
			m_vertexData[quad * 4 + 3].position= billboard * 
				m_vertexData[quad * 4 + 3].position+ vec4(particle->position,0);
			
			//Increments the quad.
			++quad;
		}
	}
}

void ParticleEmmiter::Draw()
{
	// sync the particle vertex buffer // based on how many alive particles there are
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (unsigned __int64)m_firstDead* 4 *
		sizeof(ParticleVertex), m_vertexData);// draw particles
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, (unsigned __int64)m_firstDead * 6,GL_UNSIGNED_INT, 0);
}
