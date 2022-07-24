#pragma once
#include "cube.h"

class loading
{
public:

	//costruttore
	loading(){
		loadingInitialized = false;
	}

	bool isLoading; //true se  è in caricamento
	bool drawLoadingBar; //true per il draw
	float statusLoading; //percentuale di avanzamento del caricamento

	Model* loading_boundary;
	cube* loading_bar;
	cube* background;

	unsigned int texture_statusbar;
	unsigned int texture_boundary;
	unsigned int texture_background;

	bool loadingInitialized;

	void draw(Shader simpleShader, float width);
	void init();
};

void loading::init() {

	isLoading = false;
	drawLoadingBar = false;
	statusLoading = 0.0f;

	if (loadingInitialized == false) {
		cout << "Loading Init" << endl;
		background = new cube(30.0f, 0.0f, 20.0f, glm::radians(90.0f), UNIT, 0.0f, 0.0f, 0.0f, 0.0f, -5.0f);

		loading_boundary = new Model();
		loading_boundary->loadModel("animation/card/card.obj");
		loadingInitialized = true;
	}
}

void loading::draw(Shader simpleShader, float width) {

	//camera pos
	glm::vec3 pos_camera(0.0f, 0.0f, 10.0f);
	glm::vec3 at_camera(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0, 1.0, 0.0);

	simpleShader.use();

	//view
	glm::mat4 view2 = glm::mat4(UNIT);
	view2 = glm::lookAt(pos_camera, at_camera, up);
	view_global = view2;
	simpleShader.setMat4("view", view2);

	//projection
	glm::mat4 projection2 = glm::mat4(UNIT);	//identity matrix
	projection2 = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
	simpleShader.setMat4("projection", projection2);

	background->drawCube(simpleShader, texture_background);

	glm::mat4 model = glm::mat4(UNIT);
	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(UNIT-0.2, UNIT - 0.2, UNIT - 0.2));
	simpleShader.setMat4("model", model);
	simpleShader.setInt("myTexture1", 1);

	loading_boundary->Draw(simpleShader);
	simpleShader.setInt("myTexture1", 0);

}
