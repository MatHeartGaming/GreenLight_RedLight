#pragma once
#include "button.h"
#include "model.h"
#include "skinned_mesh.h"
#include "globalData.h"
#include <irrKlang/irrKlang.h>

class mainMenu
{
public:

	mainMenu() {}

	float height;
	float x, y;
	float TimeStart = 0;

	float xPos, yPos;

	cube* background;
	unsigned int texture_background;
	unsigned int texture_splash;


	button* button_NewGame;

	glm::vec3 mousePoint;

	bool startNewGame; //quando è true starta il game

	bool buttonClicked; //true quando l'utente clicca su un bottone del meu

	bool startMenuSoundtrack;  //quando è true si attiva la soundtrack del menu

	void init();
	void setShadersProperties(Shader simpleShader, Shader lightShader);
	void draw(Shader simpleShader, Shader lightShader, float TimeVal);

	void setMousePoint(glm::vec3 w) {
		mousePoint = w;
	}

	glm::vec3 getMousePoint() {

		//Step 1: 3d Normalised Device Coordinates
		float x = (2.0f * xPos) / (float)SCR_WIDTH - UNIT;
		float y = UNIT - (2.0f * yPos) / (float)SCR_HEIGHT;
		float z = UNIT;
		glm::vec3 ray_nds(x, y, z);

		return ray_nds;
	}
};

void mainMenu::setShadersProperties(Shader simpleShader, Shader lightShader) {

	float z = 1;
	if (SCR_WIDTH > SCR_HEIGHT) {
		z = (float)SCR_HEIGHT / (float)SCR_WIDTH;
		z = 2.3532 * z - 0.9706;
	}

	//camera pos
	glm::vec3 pos_camera(0, 0, z);
	glm::vec3 at_camera(0, 0, 0.0f);
	pos_camera_mobile_global = pos_camera;
	glm::vec3 up(0.0, 1.0, 0.0);

	// ---- SIMPLE Shader ---- //
	simpleShader.use();

	//view
	glm::mat4 view2 = glm::mat4(UNIT);
	view2 = glm::lookAt(pos_camera, at_camera, up);
	view_global = view2;
	simpleShader.setMat4("view", view2);

	//projection
	glm::mat4 projection2 = glm::mat4(UNIT);	//identity matrix
	projection2 = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 30.0f);
	simpleShader.setMat4("projection", projection2);

	// ---- LIGHT Shader ---- //
	lightShader.use();

	z = 8.4645938;
	if (SCR_WIDTH > SCR_HEIGHT) {
		z = (float)SCR_HEIGHT / (float)SCR_WIDTH;
		z = 10.17432 * z;
	}

	pos_camera = glm::vec3(-0.094, 1.245, z);
	at_camera = glm::vec3(-0.094, 1.245, 0.0f);

	//projection
	glm::mat4 projection3 = glm::mat4(UNIT);	//identity matrix
	projection3 = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	lightShader.setMat4("projection", projection3);

	//view
	glm::mat4 view3 = glm::mat4(UNIT);
	view3 = glm::lookAt(pos_camera, at_camera, up);
	view_global = view3;
	lightShader.setMat4("view", view3);

	//light per la mappa
	lightShader.setVec3("light.position", 0.0f, 5.0f, 100.0f);
	lightShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
	lightShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
	lightShader.setVec3("light.specular", 0.0f, 0.0f, 0.0f);
}

void mainMenu::init() {

	startMenuSoundtrack = false;
	startNewGame = false;
	buttonClicked = false;

	height = 1;
	x = 0;
	y = 0;

	button_NewGame = new button(0, 0, 0, 0.2f, glm::radians(0.0f), UNIT, 0.0f, 0.0f);
	button_NewGame->init("models/menu/button_play.dae");

	background = new cube(2.1f, 2.0f, 1.15f, glm::radians(90.0f), UNIT, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void mainMenu::draw(Shader simpleShader, Shader lightShader, float TimeVal) {

	setShadersProperties(simpleShader, lightShader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if ((TimeVal - TimeStart) > 5) {
		button_NewGame->drawButton(lightShader);
		background->drawCube(simpleShader, texture_background);
	}
	else {
		background->drawCube(simpleShader, texture_splash);
	}

}

