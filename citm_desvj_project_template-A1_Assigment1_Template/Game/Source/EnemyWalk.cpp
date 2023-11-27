#include "EnemyWalk.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include <map>
#include <chrono>
#include <thread>

EnemyWalk::EnemyWalk() : Entity(EntityType::ENEMYWALK)
{
	name.Create("EnemyWalk");
}

EnemyWalk::~EnemyWalk()
{
}

bool EnemyWalk::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	return true;
}

bool EnemyWalk::Start() {

	
	return true;
}

bool EnemyWalk::Update(float dt)
{
	

	return true;
}

bool EnemyWalk::CleanUp()
{
	return true;
}

void EnemyWalk::OnCollision(PhysBody* physA, PhysBody* physB) {

	
	}