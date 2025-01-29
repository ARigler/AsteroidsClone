#pragma once
#ifndef RSOS_TEXTUREMANAGER
#define RSOS_TEXTUREMANAGER
#include<vector>
#include<SDL.h>
#include<SDL_ttf.h>
#include<string>
#include<unordered_map>
#include"Structures.h"

struct ColorMod {
	Uint8 red;
	Uint8 green;
	Uint8 blue;
};

struct TextureMetadata {
	int texIndex;
	int width;
	int height;
	ColorMod colorMod;
	SDL_BlendMode blendMode;
	Uint8 alpha;
};

struct TextMetadata {
	int width;
	int height;
	SDL_Color textColor;
};

struct TilesetMetadata {
	int resourceIndex;
	int cellWidth;
	int cellHeight;
};

class TextureManager {
public:
	static TextureManager* getInstance() {
		if (instance == nullptr) {
			instance = new TextureManager();
		}
		return instance;
	}
	TextureManager(const TextureManager&) = delete;
	TextureManager operator=(const TextureManager&) = delete;
	//Set color modulation
	void setColor(int index, Uint8 red, Uint8 green, Uint8 blue);
	//Set Blending
	void setBlendMode(int index, SDL_BlendMode blending);
	//Set Alpha Modulation
	void setAlpha(int index, Uint8 alpha);
	//Renders texture at given point
	void render(SDL_Renderer* renderer, int index, int x, int y, Uint8 alpha = 0xFF, ColorMod color = { 0xFF,0xFF,0xFF }, SDL_BlendMode blending = SDL_BLENDMODE_BLEND, double angle = 0.0, SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void render(SDL_Renderer* renderer, std::string textString, int x, int y, Uint8 alpha = 0xFF, ColorMod color = { 0xFF,0xFF,0xFF }, SDL_BlendMode blending = SDL_BLENDMODE_BLEND, double angle = 0.0, SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void render(SDL_Renderer* renderer, SDL_Texture* texturePointer, int x, int y, Uint8 alpha = 0xFF, ColorMod color = { 0xFF,0xFF,0xFF }, SDL_BlendMode blending = SDL_BLENDMODE_BLEND, double angle = 0.0, SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	bool loadFromFile(std::string path,SDL_Renderer* renderer, Uint8 alpha = 0xFF, ColorMod colorMod = { 0xFF,0xFF,0xFF }, SDL_BlendMode blending = SDL_BLENDMODE_BLEND, ColorMod colorKey={0,0xFF,0xFF});
	bool loadFromRenderedText(std::string textInput, TTF_Font* gFont, SDL_Renderer* renderer, SDL_Color textColor={0xFF,0xFF,0xFF});
	void removeIndex(int index);
	void removeText(std::string textInput);
	void addAnimation(AnimMetadata anim);
	TextureMetadata fetchData(int index);
	TextMetadata fetchTextData(std::string string);
	bool animMetadataExists(int resourceIndex);
	int fetchAnimMetadataListLength()const { return animMetadataList.size(); }
	void setData(int index, TextureMetadata metadata);
	~TextureManager();
private:
	std::vector<SDL_Texture*> textureList;
	std::vector<TextureMetadata> textureMetadataList;
	std::vector<AnimMetadata> animMetadataList;
	std::unordered_map<std::string, SDL_Texture*> textTextureCache;
	std::unordered_map<std::string, TextMetadata> textTextureMetadata;
	TextureManager() {
		textureList = std::vector<SDL_Texture*>();
		textureMetadataList = std::vector<TextureMetadata>();
		textTextureCache = std::unordered_map<std::string, SDL_Texture*>();
		textTextureMetadata = std::unordered_map<std::string, TextMetadata>();
	}
	inline static TextureManager* instance=nullptr;
};

#endif