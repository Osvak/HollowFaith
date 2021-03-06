#include "j1EntityManager.h"
#include "j1Entity.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"

j1Entity::j1Entity(entityType type) : type(type)
{

}

j1Entity::~j1Entity()
{
}

void j1Entity::Draw(float dt)
{
	App->render->Blit(texture, position.x, position.y,
		&current_animation->GetCurrentFrame(dt), 1.0, 1.0, flip, NULL, entity_collider.w / 2);
}

void j1Entity::PositionUpdate(float dt) // Entity movement * delta time
{

	// X AXIS POS
	position.x = position.x + speed.x * dt;

	// Y AXIS POS
	position.y = position.y + speed.y * dt;

	if (!godMode)
		speed.y = speed.y + Acceleration * dt;

	CollisionPosUpdate();
}


void j1Entity::CollisionPosUpdate()
{
	collider->SetPos(int(round(position.x)), int(round(position.y)));
}

