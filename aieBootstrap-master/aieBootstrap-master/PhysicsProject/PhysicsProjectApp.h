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

	bool DetectionOfShot();//This is incharge of the ruleset for the simulation

	void DrawingAndShooting();
	
protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	const float m_aspectRatio = 16 / 9.0f;
	const float m_extents = 100;
	Sphere* whiteBall = nullptr;
	Sphere* blackBall = nullptr;
	std::vector<Sphere*> m_ballsInScene;
	
	//Dividing between the two different types of balls to sink.
	std::vector<Sphere*> m_stripedBalls;
	std::vector<Sphere*> m_solidBalls;


	std::vector<Sphere*> m_stripedBallsOutOfScene;
	std::vector<Sphere*> m_solidBallsOutOfScene;
	std::vector<Sphere*> m_pockets;
	bool mouseHasBeenPressed = false;
	bool isItPlayer1sTurn;
	bool hasABallBeenSunk;
	bool p1SinksStriped;
	bool p2SinksSolid;
	bool firstHit;
	bool hasHit;
	bool blackBallHasSunk;



public:
	void LoadObjects();
};