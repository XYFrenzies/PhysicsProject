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
	SpringTest(10);
	return true;
}

void PhysicsProjectApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsProjectApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	if (input->isMouseButtonDown(0))
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
		aie::Gizmos::add2DCircle(worldPos, 5, 32, glm::vec4(0.7f));
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	setBackgroundColour(0, 0.5f, 0, 0);
	

	// begin drawing sprites
	m_2dRenderer->begin();


	//X-axis = -100 to 100, Y-axis = -56.25 to 56.25
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.0f, 1.0f));

	// draw your stuff here!
	char fps[32];
	sprintf_s(fps, 32, "Fps: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);


	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
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

void PhysicsProjectApp::SpringTest(int a_amount)
{
	Box* topLeftBarrier = new Box(glm::vec2(-50,54.25), glm::vec2(0), 0, 10, 40, 2);

	Box* leftBarrier = new Box(glm::vec2(-98, (56.25 / 2) - 28), glm::vec2(0), 0, 10, 2, 48);

	Box* bottomLeftBarrier = new Box(glm::vec2(-48, -54.25), glm::vec2(0), 0, 10, 41, 2);

	Box* topRightBarrier = new Box(glm::vec2(50, 54.25), glm::vec2(0), 0, 10, 40, 2);

	Box* rightBarrier = new Box(glm::vec2(98, (56.25 / 2) - 28), glm::vec2(0), 0, 10, 2, 48);

	Box* bottomRightBarrier = new Box(glm::vec2(48, -54.25), glm::vec2(0), 0, 10, 41, 2);




	Sphere* whiteBall = new Sphere(glm::vec2(-20, 0), glm::vec2(80,0),
		4, 2, glm::vec4(1, 1, 1, 1));
	Sphere* filledBall1 = new Sphere(glm::vec2(36, -4), glm::vec2(0),
		2, 2, glm::vec4(1, 1, 0, 1));
	Sphere* filledBall2 = new Sphere(glm::vec2(24, -2), glm::vec2(0),
		2, 2, glm::vec4(0, 0, 0.5f, 1));
	Sphere* filledBall3 = new Sphere(glm::vec2(36, 8), glm::vec2(0),
		2, 2, glm::vec4(0.7f, 0, 0, 1));
	Sphere* filledBall4 = new Sphere(glm::vec2(28, 0), glm::vec2(0),
		2, 2, glm::vec4(0.5f, 0, 0.5f, 1));
	Sphere* filledBall5 = new Sphere(glm::vec2(28, -4), glm::vec2(0),
		2, 2, glm::vec4(1, 0, 0, 1));
	Sphere* filledBall6 = new Sphere(glm::vec2(28, 4), glm::vec2(0),
		2, 2, glm::vec4(0, 0.2f, 0, 1));
	Sphere* filledBall7 = new Sphere(glm::vec2(32, -6), glm::vec2(0),
		2, 2, glm::vec4(0.3f, 0, 0, 1));
	Sphere* blackBall8 = new Sphere(glm::vec2(32, 2), glm::vec2(0),
		2, 2, glm::vec4(0, 0, 0, 1));
	Sphere* stripedBall9 = new Sphere(glm::vec2(32, -2), glm::vec2(0),
		2, 2, glm::vec4(1, 1, 0, 0));
	Sphere* stripedBall10 = new Sphere(glm::vec2(32, 6), glm::vec2(0),
		2, 2, glm::vec4(0, 0, 0.5f, 0));
	Sphere* stripedBall11 = new Sphere(glm::vec2(24, 2), glm::vec2(0),
		2, 2, glm::vec4(0.7f, 0, 0, 0));
	Sphere* stripedBall12 = new Sphere(glm::vec2(36, 4), glm::vec2(0),
		2, 2, glm::vec4(0.5f, 0, 0.5f, 0));			 
	Sphere* stripedBall13 = new Sphere(glm::vec2(36, 0), glm::vec2(0),
		2, 2, glm::vec4(1, 0, 0, 0));			 
	Sphere* stripedBall14 = new Sphere(glm::vec2(20, 0), glm::vec2(0),
		2, 2, glm::vec4(0, 0.2f, 0, 0));			 
	Sphere* stripedBall15 = new Sphere(glm::vec2(36, -8), glm::vec2(0),
		2, 2, glm::vec4(0.3f, 0, 0, 0));

	//Sphere* ball2 = new Sphere(glm::vec2(10, -20), glm::vec2(0),
	//	4, 4, glm::vec4(0, 0.5f, 0.5f, 1));

	//ball2->SetKinematic(true);
	//ball2->SetTrigger(true);



	topLeftBarrier->SetKinematic(true);
	leftBarrier->SetKinematic(true);
	bottomLeftBarrier->SetKinematic(true);
	topRightBarrier->SetKinematic(true);
	rightBarrier->SetKinematic(true);
	bottomRightBarrier->SetKinematic(true);


	m_physicsScene->AddActor(topLeftBarrier);
	m_physicsScene->AddActor(leftBarrier);
	m_physicsScene->AddActor(bottomLeftBarrier);
	m_physicsScene->AddActor(topRightBarrier);
	m_physicsScene->AddActor(rightBarrier);
	m_physicsScene->AddActor(bottomRightBarrier);
	m_physicsScene->AddActor(whiteBall);
	m_physicsScene->AddActor(filledBall1);
	m_physicsScene->AddActor(filledBall2);
	m_physicsScene->AddActor(filledBall3);
	m_physicsScene->AddActor(filledBall4);
	m_physicsScene->AddActor(filledBall5);
	m_physicsScene->AddActor(filledBall6);
	m_physicsScene->AddActor(filledBall7);
	m_physicsScene->AddActor(blackBall8);
	m_physicsScene->AddActor(stripedBall9);
	m_physicsScene->AddActor(stripedBall10);
	m_physicsScene->AddActor(stripedBall11);
	m_physicsScene->AddActor(stripedBall12);
	m_physicsScene->AddActor(stripedBall13);
	m_physicsScene->AddActor(stripedBall14);
	m_physicsScene->AddActor(stripedBall15);


}

void PhysicsProjectApp::DrawRect()
{

	m_physicsScene->AddActor(new Sphere(glm::vec2(10, 10), glm::vec2(-10, -17),
		1, 3, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Plane({ -0.65, 0.75},-25));

	Sphere* sphere1 = new Sphere(glm::vec2(30, 10), glm::vec2(-10, -17),
		1, 3, glm::vec4(1, 0, 1, 0));
	//sphere1->SetKinematic(true);
	sphere1->SetRotation(0.5f);
	m_physicsScene->AddActor(sphere1);

	Box* box1 = new Box(glm::vec2(0, 0), glm::vec2(16, 4), 1, 20, 8, 4, glm::vec4(1, 0, 1, 1));
	Box* box2 = new Box(glm::vec2(30, 30), glm::vec2(16, 4), 1, 20, 8, 4, glm::vec4(1, 1, 0, 1));

	box1->SetRotation(0.5f);
	box2->SetRotation(0.75f);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);


}
