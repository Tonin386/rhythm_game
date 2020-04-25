#include "keyentity.hpp"

KeyEntity::KeyEntity(char key, double pressTime, double releaseTime) : x_key(key), x_pressTime(pressTime), x_releaseTime(releaseTime) {}

void KeyEntity::setPressTime(double t)
{
	x_pressTime = t;
}

void KeyEntity::setReleaseTime(double t)
{
	x_releaseTime = t;
}

double KeyEntity::getPressTime() const
{
	return x_pressTime;
}

double KeyEntity::getReleaseTime() const
{
	return x_releaseTime;
}

KeyEntity::~KeyEntity() {}