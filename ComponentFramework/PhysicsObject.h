#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H
#include "Vector.h"
#include "Sphere.h"

using namespace MATH;

class PhysicsObject {
	friend class Physics;
protected:
	Vec3 pos, vel, accel;
	Vec3 accel2;
	float mass;
	Sphere boundingSphere;
	float angle, angularVel, angularAccel, rotationalI;
	//Physics assignment 3 variables
	Vec3 buoyancy, drag, weight;
	float dragc, ld, length, r;
	Vec3 gravityacc;
	
public:
	PhysicsObject();
	PhysicsObject(float mass, float radius, float length, float drag);
	float waterHeight;
	virtual ~PhysicsObject();
	

public:
	/// Untility functions so small they cane be inlined
	inline void setPos(const Vec3 &pos_) { pos = pos_; }
	inline Vec3 getPos() { return pos; }
	inline void setVel(const Vec3 &vel_) { vel = vel_; }
	inline Vec3 getVel() { return vel; }
	inline void setAccel(const Vec3 &accel_) { accel = accel_; }
	inline Vec3 getAccel() { return accel; }
	inline void setAngularVel(const float angularVel_) { angularVel = angularVel_; }
	inline void applyForce(const Vec3 force) { 
		accel = accel2;
		accel2 = force / mass;  }
	//inline Vec3 getmass() { return mass; }

};
#endif

