#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SDL.h>

#include "GameWindow.h"

namespace Domigon
{
	const int SOUNDS_NUMBER = 5;
	const int TEXTURES_NUMBER = 5;

	enum Sound {
		MOUSEHOVER_BUTTON_SOUND_1
		, MOUSEHOVER_BUTTON_SOUND_2
		, MOUSECLICK_BUTTON_SOUND_1
		, MOUSECLICK_BUTTON_SOUND_2
		, GAME_MUSIC
	};

	enum Texture {
		BACKGROUND_TEXTURE
		, BUTTON_TEXTURE
		, PANEL_TEXTURE
		, FRAME_TEXTURE
		, ERROR_PANEL_TEXTURE
	};

	/* This class manages all the sounds and textures that are in the game and allow to retrieve them. */
	class ResourceManager
	{
	public:
		static ResourceManager& Instance();

		// Unique instance of ResourceManager
		static ResourceManager m_instance;

		// Load resources
		void loadResources();

		// Play the sound represented by the identifier id
		void playSound(int id);

		// Returns the texture represented by the identifier id
		SDL_Texture *getTexture(int id);

		/*****************************************************************************************************************************/

	private:
		// Constructor
		ResourceManager();

		// Destructor
		~ResourceManager();

		// Audio device identifier
		SDL_AudioDeviceID m_device_id;

		// Array containing data of every sound
		Uint8 *m_sounds_data[SOUNDS_NUMBER];

		// Array containing length of every sound
		Uint32 m_sounds_length[SOUNDS_NUMBER];

		// Array containing every textures
		SDL_Texture *m_textures[TEXTURES_NUMBER];
	};
}

#endif

