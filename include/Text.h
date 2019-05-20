#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>

namespace Domigon
{
	/* This is a static class allowing to simply draw text into the game window. */
	class Text {
	public:
		// Draw simple text
		static void displayText(const SDL_Rect &rect, TTF_Font &font, const SDL_Color &font_color, const char *text);

		// Draw text with a background
		static void displayText(const SDL_Rect &rect, TTF_Font &font, const SDL_Color &font_color, const char *text, const SDL_Color &bg_color);

		// Draw transparent text
		static void displayText(const SDL_Rect &rect, TTF_Font &font, const SDL_Color &font_color, const char *text, Uint8 alpha);
	};
}

#endif

