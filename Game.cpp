#include "Engine.h"
#include"Game.h"
#include"Timer.h"
#include"TextureManager.h"
#include"Component.h"
#include<iostream>
#include<string>
#include<algorithm>
#include<SDL.h>
#include<SDL_mixer.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<map>
#include<functional>


Game::~Game() {
	close();
}


void Game::add_actor(Actor* actor) {
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

void Game::remove_actor(Actor* actor) {
	auto actorToRemove = std::remove(mActors.begin(), mActors.end(), actor);
	mActors.erase(actorToRemove, mActors.end());
}

bool Game::loadMedia() {
	bool success = true;

	gFont = TTF_OpenFont("assets/lucon.ttf", 28);
	if (gFont == NULL) {
		SDL_Log("Failed to load lucida console font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	//TextureManager::getInstance()->loadFromFile("assets/image.png",Engine::getInstance()->pass_renderer(),0xFF,{0xFF,0xFF,0xFF},SDL_BLENDMODE_BLEND,{0,0xFF,0xFF})

	return success;
}

bool Game::init() {
	bool success = true;
	quit = false;

	if (!loadMedia()) {
		SDL_Log("Loading assets failed!");
		success = false;
	}

	mActors = std::vector<Actor*>();
	mPendingActors = std::vector<Actor*>();
	mRenderLookupTable = std::multimap<int, SpriteComponent*>();

	return success;
}

void Game::close() {
	for (auto actor : mActors) {
		delete actor;
	}
	mActors.clear();

	gFont = NULL;

}

void Game::processInput() {
	SDL_Event e;
	//handle/poll events on queue
	while (SDL_PollEvent(&e) != 0) {
		//user requests quit
		if (e.type == SDL_QUIT) {
			quit = true;
		}
		else if (e.type == SDL_KEYDOWN) {
			
		}
	}
}

void Game::update(float deltaTime) {
	mUpdatingActors = true;
	//populate update functions in a multimap based on update order
	for (Actor* actor : mActors) {
		for (Component* component : actor->getComponents()) {
			mUpdateLookupTable.insert(std::make_pair(component->get_uO(), std::bind(&Component::update, component, deltaTime)));
		}
	}

	//execute update functions based on update order (ascending)
	for (auto element : mUpdateLookupTable) {
		element.second(deltaTime);
	}

	mUpdateLookupTable.clear();
	mUpdatingActors = false;
	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->get_state() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors) {
		delete actor;
	}
	deadActors.clear();
}

void Game::render() {
	//Clear screen
	SDL_Renderer* gRenderer = Engine::getInstance()->pass_renderer();
	SDL_SetRenderDrawColor(gRenderer, 0x33, 0x33, 0x33, 0xFF);
	SDL_RenderClear(gRenderer);

	TextureManager* texMan = TextureManager::getInstance();
	//painter's algorithm
	//iterate through render lookup table and draw
	double angle;
	int x, y, w, h;
	for (Actor* actor : mActors) {
		auto components = actor->getComponents();
		for (Component* component : components) {
			SpriteComponent* spriteComponent = dynamic_cast<SpriteComponent*>(component);
			if (spriteComponent != nullptr) {
				int drawOrder = spriteComponent->getDrawOrder();
				mRenderLookupTable.insert(std::make_pair(drawOrder, spriteComponent));
			}
			spriteComponent = nullptr;
		}
				//if there's an animation metadata struct that points to this index, use the animation metadata and play animations instead of a static image
	}
	for (auto element : mRenderLookupTable) {
		angle = 180 * (element.second->passOwner()->getRot()) / 3.14159;
		//render static image
		element.second->Draw(texMan, angle);
	}

	mRenderLookupTable.clear();

	//Update screen
	SDL_RenderPresent(gRenderer);
	gRenderer = nullptr;
}

void Game::runFrame(float deltaTime) {
	processInput();
	update(deltaTime);
	//collision subroutine
	render();
}

void Game::runGame() {
	//run the first frame with a dummy timestep so can calculate deltaTime
	RSOS_Perf_Timer frameTimer;
	float timeStep = (1.f/FRAME_CAP) - (1.f / FRAME_CAP);
	frameTimer.start();
	float deltaTime = timeStep;
	
	while (!quit) {
		if (timeStep >= 0.0f) {
			runFrame(deltaTime);
		}
		Uint64 frameTicks = frameTimer.getTicks();
		frameTimer.reset();
		deltaTime = frameTicks / 1000.0f;
		timeStep = deltaTime - (1.f / FRAME_CAP);
	}
}
