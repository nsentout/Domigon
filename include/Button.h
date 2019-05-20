#ifndef BUTTON_H
#define BUTTON_H

#include "Text.h"
#include "Block.h"
#include "GameWindow.h"

namespace Domigon
{
	/* Represents a simple button. It widens when hovered by the mouse and it performs an action when the player clicks on it. */
	class Button
	{
	public:
		// Constructor used to draw simple text that gets bigger when hovered
		Button(std::string text, SDL_Rect rect, TTF_Font *font, TTF_Font *hovered_font);

		// Constructor used to draw block that gets bigger when hovered 
		Button(Block *block, int x, int y);

		// Destructor
		~Button();

		/*****************************************************************************************************************************/

		// Compute rectangles representing the button when hovered and when not hovered
		void init();

		// Handle button hovering. Returns true if a button was updated
		bool handleButtonHovering(SDL_Event e);

		// Draw the hovered button
		void drawEnlarged() const;

		// Draw the non hovered button
		void draw() const;

		/*****************************************************************************************************************************/

		// Returns the button text
		std::string getText() const;

		// Rectangle containing the information about the image when not hovered
		SDL_Rect getRect() const;

		// Rectangle containing the information about the image when hovered
		SDL_Rect getHoveredRect() const;

		// Indicate whether the button is hovered by the mouse
		bool isHovered() const;

		// Returns the block representing the button
		Block *getBlock() const;

		// Set the hovering of the button
		void setHovered(bool is_hovered);


		/*****************************************************************************************************************************/

	private:
		// Rectangle containing the information about the text when not hovered
		SDL_Rect m_text_rect;

		// Rectangle containing the information about the text when hovered
		SDL_Rect m_text_hovered_rect;

		// Rectangle containing the information about the image when not hovered
		SDL_Rect m_img_rect;

		// Rectangle containing the information about the image when hovered
		SDL_Rect m_img_hovered_rect;

		// Rectangle containing the information about the background's area being refreshed
		SDL_Rect m_refresh_rect;

		// Button font
		TTF_Font *m_font;

		// Button font when hovered
		TTF_Font *m_hovered_font;

		// Button text
		std::string m_text;

		// Block representing the button
		Block *m_block;

		// Indicate whether the button is hovered by the mouse
		bool m_hovered;
	};
}

#endif 

