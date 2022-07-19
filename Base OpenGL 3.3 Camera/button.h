#pragma once
#include "cube.h"
#include "model.h"

class button
{
public:

	button(){}

	//costruttore 
	button(float posx, float posy, float posz, float d, float a, float rotatex, float rotatey, float rotatez) {
		x = posx;
		y = posy;
		z = posz;

		dim = d;

		angle = a;
		rotate_x = rotatex;
		rotate_y = rotatey;
		rotate_z = rotatez;
	}

	//coordinate
	float x; 
	float y;
	float z;

	//dimensione
	float dim;

	//rotazione
	float angle;
	float rotate_x;
	float rotate_y;
	float rotate_z;

	//modello del bottone
	Model* buttonSelection;

	bool isSelected; //true se il bottone è selezionato
	bool cursorIsAbove; //true se il cursore è sopra il bottone

	void init(string path_button); //inizializza il bottone
	void drawButton(Shader shader); //disegna il bottone

};

void button::init(string path_button) {

	isSelected = false;
	cursorIsAbove = false;
	buttonSelection = new Model();
	buttonSelection->loadModel(path_button);

}

void button::drawButton(Shader shader) {

	shader.use();

	// material properties
	shader.setVec3("material.ambient", 0.9f, 0.9f, 0.9f);
	shader.setVec3("material.diffuse", UNIT, UNIT, UNIT);
	shader.setVec3("material.specular", UNIT, UNIT, UNIT);
	shader.setFloat("material.shininess", 76.8f);

	if (!isSelected && !cursorIsAbove) {	 //se il bottone non è nè selezionato nè il cursore è sopra
		glm::mat4 model = glm::mat4(UNIT);
		model = glm::translate(model, glm::vec3(x, y, z));
		model = glm::rotate(model, angle, glm::vec3(rotate_x, rotate_y, rotate_z));
		model = glm::scale(model, glm::vec3(dim, dim, dim));
		shader.setMat4("model", model);
		shader.setVec3("colormodel", UNIT, UNIT, UNIT);

		buttonSelection->Draw(shader);
	}
	else if (isSelected) {  //se l'utente seleziona il bottone (click)
		glm::mat4 model = glm::mat4(UNIT);
		model = glm::translate(model, glm::vec3(x, y - 0.1f, z));
		model = glm::rotate(model, angle, glm::vec3(rotate_x, rotate_y, rotate_z));
		model = glm::scale(model, glm::vec3(dim, dim, dim));
		shader.setMat4("model", model);
		shader.setVec3("colormodel", UNIT, UNIT, UNIT);

		buttonSelection->Draw(shader);
	}
	else if (!isSelected && cursorIsAbove) { //se il cursore è sul bottone, ma non è selezionato
		glm::mat4 model = glm::mat4(UNIT);
		model = glm::translate(model, glm::vec3(x, y, z));
		model = glm::rotate(model, angle, glm::vec3(rotate_x, rotate_y, rotate_z));
		model = glm::scale(model, glm::vec3(dim, dim, dim));
		shader.setMat4("model", model);
		shader.setVec3("colormodel", 0.85f, 0.85f, 0.85f);

		buttonSelection->Draw(shader);
	}


}




