#ifndef MENU_H
#define MENU_H

#include <list>

#include "Button.h"

namespace Domigon
{

	/* Represents the main menu that appears when you start the game. */
	class Menu {
	public:
		// Constructor
		Menu();

		// Destructor
		~Menu();

		// Create the main menu buttons and initialize font
		void createMainMenu();

		// Create the ingame/pause menu buttons and initialize font
		void createPauseMenu();

		// Display menu
		void display();

		/*****************************************************************************************************************************/

		Button* getPressedButton() const;

		/*****************************************************************************************************************************/

	private:
		// Button pressed by the player
		Button *m_pressed_button;

		// Menu buttons
		std::list<Button*> m_buttons;

		// Font
		TTF_Font *m_font;

		// Font when button is hovered
		TTF_Font *m_hovered_font;

		// Handle mouse motion and mouse clicks events
		void handleMouseEvents();
	};
}

#endif

