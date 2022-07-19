#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstring>
#include <string>
#include <stack>
#include "shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include <vector> 
#include "globalData.h"

unsigned int cubeVBO, cubeVAO;

float verticesCube[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f, UNIT, 0.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,   0.0f, UNIT, 0.0f,  UNIT,  0.0f,
	0.5f,  -0.5f, 0.5f,   0.0f, UNIT, 0.0f,  UNIT,  UNIT,
	0.5f,  -0.5f, 0.5f,   0.0f, UNIT, 0.0f,  UNIT,  UNIT,
	-0.5f,  -0.5f, 0.5f,  0.0f, UNIT, 0.0f,  0.0f,  UNIT,
	-0.5f, -0.5f, -0.5f,  0.0f, UNIT, 0.0f,  0.0f,  0.0f
};

class cube {

private:

	float dimensione_x;
	float dimensione_y;
	float dimensione_z;

	float angle;
	float rotation_x;
	float rotation_y;
	float rotation_z;

	float posizione_x;
	float posizione_y;
	float posizione_z;

public:

	//Costruttore con argomenti
	cube(float d_x, float d_y, float d_z,
		float a, float r_x, float r_y, float r_z,
		float p_x, float p_y, float p_z)
	{

		dimensione_x = d_x;
		dimensione_y = d_y;
		dimensione_z = d_z;

		angle = a;
		rotation_x = r_x;
		rotation_y = r_y;
		rotation_z = r_z;

		posizione_x = p_x;
		posizione_y = p_y;
		posizione_z = p_z;

	}


	void drawCube(Shader lightShader, unsigned int texture) {

		lightShader.use();

		// texture
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//lightShader.setInt("myTexture1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(cubeVAO);

		glm::mat4 model = glm::mat4(UNIT);	//identity matrix
		model = glm::translate(model, glm::vec3(posizione_x, posizione_y, posizione_z));
		model = glm::rotate(model, angle, glm::vec3(rotation_x, rotation_y, rotation_z));
		model = glm::scale(model, glm::vec3(dimensione_x, dimensione_y, dimensione_z));
		lightShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);
	}



	void setRotation_x(float x) {
		rotation_x = x;
	}

	void setRotation_y(float y) {
		rotation_y = y;
	}

	void setRotation_z(float z) {
		rotation_z = z;
	}

	void setX(float x) {
		posizione_x = x;
	}

	void setY(float y) {
		posizione_y = y;
	}

	void setZ(float z) {
		posizione_z = z;
	}

	float getX() {
		return posizione_x;
	}

	float getY() {
		return posizione_y;
	}

	float getZ() {
		return posizione_z;
	}

	float getAngle() {
		return angle;
	}

	void setAngle(float a) {
		angle = a;
	}

	void setDimX(float d_x) {
		dimensione_x = d_x;
	}


};

