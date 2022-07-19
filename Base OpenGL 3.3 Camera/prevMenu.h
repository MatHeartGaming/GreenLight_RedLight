#pragma once
#include "cube.h"

class prevMenu
{
public:

	prevMenu() {}

	cube* background;

	unsigned int texture_unibas;
	unsigned int texture_name;
	unsigned int texture_team;


	void setShadersProperties(Shader simpleShader, Shader lightShader);
	void draw(Shader simpleShader, Shader lightShader, int intro);

	void init();
};


void prevMenu::setShadersProperties(Shader simpleShader, Shader lightShader) {

	//camera pos
	glm::vec3 pos_camera(0.0f, 12.0f, UNIT);
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
	lightShader.setVec3("light.position", lightPos);
	lightShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
	lightShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);
	lightShader.setVec3("light.specular", UNIT, UNIT, UNIT);
	lightShader.setVec3("colormodel", UNIT, UNIT, UNIT);
}

void prevMenu::init() {


	background = new cube(18.0f, 0.0f, 18.0f, glm::radians(4.9f), UNIT, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

}


void prevMenu::draw(Shader simpleShader, Shader lightShader, int intro) {
	
	setShadersProperties(simpleShader, lightShader);
	if (intro == FIRST_INTRO) {
		background->drawCube(simpleShader, texture_unibas);
	}
	else if (intro == SECOND_INTRO) {
		background->drawCube(simpleShader, texture_name);
	}
	else {
		background->drawCube(simpleShader, texture_team);
	}

}