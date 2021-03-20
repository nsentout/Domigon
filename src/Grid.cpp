#include <math.h>
#include <iostream>
#include <string>

#include "Grid.h"

namespace Domigon
{
	Grid::Grid() : m_grid()
	{}

	Grid::~Grid() {}

	/*****************************************************************************************************************************/

	void Grid::init()
	{
		m_grid.fill(EMPTY);
	}

	/*****************************************************************************************************************************/

	bool Grid::addBlock(Block &block)
	{
		int data_mat_size = (int)pow(block.getData().size(), 0.5);
		int i;

		for (int r = 0; r < data_mat_size; r++) {
			for (int c = 0; c < data_mat_size; c++) {

				if (block.getData().at(c + data_mat_size * r) == true) {
					i = (block.getDataColumn() + c) + ((block.getDataRow() + r) * GridConstants::WIDTH_CELLS);

					if (m_grid[i] != EMPTY)
						return false;
					m_grid[i] = CURRENT_BLOCK;
				}
			}
		}

		std::cout << "adding block ..." << std::endl;
		return true;
	}

	/*****************************************************************************************************************************/

	void Grid::updateBlock(Block &block)
	{
		int data_mat_size = (int)pow(block.getData().size(), 0.5);
		int i;
		
		for (int r = 0; r < data_mat_size; r++) {
			for (int c = 0; c < data_mat_size; c++) {
				i = (block.getDataColumn() + c) + ((block.getDataRow() + r) * GridConstants::WIDTH_CELLS);

				if (block.getData().at(c + data_mat_size * r) == true)
					m_grid[i] = CURRENT_BLOCK;
				else if (m_grid[i] == CURRENT_BLOCK)
					m_grid[i] = EMPTY;
			}
		}
	}

	/*****************************************************************************************************************************/

	void Grid::placeBlock(Block &block)
	{
		int data_mat_size = (int)pow(block.getData().size(), 0.5);

		for (int r = 0; r < data_mat_size; r++) {
			for (int c = 0; c < data_mat_size; c++) {
				int block_pos = (block.getDataColumn() + c) + GridConstants::WIDTH_CELLS * (block.getDataRow() + r);

				if (block_pos < GridConstants::WIDTH_CELLS * GridConstants::HEIGHT_CELLS)	// If not out of borders
					if (block.getData().at(c + data_mat_size * r) == true)
						m_grid[block_pos] = block.getType();
			}
		}
	}

	/*****************************************************************************************************************************/

	void Grid::eraseBlock(Block &block)
	{
		// Erase old block
		int data_mat_size = (int)pow(block.getData().size(), 0.5);

		for (int r = 0; r < data_mat_size; r++) {
			for (int c = 0; c < data_mat_size; c++) {

				if (block.getData().at(c + data_mat_size * r) == true) {
					m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS] = EMPTY;
				}
			}
		}
	}

	/*****************************************************************************************************************************/

	bool Grid::blockCollide(Block &block, SDL_Keycode direction) const
	{
		int data_mat_size = (int)pow(block.getData().size(), 0.5);

		switch (direction) {

		case SDLK_LEFT:

			// Check collision with the left of the grid //
			if (block.getColumn() == 0)
				return true;

			// Check collisions with other blocks //
			for (int r = 0; r < data_mat_size; r++) {
				for (int c = 0; c < data_mat_size; c++) {

					if (block.getData().at(c + data_mat_size * r) == true
						&& m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS - 1] != CURRENT_BLOCK
						&& m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS - 1] != EMPTY) {

						return true;
					}
				}
			}
			break;

		case SDLK_RIGHT:

			// Check collision with the right of the grid //
			if (block.getColumn() >= GridConstants::WIDTH_CELLS - block.getWidth())
				return true;

			// Check collisions with other blocks //
			for (int r = 0; r < data_mat_size; r++) {
				for (int c = 0; c < data_mat_size; c++) {

					if (block.getData().at(c + data_mat_size * r) == true
						&& m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS + 1] != CURRENT_BLOCK
						&& m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS + 1] != EMPTY) {

						return true;
					}
				}
			}
			break;

		case SDLK_DOWN:

			// Check collision with the bottom of the grid //
			if (block.getRow() >= GridConstants::HEIGHT_CELLS - block.getHeight())
				return true;

			// Check collisions with other blocks //
			for (int r = 0; r < data_mat_size; r++) {
				for (int c = 0; c < data_mat_size; c++) {

					if (block.getData().at(c + data_mat_size * r) == true
						&& m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS + GridConstants::WIDTH_CELLS] != CURRENT_BLOCK
						&& m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS + GridConstants::WIDTH_CELLS] != EMPTY) {

						return true;
					}
				}
			}
			break;
		}

		return false;
	}

	/*****************************************************************************************************************************/

	bool Grid::moveBlock(Block &block, SDL_Keycode direction)
	{
		if (blockCollide(block, direction))
			return false;

		int data_mat_size = (int)pow(block.getData().size(), 0.5);

		switch (direction) {

		case SDLK_LEFT:

			// Move the block to the left
			for (int r = block.getRow() - block.getDataRow(); r < data_mat_size; r++) {
				for (int c = block.getColumn() - block.getDataColumn(); c < data_mat_size; c++) {

					if (block.getData().at(c + data_mat_size * r) == true) {
						m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS] = EMPTY;
						m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS - 1] = CURRENT_BLOCK;
					}
				}
			}
			
			block.moveLeft();

			break;

		case SDLK_RIGHT:

			// Move the block to the right
			for (int r = data_mat_size - 1; r >= block.getRow() - block.getDataRow(); r--) {
				for (int c = data_mat_size - 1; c >= block.getColumn() - block.getDataColumn(); c--) {

					if (block.getData().at(c + data_mat_size * r) == true) {
						m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS] = EMPTY;
						m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS + 1] = CURRENT_BLOCK;
					}
				}
			}

			block.moveRight();

			break;

		case SDLK_DOWN:

			// Move the block down
			for (int r = data_mat_size - 1; r >= block.getRow() - block.getDataRow(); r--) {
				for (int c = data_mat_size - 1; c >= block.getColumn() - block.getDataColumn(); c--) {

					if (block.getData().at(c + data_mat_size * r) == true) {
						m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS] = EMPTY;
						m_grid[block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS + GridConstants::WIDTH_CELLS] = CURRENT_BLOCK;
					}
				}
			}

			block.moveDown();

			break;
		}

		return true;
	}

	/*****************************************************************************************************************************/

	bool Grid::rotateBlock(Block &block)
	{
		if (block.getDataRow() < 0) {
			std::cout << "out of borders!!" << std::endl;
			return false;
		}

		auto block_data = block.getData();

		int data_mat_size = (int)pow(block_data.size(), 0.5);

		// Transpose the block data's matrix //
		for (int r = 0; r < data_mat_size; r++) {
			for (int c = 0; c < r; c++) {
				bool tmp = block_data.at(r + c * data_mat_size);
				block_data.at(r + c * data_mat_size) = block_data.at(c + r * data_mat_size);
				block_data.at(c + r * data_mat_size) = tmp;
			}
		}

		// Reverse block data's rows //
		for (int r = 0; r < data_mat_size; r++) {
			for (int c = 0; c < data_mat_size / 2; c++) {
				bool tmp = block_data.at(c + r * data_mat_size);
				block_data.at(c + r * data_mat_size) = block_data.at((data_mat_size - c - 1) + r * data_mat_size);
				block_data.at((data_mat_size - c - 1) + r * data_mat_size) = tmp;
			}
		}

		// Set upper left row and column of the block //
		int topmost_row = GridConstants::HEIGHT_CELLS;
		int undermost_row = 0;
		int leftmost_column = GridConstants::WIDTH_CELLS;
		int rightmost_column = 0;

		for (int r = 0; r < data_mat_size; r++) {
			for (int c = 0; c < data_mat_size; c++) {
				if (block_data.at(c + r * data_mat_size) == true && topmost_row > r)
					topmost_row = r;
				if (block_data.at(c + r * data_mat_size) == true && undermost_row < r)
					undermost_row = r;
				if (block_data.at(c + r * data_mat_size) == true && leftmost_column > c)
					leftmost_column = c;
				if (block_data.at(c + r * data_mat_size) == true && rightmost_column < c)
					rightmost_column = c;
			}
		}

		// Define the rotated block //
		Block *rotated_block = new Block(block);
		rotated_block->setData(block_data);
		rotated_block->setRow(block.getDataRow() + topmost_row);
		rotated_block->setColumn(block.getDataColumn() + leftmost_column);

		int tmp = block.getWidth();
		rotated_block->setWidth(block.getHeight());
		rotated_block->setHeight(tmp);

		// Check if the position of the block after the rotation is out of borders and kick the block of one to the left or right if possible //
		bool right_kick = false;
		bool left_kick = false;

		if (rotated_block->getDataColumn() + leftmost_column < -1) {
			std::cout << "out of borders!!" << std::endl;
			return false;
		}

		if (rotated_block->getDataColumn() + leftmost_column == -1) {
			right_kick = true;
			std::cout << "out of borders!!" << std::endl;
			std::cout << "shifted the block to the right" << std::endl;
		}

		if (rotated_block->getDataColumn() + rightmost_column > GridConstants::WIDTH_CELLS) {
			std::cout << "out of borders!!" << std::endl;
			return false;
		}

		if (rotated_block->getDataColumn() + rightmost_column == GridConstants::WIDTH_CELLS) {
			left_kick = true;
			std::cout << "out of borders!!" << std::endl;
			std::cout << "shifted the block to the left" << std::endl;
		}

		if (rotated_block->getRow() >= GridConstants::HEIGHT_CELLS - rotated_block->getHeight()) {
			std::cout << "out of borders!!" << std::endl;
			return false;
		}
		
		// Check if the position of the block after the rotation is obstructed by a block and kick the block of one to the left or right if possible //
		for (int r = 0; r < data_mat_size; r++) {
			for (int c = 0; c < data_mat_size; c++) {

				int block_pos = block.getDataColumn() + c + (block.getDataRow() + r) * GridConstants::WIDTH_CELLS;

				if (block_data.at(c + data_mat_size * r) == true)
				{
					if (m_grid[block_pos] != CURRENT_BLOCK && m_grid[block_pos] != EMPTY) {
						std::cout << "blocked by a block!!" << std::endl;
						
						// Try to kick right
						if (!blockCollide(*rotated_block, SDLK_RIGHT))
						{
							// If need more than one kick, the block is blocked
							if (right_kick)
								return false;

							right_kick = true;
							std::cout << "shifted the block to the right" << std::endl;
						}

						// Try to kick left
						else if (!blockCollide(*rotated_block, SDLK_LEFT))
						{
							// If need more than one kick, the block is blocked
							if (left_kick)
								return false;

							left_kick = true;
							std::cout << "shifted the block to the left" << std::endl;
						}

						else return false;
					}
				}
			}
		}

		delete rotated_block;

		// Kick the block if necessary //
		if (right_kick)
			moveBlock(block, SDLK_RIGHT);
		else if (left_kick)
			moveBlock(block, SDLK_LEFT);
		
		// Update the block's attributes //
		block.setData(block_data);
		block.setRow(block.getDataRow() + topmost_row);
		block.setColumn(block.getDataColumn() + leftmost_column);

		tmp = block.getWidth();
		block.setWidth(block.getHeight());
		block.setHeight(tmp);
		
		return true;
	}

	/*****************************************************************************************************************************/

	int Grid::completedLinesNumber(Block &block)
	{	
		bool lineComplete = true;

		for (int h = 0; h < block.getHeight(); h++) {
			for (int c = 0; c < GridConstants::WIDTH_CELLS; c++) {

				if (m_grid[c + (block.getRow() + h) * GridConstants::WIDTH_CELLS] == EMPTY) {
					lineComplete = false;
					break;
				}
			}

			if (lineComplete)
				m_completed_lines.push_back(block.getRow() + h);
			else
				lineComplete = true;	// if the "for" broke
		}

		if (!m_completed_lines.empty()) {
			std::cout << "line completed!" << std::endl;
		}

		return static_cast<int>(m_completed_lines.size());
	}

	/*****************************************************************************************************************************/

	void Grid::deleteCompletedLines()
	{
		for (int completed_row : m_completed_lines) {

			for (int r = completed_row; r > 0; r--) {
				for (int c = 0; c < GridConstants::WIDTH_CELLS; c++) {

					// row "r" is replaced by the line above it
					m_grid[c + r * GridConstants::WIDTH_CELLS] = m_grid[c + r * GridConstants::WIDTH_CELLS - GridConstants::WIDTH_CELLS];
					m_grid[c + r * GridConstants::WIDTH_CELLS - GridConstants::WIDTH_CELLS] = EMPTY;
				}
			}
		}

		m_completed_lines.clear();
	}

	/*****************************************************************************************************************************/

	void Grid::displayTxtGrid() const
	{
		std::cout << std::endl;

		for (int r = 0; r < GridConstants::HEIGHT_CELLS; r++) {
			for (int c = 0; c <  GridConstants::WIDTH_CELLS; c++) {
				std::cout << m_grid[c + GridConstants::WIDTH_CELLS * r] + 2 << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}


	/*****************************************************************************************************************************/
	/*****************************************************************************************************************************/

	// Getters
	std::array<int, GridConstants::WIDTH_CELLS * GridConstants::HEIGHT_CELLS>
	Grid::getGrid() const { return m_grid; }
}
