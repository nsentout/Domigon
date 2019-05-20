#include "Block.h"

namespace Domigon
{
	std::vector<std::shared_ptr<Block>> Block::alltypesblocks;

	/*****************************************************************************************************************************/

	Block::Block()
		: m_row(0), m_column(0), m_data_row(0), m_data_column(0), m_type(-2), m_width(0), m_height(0)
	{}

	Block::Block(int type, int width, int height, std::vector<bool> data)
		: m_row(0), m_column(0), m_data_row(0), m_data_column(0), m_type(type), m_width(width), m_height(height), m_data(data)
	{}

	Block::~Block() {}

	/*****************************************************************************************************************************/

	void Block::init()
	{
		int data_mat_size = (int)pow(m_data.size(), 0.5);
		int empty_cell = 0;
		bool found_filled_cell = false;

		// Set data column //
		if (m_width > 2)
			m_data_column = m_column = GridConstants::WIDTH_CELLS / 2 - 2;
		else
			m_data_column = m_column = GridConstants::WIDTH_CELLS / 2 - 1;
		
		for (int c = 0; c < data_mat_size; c++) {
			for (int r = 0; r < data_mat_size; r++) {

				if (m_data.at(c + data_mat_size * r) == true)
					found_filled_cell = true;

				// If there are one or more empty columns before the first filled cell, update the data column of the block
				else if (!found_filled_cell) {
					empty_cell++;

					if (empty_cell == data_mat_size) {
						m_data_column--;
						empty_cell = 0;
					}
				}
			}

			if (found_filled_cell)
				break;
		}

		// Set data row //
		found_filled_cell = false;
		empty_cell = 0;

		for (int r = 0; r < data_mat_size; r++) {
			for (int c = 0; c < data_mat_size; c++) {

				if (m_data.at(c + data_mat_size * r) == true)
					found_filled_cell = true;

				// If there are one or more empty rows before the first filled cell, update the data row of the block
				else if (!found_filled_cell) {
					empty_cell++;

					if (empty_cell == data_mat_size) {
						m_data_row--;
						empty_cell = 0;
					}
				}
			}

			if (found_filled_cell)
				break;
		}
	}

	/*****************************************************************************************************************************/

	void Block::moveLeft()
	{ 
		m_data_column --;
		m_column --; 
	}

	void Block::moveRight()
	{ 
		m_data_column ++;
		m_column ++; 
	}

	void Block::moveDown()
	{ 
		m_data_row ++;
		m_row ++;
	}

	/*****************************************************************************************************************************/

	void Block::operator=(const Block& other)
	{
		m_row = other.m_row;
		m_column = other.m_column;
		m_data_row = other.m_data_row;
		m_data_column = other.m_data_column;
		m_type = other.m_type;
		m_width = other.m_width;
		m_height = other.m_height;
		m_data = other.m_data;
	}

	/*****************************************************************************************************************************/
	/*****************************************************************************************************************************/

	// Getters
	int Block::getRow() const { return m_row; }
	int Block::getColumn() const { return m_column; }
	int Block::getDataRow() const { return m_data_row; }
	int Block::getDataColumn() const { return m_data_column; }
	int Block::getType() const { return m_type; }
	int Block::getWidth() const { return m_width; }
	int Block::getHeight() const { return m_height; }
	std::vector<bool> Block::getData() const { return m_data; }

	// Setters
	void Block::setRow(int row) { m_row = row; }
	void Block::setColumn(int column) { m_column = column; }
	void Block::setWidth(int width_square) { m_width = width_square; }
	void Block::setHeight(int height_square) { m_height = height_square; }
	void Block::setData(std::vector<bool> data) { m_data = data; }
}
