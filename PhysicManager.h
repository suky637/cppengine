#pragma once
#include <glm/glm.hpp>
#include <vector>
struct BoxCollider
{
	glm::vec3 begin;
	glm::vec3 size;
};

class PhysicManager
{
private:
	BoxCollider* lastCollision;
public:
	std::vector<BoxCollider> colliders{};
	bool isPointCollidesWithBox(BoxCollider& col, glm::vec3 point);
	bool isPointCollides(glm::vec3 point);
	void addCollider(BoxCollider& col);
	BoxCollider* getLastCollision();
	bool Raycast(int steps, float densityOfRays, glm::vec3 orientation, glm::vec3 position);
};