#ifndef KEYENTITY_HPP
#define KEYENTITY_HPP

#include <SFML/Graphics.hpp>

class KeyEntity {
public:
	KeyEntity(char key, double pressTime = -1, double releaseTime = -1);
	virtual void setPressTime(double t);
	virtual void setReleaseTime(double t);
	virtual double getPressTime() const;
	virtual double getReleaseTime() const;
	virtual ~KeyEntity();

protected:
private:
	double x_pressTime;
	double x_releaseTime;
	char x_key;

};

#endif