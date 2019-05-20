#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <SDL.h>
#include <string>

#include "Text.h"

namespace Domigon
{
	/* Represents the message boxes that appears when the player does something forbidden in the block editor menu */
	class MessageBox {
	public:
		// Constructor
		MessageBox(SDL_Rect dstrect, std::string text);

		// Destructor
		~MessageBox();

		/*****************************************************************************************************************************/

		// Display the message box
		void display();

		// Fade out the message box. Returns false if the message box has become invisible
		bool fadeOut();

		// Erase the message box from the window
		void erase();

		/*****************************************************************************************************************************/

	private:

		// Destination rectangle of the message box in the window
		const SDL_Rect m_dstrect;

		// Source rectangle of the background
		SDL_Rect m_srcrect;

		// Rectangle alpha
		Uint8 m_alpha;

		// Message font
		TTF_Font *m_font;

		// Message rect
		SDL_Rect m_msgrect;

		// Message
		const std::string m_msg;
	};
}

#endif

