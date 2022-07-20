#pragma once
//GAME SETTINGS

//TIME
const float TIMEBASE_START = 0.0f;
const double TIME_START = 0.0f;
const float DELTA_TIME_START = 0.0f;
const float LAST_FRAME_START = 0.0f;

//PLANE HIT MOUSE
const float PLANE_NORMAL_X = 0.0f;
const float PLANE_NORMAL_Y = 1.0f;
const float PLANE_NORMAL_Z = 0.0f;
const float PLANE_POS_X = 0.75f;
const float PLANE_POS_Y = 1.0f;
const float PLANE_POS_Z = 0.75f;

//CLEAR COLOR PARAM
const float COLOR_R = 0.2f;
const float COLOR_G = 0.3f;
const float COLOR_B = 0.3f;

//TEXTURE FONT
const int FIRST_NAMBER = 0;
const int LAST_NAMBER = 10;

// RESOLUTIONS
unsigned int SCR_WIDTH = 1024;
unsigned int SCR_HEIGHT = 576;

// -- CAMERA -- //
glm::vec3 lightPos(0.0f, 20.0f, 0.0f); //posizione luce
const float MOBILE_CAMERA_OFFSET_Y = 3.0f;
const float MOBILE_CAMERA_OFFSET_Z = 10.0f;
const float MOBILE_CAMERA_AT_Y = 1.5f;

const float UP_X = 0.0f;
const float UP_Y = 1.0f;
const float UP_Z = 0.0f;

glm::vec3 pos_camera_mobile_global(1.0f);
glm::mat4 view_global(1.0f);

const float ANGLE_PLAYER_ROTATE = 90.0f;

const float ANGLE_0 = 0.0f;
const float ANGLE_180 = 180.0f;
const float ANGLE_360 = 360.0f;
const float ANGLE_270 = 270.0f;
const float ANGLE_45 = 45.0f;
const float ANGLE_135 = 135.0f;
const float ANGLE_225 = 225.0f;
const float ANGLE_315 = 315.0f;
const float ANGLE_90 = 90.0f;

const float ANGLE_OFFSET = 15.0f;

const float SHADOW_OFFSET_Y_PLAYER = 0.02f;
const float SHADOW_SCALE_PLAYER = 1.4f;

const float OFFSET_SHOT_BAR = 2.22f;
const float SCALE_OFFSET_SHOT_BAR = 0.03f;
const float SCALE_SHOT_BAR_Y = 0.01f;
const float SCALE_SHOT_BAR_Z = 0.10f;

const float OFFSET_POSITION_LIFEBAR = 2.4f;
const float SCALE_LIFEBARZ = 0.15f;

const float OFFSET_Y_CHESTlIFE_BAR = 1.5f;

// -- DIREZIONI bot (riferimenti per i path) -- //
const int DIRECTION_RIGHT = 0;
const int DIRECTION_LEFT = 1;
const int DIRECTION_DOWN = 2;
const int DIRECTION_UP = 3;

const int ZOMBIE_PRISONER = 1;
const int ZOMBIE_DERRICK = 2;
const int ZOMBIE_COP = 3;

// -- SPECULAR VALUE -- //
const int HIGH = 2;
const int MODERATE = 1;
const int NONE = 0;

// -- KEYBOARD/MOUSE BUTTON CONTROLS-- //
bool moveDx = false;
bool muoviSx = false;
bool muoviSu = false;
bool muoviGiu = false;
bool run = false;

bool mouseSx = false;
bool buttonEsc = false;

bool quit = false;
bool startPlayerShot = false;

// -- INTRO / LOADING -- //
const int FIRST_INTRO = 1;
const int SECOND_INTRO = 2;
const int LAST_INTRO = 3;

const int TIME_END_FIRST_INTRO = 15;
const int TIME_END_SECOND_INTRO = 20;
const int TIME_END_LAST_INTRO = 25;

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

const int DIM = 18*98; //Dimensione floor 20x20
float TILE_DIM = 1.0; //Dimensione singola mattonella

float ROOM_DIM_X1 = 8.4f;
float ROOM_DIM_X2 = -8.4f;
float ROOM_DIM_Z1 = -49.0f;
float ROOM_DIM_Z2 = 49.0f;
float MOVE_STEP = TILE_DIM ; //era /9		// = 0.1000f; //Incremento dello spostamento del player 
float BOT_MOVE_STEP = TILE_DIM / 30; // = 0.05; //Incremento dello spostamento del BOT 

float EPSILON_1 = MOVE_STEP / 2;
float EPSILON_2 = BOT_MOVE_STEP / 100;
float EPSILON_3 = 0.25f;

const float SCALE_SCORE_X = 2.5f;
const float SCALE_SCORE_Y = 0.1f;
const float SCALE_SCORE_Z = 2.5f;
const float OFFSET_Y_SCORE = 3.0f;
const float OFFSET_Z_SCORE = 5.0f;
const float OFFSET_TEN_X = 1.8f;
const float OFFSET_TEN_Y = 3.0f;
const float OFFSET_TEN_Z = 5.0f;
const float SCALE_TEN_X = 0.7f;
const float SCALE_TEN_Y = 0.1f;
const float SCALE_TEN_Z = 0.7F;
const float OFFSET_UNIT_X = 2.5f;

const int STONE1 = 15;
const int CHEST = 16;


// -- OTHERS -- //
// FOR FIRST
const int FIRST = 0;
// valore unitario
const float UNIT = 1.0f;
const float INIT_ZERO = 0.0f;
//PI
const float PI = 3.14f;
const float HALF_PI = 3.14f / 2.0F;

const float PROJECTION_ANGLE_45 = 45.0f;

bool isColliding = false;