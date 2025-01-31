#include"Component.h"
#include"TextureManager.h"
#include"Engine.h"
#include"Utilities.h"
#include"Math.h"
#include<cmath>

Component::Component(class Actor* owner, int uO){
	mOwner = owner;
	mUpdateOrder = uO;
}

Component::~Component() {
	mOwner = nullptr;
}

void Component::update(float deltaTime) {

}

void SpriteComponent::Draw(TextureManager* textureManager, double angle,SDL_RendererFlip renderFlip) {
	TextureMetadata mTextureMetadata = textureManager->fetchData(texIndex);
	textureManager->render(Engine::getInstance()->pass_renderer(), texIndex, mOwner->getPos().x, mOwner->getPos().y, mTextureMetadata.alpha, mTextureMetadata.colorMod, mTextureMetadata.blendMode,mOwner->getSca(),angle);
}

void SpriteComponent::SetTexture(TextureManager* textureManager, int index) {
	if (textureManager->fetchTextureListLength() > index && index >= 0) {
		texIndex = index;
	}
}

void SpriteComponent::setDrawOrder(TextureManager* textureManager, int drawOrder) {
	mDrawOrder = drawOrder;
}

SpriteComponent::SpriteComponent(class Actor* owner, int texIndex, int drawOrder, int uO) : Component(owner, uO) {
	mDrawOrder = drawOrder;
	mOwner = owner;
	SetTexture(TextureManager::getInstance(), texIndex);
}

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int texIndex, int cWidth, int cHeight, int fpsA, bool loop, int drawOrder, int uO):SpriteComponent(owner,texIndex,drawOrder,uO)
{
	cellWidth = cWidth;
	cellHeight = cHeight;
	mLoop = loop;
	sequences = std::map<int, int>();
	currentCell = { 0,0 };
	fps = fpsA;
	animationTimer = RSOS_Timer();
	mAnimating = false;

}

void AnimSpriteComponent::update(float deltaTime) {
	if (mAnimating) {
		float timerTick = animationTimer.getTicks();
		float deltaAnimation = (timerTick / 1000.0f) - (1.f / fps);
		if (deltaAnimation > 1.0f) {
			if (mLoop) {
				deltaAnimation = fmod(deltaAnimation,1.0f);
			}
			else {
				deltaAnimation = 1.0f;
			}
		}
		else if (deltaAnimation < 0.0f) {
			deltaAnimation = 0.0f;
		}
		int currentFrame = deltaAnimation * sequences[currentCell.y];
		currentCell.x = currentFrame;
	}
}

void AnimSpriteComponent::animate() {
	if (!mAnimating) {
		if (!animationTimer.isStarted()) {
			animationTimer.start();
		}
		if (animationTimer.isPaused()) {
			animationTimer.unpause();
		}
		mAnimating = true;
	}
	else {
		animationTimer.pause();
		mAnimating = false;
	}
}

void AnimSpriteComponent::resetAnimation() {
	animationTimer.stop();
	mAnimating = false;
}

void AnimSpriteComponent::Draw(TextureManager* textureManager, double angle, SDL_RendererFlip renderFlip) {
	TextureMetadata mTextureMetadata = textureManager->fetchData(texIndex);
	Point adjustedStart = { mOffset.x + (currentCell.x * cellWidth),mOffset.y + (currentCell.y * cellHeight) };
	SDL_Rect clip;
	clip = { mOffset.x,mOffset.y,cellWidth,cellHeight };
	SDL_Rect* clipP = &clip;
	textureManager->render(Engine::getInstance()->pass_renderer(), texIndex, adjustedStart.x, adjustedStart.y, mTextureMetadata.alpha, mTextureMetadata.colorMod, mTextureMetadata.blendMode, mOwner->getSca(), angle, clipP);
	clipP = nullptr;
}

void AnimSpriteComponent::addSequence(int length) {
	int highestKey = 0; 
	int newKey = highestKey;
	if (!sequences.empty()) {
		highestKey = sequences.rbegin()->first;
		newKey = highestKey + 1;
	}
	sequences[newKey] = length;
}

void AnimSpriteComponent::removeSequence(int sequence) {
	//if the sequence is in the map, remove it
	if (sequences.find(sequence)!=sequences.end()) {
		sequences.erase(sequence);
	}
}

void AnimSpriteComponent::resizeSequence(int sequence, int length) {
	if (sequences.find(sequence) != sequences.end()) {
		sequences[sequence]=length;
	}
}

void AnimSpriteComponent::changeActiveSequence(int sequence){
	if (sequences.find(sequence) != sequences.end()) {
		currentCell.y = sequence;
		currentCell.x = 0;
		resetAnimation();
		animate();
	}
}

BGSpriteComponent::BGSpriteComponent(Actor* owner, int texIndex, float clipW, float clipH, float scrollSpeedX, float scrollSpeedY,int drawOrder, int uO) : SpriteComponent(owner,texIndex,drawOrder,uO){
	mOffset = Vector2::Zero;
	mClipSize = Vector2(clipW,clipH);
	mScrollSpeed.x = scrollSpeedX;
	mScrollSpeed.y = scrollSpeedY;
	mScrolling.xFlag = false;
	mScrolling.xFlag = false;
}

void BGSpriteComponent::update(float deltaTime) {
	float width = getWidth();
	float height = getHeight();
	if (mScrolling.xFlag) {
		if (mOffset.x + mClipSize.x <= width || mScrollSpeed.x>0.0f) {
			mOffset.x += (mScrollSpeed.x * deltaTime);
		}
		else if (mOffset.x >= 0.0f || mScrollSpeed.x<0.0f) {
			mOffset.x += (mScrollSpeed.x * deltaTime);
		}
		if (mOffset.x + mClipSize.x > width) {
			if (mLooping.xFlag) {
				mOffset.x = 0;
			}
			else {
				mOffset.x = width - mClipSize.x;
			}
		}
		else if (mOffset.x<0){
			if (mLooping.xFlag) {
				mOffset.x = width - mClipSize.x; 
			}
			else {
				mOffset.x = 0;
			}
		}
	}
	if (mScrolling.yFlag) {
		if (mOffset.y + mClipSize.y <= height || mScrollSpeed.y > 0.0f) {
			mOffset.y += (mScrollSpeed.y * deltaTime);
		}
		else if (mOffset.y >= 0.0f || mScrollSpeed.y < 0.0f) {
			mOffset.y += (mScrollSpeed.y * deltaTime);
		}
		if (mOffset.y + mClipSize.y > height) {
			if (mLooping.yFlag) {
				mOffset.y = 0;
			}
			else {
				mOffset.y = height - mClipSize.y;
			}
		}
		else if (mOffset.y < 0) {
			if (mLooping.yFlag) {
				mOffset.y = width - mClipSize.y;
			}
			else {
				mOffset.y = 0;
			}
		}
	}
}

void BGSpriteComponent::Draw(TextureManager* textureManager, double angle, SDL_RendererFlip renderFlip) {
	TextureMetadata mTextureMetadata = textureManager->fetchData(texIndex);
	SDL_Rect clip;
	clip = { (int)mOffset.x,(int)mOffset.y,(int)mClipSize.x,(int)mClipSize.y };
	SDL_Rect* clipP = &clip;
	textureManager->render(Engine::getInstance()->pass_renderer(), texIndex, mOffset.x, mOffset.y, mTextureMetadata.alpha, mTextureMetadata.colorMod, mTextureMetadata.blendMode, mOwner->getSca(), angle, clipP);
	clipP = nullptr;
}

MoveComponent::MoveComponent(class Actor* owner, int uO) : Component(owner,uO) {

}

void MoveComponent::update(float deltaTime) {
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = passOwner()->getRot();
		rot += mAngularSpeed * deltaTime;
		mOwner->set_rot(rot);
	}
	if (!Math::NearZero(mForwardSpeed))
	{
		Vector2 pos = passOwner()->getPos();
		pos.x += (mOwner->getForward().x)*mForwardSpeed*deltaTime;
		pos.y += (mOwner->getForward().y)*mForwardSpeed * deltaTime;
		mOwner->set_pos(pos.x,pos.y);
	}
}