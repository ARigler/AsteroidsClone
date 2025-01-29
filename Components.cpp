#include"Component.h"
#include"TextureManager.h"
#include"Engine.h"

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
	textureManager->render(Engine::getInstance()->pass_renderer(), texIndex, getWidth()*mOwner->getSca(), getHeight()*mOwner->getSca(), mTextureMetadata.alpha, mTextureMetadata.colorMod, mTextureMetadata.blendMode);
}

void SpriteComponent::SetTexture(TextureManager* textureManager, int index) {
	texIndex = index;
}

void SpriteComponent::setDrawOrder(TextureManager* textureManager, int drawOrder) {

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
	mloop = loop;
	sequences = std::map<int, int>();
	currentCell = { 0,0 };
	fps = fpsA;
}
