#pragma once

#ifndef  _ASSET_H_
#define _ASSET_H_

#include "Object.h"

/**
 * @class Asset
 *  
 * @brief An Asset is a game resource that is requested and managed through the AssetManager.
 */
class Asset : public Object
{
protected:
	/**
	 * @brief Asset resource file, relative to the current directory.
	 * 
	 */
	std::string filepath;

	DECLARE_ABSTRACT_DERIVED_CLASS(Asset, Object)
	Asset() = default;
	~Asset() override = default;
	void Initialize() override;
	void Load(json::JSON&) override;

	/**
	 * @brief Set the filepath for the Asset.
	 * 
	 * @param path The Asset's file path.
	 */
	void SetFilepath(std::string path);

	friend class AssetManager;
};

#endif // !_ASSET_H_
