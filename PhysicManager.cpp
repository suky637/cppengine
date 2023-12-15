#include "PhysicManager.h"
#include <iostream>
bool PhysicManager::isPointCollidesWithBox(BoxCollider& col, glm::vec3 point)
{
    if (
        point.x >= col.begin.x &&
        point.x <= col.begin.x + col.size.x &&
        point.y >= col.begin.y &&
        point.y <= col.begin.y + col.size.y &&
        point.z >= col.begin.z &&
        point.z <= col.begin.z + col.size.z
    )
    {
        //std::cout << "COLL" << std::endl;
        lastCollision = &col;
        return true;
    }

    return false;
}

bool PhysicManager::isPointCollides(glm::vec3 point)
{
    for (auto& col : colliders)
    {
        if (this->isPointCollidesWithBox(col, point))
            return true;
    }

    return false;
}

void PhysicManager::addCollider(BoxCollider& col)
{
    colliders.push_back(col);
}

BoxCollider* PhysicManager::getLastCollision()
{
    return lastCollision;
}

bool PhysicManager::Raycast(int steps, float densityOfRays, glm::vec3 orientation, glm::vec3 position)
{
    glm::vec3 ori = orientation * densityOfRays;
    glm::vec3 cur = position + ori;
    for (int i = 1; i < steps; ++i)
    {
        cur += ori;
        if (isPointCollides(cur))
        {
            return true;
        }
    }

    return false;
}
