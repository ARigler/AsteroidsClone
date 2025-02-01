#pragma once
#ifndef RSOS_ACTOR
#define RSOS_ACTOR
#include"Structures.h"
#include"Component.h"
#include"Math.h"
#include<vector>
enum class ActorType {
	Actor,
	Ship,
	Asteroid,
	Laser,
	WarpZone
};

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
	void set_aType(ActorType argType) { aType = argType; }
	void set_pos(const Vector2& posArg) { mPos = posArg; }
	void set_rot(float rot) { mRotation = rot; }
	void set_sca(float sca) { mScale = sca; }
	Vector2 getPos() const { return mPos; }
	float getRot() const { return mRotation; }
	float getSca() const { return mScale; }
	Vector2 getForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }
	std::vector<class Component*> getComponents() const {
		return mComponents;
	}
	void addComponent(class Component* component) { mComponents.push_back(component); }
	EngineState get_state() {
		return eState;
	}
	virtual void updateActor(float deltaTime);
	void processInput(const uint8_t* keyState);
	virtual void actorInput(const uint8_t* keyState);
	ActorType get_aType()const { return aType; }
private:
	EngineState eState;
	Vector2 mPos;
	float mScale;
	float mRotation;
	std::vector<Component*> mComponents;
	ActorType aType;
	class Game* mGame;
};

class Ship : public Actor {
public:
	Ship(class Game* game, Vector2 pos = Vector2::Zero, float scale = 1.0f, float rot = 0.0f);
	void updateActor(float deltatime) override;
	void actorInput(const uint8_t* keyState) override;
private:
	float mLaserCooldown;
	
};

class Asteroid : public Actor {
public:
	Asteroid(class Game* game);
	~Asteroid();

	class CircleComponent* GetCircle() { return mCircle; }
private:
	class CircleComponent* mCircle;
};

class Laser : public Actor
{
public:
	Laser(class Game* game);

	void updateActor(float deltaTime) override;
private:
	CircleComponent* mCircle;
	float mDeathTimer;
};

class WarpZone :public Actor {
public:
	WarpZone(class Game* game);

};
#endif