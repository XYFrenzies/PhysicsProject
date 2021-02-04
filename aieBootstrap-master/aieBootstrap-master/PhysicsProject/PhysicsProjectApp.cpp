#include "PhysicsProjectApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include <Gizmos.h>
#include "Sphere.h"
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


	m_physicsScene->SetGravity(glm::vec2(0, 0));
	//Lower the TimeStep value, the more accurate the simulation will be;
	//but it will increase the processing time required. If it is
	//too high it will cause the simulation to stutter and reduce stability.

	m_physicsScene->SetTimeStep(0.01f);

	Sphere* ball;
  	ball = new Sphere(glm::vec2(40, 0), glm::vec2(10, 30), 3.0f, 1.0f,
		glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);

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

	//static const  glm::vec4 colours[] =
	//{
	//	glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1),
	//	glm::vec4(0, 0, 1, 1), glm::vec4(0.8f, 0, 0.5f, 1),
	//	glm::vec4(0, 1, 1, 1)
	//};

	//static const int rows = 5;
	//static const int columns = 10;
	//static const int hSpace = 1;
	//static const int vSpace = 1;

	//static const glm::vec2 scrExtents(100, 50);
	//static const glm::vec2 boxExtents(7, 3);
	//static const glm::vec2 startPos
	//(
	//	-(columns >> 1)*((boxExtents.x * 2) + vSpace) + 
	//		boxExtents.x + (vSpace / 2.0f), scrExtents.y - 
	//		((boxExtents.y* 2) + hSpace)
	//);

	////Draw Blocks

	//glm::vec2 pos;
	//for (size_t y = 0; y < rows; y++)
	//{
	//	pos = glm::vec2(startPos.x, startPos.y -
	//		(y * ((boxExtents.y * 2) + hSpace)));
	//	for (size_t x = 0; x < columns; x++)
	//	{
	//		aie::Gizmos::add2DAABBFilled(pos, boxExtents, colours[y]);
	//		pos.x += (boxExtents.x * 2) + vSpace;
	//	}
	//}


	//aie::Gizmos::add2DCircle(glm::vec2(0, -35), 3, 12, glm::vec4(1, 1, 0, 1));

	//aie::Gizmos::add2DAABBFilled(glm::vec2(0, -40), glm::vec2(12, 2), glm::vec4(1, 0, 1, 1));
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