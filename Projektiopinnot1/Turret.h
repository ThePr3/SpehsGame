#ifndef TURRET_H
#define TURRET_H

class Object;
class Bullet;

class Turret : public Component
{
public:
	~Turret();
	Turret(Object*, Object*, double, double);
	
	void fire();
	bool update();
	void updateBullets();

private:
	std::vector<Bullet*>::iterator bulletIt;
	double maxAngle;
};

#endif