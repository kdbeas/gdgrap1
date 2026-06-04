#include "P6Particle.h"

namespace P6 {
	void P6Particle::UpdatePosition(float deltaTime)
	{
		this->Position = this->Position + this->Velocity * deltaTime + (1.0f/2.0f) * (this->Acceleration * deltaTime * deltaTime);
	}

	void P6Particle::UpdateVelocity(float deltaTime)
	{
		this->Velocity = this->Velocity + this->Acceleration * deltaTime;
	}

	void P6Particle::Update(float time)
	{
		this->UpdatePosition(time);
		this->UpdateVelocity(time);
	}

	P6Particle::P6Particle()
	{
		this->Position = glm::vec3(0, 0, 0);
		this->Velocity = glm::vec3(0, 0, 0);
		this->Acceleration = glm::vec3(0, 0, 0);
	}
}