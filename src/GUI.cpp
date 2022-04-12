//
// Created by stenm on 12.04.2022.
//

#include "GUI.h"

float GUI::_gravity[2] = { 0.0, 50.0 };
int GUI::_spawnRate = 50;
int GUI::_maxParticleSpeed = 100;
bool GUI::_hasFocus = false;
int GUI::_particleLifeTime = 3000;
int GUI::_particleSize = 1;
float GUI::_particleStartColor[4] = {1,.596,0, 1};
float GUI::_particleEndColor[4] = {1,0,0, 1};