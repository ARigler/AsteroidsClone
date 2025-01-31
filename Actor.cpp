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
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	addComponent(std::move(ac));
	addComponent(std::move(ic));
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
		laser->set_pos(getPos());

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
	mc->setForwardSpeed(150.0f);
	mCircle = new CircleComponent(this);
	addComponent(mCircle);
}

Asteroid::~Asteroid() {
	Game::getInstance()->remove_actor(this);
}

Laser::Laser(Game* game):Actor(game)
{
	mDeathTimer = 1.0f;
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this,4);

	// Create a move component, and set a forward speed
	MoveComponent* mc = new MoveComponent(this);
	mc->setForwardSpeed(800.0f);

	// Create a circle component (for collision)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.0f);

	addComponent(std::move(sc));
	addComponent(std::move(mc));
	addComponent(mCircle);

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
		for (auto ast : Game::getInstance()->getActors())
		{
			if (ast->get_aType() == ActorType::Asteroid) {
				Asteroid* aster = dynamic_cast<Asteroid*>(ast);
				if (Intersect(*mCircle, *(aster->GetCircle())))
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