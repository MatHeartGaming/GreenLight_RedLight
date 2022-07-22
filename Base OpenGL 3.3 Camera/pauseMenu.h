#pragma once
#include "button.h"
#include "model.h"
#include "skinned_mesh.h"
#include "globalData.h"

class pauseMenu
{
public:
	pauseMenu() {}

	button* returnGame;
	button* goToMainMenu;
	button* quit;

	float xPos, yPos;

	cube* background;
	unsigned int texture_background;
	unsigned int texture_gameover;
	unsigned int texture_you_win;


	bool buttonClicked; //true quando l'utente clicca un bottone del menu

	glm::vec3 mousePoint;

	void init();
	void setShadersProperties(Shader simpleShader, Shader lightShader);
	void draw(Shader simpleShader, Shader lightShader, bool gameover, bool youWin);
	void drawGameOver(Shader simpleShader);
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

void pauseMenu::init() {

	buttonClicked = false;
	background = new cube(1.7f, 0.0f, 1.0f, glm::radians(90.0f), UNIT, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	returnGame = new button(0.0f, 0.18f, 0.0f, 0.024f, glm::radians(0.0f), UNIT, 0.0f, 0.0f);
	returnGame->init("models/menu/button_return.dae");

	goToMainMenu = new button(0.0f, 0.0, 0.0f, 0.024f, glm::radians(0.0f), UNIT, 0.0f, 0.0f);
	goToMainMenu->init("models/menu/button_mainMenu.dae");

	quit = new button(0.0f, -0.18, 0.0f, 0.024f, glm::radians(0.0f), UNIT, 0.0f, 0.0f);
	quit->init("models/menu/button_quit.dae");


}

void pauseMenu::setShadersProperties(Shader simpleShader, Shader lightShader) {


	//camera pos
	glm::vec3 pos_camera(0.0f, 0.0f, 1.0);
	glm::vec3 at_camera(0.0f, 0.0f, 0.0f);
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
	projection2 = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	simpleShader.setMat4("projection", projection2);

	// ---- LIGHT Shader ---- //
	lightShader.use();

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

void pauseMenu::draw(Shader simpleShader, Shader lightShader, bool gameover, bool youWin) {
	setShadersProperties(simpleShader, lightShader);


	goToMainMenu->drawButton(lightShader);
	quit->drawButton(lightShader);
	if (!gameover) {
		returnGame->drawButton(lightShader);
		background->drawCube(simpleShader, texture_background);

	}
	else if (youWin) {
		//drawGameOver(simpleShader);
		background->drawCube(simpleShader, texture_you_win);
	}
	else {
		//drawGameOver(simpleShader);
		returnGame->drawButton(lightShader);
		background->drawCube(simpleShader, texture_gameover);
	}

	goToMainMenu->drawButton(lightShader);
	quit->drawButton(lightShader);
}

void pauseMenu::drawGameOver(Shader simpleShader) {
	simpleShader.use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texture_gameover);
	glBindVertexArray(cubeVAO);

	//simpleShader.setVec3("colorcube", UNIT, 0.0f, 0.0f);
	glm::mat4 modelS = glm::mat4(UNIT);

	modelS = glm::translate(modelS, glm::vec3(0.0f, 2.0f, -2.0f));
	modelS = glm::rotate(modelS, glm::radians(180.0f), glm::vec3(UNIT, 0.0f, 0.0f));
	modelS = glm::translate(modelS, glm::vec3(0.0f, 0.0f, 0.0f));
	modelS = glm::scale(modelS, glm::vec3(3.4f, 0.02f, 2.0f));


	simpleShader.setMat4("model", modelS);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisable(GL_BLEND);
}
