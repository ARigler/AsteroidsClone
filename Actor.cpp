#include"Actor.h"
#include"Game.h"
#include<vector>

Actor::Actor(class Game* game,Vector2 Pos, float scale, float rot) {
	set_state(EActive);
	mComponents = std::vector<class Component*>();
	set_sca(scale);
	set_rot(rot);
	set_pos(Pos.x,Pos.y);
	game->add_actor(this);
}

Actor::~Actor() {
	for (Component* component : mComponents) {
		delete component;
	}
	mComponents.clear();
}

void Actor::update(float deltaTime) {

}

void Actor::updateActor(float deltaTime) {

}