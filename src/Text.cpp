#include <iostream>

#include "Text.h"
#include "constants.h"
#include "GameWindow.h"

namespace Domigon
{
	void Text::displayText(const SDL_Rect &rect, TTF_Font &font, const SDL_Color &font_color, const char *text)
	{
		SDL_Surface *text_surface;
		SDL_Texture *text_texture;

		text_surface = TTF_RenderText_Blended(&font, text, font_color);

		if (text_surface == nullptr) {
			std::cerr << "Font could not be rendered! TTF error: " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		text_texture = SDL_CreateTextureFromSurface(GameWindow::renderer, text_surface);
		SDL_FreeSurface(text_surface);
		if (text_texture == nullptr) {
			std::cerr << "Could not create texture from surface! SDL error: " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		SDL_RenderCopy(GameWindow::renderer, text_texture, NULL, &rect);
		SDL_DestroyTexture(text_texture);
	}

	/*****************************************************************************************************************************/

	void Text::displayText(const SDL_Rect &rect, TTF_Font &font, const SDL_Color &font_color, const char *text
		, const SDL_Color &bg_color)
	{
		SDL_Surface *text_surface;
		SDL_Texture *text_texture;

		text_surface = TTF_RenderText_Shaded(&font, text, font_color, bg_color);

		if (text_surface == nullptr) {
			std::cerr << "Font could not be rendered! TTF error: " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		text_texture = SDL_CreateTextureFromSurface(GameWindow::renderer, text_surface);
		SDL_FreeSurface(text_surface);
		if (text_texture == nullptr) {
			std::cerr << "Could not create texture from surface! SDL error: " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		SDL_RenderCopy(GameWindow::renderer, text_texture, NULL, &rect);
		SDL_DestroyTexture(text_texture);
	}

	/*****************************************************************************************************************************/

	void Text::displayText(const SDL_Rect &rect, TTF_Font &font, const SDL_Color &font_color, const char *text, Uint8 alpha)
	{
		SDL_Surface *text_surface;
		SDL_Texture *text_texture;

		text_surface = TTF_RenderText_Solid(&font, text, font_color);

		if (text_surface == nullptr) {
			std::cerr << "Font could not be rendered! TTF error: " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		text_texture = SDL_CreateTextureFromSurface(GameWindow::renderer, text_surface);
		SDL_FreeSurface(text_surface);
		if (text_texture == nullptr) {
			std::cerr << "Could not create texture from surface! SDL error: " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		SDL_SetTextureAlphaMod(text_texture, alpha);
		SDL_RenderCopy(GameWindow::renderer, text_texture, NULL, &rect);
		SDL_DestroyTexture(text_texture);
	}
}
