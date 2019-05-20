#include "ResourceManager.h"

#include <SDL_image.h>

#include <iostream>

namespace Domigon
{
	ResourceManager ResourceManager::m_instance = ResourceManager();

	/*****************************************************************************************************************************/

	ResourceManager::ResourceManager() {}


	ResourceManager::~ResourceManager()
	{
		for (int i = 0; i < SOUNDS_NUMBER; i++)
			SDL_FreeWAV(m_sounds_data[i]);
		SDL_CloseAudioDevice(m_device_id);

		for (int i = 0; i < TEXTURES_NUMBER; i++)
			SDL_DestroyTexture(m_textures[i]);
		
	}

	/*****************************************************************************************************************************/

	void ResourceManager::loadResources()
	{
		SDL_AudioSpec want, have;
		SDL_memset(&want, 0, sizeof(want));
		want.freq = 44100;
		want.format = AUDIO_S16;
		want.channels = 1;
		want.samples = 4096;

		m_device_id = SDL_OpenAudioDevice(nullptr, false, &want, &have, 0);
		SDL_PauseAudioDevice(m_device_id, false);

		// Load audio sounds //
		SDL_LoadWAV("sounds/rollover1.wav", &have, &(m_sounds_data[MOUSEHOVER_BUTTON_SOUND_1]), &(m_sounds_length[MOUSEHOVER_BUTTON_SOUND_1]));
		SDL_LoadWAV("sounds/rollover2.wav", &have, &(m_sounds_data[MOUSEHOVER_BUTTON_SOUND_2]), &(m_sounds_length[MOUSEHOVER_BUTTON_SOUND_2]));
		SDL_LoadWAV("sounds/click1.wav", &have, &(m_sounds_data[MOUSECLICK_BUTTON_SOUND_1]), &(m_sounds_length[MOUSECLICK_BUTTON_SOUND_1]));
		SDL_LoadWAV("sounds/click2.wav", &have, &(m_sounds_data[MOUSECLICK_BUTTON_SOUND_2]), &(m_sounds_length[MOUSECLICK_BUTTON_SOUND_2]));
		SDL_LoadWAV("sounds/tetris.wav", &have, &(m_sounds_data[GAME_MUSIC]), &(m_sounds_length[GAME_MUSIC]));

		// Load textures //
		/*
		m_textures[BACKGROUND_TEXTURE] = IMG_LoadTexture(GameWindow::renderer, PathConstants::BACKGROUND_SPRITE_PATH.c_str());
		if (m_textures[BACKGROUND_TEXTURE] == NULL) {
			std::cerr << "IMG_LoadTexture: cannot load background texture" << std::endl;
			std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		m_textures[BUTTON_TEXTURE] = IMG_LoadTexture(GameWindow::renderer, PathConstants::BUTTON_SPRITE_PATH.c_str());
		if (m_textures[BUTTON_TEXTURE] == NULL) {
			std::cerr << "IMG_LoadTexture: cannot load button texture" << std::endl;
			std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		m_textures[PANEL_TEXTURE] = IMG_LoadTexture(GameWindow::renderer, PathConstants::PANEL_SPRITE_PATH.c_str());
		if (m_textures[PANEL_TEXTURE] == NULL) {
			std::cerr << "IMG_LoadTexture: cannot load panel texture" << std::endl;
			std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		m_textures[FRAME_TEXTURE] = IMG_LoadTexture(GameWindow::renderer, PathConstants::TITLE_PANEL_SPRITE_PATH.c_str());
		if (m_textures[FRAME_TEXTURE] == NULL) {
			std::cerr << "IMG_LoadTexture: cannot load frame texture" << std::endl;
			std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		m_textures[ERROR_PANEL_TEXTURE] = IMG_LoadTexture(GameWindow::renderer, PathConstants::MSG_SPRITE_PATH.c_str());
		if (m_textures[ERROR_PANEL_TEXTURE] == NULL) {
			std::cerr << "IMG_LoadTexture: cannot load message panel texture" << std::endl;
			std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}
		*/
	}

	/*****************************************************************************************************************************/

	void ResourceManager::playSound(int id)
	{
		SDL_QueueAudio(m_device_id, m_sounds_data[id], m_sounds_length[id]);
	}

	/*****************************************************************************************************************************/

	SDL_Texture *ResourceManager::getTexture(int id)
	{
		return m_textures[id];
	}

	/*****************************************************************************************************************************/

	ResourceManager& ResourceManager::Instance()
	{
		return m_instance;
	}
}