#pragma once
#include "CoreDataTypes.h"

#include <map>

class TextureManager
{
	friend class ShaderManager;

	TextureManager() = default;
	~TextureManager();

public:

	void AddTexture(Texture* toAdd, const UserTexturesNames name);
	void AddTexture(Texture* toAdd, const GeneratedTexturesNames name);
	const Texture* GetTexture(const UserTexturesNames name) const;
	Texture* GetTexture(const GeneratedTexturesNames name) const;
	const std::map<UserTexturesNames, Texture*>* GetUserTextures();
	const std::map<GeneratedTexturesNames, Texture*>* GetGeneratedTextures();

protected:

	std::map<UserTexturesNames, Texture*> _userTexturesMap;
	std::map<GeneratedTexturesNames, Texture*> _generatedTexturesMap;
};
