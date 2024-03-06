#pragma once

#ifndef _AUDIO_SYSTEM_H_
#define _AUDIO_SYSTEM_H_

class SoundAsset;
class MusicAsset;

class AudioSystem {
	/**
	 * @brief Sample Rate to use.
	 * 
	 */
	int sampleRate = 44100;
	DECLARE_SINGLETON(AudioSystem);
	friend class Engine;
protected:

	void Initialize();

	void Destroy();
	
	/**
	 * @brief Load the AudioSystem's settings.
	 * 
	 * @param config_file File to get configuration values from.
	 */
	void Load(const std::string& config_file);
public:
	/**
	 * @brief Set the currently playing Music.
	 * 
	 * @param music MusicAsset to play.
	 */
	void SetMusic(MusicAsset* music);

	/**
	 * @brief Play a SoundAsset.
	 * 
	 * @param sound SoundAsset to play.
	 */
	void PlayFX(SoundAsset* sound);
};

#endif