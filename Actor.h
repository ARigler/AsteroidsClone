#pragma once
#ifndef RSOS_ACTOR
#define RSOS_ACTOR
#include"Structures.h"
#include"Component.h"
#include"Math.h"
#include<vector>
class Actor {
public:
	enum EngineState {
		EActive,
		EPaused,
		EDead,
	};
	Actor(class Game* game, Vector2 pos=Vector2::Zero, float scale = 1.0f, float rot = 0.0f);
	virtual ~Actor();
	void update(float deltaTime);
	void set_state(EngineState stateArg) {
		eState = stateArg;
	}
	void set_pos(float x, float y) { mPos.x = x; mPos.y = y; }
	void set_rot(float rot) { mRotation = rot; }
	void set_sca(float sca) { mScale = sca; }
	Vector2 getPos() const { return mPos; }
	float getRot() const { return mRotation; }
	float getSca() const { return mScale; }
	Vector2 getForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }
	std::vector<Component*> getComponents() const {
		return mComponents;
	}
	void addComponent(class Component* component) { mComponents.push_back(component); }
	EngineState get_state() {
		return eState;
	}
	virtual void updateActor(float deltaTime);
private:
	EngineState eState;
	Vector2 mPos;
	float mScale;
	float mRotation;
	std::vector<class Component*> mComponents;
	class Game* mGame;
};

class Ship : public Actor {
public:
	Ship(class Game* game, Vector2 pos = Vector2::Zero, float scale = 1.0f, float rot = 0.0f);
	void updateActor(float deltatime) override;
	void processKeyboard(const uint8_t* state);;
	
};

class Asteroid : public Actor {
public:
	Asteroid(class Game*);
};
#endif