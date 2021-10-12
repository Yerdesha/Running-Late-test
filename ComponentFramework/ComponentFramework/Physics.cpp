#include "Physics.h"
#include <iostream>
#include <cmath>

void Physics::SimpleNewtonMotion(PhysicsObject &object, const float deltaTime) {
	object.pos += object.vel * deltaTime + (0.5f * object.accel2 * deltaTime * deltaTime);
	object.vel += 0.5 * (object.accel + object.accel2) * deltaTime;
}

void Physics::RigidBodyRotation(PhysicsObject& object, const float deltaTime) {
	object.angle += object.angularVel * deltaTime + 0.5f * object.angularAccel * deltaTime * deltaTime;
	object.angularVel += object.angularAccel * deltaTime;
}

void Physics::ApplyForces(PhysicsObject& body, float waterHeight) {
	//part 2
	float feta; //theta
	float VeeVee; //total volume
	//v = pi(r^2)(length)
	feta = 2 * (acos(body.pos.y - waterHeight)/body.r);

	//if the water is a little bit submerged
	if (body.pos.y > waterHeight && body.pos.y - body.r < waterHeight) {
		//v = (0.5) (r^2) ((degree) - sin(degree)) * length
		VeeVee = 0.5f * (body.r * body.r) * (feta - sin(feta)) * body.length;
	}
	//if water is a lot more submerged
	else if (body.pos.y < waterHeight && body.pos.y + body.r > waterHeight) {
		//v = pi(r^2)(length - 0.5) (r^2) ((degree) - sin(degree)) * length
		VeeVee = 3.14f * (body.r * body.r) * body.length - 0.5 * (body.r * body.r) * (feta - sin(feta)) * body.length;
	}
	//if the cylinder is above the water
	else if (body.pos.y > waterHeight + body.r) {
		VeeVee = 0;
	}
	//if neither happens
	else {
		// 0 = 2cos-1(m/r)
		VeeVee = 3.14f * (body.r * body.r) * body.length;
	}


	//part 1

	//F.weight = (mass * gravityacc)
	body.weight = body.mass * body.gravityacc;

	//F.drag = -drag coeddicient * linear velocity
	body.drag = -body.dragc * body.vel;

	//F.buoyancy = liquid density * -gravityacc * totalvolume
	body.buoyancy = body.ld * (-body.gravityacc) * VeeVee;

	Vec3 force1;
	force1 = body.weight + body.drag + body.buoyancy;
	body.applyForce(force1);

	

}

bool Physics::PlaneSphereCollision(const PhysicsObject &object, const Plane &p) {
	return false;
}

void Physics::PlaneSphereCollisionResponse(PhysicsObject &object, const Plane &p) {
}

bool Physics::SphereSphereCollision(const PhysicsObject &object1, const PhysicsObject &object2) {
	return false;
}

void Physics::SphereSphereCollisionResponse(PhysicsObject &object1, PhysicsObject &object2) {
}
