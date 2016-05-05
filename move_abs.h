#ifndef MOVE_ABS
#define MOVE_ABS

#include "delta_cal.h"
#include <cmath>
#include <iostream>
#include "tools.h"
#include "log.h"

typedef unsigned int dig_uni;
typedef double ana_uni;

ana_uni fromDigCodeToRad(dig_uni digCode,dig_uni codeBitNum);

dig_uni fromRadToDigCode(ana_uni rad,dig_uni codeBitNum);

ana_uni* delta_move_abs_path(Delta &delta,ana_uni x,ana_uni y,ana_uni z,
	dig_uni axis1,dig_uni axis2,dig_uni axis3,
	ana_uni maxVel,ana_uni maxAcc,
	dig_uni accPathNum,dig_uni conVelPathNum);

ana_uni* delta_move_abs_circle_path(Delta &delta,ana_uni x2,ana_uni y2,ana_uni z2,
	ana_uni x3,ana_uni y3,ana_uni z3,
	dig_uni axis1,dig_uni axis2,dig_uni axis3,
	dig_uni pathNum);

#endif