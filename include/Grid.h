#ifndef GRID_H
#define GRID_H

#include <list>
#include <array>

#include "Block.h"

namespace Domigon
{
	/* Represents the grid of the game. It can add, erases, moves and rotates all the blocks in the grid. It checks the collisions */
	/* and if lines have been completed */
	class Grid
	{
	public:
		// Constructor
		Grid();

		// Destructor
		~Grid();

		/*****************************************************************************************************************************/

		// Fill the grid with empty type blocks
		void init();

		// Add a new block to the grid. Returns false if the block can't be placed (means game over)
		bool addBlock(Block &block);

		// Update a block already in the grid
		void updateBlock(Block &block);

		// Place a block in his current position in the grid
		void placeBlock(Block &block);

		// Erase a block from the grid
		void eraseBlock(Block &block);

		// Indicate whether a block will collide if it moves into a direction
		bool blockCollide(Block &block, SDL_Keycode direction) const;

		// Check if a block can move and move it if so
		bool moveBlock(Block &block, SDL_Keycode direction);
		
		// Check if the block can be rotated. If yes, returns true and rotate it, otherwise returns false
		bool rotateBlock(Block &block);

		// Return the number of completed lines
		int completedLinesNumber(Block &block);

		// Delete completed lines
		void deleteCompletedLines();

		// Display the grid in text format
		void displayTxtGrid() const;

		/*****************************************************************************************************************************/

		// Return an array representing the grid
		std::array<int, GridConstants::WIDTH_CELLS * GridConstants::HEIGHT_CELLS> getGrid() const;

		/*****************************************************************************************************************************/

	private:

		// Completed lines of the grid
		std::list<int> m_completed_lines;

		// Indicate how the squares of the grid are filled (0 = empty)
		std::array<int, GridConstants::WIDTH_CELLS * GridConstants::HEIGHT_CELLS> m_grid;
	};
}

#endif

