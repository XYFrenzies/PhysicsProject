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

	bool IsBallsStillMoving();

	glm::vec2 ScreenToWorld(glm::vec2 a_screenPos);

	void MoveBallLocation();

	

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	const float m_aspectRatio = 16 / 9.0f;
	const float m_extents = 100;
	Sphere* whiteBall = nullptr;
	Sphere* blackBall = nullptr;
	std::vector<Sphere*> m_ballsInScene;
	
	std::vector<Sphere*> m_stripedBalls;
	std::vector<Sphere*> m_filledBalls;


	std::vector<Sphere*> m_ballsOutOfScene;
	std::vector<Sphere*> m_pockets;
	bool mouseHasBeenPressed = false;
	bool isItPlayer1sTurn;


public:
	void SpringTest();
	void DrawRect();
};