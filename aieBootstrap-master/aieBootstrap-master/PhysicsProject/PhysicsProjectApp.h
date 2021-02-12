#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Sphere.h"


class PhysicsProjectApp : public aie::Application {
public:

	PhysicsProjectApp();
	virtual ~PhysicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	glm::vec2 ScreenToWorld(glm::vec2 a_screenPos);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	const float m_aspectRatio = 16 / 9.0f;
	const float m_extents = 100;
	Sphere* whiteBall = nullptr;

public:
	void SpringTest();
	void DrawRect();
};