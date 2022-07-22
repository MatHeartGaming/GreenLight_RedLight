#include "skinned_mesh.h"
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
#include <map>
#include "shader_s.h"
#include "stb_image.h"
#include <gl/glu.h>
#include "update.h"
#include "updateAnimation.h"
#include "model.h"
#include "game.h"
#include "cube.h"
#include "GameArena.h"
#include "player.h"
#include "globalData.h"
#include "mainMenu.h"
#include "pauseMenu.h"
#include <irrKlang/irrKlang.h>

using namespace glm;
using namespace std;

//Dichiarazione oggetti
game* gameuno = new game();
mainMenu* main_menu = new mainMenu();
pauseMenu* pause_menu = new pauseMenu();
update* update_game = new update();
updateAnimation* update_animation = new updateAnimation();

//Time
float timebase = TIMEBASE_START;
double currentTime = TIME_START;
double previousTime = glfwGetTime();

float deltaTime = DELTA_TIME_START;	// time between current frame and last frame
float lastFrame = LAST_FRAME_START;

GLFWwindow* window;

//Camera UP
glm::vec3 up(UP_X, UP_Y, UP_Z);

float xKey = 0;
float yKey = 0;
float zKey = 0;

// Callback input mouse/keyboard
void processInput(GLFWwindow* window)
{	
	if (quit == true ) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		zKey += 0.001;
		printf("x = %f, y = %f, z = %f\n", xKey, yKey, zKey);
	}

	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		zKey -= 0.001;
		printf("x = %f, y = %f, z = %f\n", xKey, yKey, zKey);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		yKey += 0.001;
		printf("x = %f, y = %f, z = %f\n", xKey, yKey, zKey);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		yKey -= 0.001;
		printf("x = %f, y = %f, z = %f\n", xKey, yKey, zKey);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		xKey += 0.001;
		printf("x = %f, y = %f, z = %f\n", xKey, yKey, zKey);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		xKey -= 0.001;
		printf("x = %f, y = %f, z = %f\n", xKey, yKey, zKey);
	}

	//Callback MENU
	if (!gameuno->inGame || gameuno->gamePause) {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (!mouseSx) {
				mouseSx = true;
			}
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			if (mouseSx) {

				double xpos, ypos;
				//getting cursor position
				glfwGetCursorPos(window, &xpos, &ypos);

				main_menu->xPos = xpos;
				main_menu->yPos = ypos;

				pause_menu->xPos = xpos;
				pause_menu->yPos = ypos;

				main_menu->buttonClicked = true;
				pause_menu->buttonClicked = true;
			}
			mouseSx = false;
		}
	}

	//Callback GAME
	if (gameuno->inGame) {

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || gameuno->gameOver == true) {
			if (buttonEsc == false) {
				buttonEsc = true;
				if (gameuno->gamePause == false) {
					gameuno->gamePause = true;
				}
				else {
					if (gameuno->gameOver == false) {
						gameuno->gamePause = false;
					}
				}
			}	
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && gameuno->gameOver == false) {
			buttonEsc = false;
		}

		if (!gameuno->doll->start) {
			return;
		}

		//To Run!
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			run = true;
		}
		else {
			run = false;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (muoviSx != true) {
				moveDx = true;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
			moveDx = false;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			if (moveDx != true) {
				muoviSx = true;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
			muoviSx = false;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			if (muoviGiu != true) {
				muoviSu = true;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
			muoviSu = false;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			if (muoviSu != true) {
				muoviGiu = true;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
			muoviGiu = false;
		}
	}
}

void ray_plane(glm::vec3 plane_normal_word, glm::vec3 plane_pos_word, glm::vec3 ray_word, glm::vec3 origin, float dim_square) {

	float denom = dot(plane_normal_word, ray_word);
	if (abs(denom) > 0.0001f)
	{
		float t = dot((plane_pos_word - origin), plane_normal_word) / denom;
		glm::vec3 p = origin + t * ray_word; //calcolo del punto intersecato p

		if ((t >= 0.0f) && (p.z >= plane_pos_word.z - dim_square && p.z <= plane_pos_word.z + dim_square) && (p.x >= plane_pos_word.x - dim_square && p.x <= plane_pos_word.x + dim_square)) {

			float player_xpos = gameuno->getPlayer()->getX(); //coordinata x del player
			float player_zpos = gameuno->getPlayer()->getZ(); //coordinata z del player
			//cout << "*** PLAYER Position (X,Z): (" << player_xpos << ", " << player_zpos << ")" << endl;
			//cout << "*** Mouse Position (X,Z): (" << p.x << ", " << p.z << ")" << endl;

			gameuno->setMousePoint(p);
			main_menu->setMousePoint(p);
			//pause_menu->setMousePoint(p);

		}

	}

}

void mouse_position() {

	POINT cp;
	GetCursorPos(&cp);	//Projection e View

	glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view_matrix = view_global;


	//Step 1: 3d Normalised Device Coordinates
	float x = (2.0f * cp.x) / (float)SCR_WIDTH - UNIT;
	float y = UNIT - (2.0f * cp.y) / (float)SCR_HEIGHT;
	float z = UNIT;
	glm::vec3 ray_nds(x, y, z);

	//Step 2: 4d Homogeneous Clip Coordinates
	glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -UNIT, UNIT);

	//Step 3: 4d Eye (Camera) Coordinates
	glm::vec4 ray_eye = inverse(projection_matrix) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -UNIT, 0.0);

	//Step 4: 4d World Coordinates
	glm::vec3 ray_word((inverse(view_matrix) * ray_eye).x, (inverse(view_matrix) * ray_eye).y, (inverse(view_matrix) * ray_eye).z);
	ray_word = glm::normalize(ray_word);

	glm::vec3 plane_normal_word(0, 0, 1);
	glm::vec3 plane_pos_word(0, 0, 0);
	ray_plane(plane_normal_word, plane_pos_word, ray_word, pos_camera_mobile_global, DIM);


	double xpos, ypos;
	//getting cursor position
	glfwGetCursorPos(window, &xpos, &ypos);

	main_menu->xPos = xpos;
	main_menu->yPos = ypos;

	pause_menu->xPos = xpos;
	pause_menu->yPos = ypos;

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	SCR_HEIGHT = height;
	SCR_WIDTH = width;
}


void renderGame(Shader simpleShader, Shader lightShader, Shader animShader) {
	glClearColor(COLOR_R, COLOR_G, COLOR_B, UNIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentTime = glfwGetTime();
	double timeInterval = currentTime - previousTime;
	if (timeInterval >= RENDER_SPEED) {
	
		player* player = gameuno->getPlayer();

		// ------- PLAYER MOVES ------- //

		if (moveDx) {
			update_game->moveRight(player, animShader);
		}

		if (muoviSx) {
			update_game->moveLeft(player, animShader);
		}

		if (muoviSu) {
			update_game->moveUp(player, animShader);
		}

		if (muoviGiu) {
			update_game->moveDown(player, animShader);
		}
		if (mouseSx) {
			gameuno->getPlayer()->mouseSxIsSelected = true;
		}
		else {
			gameuno->getPlayer()->mouseSxIsSelected = false;
		}
		
		/*if (gameuno->getPlayer()->life <= MIN_LIFE_PLAYER) {
			gameuno->gameOver = true;
		}*/

		update_game->calculateAnglePlayer(player); //player rotation

		// ------- MOUSE ------- //
		mouse_position();


		// ----- ANIMATION ----- //
		update_animation->updateAllAnimations(player);

		gameuno->doll->animate(currentTime - previousTime);
		gameuno->p->animate(currentTime - previousTime);
		for (int i = 0; i < N; i++) {

			if (!gameuno->p->players[i].dead) {

				if (!gameuno->doll->start) {
					gameuno->p->players[i].move = false;
				}

				if (!gameuno->p->players[i].finish) {
					if (gameuno->doll->animState == 1) { // Forward
						if (gameuno->p->players[i].velocity > 0) {
							if (gameuno->p->players[i].z > -42 && !gameuno->p->players[i].userControlled) {
								gameuno->p->players[i].dead = true;
								gameuno->p->players[i].move = false;
							}
							else if (gameuno->p->players[i].userControlled && gameuno->p->players[i].z > -42 && (muoviGiu || muoviSu || muoviSx || moveDx)) {
								gameuno->p->players[i].dead = true;
								gameuno->p->players[i].move = false;
							}
						}

						//if (!gameuno->p->players[i].userControlled) {
							if ((rand() % 7000) == 0) {
								gameuno->p->players[i].velocity = 0.5;
							}
						//}
					}
					else if (gameuno->doll->animState == 0) { // Moving Forward
						if (gameuno->p->players[i].velocity > 0) {
							if (gameuno->p->players[i].z > -42) {
								gameuno->p->players[i].move = false;
							}
						}
						gameuno->p->deadStatePlay = 0;
					}
					else if (gameuno->doll->animState == 3) { // Forward
						gameuno->p->deadStatePlay = 0;
						gameuno->p->players[i].move = true;
					}
				}
			}

			if (gameuno->p->players[i].animationTime_playerVictory_1 >= 4 && gameuno->p->players[i].userControlled) {
				gameuno->gameOver = true;
				gameuno->p->players[i].victory = true;
			}

		}

		// ------- SOUND ------- //
		previousTime = currentTime;
	}

	// ------- CAMERA ------- //
//	float x = gameuno->getPlayer()->getX();
//	float z = gameuno->getPlayer()->getZ();

	float x = gameuno->p->players[gameuno->p->userPlayerIndex].x;
	float z = gameuno->p->players[gameuno->p->userPlayerIndex].z;
	float z_doll = -100;

	//coordinate camera
	glm::vec3 pos_camera_mobile(x, MOBILE_CAMERA_OFFSET_Y, z + MOBILE_CAMERA_OFFSET_Z);
	glm::vec3 at_camera_mobile(0, MOBILE_CAMERA_AT_Y, z_doll);

	//view
	glm::mat4 view = glm::mat4(UNIT);
	view = glm::lookAt(pos_camera_mobile, at_camera_mobile, up);
	pos_camera_mobile_global = pos_camera_mobile;
	view_global = view;
	lightShader.setMat4("view", view);
	simpleShader.setMat4("view", view);

	// ------- DRAW ------- //
	gameuno->draw(simpleShader, lightShader, animShader, view);
}

void renderMainMenu(Shader simpleShader, Shader lightShader) {
	glClearColor(COLOR_R, COLOR_G, COLOR_B, UNIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentTime = glfwGetTime();
	double timeInterval = currentTime - previousTime;
	if (timeInterval >= RENDER_SPEED) {

		// ------- MOUSE ------- //
		mouse_position();
		update_game->cursorMainMenu(main_menu); //update del cursore
		//update_animation->menuSound(main_menu);

		if (main_menu->startNewGame) { //il flag startNewGame è true (l'utente ha cliccato su play)

			currentTime = glfwGetTime();
			srand(currentTime);

			gameuno->gameOver = false;
			gameuno->loadingGame->init(); //inizializzo la barra di caricamento 
			gameuno->loadingGame->isLoading = true; //setto il caricamento a true
			main_menu->startNewGame = false; //resetto il flag startNewGame
			
		}

		previousTime = currentTime;
	}

	main_menu->draw(simpleShader, lightShader, currentTime);

	

}

void renderLoading(Shader simpleShader) {

	glClearColor(COLOR_R, COLOR_G, COLOR_B, UNIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//setto a false la soundtrack del menu
	if (main_menu->startMenuSoundtrack == true) {
		//update_animation->SoundEngine_soundtrack->stopAllSounds();
		main_menu->startMenuSoundtrack = false;
	}

	cout << "gameuno->loadingGame->statusLoading" << gameuno->loadingGame->statusLoading << endl;
	gameuno->loadingGame->draw(simpleShader, gameuno->loadingGame->statusLoading/13.2f); //draw della barra di caricamento

	if (gameuno->loadingGame->statusLoading == 100.0f) { //quando statusLoading ha raggiunto il 100%
		gameuno->loadingGame->isLoading = false;  //setto il loading completato
		gameuno->gamePause = false;				  //setto il flag gamePause a false
		gameuno->inGame = true;                   //setto il flag inGame a true
	}

}

void renderPauseMenu(Shader simpleShader, Shader lightShader) {
	glClearColor(COLOR_R, COLOR_G, COLOR_B, UNIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentTime = glfwGetTime();
	double timeInterval = currentTime - previousTime;
	if (timeInterval >= RENDER_SPEED) {

		// ------- MOUSE ------- //
		mouse_position();
		update_game->cursorPauseMenu(pause_menu, gameuno);

		previousTime = currentTime;
	}

	bool youWin = false;
	bool gameOver = false;
	for (int i = 0; i < N; i++) {
		if (gameuno->p->players[i].userControlled && gameuno->p->players[i].dead) {
			youWin = false;
			gameOver = true;
			break;
		}
		else if(gameuno->p->players[i].userControlled && gameuno->p->players[i].victory){
			youWin = true;
			gameOver = true;
			break;
		}
	}
	pause_menu->draw(simpleShader, lightShader, gameOver, youWin);
}

//Render principale che gestisce tutte le altre schermate di render (intro, main_menù, game, pausa_menù)
void render(Shader simpleShader, Shader lightShader, Shader animShader, Shader simpleShaderMenu, Shader lightShaderMenu)
{
	currentTime = glfwGetTime();

		if (!gameuno->inGame && !gameuno->loadingGame->isLoading) {
			//main_menu->music->setSoundVolume(1.0);
			renderMainMenu(simpleShaderMenu, lightShaderMenu);
		}
		else if (!gameuno->inGame && gameuno->loadingGame->isLoading && !gameuno->loadingGame->drawLoadingBar) {
			renderLoading(simpleShader);
			gameuno->loadingGame->drawLoadingBar = true;
		}
		else if (!gameuno->inGame && gameuno->loadingGame->isLoading && gameuno->loadingGame->drawLoadingBar) {
			renderLoading(simpleShader);
			gameuno->init();
			gameuno->loadingGame->drawLoadingBar = false;
			gameuno->loadingGame->statusLoading++;
		}
		else if (gameuno->inGame && !gameuno->loadingGame->isLoading && !gameuno->gamePause) {
			//main_menu->music->setSoundVolume(0.1);
			renderGame(simpleShader, lightShader, animShader);
		}
		else if (gameuno->inGame && !gameuno->loadingGame->isLoading && gameuno->gameOver) {
			//main_menu->music->setSoundVolume(1.0);
			renderPauseMenu(simpleShader, lightShader);
		}
		else if (gameuno->inGame && !gameuno->loadingGame->isLoading && gameuno->gamePause) {
			//main_menu->music->setSoundVolume(1.0);
			renderPauseMenu(simpleShader, lightShader);
		}
}

//Inizializza i menù
void init() {
	main_menu->init();
	pause_menu->init();
}

// load and create a texture 
unsigned int loadtexture(std::string filename, bool png, bool flip)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(flip); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		{
			if (nrChannels == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
	}
	else
	{
		std::cout << "Failed to load texture " << filename.c_str() << std::endl;
		return -1;
	}
	stbi_image_free(data);

	return texture;
}

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Squid Game - Red Light Green Light", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glEnable(GL_MULTISAMPLE);

	// caricamento texture
	
	gameuno->loadingGame->texture_background = loadtexture("texture/Squid_loading.jpg", false, false);

	main_menu->texture_splash = loadtexture("texture/SplashScreen.png", false, false);
	main_menu->texture_background = loadtexture("texture/MenuSquidGame.jpg", false, false);

	//Pause menu textures
	pause_menu->texture_background = main_menu->texture_background;
	pause_menu->texture_gameover = loadtexture("texture/gameover2.png", true, false);
	pause_menu->texture_you_win = loadtexture("texture/you_win2.jpg", false, false);

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(false);

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	//glEnable(GL_DEPTH);

	//dichiarazione degli shader
	Shader simpleShader("shaders/vertex_simple.vs", "shaders/fragment_simple.fs");
	Shader simpleShaderMenu("shaders/vertex_simple_menu.vs", "shaders/fragment_simple_menu.fs");
	Shader lightShader("shaders/vertex_lights.vs", "shaders/fragment_lights.fs");
	Shader lightShaderMenu("shaders/vertex_lights_menu.vs", "shaders/fragment_lights_menu.fs");
	Shader animationShader("shaders/vertex_anim_lights.vs", "shaders/fragment_anim_lights.fs");
	Shader simpleDepthShader("shaders/shadow_mapping_depth.vs", "shaders/shadow_mapping_depth.fs");
	Shader animDepthShader("shaders/shadow_mapping_depth_anim.vs", "shaders/shadow_mapping_depth.fs");

	init();

	// configure depth map FBO
   // -----------------------
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	animDepthShader.use();
	glBindTexture(GL_TEXTURE_2D, depthMap);

	animationShader.use();

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	lightProjection = glm::ortho(-10.0f, 10.0f, -49.0f, 49.0f, -50.0f, 27.0f);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 0.0, 1.0));
	lightSpaceMatrix = lightProjection * lightView;
	animationShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	animationShader.setVec3("lightPos", lightPos);
	animationShader.setInt("shadowMap", 0);
	animationShader.setInt("textureDiffuse", 1);


	// shader configuration
	// --------------------
	lightShader.use();
	lightShader.setInt("diffuseTexture", 1);
	lightShader.setInt("shadowMap", 0);

	// lighting info
	// -------------
	
	main_menu->TimeStart = glfwGetTime();

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		//cout << "X: " << gameuno->getPlayer()->getX() << ", Y: " << gameuno->getPlayer()->getY() << ", Z: " << gameuno->getPlayer()->getZ() << endl;

		mat4 lightProjection, lightView;
		mat4 lightSpaceMatrix;
		float near_plane = -500.0f, far_plane = 500.0f;
		//lightProjection = perspective(radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		lightProjection = ortho(-500.0f, 500.0f, -500.0f, 500.0f, near_plane, far_plane);
		lightView = lookAt(lightPos, vec3(0.0f), vec3(0.0, 0.0, 1.0));
		lightSpaceMatrix = glm::mat4(UNIT) ;

		animDepthShader.use();
		animDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);


		// render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		render(simpleShader, simpleDepthShader, animDepthShader, simpleShaderMenu, lightShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightShader.use();
		// set light uniforms
		lightShader.setVec3("lightPos", lightPos);
		lightShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		lightShader.setVec3("viewPos", pos_camera_mobile_global);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		render(simpleShader, lightShader, animationShader, simpleShaderMenu, lightShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}
