#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <memory>

#include <SDL.h>

#include "constants.h"

namespace Domigon
{
	enum CellType {
		EMPTY = -2,					// Empty cell (in the grid)
		CURRENT_BLOCK = -1			// Cell filled by the block currently controlled by the player
	};

	/* Represents a block in the grid. It can be moved to the left, right and bottom and rotated. */
	class Block {
	public:
		// Default constructor
		Block();

		// Constructor
		Block(int type, int width_square, int height_square, std::vector<bool> data);

		// Destructor
		~Block();

		/*****************************************************************************************************************************/

		// Set the data row and data column of this block
		void init();

		/*****************************************************************************************************************************/

		// All types of blocks existing
		static std::vector<std::shared_ptr<Block>> alltypesblocks;

		/*****************************************************************************************************************************/

		// "=" operator definition
		void operator=(const Block& other);

		/*****************************************************************************************************************************/

		// Decrements the column of the block in the grid
		void moveLeft();

		// Increments the column of the block in the grid
		void moveRight();

		// Increments the row of the block in the grid
		void moveDown();

		// Return the row of this block
		int getRow() const;

		// Return the column of this block
		int getColumn() const;

		// Return the row of the m_data matrix
		int getDataRow() const;

		// Return the column of the m_data_matrix
		int getDataColumn() const;

		// Return the id of this block
		int getType() const;

		// Return the width of this block (in number of squares)
		int getWidth() const;

		// Return the height of this block (in number of squares)
		int getHeight() const;

		// Return the array representing the current data of this block
		std::vector<bool> getData() const;

		// Set the row of this block
		void setRow(int row);

		// Set the column of this block
		void setColumn(int column);

		// Set the row of this block (in number of squares)
		void setWidth(int width_square);

		// Set the column of this block (in number of squares)
		void setHeight(int height_square);

		// Set the current data of this block
		void setData(std::vector<bool> data);

		/*****************************************************************************************************************************/

	private:

		// Row of this block in the grid
		int m_row;

		// Column of this block in the grid
		int m_column;

		// Row of the m_data matrix upperleft corner
		int m_data_row;

		// Column of the m_data matrix upperleft corner
		int m_data_column;

		// Type of this block
		int m_type;

		// Width of this block (in number of squares)
		int m_width;

		// Height of this block (in number of squares)
		int m_height;

		// Current block data
		std::vector<bool> m_data;
	};
}

#endif