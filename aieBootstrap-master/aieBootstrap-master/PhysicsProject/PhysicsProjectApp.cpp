#include "PhysicsProjectApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include <Gizmos.h>
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
PhysicsProjectApp::PhysicsProjectApp() {

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


	m_physicsScene->SetGravity(glm::vec2(0, -10));
	//Lower the TimeStep value, the more accurate the simulation will be;
	//but it will increase the processing time required. If it is
	//too high it will cause the simulation to stutter and reduce stability.

	m_physicsScene->SetTimeStep(0.01f);

	DrawRect();
	//SphereAndPlane();
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

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, 
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// draw your stuff here!
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsProjectApp::DrawRect()
{
	m_physicsScene->AddActor(new Sphere(glm::vec2(10, 10), glm::vec2(-10, -17),
		1, 3, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Plane({ -0.65, 0.75},-25));

	Sphere* sphere1 = new Sphere(glm::vec2(30, 10), glm::vec2(-10, -17),
		1, 3, glm::vec4(1, 0, 1, 0));
	sphere1->SetKinematic(true);
	m_physicsScene->AddActor(sphere1);

	Box* box1 = new Box(glm::vec2(0, 0), glm::vec2(16, 4), 1, 4, 8, 4, glm::vec4(1, 0, 1, 1));
	Box* box2 = new Box(glm::vec2(15, 0), glm::vec2(16, 4), 1, 4, 8, 4, glm::vec4(1, 1, 0, 1));

	box1->SetRotation(0.5f);
	box2->SetRotation(0.75f);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);


}

void PhysicsProjectApp::SphereAndPlane()
{
	Sphere* ball1;
	ball1 = new Sphere(glm::vec2(-40, 50), glm::vec2(20, 0), 10.0f, 6.0f,
		glm::vec4(1, 0, 0, 1));


	Sphere* ball2;
	ball2 = new Sphere(glm::vec2(40, 50), glm::vec2(-20, 0), 10.0f, 6.0f,
		glm::vec4(1, 1, 0, 1));



	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForce(glm::vec2(15, 0), glm::vec2(0));
	ball2->ApplyForce(glm::vec2(-15, 0), glm::vec2(0));

	Plane* plane = new Plane();
	m_physicsScene->AddActor(plane);
}
