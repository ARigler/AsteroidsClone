#include"Actor.h"
#include"Game.h"
#include"Random.h"
#include<vector>

Actor::Actor(class Game* game,Vector2 pos, float scale, float rot) {
	set_state(EActive);
	mComponents = std::vector<class Component*>();
	set_sca(scale);
	set_rot(rot);
	set_pos(pos);
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

void Actor::processInput(const uint8_t* keyState) {
	if (eState == EActive) {
		for (auto comp : mComponents) {
			comp->processInput(keyState);
		}
		actorInput(keyState);
	}
}

void Actor::actorInput(const uint8_t* keyState)
{
}

Ship::Ship(class Game* game, Vector2 pos, float scale, float rot) :Actor(game, pos, scale, rot) {
	mLaserCooldown = 0.0f;
	// Create a sprite component
	//SpriteComponent* sc = new SpriteComponent(this, 150);
	//sc->SetTexture(game->GetTexture("Assets/Ship.png"));
	AnimSpriteComponent* ac = new AnimSpriteComponent(this, 0, 15, 20, 24, true, { 2,5 }, 150,25);
	ac->addSequence(11);
	ac->set_loop(true);
	ac->setAngleOffset(90.0f);

	// Create an input component and set keys/speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(150.0f);
	ic->SetMaxAngularSpeed(Math::Pi);

	addComponent(ac);
	addComponent(ic);
	set_aType(ActorType::Ship);
}

void Ship::updateActor(float deltaTime) {
	mLaserCooldown -= deltaTime;
}

void Ship::actorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(Game::getInstance());
		laser->set_pos(getPos());
		laser->set_rot(getRot());
		Game::getInstance()->add_actor(laser);

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}


Asteroid::Asteroid(class Game* game) :Actor(game) {
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
	set_pos(randPos);
	set_rot(Random::GetFloatRange(0.0f, Math::TwoPi));
	addComponent(new SpriteComponent(this, 2));
	MoveComponent* mc = new MoveComponent(this);
	addComponent(mc);
	mc->setForwardSpeed(20.0f);
	CircleComponent* mCircle = new CircleComponent(this);
	mCircle->SetRadius(14.0f);
	addComponent(mCircle);
	set_aType(ActorType::Asteroid);
}

Asteroid::~Asteroid() {
}

CircleComponent* Asteroid::GetCircle() {
	CircleComponent* circleComp;
	for (class Component* component : getComponents()) {
		if (component->get_cType() == ComponentType::CircleComponent) {
			circleComp = dynamic_cast <CircleComponent*>(component);
			return circleComp;
		}
	}
}

Laser::Laser(Game* game):Actor(game)
{
	mDeathTimer = 1.0f;
	set_sca(0.2f);
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this,4);

	// Create a move component, and set a forward speed
	MoveComponent* mc = new MoveComponent(this);
	mc->setForwardSpeed(200.0f);

	// Create a circle component (for collision)
	CircleComponent* mCircle = new CircleComponent(this);
	mCircle->SetRadius(1.0f);

	addComponent(sc);
	addComponent(mc);
	addComponent(mCircle);
	set_aType(ActorType::Laser);
}

Laser::~Laser() {
}

void Laser::updateActor(float deltaTime)
{
	// If we run out of time, laser is dead
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.0f)
	{
		set_state(EDead);
	}
	else
	{
		// Do we intersect with an asteroid?
		std::vector<class Actor*> actors = Game::getInstance()->getActors();
		for (auto ast : actors)
		{
			if (ast->get_state()!=Actor::EDead && ast->get_aType() == ActorType::Asteroid) {
				Asteroid* aster = dynamic_cast<Asteroid*>(ast);
				if (aster && GetCircle() && aster->GetCircle() && Intersect(*GetCircle(), *(aster->GetCircle())))
				{
					//The first asteroid we intersect with,
					// set ourselves and the asteroid to dead
					set_state(EDead);
					aster->set_state(EDead);
					break;
				}
			}
		}
	}
}

CircleComponent* Laser::GetCircle() {
	CircleComponent* circleComp;
	for (class Component* component : getComponents()) {
		if (component->get_cType() == ComponentType::CircleComponent) {
			circleComp = dynamic_cast <CircleComponent*>(component);
			return circleComp;
		}
	}
}

WarpZone::WarpZone(Game* game) : Actor(game) {
	Engine* engine = Engine::getInstance();
	WarpComponent* wc = new WarpComponent(this, 10,0,0,engine->SCREEN_WIDTH,engine->SCREEN_HEIGHT);
	addComponent(wc);
	set_aType(ActorType::WarpZone);
}