#ifndef __j1ENEMY_H__
#define __j1ENEMY_H__

#include "p2Point.h"
#include "p2Animation.h"
#include "j1Entity.h"
#include "j1Module.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class j1Enemy : public j1Entity
{

public:
	
	p2DynArray<iPoint> pathToPlayer;
	fPoint originalPos;
	fPoint	movementForce = { 80.0f, -420.0f };

	p2SString death;
	float timer;

	bool chase = false;
	bool checkingFall = false;
	bool canJump = false;

	
	int agroDistance = 200;	

	float pathCadency = 0.25f;
	int pathMinDist = 2;
	

public:

	j1Enemy(j1Entity::entityType type);
	j1Enemy(j1Entity::entityType type, fPoint pos);
	// Destructor
	~j1Enemy();

	void Load(pugi::xml_node& file);
	void Save(pugi::xml_node& file) const;

	
};

#endif // __ENEMY_H__