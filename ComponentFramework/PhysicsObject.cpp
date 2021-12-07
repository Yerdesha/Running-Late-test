#include "PhysicsObject.h"

PhysicsObject::PhysicsObject():pos(0.0f),vel(0.0f),accel(0.0f),boundingSphere(0.0f), drag(0.0f), weight(0.0f), buoyancy(0.0f), ld(0.0f), dragc(0.0f) {
	/// Another way to initialize within the constructor
	angle = angularVel = angularAccel = 0.0f;
	rotationalI = 1.0f;
	length = 1.0f;
	r = 1.0f;
	mass = 1.0f;
	//physics assignment 3 variables
	gravityacc = Vec3(0, -9.8, 0);
	waterHeight = 1.0f;
}

PhysicsObject::PhysicsObject(float mass_, float radius_, float length_, float drag_)
{
	mass = mass_;
	r = radius_;
	length = length_;
	drag = Vec3(0.0f, 0.0f, 0.0f);
	angle = angularVel = angularAccel = 0.0f;
	rotationalI = 1.0f;
	gravityacc = Vec3(0, -9.8, 0);
	waterHeight = 10.0f;
	dragc = drag_;
	pos = vel = accel = Vec3(0.0f , 0.0f, 0.0f);
	ld = 997.0f;
}


PhysicsObject::~PhysicsObject(){

}

