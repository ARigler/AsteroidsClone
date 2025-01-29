#pragma once
#ifndef RSOS_COMPONENT
#define RSOS_COMPONENT
#include"Structures.h"
#include"TextureManager.h"
#include<vector>
#include<SDL.h>
#include<map>

class Component {
public:
	Component(class Actor* owner, int uO = 100);
	virtual ~Component();
	virtual void update(float deltaTime);
	class Actor* passOwner() const { return mOwner; }
	int get_uO() const { return mUpdateOrder; }
protected:
	class Actor* mOwner;
	int mUpdateOrder;
};

class SpriteComponent : public Component {
public:
	SpriteComponent(class Actor* owner, int texIndex,int drawOrder = 100, int uO=100);
	virtual void Draw(TextureManager* textureManager,double angle,SDL_RendererFlip renderFlip = SDL_FLIP_NONE);
	virtual void SetTexture(TextureManager* textureManager,int index);
	int getTexIndex() const { return texIndex; }
	int getDrawOrder() const { return mDrawOrder; }
	void setDrawOrder(TextureManager* textureManager,int drawOrder);
	int getWidth() const { return TextureManager::getInstance()->fetchData(texIndex).width; }
	int getHeight() const { return TextureManager::getInstance()->fetchData(texIndex).height; }
protected:
	int texIndex;
	int mDrawOrder;
};

class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int texIndex,int cWidth, int cHeight, int fps,bool loop=false, int drawOrder = 100, int uO = 100);
	void update(float deltaTime) override;
	void setTexIndex(int texIndex);
	float getAnimFPS()const { return fps; }
	void setAnimFPS(int afps) { fps = afps; }
	Point getCellDimensions()const { return { cellWidth,cellHeight }; }
	void setCellDimensions(int w, int h) { cellWidth = w; cellHeight = h; }
	void addSequence(int length);
	void removeSequence(int sequence);
	void resizeSequence(int sequence, int length);
private:
	int resourceIndex;
	int fps;
	int cellWidth;
	int cellHeight;
	std::map<int, int> sequences;
	bool mloop;
	Point currentCell;
};

class BGSpriteComponent : public SpriteComponent {
public:
	BGSpriteComponent(class Actor* owner, int texIndex, int drawOrder = 100, int uO = 100);
	void update(float deltaTime) override;
	void setTexIndex(int texIndex);
	void setScreenSize(const Vector2& size) { mScreenSize = size; }
	void setScrollSpeed(float speed) { mScrollSpeed = speed; }
	int getTexIndex()const { return texIndex; }
	float getScrollSpeed()const { return mScrollSpeed; }
private:
	int texIndex;
	Vector2 mOffset;
	Vector2 mScreenSize;
	float mScrollSpeed;
};
#endif