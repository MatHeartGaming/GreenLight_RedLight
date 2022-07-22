#pragma once
//GAME SETTINGS

//TIME
const float TIMEBASE_START = 0.0f;
const double TIME_START = 0.0f;
const float DELTA_TIME_START = 0.0f;
const float LAST_FRAME_START = 0.0f;

//CLEAR COLOR PARAM
const float COLOR_R = 0.2f;
const float COLOR_G = 0.3f;
const float COLOR_B = 0.3f;

// RESOLUTIONS
unsigned int SCR_WIDTH = 1024;
unsigned int SCR_HEIGHT = 576;

// -- CAMERA -- //
glm::vec3 lightPos(0.0f, 25.0f, 0.0f); //posizione luce
const float MOBILE_CAMERA_OFFSET_Y = 3.0f;
const float MOBILE_CAMERA_OFFSET_Z = 10.0f;
const float MOBILE_CAMERA_AT_Y = 1.5f;

const float UP_X = 0.0f;
const float UP_Y = 1.0f;
const float UP_Z = 0.0f;

glm::vec3 pos_camera_mobile_global(1.0f);
glm::mat4 view_global(1.0f);

const float ANGLE_180 = 180.0f;
const float ANGLE_360 = 360.0f;
const float ANGLE_270 = 270.0f;
const float ANGLE_45 = 45.0f;
const float ANGLE_135 = 135.0f;
const float ANGLE_225 = 225.0f;
const float ANGLE_315 = 315.0f;
const float ANGLE_90 = 90.0f;

const float ANGLE_OFFSET = 15.0f;

// -- KEYBOARD/MOUSE BUTTON CONTROLS-- //
bool moveDx = false;
bool muoviSx = false;
bool muoviSu = false;
bool muoviGiu = false;
bool run = false;

bool mouseSx = false;
bool buttonEsc = false;

bool quit = false;

// Players number
const int N = 456;

// -- INTRO / LOADING -- //
const int FIRST_INTRO = 1; // Da togliere dopo aver tolto prevMenu

const int TIME_END_FIRST_INTRO = 15; // Da togliere dopo aver tolto prevMenu

const int STATUS_LOADING_0 = 0;
const int STATUS_LOADING_1 = 15;
const int STATUS_LOADING_2 = 50;
const int STATUS_LOADING_3 = 60;
const int STATUS_LOADING_4 = 80;
const int STATUS_LOADING_5 = 90;

const float PLAYER_SPOW_X = 0.0f;
const float PLAYER_SPOW_Y = 0.0f;
const float PLAYER_SPOW_Z = 20.0f;

// -- GAME PARAMETERS -- //
const int STRIDE = 8;

float RENDER_SPEED = 0.03f; //Più è alto e più rallenta; più è basso e più velocizza

const int DIM = 18 * 98; //Dimensione floor 20x20

float ROOM_DIM_X1 = 9.0f;
float ROOM_DIM_X2 = -9.0f;
float ROOM_DIM_Z1 = -49.0f;
float ROOM_DIM_Z2 = 49.0f;

// valore unitario
const float UNIT = 1.0f;
const float INIT_ZERO = 0.0f;

const float PROJECTION_ANGLE_45 = 45.0f;

bool isColliding = false;