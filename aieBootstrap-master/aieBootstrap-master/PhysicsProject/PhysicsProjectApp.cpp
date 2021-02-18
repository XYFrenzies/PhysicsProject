#include "PhysicsProjectApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include <Gizmos.h>
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"
PhysicsProjectApp::PhysicsProjectApp() : m_2dRenderer(), m_font(), m_physicsScene()
{
	isItPlayer1sTurn = true;
	firstHit = false;
	hasHit = false;
	blackBallHasSunk = false;
}

PhysicsProjectApp::~PhysicsProjectApp() {

}

bool PhysicsProjectApp::startup() {

	//Increases the 2D line count to maximise the number of objects.
	//We can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();


	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();


	m_physicsScene->SetGravity(glm::vec2(0, 0));
	//Lower the TimeStep value, the more accurate the simulation will be;
	//but it will increase the processing time required. If it is
	//too high it will cause the simulation to stutter and reduce stability.

	m_physicsScene->SetTimeStep(0.01f);

	setVSync(true);


	//DrawRect();
	SpringTest();
	return true;
}

void PhysicsProjectApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
	for (auto balls : m_ballsInScene)
		delete balls;
	for (auto pockets : m_pockets)
		delete pockets;
	for (auto balls : m_ballsOutOfScene)
		delete balls;
}

void PhysicsProjectApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();
	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();



	//Getting the position of the whiteball and drawing a line from it to the mouse
	if (whiteBall != nullptr && abs(whiteBall->GetVelocity().x) <= 0.2f &&
		abs(whiteBall->GetVelocity().y) <= 0.2f && HasBlackBallBeenSunk() == false)
	{
		if (IsBallsStillMoving() == false)
		{
			if (firstHit == true && hasHit == true)
			{
				if (isItPlayer1sTurn)
					isItPlayer1sTurn = false;
				else
					isItPlayer1sTurn = true;
				hasHit = false;
			}
			for (int i = 0; i < m_ballsInScene.size(); i++)
			{
				m_ballsInScene[i]->SetVelocity(glm::vec2(0));
				m_ballsInScene[i]->SetAngularVelocity(0);
			}
			glm::vec2 whiteBallPos = whiteBall->GetPosition();
			int xScreen, yScreen;
			input->getMouseXY(&xScreen, &yScreen);
			glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
			aie::Gizmos::add2DLine(whiteBallPos, worldPos, glm::vec4(1, 1, 1, 1));
			mouseHasBeenPressed = false;
			if (input->wasMouseButtonPressed(0) && mouseHasBeenPressed == false)
			{
				mouseHasBeenPressed = true;
				whiteBall->ApplyForce(glm::vec2((worldPos.x * 8) - (whiteBallPos.x * 8),
					(worldPos.y * 8) - (whiteBallPos.y * 8)), glm::vec2(0));

				firstHit = true;
				hasHit = true;
			}

		}

	}
	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	setBackgroundColour(0, 0.5f, 0, 0);
	aie::Input* input = aie::Input::getInstance();

	// begin drawing sprites
	m_2dRenderer->begin();


	//X-axis = -100 to 100, Y-axis = -56.25 to 56.25
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.0f, 1.0f));

	// draw your stuff here!
	if (input->isKeyDown(aie::INPUT_KEY_TAB))
	{
		char fps[32];
		sprintf_s(fps, 32, "Fps: %i", getFPS());
		m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	}
	if (HasBlackBallBeenSunk() == false)
	{
		if (isItPlayer1sTurn)
		{
			m_2dRenderer->drawText(m_font, "It is Player 1's Turn", 300, 700);
		}
		else
		{
			m_2dRenderer->drawText(m_font, "It is Player 2's Turn", 300, 700);
		}
		m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);
	}
	else
	{
		m_2dRenderer->drawText(m_font, "GAME OVER", 300, 350);
		m_2dRenderer->drawText(m_font, "Press ESC to quit", 300, 300);
	}



	// output some text, uses the last used colour


	// done drawing sprites
	m_2dRenderer->end();
}

bool PhysicsProjectApp::IsBallsStillMoving()
{
	for (int i = 0; i < m_ballsInScene.size(); i++)
	{
		if (abs(m_ballsInScene[i]->GetVelocity().x) > 0.2f && abs(m_ballsInScene[i]->GetVelocity().y) > 0.2f)
			return true;
	}

	return false;
}

glm::vec2 PhysicsProjectApp::ScreenToWorld(glm::vec2 a_screenPos)
{
	glm::vec2 worldPos = a_screenPos;

	// We will move the center of the screen to (0, 0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;
	//Scale this according to the extents.
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());
	return worldPos;
}


void PhysicsProjectApp::MoveBallLocation()
{
	int newLoc = -48;
	for (int i = 0; i < m_ballsOutOfScene.size(); i++)
	{

		dynamic_cast<Rigidbody*>
			(m_ballsOutOfScene[i])->SetPosition(glm::vec2(newLoc, 54));
		newLoc += 5;
	}
}

bool PhysicsProjectApp::HasBlackBallBeenSunk()
{
	if (blackBallHasSunk == true)
	{
		return true;
	}
	else
	{
		for (int j = 0; j < m_pockets.size(); j++)
		{
			m_pockets[j]->m_triggerEnter = [=](PhysicsObject* other)
			{
				if (other == whiteBall)
				{
					whiteBall->SetPosition(glm::vec2(-20, 0));
					whiteBall->SetVelocity(glm::vec2(0));
					whiteBall->SetAngularVelocity(0);
				}
				else if (other == blackBall)
				{
					blackBallHasSunk = true;
					return true;
				}
				else if (other != nullptr)
				{
					for (int i = 0; i < m_ballsInScene.size(); i++)
					{
						if (other == m_ballsInScene[i])
						{

							m_ballsOutOfScene.push_back(m_ballsInScene[i]);
							m_ballsInScene[i]->SetVelocity(glm::vec2(0));
							m_ballsInScene[i]->SetAngularVelocity(0);
							m_ballsInScene.erase(m_ballsInScene.begin() + i);

							MoveBallLocation();
						}
					}

				}
				else
					return false;
			};
		}
	}


	return false;
}

void PhysicsProjectApp::SpringTest()
{
	//Barriers for the pool table
	Box* topLeftBarrier = new Box(glm::vec2(-48, 54.25f),
		glm::vec2(0), 0, 10, 42, 2);
	Box* leftBarrier = new Box(glm::vec2(-98, (56.25f / 2) - 28),
		glm::vec2(0), 0, 10, 2, 48);
	Box* bottomLeftBarrier = new Box(glm::vec2(-48, -54.25f),
		glm::vec2(0), 0, 10, 42, 2);
	Box* topRightBarrier = new Box(glm::vec2(48, 54.25f),
		glm::vec2(0), 0, 10, 42, 2);
	Box* rightBarrier = new Box(glm::vec2(98, (56.25f / 2) - 28),
		glm::vec2(0), 0, 10, 2, 48);
	Box* bottomRightBarrier = new Box(glm::vec2(48, -54.25f),
		glm::vec2(0), 0, 10, 42, 2);

	//Ball's for the pool game
	whiteBall = new Sphere(glm::vec2(-20, 0), glm::vec2(0), 1,
		10, 2, glm::vec4(1, 1, 1, 1));
	Sphere* filledBall1 = new Sphere(glm::vec2(36, -4), glm::vec2(0), 1,
		10, 2, glm::vec4(1, 1, 0, 1));
	Sphere* filledBall2 = new Sphere(glm::vec2(24, -2), glm::vec2(0), 1,
		10, 2, glm::vec4(0, 0, 0.5f, 1));
	Sphere* filledBall3 = new Sphere(glm::vec2(36, 8), glm::vec2(0), 1,
		10, 2, glm::vec4(0.7f, 0, 0, 1));
	Sphere* filledBall4 = new Sphere(glm::vec2(28, 0), glm::vec2(0), 1,
		10, 2, glm::vec4(0.5f, 0, 0.5f, 1));
	Sphere* filledBall5 = new Sphere(glm::vec2(28, -4), glm::vec2(0), 1,
		10, 2, glm::vec4(1, 0, 0, 1));
	Sphere* filledBall6 = new Sphere(glm::vec2(28, 4), glm::vec2(0), 1,
		10, 2, glm::vec4(0, 0.2f, 0, 1));
	Sphere* filledBall7 = new Sphere(glm::vec2(32, -6), glm::vec2(0), 1,
		10, 2, glm::vec4(0.3f, 0, 0, 1));
	blackBall = new Sphere(glm::vec2(32, 2), glm::vec2(0), 1,
		10, 2, glm::vec4(0, 0, 0, 1));
	Sphere* stripedBall9 = new Sphere(glm::vec2(32, -2), glm::vec2(0), 1,
		10, 2, glm::vec4(1, 1, 0, 0));
	Sphere* stripedBall10 = new Sphere(glm::vec2(32, 6), glm::vec2(0), 1,
		10, 2, glm::vec4(0, 0, 0.5f, 0));
	Sphere* stripedBall11 = new Sphere(glm::vec2(24, 2), glm::vec2(0), 1,
		10, 2, glm::vec4(0.7f, 0, 0, 0));
	Sphere* stripedBall12 = new Sphere(glm::vec2(36, 4), glm::vec2(0), 1,
		10, 2, glm::vec4(0.5f, 0, 0.5f, 0));
	Sphere* stripedBall13 = new Sphere(glm::vec2(36, 0), glm::vec2(0), 1,
		10, 2, glm::vec4(1, 0, 0, 0));
	Sphere* stripedBall14 = new Sphere(glm::vec2(20, 0), glm::vec2(0), 1,
		10, 2, glm::vec4(0, 0.2f, 0, 0));
	Sphere* stripedBall15 = new Sphere(glm::vec2(36, -8), glm::vec2(0), 1,
		10, 2, glm::vec4(0.3f, 0, 0, 0));

	m_ballsInScene.push_back(whiteBall);
	m_ballsInScene.push_back(filledBall1);
	m_ballsInScene.push_back(filledBall2);
	m_ballsInScene.push_back(filledBall3);
	m_ballsInScene.push_back(filledBall4);
	m_ballsInScene.push_back(filledBall5);
	m_ballsInScene.push_back(filledBall6);
	m_ballsInScene.push_back(filledBall7);
	m_ballsInScene.push_back(blackBall);
	m_ballsInScene.push_back(stripedBall9);
	m_ballsInScene.push_back(stripedBall10);
	m_ballsInScene.push_back(stripedBall11);
	m_ballsInScene.push_back(stripedBall12);
	m_ballsInScene.push_back(stripedBall13);
	m_ballsInScene.push_back(stripedBall14);
	m_ballsInScene.push_back(stripedBall15);

	//Dividing the balls into seperate vectors as well to keep an idea as to what player is what team.


	//Triggers (outter triggers for the holes)
	Sphere* topLeftHole = new Sphere(glm::vec2(-96, 54.25f), glm::vec2(0), 0,
		2, 6, glm::vec4(0, 0, 0, 1));
	Sphere* bottomLeftHole = new Sphere(glm::vec2(-96, -54.25f), glm::vec2(0), 0,
		2, 6, glm::vec4(0, 0, 0, 1));
	Sphere* bottomMiddleHole = new Sphere(glm::vec2(0, -58.25f), glm::vec2(0), 0,
		2, 6, glm::vec4(0, 0, 0, 1));
	Sphere* topMiddleHole = new Sphere(glm::vec2(0, 58.25f), glm::vec2(0), 0,
		2, 6, glm::vec4(0, 0, 0, 1));
	Sphere* bottomRightHole = new Sphere(glm::vec2(96, -54.25f), glm::vec2(0), 0,
		2, 6, glm::vec4(0, 0, 0, 1));
	Sphere* topRightHole = new Sphere(glm::vec2(96, 54.25f), glm::vec2(0), 0,
		2, 6, glm::vec4(0, 0, 0, 1));

	//Setting the elasticity of the barriers so that the balls can bounce off them.
	topLeftBarrier->SetElasticity(5.0f);
	leftBarrier->SetElasticity(5.0f);
	bottomLeftBarrier->SetElasticity(5.0f);
	topRightBarrier->SetElasticity(5.0f);
	rightBarrier->SetElasticity(5.0f);
	bottomRightBarrier->SetElasticity(5.0f);


	//Setting the barriers to not move and be static
	//Kinematic objects
	topLeftBarrier->SetKinematic(true);
	leftBarrier->SetKinematic(true);
	bottomLeftBarrier->SetKinematic(true);
	topRightBarrier->SetKinematic(true);
	rightBarrier->SetKinematic(true);
	bottomRightBarrier->SetKinematic(true);

	m_pockets.push_back(topLeftHole);
	m_pockets.push_back(bottomLeftHole);
	m_pockets.push_back(bottomMiddleHole);
	m_pockets.push_back(topMiddleHole);
	m_pockets.push_back(bottomRightHole);
	m_pockets.push_back(topRightHole);

	//Barriers
	m_physicsScene->AddActor(topLeftBarrier);
	m_physicsScene->AddActor(leftBarrier);
	m_physicsScene->AddActor(bottomLeftBarrier);
	m_physicsScene->AddActor(topRightBarrier);
	m_physicsScene->AddActor(rightBarrier);
	m_physicsScene->AddActor(bottomRightBarrier);


	//Different ball types
	for (int i = 0; i < m_ballsInScene.size(); i++)
	{
		m_physicsScene->AddActor(m_ballsInScene[i]);
	}
	//Tiggered Holes also triggers
	for (int i = 0; i < m_pockets.size(); i++)
	{
		m_pockets[i]->SetTrigger(true);
		m_physicsScene->AddActor(m_pockets[i]);
	}
}

void PhysicsProjectApp::DrawRect()
{

	m_physicsScene->AddActor(new Sphere(glm::vec2(-30, 0), glm::vec2(30, 0), 1,
		1, 3, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Plane());

	Sphere* sphere1 = new Sphere(glm::vec2(30, 0), glm::vec2(-30, 0), 1,
		1, 3, glm::vec4(1, 0, 1, 0));
	//sphere1->SetKinematic(true);
	sphere1->SetRotation(0.5f);
	m_physicsScene->AddActor(sphere1);

	Box* box1 = new Box(glm::vec2(0, 0), glm::vec2(0), 1, 2, 8, 4, glm::vec4(1, 0, 1, 1));
	Box* box2 = new Box(glm::vec2(30, 10), glm::vec2(0), 1, 2, 8, 4, glm::vec4(1, 1, 0, 1));

	box1->SetRotation(0.5f);
	box2->SetRotation(0.75f);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);


}
