#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <map>

#include "GameManager.h"

namespace Domigon
{
	/* Represents the game window. It draws all the components of the game : the grid, blocks, scores, etc. */
	class GameWindow {
	public:
		// Constructor
		GameWindow();

		// Destructor
		~GameWindow();

		/*****************************************************************************************************************************/

		// Renderer of the window
		static SDL_Renderer *renderer;

		// Map that associates each block type with a SDL_Color
		static std::map<int, SDL_Color> blocktype_color;

		// Game's background texture
		static SDL_Texture *background;

		// Button texture
		static SDL_Texture *button;

		// Big panel texture
		static SDL_Texture *big_panel;

		// Small panel texture
		static SDL_Texture *small_panel;

		// Frame texture
		static SDL_Texture *frame;

		// Bubble message texture
		static SDL_Texture *error_panel;

		/*****************************************************************************************************************************/

		// Create the window
		bool create();

		// Main drawing function
		void draw();

		// Draw grid and blocks
		void drawGrid();

		// Draw following blocks
		void drawNextBlocks();

		// Draw hold block
		void drawHoldBlock();

		// Draw score
		void drawScore();

		// Draw level
		void drawLevel();

		// Draw elements that will not be refreshed
		void firstDraw();

		/*****************************************************************************************************************************/

		// Returns the window
		SDL_Window* getWindow() const;

		// Returns the renderer of the window
		SDL_Renderer* getRenderer() const;

		// Returns the width of the window
		int getWidth() const;

		// Returns the height of the window
		int getHeight() const;


		// Set the game manager
		void setGameManager(GameManager *gamemanager);

		/*****************************************************************************************************************************/

	private:
		// Game window
		SDL_Window *m_window;

		// Game manager
		GameManager *m_gamemanager;

		// Font
		TTF_Font *m_font;

		// Width of the window
		int m_width;

		// Height of the window
		int m_height;

		// Rectangle representing the next blocks panel
		SDL_Rect m_next_rect;	

		// Rectangle representing the hold block panel
		SDL_Rect m_hold_rect;

		// Rectangle representing the score panel
		SDL_Rect m_score_rect;

		// Rectangle representing the level panel
		SDL_Rect m_level_rect;
	};
}

#endif