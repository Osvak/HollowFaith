#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Textures.h"
#include "j1Enemy.h"
#include "j1Particles.h"
#include "j1EnemyFly.h"
#include "j1EnemyLand.h"
#include "j1Bonfire.h"
#include "j1Audio.h"
#include "j1Coins.h"


j1EntityManager::j1EntityManager()
{
	name.create("objects");
}

j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	config_node = config;
	

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	
	return ret;
}

bool j1EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("AllEntities_PreUpdate", Profiler::Color::PaleTurquoise);

	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;

	if (App->dt == 0.0f)
		return true;

	while (tmp != nullptr)
	{
		ret = tmp->data->PreUpdate();
		tmp = tmp->next;
	}

	return ret;
}
bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("AllEntities_Update", Profiler::Color::Turquoise);	

	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;

	if (dt == 0.0f) 
		return true;

	// Prevent FPS failures
	if (dt > 0.15)
		dt = 0.15;

	while (tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("AllEntities_PostUpdate", Profiler::Color::DarkTurquoise);

	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->PostUpdate();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;
		
	p2List_item<j1Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->CleanUp();
		RELEASE(tmp->data);
		Entities.del(tmp);
		tmp = tmp->next;
	}

	App->objects->player = nullptr;
	App->objects->particle = nullptr;		

	for (int i = 0; i <= MAX_BONFIRES; i++) 
	{
		if(App->objects->bonfire[i] != nullptr)
			App->objects->bonfire[i] = nullptr;
	}	

	count = 0;

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& file) const
{
	
	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Save(file);
		tmp = tmp->next;
	}
	
	return ret;
}

bool j1EntityManager::Load(pugi::xml_node& file)
{
	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;

	while (tmp != nullptr)
	{
		if (tmp->data->type == j1Entity::entityType::PLAYER)
		{
			tmp->data->Load(file.child("player"));
		}
		if (tmp->data->type == j1Entity::entityType::BONFIRE)
		{
			tmp->data->Load(file.child("Bonfire"));
		}
		tmp = tmp->next;
	}

	DeleteEnemies();

	p2SString enemyLand="EnemyLand";
	p2SString enemyFly="EnemyFly";	
	p2SString coins = "Coins";

	for (pugi::xml_node iterator = file.child("EnemyLand"); iterator; iterator = iterator.next_sibling("EnemyLand"))
	{		
		if (enemyLand == iterator.name())
		{
			AddEntity(j1Entity::entityType::ENEMY_LAND, { iterator.child("position").attribute("x").as_float(),iterator.child("position").attribute("y").as_float() });
		}
	}

	for (pugi::xml_node iterator = file.child("EnemyFly"); iterator; iterator = iterator.next_sibling("EnemyFly"))
	{
		if (enemyFly == iterator.name())
		{
			AddEntity(j1Entity::entityType::ENEMY_FLY, { iterator.child("position").attribute("x").as_float(),iterator.child("position").attribute("y").as_float() });
		}
	}

	for (pugi::xml_node iterator = file.child("Coins"); iterator; iterator = iterator.next_sibling("Coins"))
	{
		if (coins == iterator.name())
		{
			AddEntity(j1Entity::entityType::COINS, { iterator.child("position").attribute("x").as_float(),iterator.child("position").attribute("y").as_float() });
		}
	}

	return ret;
}


bool j1EntityManager::Draw(float dt)
{

	bool ret = true;
	p2List_item<j1Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Draw(dt);
		tmp = tmp->next;
	}
	return ret;
}


j1Entity* j1EntityManager::AddEntity(j1Entity::entityType type, fPoint position,  bool active)
{
	j1Entity* tmp = nullptr;	

	switch (type)
	{
	case j1Entity::entityType::PLAYER:
		tmp = new j1Player();
		break;

	case j1Entity::entityType::ENEMY_FLY:
		
		tmp = new j1EnemyFly(position);		
		break;

	case j1Entity::entityType::ENEMY_LAND:

		tmp = new j1EnemyLand(position);	
		break;

	case j1Entity::entityType::STONE:
		tmp = new j1Particles();
		break;

	case j1Entity::entityType::COINS:
		tmp = new j1Coins();
		break;

	case j1Entity::entityType::BONFIRE:
		tmp = new j1Bonfire(position, count, active);
		count++;
		break;
	}

	if (tmp) 
	{
		tmp->position.x = position.x;
		tmp->position.y = position.y;


		Entities.add(tmp);
		tmp->Awake(config_node);
		tmp->Start();
	}
	
	return tmp;
}

void j1EntityManager::DeleteEntities()
{
	p2List_item<j1Entity*>* tmp = Entities.end;
	while (tmp != nullptr)
	{
		p2List_item<j1Entity*>* tmp2 = tmp;
		RELEASE(tmp->data);
		Entities.del(tmp2);
		tmp = tmp->prev;
	}	
}

void j1EntityManager::DeleteEntity()
{
	p2List_item<j1Entity*>* tmp = Entities.end;

	while (tmp != nullptr)
	{
		p2List_item<j1Entity*>* tmp2 = tmp;

		if (tmp->data->elim) 
		{
			tmp->data->CleanUp();
			RELEASE(tmp->data);
			Entities.del(tmp2);
		}
		
		tmp = tmp->prev;
	}
	
}

void j1EntityManager::DeleteEnemies()
{
	p2List_item<j1Entity*>* tmp = Entities.end;

	while (tmp != nullptr)
	{
		p2List_item<j1Entity*>* tmp2 = tmp;

		if ((tmp->data->type == j1Entity::entityType::ENEMY_FLY) || (tmp->data->type == j1Entity::entityType::ENEMY_LAND)
			|| (tmp->data->type == j1Entity::entityType::COINS))
		{		
			tmp->data->CleanUp();
			tmp->data->collider->to_delete = true;
			RELEASE(tmp->data);
			Entities.del(tmp);
			tmp = tmp->prev;
		}
		else
			tmp = tmp->prev;		
	}

}