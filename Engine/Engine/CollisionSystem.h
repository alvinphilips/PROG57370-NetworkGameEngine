#pragma once

#ifndef _COLLISIONSYSTEM_H
#define _COLLISIONSYSTEM_H

#include "ICollider.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "EngineCore.h"
#include <utility>
#include "SDL.h"
#include<set>
#include "Math.h"
/**
 * @class CollisionSystem
 *
 * The CollisionSystem class is a singleton used by Engine that handles all of the colliders that exist.
 * An Instance has been made to call its functions. To use it, use CollisionSystem::Instance().SomeFunction()
 * To check for collisions, see the BoxCollider or CircleCollider classes. Game Code should NOT have to ever use the CollisionSystem class directly
 */
class CollisionSystem
{
public:

	std::list<ICollider*> colliders;
	//for resolve collision, may need to make colliders a list of pairs of colliders and their last non-colliding position
	std::set<std::pair<ICollider*, ICollider*>> ongoingCollisions;


public:
	static CollisionSystem& Instance()
	{
		if (instance == nullptr)
		{
			instance = new CollisionSystem();
		}
		return *instance;
	}

	void AddCollider(ICollider*);
	void RemoveCollider(ICollider*);


	/**
	* @brief narrow down what might be colliding, no need for square/circle specifics
	*
	* @param a list of pairs of colliders that might be colliding
	*/
	std::list<std::pair<ICollider*,ICollider*>> BroadPhaseDetection();

	/**
	* @brief check if the potential collisions are actually colliding
	*
	* @param a list passed in by BroadPhaseDetection
	*/
	std::set<std::pair<ICollider*, ICollider*>> NarrowPhaseDetection(const std::list<std::pair<ICollider*, ICollider*>>& potentialCollisions);

	/**
	* @brief Resolve Collision
	*	
	* @param two colliders that are colliding
	*/

	void ResolveCollision(ICollider*, ICollider*);


protected:

	void Initialize();
	void Destroy();
	void Update();
private:

	/**
	 * @brief Checks for collision between two circle colliders.
	 *
	 * @param collider1 Pointer to the first circle collider.
	 * @param collider2 Pointer to the second circle collider.
	 * @return True if there is a collision, false otherwise.
	 */
	bool CircleCircleCollision(ICollider*, ICollider*);

	/**
	 * @brief Checks for collision between two box colliders.
	 *
	 * @param collider1 Pointer to the first box collider.
	 * @param collider2 Pointer to the second box collider.
	 * @return True if there is a collision, false otherwise.
	 */
	bool BoxBoxCollision(ICollider*, ICollider*);
	/**
	 * @brief Checks for collision between a circle collider and a box collider.
	 *
	 * @param circleCollider Pointer to the circle collider.
	 * @param boxCollider Pointer to the box collider.
	 * @return True if there is a collision, false otherwise.
	 */
	bool CircleBoxCollision(ICollider*, ICollider*);


	/**
	* @brief check if two colliders are colliding
	*	
	* @param two colliders
	*/
	std::list<std::pair<ICollider*, ICollider*>> enterCollisions;
	std::list<std::pair<ICollider*, ICollider*>> stayCollisions;
	std::list<std::pair<ICollider*, ICollider*>> exitCollisions;


private:
	static CollisionSystem* instance;
	std::list<int> idList;
	int idCount = 0;


	friend class Engine;
	friend class ICollider;
	friend class BoxCollider;
	friend class CircleCollider;

private:
	inline explicit CollisionSystem() = default;
	inline ~CollisionSystem() = default;
	inline explicit CollisionSystem(CollisionSystem const&) = delete;
	inline CollisionSystem& operator=(CollisionSystem const&) = delete;
};

#endif