#include"Actor.h"
#include"Game.h"
#include"Random.h"
#include<vector>

Actor::Actor(class Game* game,Vector2 pos, float scale, float rot) {
	set_state(EActive);
	mComponents = std::vector<class Component*>();
	set_sca(scale);
	set_rot(rot);
	set_pos(pos.x,pos.y);
	game->add_actor(this);
}

Actor::~Actor() {
	for (Component* component : mComponents) {
		delete component;
	}
	mComponents.clear();
}

void Actor::update(float deltaTime) {
	updateActor(deltaTime);
}

void Actor::updateActor(float deltaTime) {

}

Ship::Ship(class Game* game, Vector2 pos, float scale, float rot) :Actor(game, pos, scale, rot) {

}

void Ship::updateActor(float deltaTime) {

}

void Ship::processKeyboard(const uint8_t* state) {

}

Asteroid::Asteroid(class Game* game) :Actor(game) {
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
	set_pos(randPos.x, randPos.y);
	set_rot(Random::GetFloatRange(0.0f, Math::TwoPi));
	addComponent(new SpriteComponent(this, 2));
	MoveComponent* mc = new MoveComponent(this);
	addComponent(mc);
	mc->setForwardSpeed(150.0f);
}