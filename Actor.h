#pragma once
#ifndef RSOS_ACTOR
#define RSOS_ACTOR
#include"Structures.h"
#include"Component.h"
#include<vector>
class Actor {
public:
	enum EngineState {
		EActive,
		EPaused,
		EDead,
	};
	Actor(class Game* game, Vector2 pos ={0.f,0.f},float scale=1.0f,float rot=0.0f);
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
	std::vector<Component*> getComponents() const {
		return mComponents;
	}
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


#endif