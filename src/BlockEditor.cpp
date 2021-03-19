#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "BlockEditor.h"
#include "constants.h"
#include "GameWindow.h"

namespace Domigon
{
	BlockEditor::BlockEditor() 
		: m_grid_size(0), m_pressed_button(nullptr), m_font(nullptr), m_hovered_font(nullptr), m_message_box(nullptr),
		hovered_square_row(0), hovered_square_column(0)
	{
		for (int i = 0; i < m_block_data.size(); i++)
			m_block_data[i] = new Cell();
	}


	BlockEditor::~BlockEditor()
	{
		TTF_CloseFont(m_font);
		TTF_CloseFont(m_hovered_font);

		for (auto button : m_buttons)
			delete button;

		for (auto cell : m_block_data)
			delete cell;
	}

	/*****************************************************************************************************************************/

	void BlockEditor::display()
	{
		// Create fonts //
		m_font = TTF_OpenFont(PathConstants::FONT_PATH.c_str(), BlockEditorConstants::BUTTON_TEXT_FONT_SIZE);
		m_hovered_font = TTF_OpenFont(PathConstants::FONT_PATH.c_str(), BlockEditorConstants::BUTTON_TEXT_FONT_SIZE_HOVER);

		if (m_font == nullptr || m_hovered_font == nullptr) {
			std::cerr << "Font could not be loaded! TTF error: " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		// Block editor main loop //
		while (1)
		{
			createMainScene();
			displayMainScene();

			// Go to the block creation scene //
			if (m_pressed_button->getText() == BlockEditorConstants::NEW_BLOCK_BUTTON)
			{
				createBlockCreationScene();
				displayBlockCreationScene();

				while (1)
				{
					// Create new block //
					if (m_pressed_button->getText() == BlockEditorConstants::CREATE_BUTTON)
					{
						if (canCreateBlock()) {
							createNewBlock();

							int text_width, text_height;
							TTF_SizeText(m_hovered_font, BlockEditorConstants::ADDED_BLOCK_TEXT.c_str(), &text_width, &text_height);
							Text::displayText({ WindowConstants::WIDTH / 2 - text_width / 2, 30, text_width, text_height }, *m_hovered_font
								, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }, BlockEditorConstants::ADDED_BLOCK_TEXT.c_str());
						}

						displayBlockCreationScene();
					}

					// Go back to the main scene //
					else if (m_pressed_button->getText() == BlockEditorConstants::BACK_BUTTON)
					{
						resetBlockData();

						// Erase old message box
						if (m_message_box != nullptr) {
							m_message_box->erase();
							m_message_box.reset();
						}
						break;
					}

					// Change the grid's size //
					else
					{
						int requested_grid_size = m_pressed_button->getText().at(0) - '0';
						m_grid_size = requested_grid_size;

						// There is no hovered square
						hovered_square_row = -1;
						hovered_square_column = -1;

						// Erase old message box
						if (m_message_box != nullptr) {
							m_message_box->erase();
							m_message_box.reset();
						}

						// Refresh the scene
						SDL_RenderCopy(GameWindow::renderer, GameWindow::background, NULL, NULL);
						displayBlockCreationScene();
					}
				}
			}

			// Go the delete block scene //
			else if (m_pressed_button->getText() == BlockEditorConstants::DELETE_BLOCK_BUTTON)
			{
				createDeleteBlockScene();
				displayDeleteBlockScene();

				if (m_pressed_button->getBlock() != nullptr) {
					deleteBlock();
				}
			}

			// Go back to the main menu
			else if (m_pressed_button->getText() == BlockEditorConstants::BACK_BUTTON)
			{
				break;
			}

			// Error
			else
			{
				std::cerr << "Unexpected error in edit blocks menu!!" << std::endl;
			}
		}
	}

	/*****************************************************************************************************************************/

	void BlockEditor::deletePreviousScene()
	{
		// Clean the window //
		SDL_RenderCopy(GameWindow::renderer, GameWindow::background, NULL, NULL);

		// Free buttons of the previous scene
		for (Button *button : m_buttons)
			delete button;
		m_buttons.clear();
		m_pressed_button = nullptr;
	}

	/*****************************************************************************************************************************/

	void BlockEditor::createMainScene()
	{
		deletePreviousScene();

		// Set buttons //
		int y = 750;
		int text_width, text_height;
		int shift = 100;

		TTF_SizeText(m_font, BlockEditorConstants::NEW_BLOCK_BUTTON.c_str(), &text_width, &text_height);
		Button *button = new Button(BlockEditorConstants::NEW_BLOCK_BUTTON, { shift, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);

		TTF_SizeText(m_font, BlockEditorConstants::DELETE_BLOCK_BUTTON.c_str(), &text_width, &text_height);
		button = new Button(BlockEditorConstants::DELETE_BLOCK_BUTTON, { WindowConstants::WIDTH - text_width - shift, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);

		y += text_height + shift;
		TTF_SizeText(m_font, BlockEditorConstants::BACK_BUTTON.c_str(), &text_width, &text_height);
		button = new Button(BlockEditorConstants::BACK_BUTTON, { WindowConstants::WIDTH - text_width - shift, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);
	}

	/*****************************************************************************************************************************/

	void BlockEditor::createBlockCreationScene()
	{
		deletePreviousScene();

		// Erase old message box
		if (m_message_box != nullptr) {
			m_message_box->erase();
			m_message_box.reset();
		}

		// Set buttons //
		int y = 240;
		int text_width, text_height;
		int shift = 100;

		for (int i = GameConstants::MAX_BLOCK_SIZE; i >= GameConstants::MIN_BLOCK_SIZE; i--) {
			std::string button_txt = std::to_string(i) + "x" + std::to_string(i);
			TTF_SizeText(m_font, button_txt.c_str(), &text_width, &text_height);
			Button *button = new Button(button_txt, { 50, y, text_width, text_height }, m_font, m_hovered_font);
			button->init();
			m_buttons.push_back(button);
			y += text_height + 40;
		}

		y = 850;
		TTF_SizeText(m_font, BlockEditorConstants::CREATE_BUTTON.c_str(), &text_width, &text_height);
		Button *button = new Button(BlockEditorConstants::CREATE_BUTTON, { shift, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);

		TTF_SizeText(m_font, BlockEditorConstants::BACK_BUTTON.c_str(), &text_width, &text_height);
		button = new Button(BlockEditorConstants::BACK_BUTTON, { WindowConstants::WIDTH - text_width - shift, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);

		// Init some values
		m_grid_size = 4;
		hovered_square_row = -1;
		hovered_square_column = -1;
	}

	/*****************************************************************************************************************************/

	void BlockEditor::createDeleteBlockScene()
	{
		deletePreviousScene();

		// Set buttons //
		int data_mat_size;
		int grid_square_pix = BlockEditorConstants::CELL_PIXELS;
		int x = BlockEditorConstants::EXISTING_BLOCKS_X;
		int y = BlockEditorConstants::EXISTING_BLOCKS_Y;
		int blocks_row = 0;

		for (auto block : Block::alltypesblocks)
		{
			data_mat_size = (int)pow(block->getData().size(), 0.5);

			Button *button = new Button(block.get(), x, y);
			button->init();
			m_buttons.push_back(button);

			// Update coordinates for the next block //
			if (x > WindowConstants::WIDTH - grid_square_pix * 8) {
				blocks_row++;
				x = BlockEditorConstants::EXISTING_BLOCKS_X;
			}
			else {
				x += grid_square_pix * (data_mat_size + 1);
			}

			y = BlockEditorConstants::EXISTING_BLOCKS_Y + blocks_row * (5 * grid_square_pix);
		}

		y = 750;
		int text_width, text_height;
		int shift = 150;

		TTF_SizeText(m_font, BlockEditorConstants::CANCEL_BUTTON.c_str(), &text_width, &text_height);
		Button *button = new Button(BlockEditorConstants::CANCEL_BUTTON, { WindowConstants::WIDTH - text_width - shift, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);
	}

	/*****************************************************************************************************************************/

	void BlockEditor::displayMainScene()
	{
		SDL_Renderer *renderer = GameWindow::renderer;

		// Draw title //
		m_font = TTF_OpenFont(PathConstants::FONT_PATH.c_str(), BlockEditorConstants::TITLE_FONT_SIZE);

		int title_width, title_height;
		TTF_SizeText(m_font, BlockEditorConstants::TITLE.c_str(), &title_width, &title_height);

		SDL_Rect title_rect = { WindowConstants::WIDTH / 2 - title_width / 2, 80, title_width, title_height };

		int enlargement = MenuConstants::BUTTON_HOVERED_ENLARGEMENT;
		SDL_Rect dstrect = { title_rect.x - enlargement / 2 - 5, title_rect.y - enlargement / 2 - 5, title_rect.w + enlargement, title_rect.h + enlargement };
		SDL_RenderCopy(renderer, GameWindow::frame, NULL, &dstrect);

		Text::displayText(title_rect, *m_font, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }, BlockEditorConstants::TITLE.c_str());
		m_font = TTF_OpenFont(PathConstants::FONT_PATH.c_str(), BlockEditorConstants::BUTTON_TEXT_FONT_SIZE);

		// Draw existing blocks //
		int data_mat_size;
		SDL_Rect dest;

		int grid_square_pix = BlockEditorConstants::CELL_PIXELS;
		int x = BlockEditorConstants::EXISTING_BLOCKS_X, block_x;
		int y = BlockEditorConstants::EXISTING_BLOCKS_Y, block_y;
		int blocks_row = 0;

		for (auto block : Block::alltypesblocks)
		{
			data_mat_size = (int)pow(block->getData().size(), 0.5);
			block_x = x;
			block_y = y;

			// Draw block //
			int blocktype = block->getType();
			SDL_SetRenderDrawColor(renderer, GameWindow::blocktype_color[blocktype].r, GameWindow::blocktype_color[blocktype].g,
				GameWindow::blocktype_color[blocktype].b, GameWindow::blocktype_color[blocktype].a);

			for (int row = 0; row < data_mat_size; row++) {
				for (int column = 0; column < data_mat_size; column++) {

					if (block->getData().at(column + data_mat_size * row) == true)
					{
						dest = { x, y, grid_square_pix, grid_square_pix };
						SDL_RenderFillRect(renderer, &dest);
					}
					x += grid_square_pix;
				}
				x -= grid_square_pix * data_mat_size;
				y += grid_square_pix;
			}

			// Draw block grid //
			SDL_SetRenderDrawColor(renderer, GridConstants::LINE_R, GridConstants::LINE_G, GridConstants::LINE_B, GridConstants::LINE_A);

			for (int i = 0; i < grid_square_pix * data_mat_size + 1; i += grid_square_pix) {
				SDL_RenderDrawLine(renderer, i + block_x, block_y, i + block_x, block_y + grid_square_pix * data_mat_size);	// vertical
				SDL_RenderDrawLine(renderer, block_x, block_y + i, block_x + grid_square_pix * data_mat_size, block_y + i);	// horizontal
				SDL_RenderDrawLine(renderer, i + block_x + 1, block_y, i + block_x + 1, block_y + grid_square_pix * data_mat_size);	// vertical + 1 pix
				SDL_RenderDrawLine(renderer, block_x, block_y + i + 1, block_x + grid_square_pix * data_mat_size, block_y + i + 1);	// horizontal + 1 pix
			}
			
			// Update coordinates for the next block //
			if (x > WindowConstants::WIDTH - grid_square_pix * 8) {
				blocks_row++;
				x = BlockEditorConstants::EXISTING_BLOCKS_X;
			}
			else {
				x += grid_square_pix * (data_mat_size + 1);
			}

			y = BlockEditorConstants::EXISTING_BLOCKS_Y + blocks_row * (5 * grid_square_pix);
		}

		// Draw buttons //
		for (auto button : m_buttons)
			button->draw();

		SDL_RenderPresent(renderer);

		// Listen mouse events //
		listenMouseEvents(true);
	}

	/*****************************************************************************************************************************/

	void BlockEditor::displayBlockCreationScene()
	{
		SDL_Renderer *renderer = GameWindow::renderer;

		// Reset
		m_pressed_button = nullptr;
		resetBlockData();

		// Draw block size text //
		int text_width, text_height;
		TTF_SizeText(m_font, BlockEditorConstants::BLOCK_SIZE_TEXT.c_str(), &text_width, &text_height);
		Text::displayText({ 30, 170, text_width, text_height }, *m_font, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }, BlockEditorConstants::BLOCK_SIZE_TEXT.c_str());

		// Draw creation block grid //
		int grid_square_pix = BlockEditorConstants::GRID_CELL_PIXELS;

		int grid_x = (WindowConstants::WIDTH - m_grid_size * grid_square_pix - 1) / 2;
		int grid_y = 150;
		int grid_w = grid_x + grid_square_pix * m_grid_size - grid_x;
		int grid_h = grid_y + grid_square_pix * m_grid_size - grid_y;
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_Rect grid_rect = { grid_x, grid_y, grid_w, grid_h };
		SDL_RenderFillRect(renderer, &grid_rect);

		SDL_SetRenderDrawColor(renderer, GridConstants::LINE_R, GridConstants::LINE_G, GridConstants::LINE_B, GridConstants::LINE_A);

		for (int i = 0; i < grid_square_pix * m_grid_size + 1; i += grid_square_pix) {
			SDL_RenderDrawLine(renderer, i + grid_x, grid_y, i + grid_x, grid_y + grid_h);	// vertical
			SDL_RenderDrawLine(renderer, grid_x, grid_y + i, grid_x + grid_w, grid_y + i);	// horizontal
		}

		// Draw shape block text //
		TTF_SizeText(m_hovered_font, BlockEditorConstants::SHAPE_BLOCK_TEXT.c_str(), &text_width, &text_height);
		Text::displayText({ WindowConstants::WIDTH / 2 - text_width / 2, grid_y + m_grid_size * grid_square_pix + 20,
			text_width, text_height }, *m_hovered_font, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }, BlockEditorConstants::SHAPE_BLOCK_TEXT.c_str());

		// Draw buttons //
		for (auto button : m_buttons)
			button->draw();

		SDL_RenderPresent(renderer);

		// Listen mouse events //
		listenMouseEvents(false, grid_x, grid_y);
	}

	/*****************************************************************************************************************************/

	void BlockEditor::displayDeleteBlockScene()
	{
		SDL_Renderer *renderer = GameWindow::renderer;

		// Draw delete block text //
		int text_width, text_height;
		TTF_SizeText(m_hovered_font, BlockEditorConstants::DELETE_BLOCK_TEXT.c_str(), &text_width, &text_height);
		Text::displayText({ WindowConstants::WIDTH / 2 - text_width / 2, 600, text_width, text_height }
			, *m_hovered_font, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }, BlockEditorConstants::DELETE_BLOCK_TEXT.c_str());

		// Draw buttons //
		for (auto button : m_buttons)
			button->draw();

		SDL_RenderPresent(renderer);

		// Listen mouse events //
		int timeElapsed = SDL_GetTicks();

		while (m_pressed_button == nullptr) {
			if (SDL_GetTicks() > (timeElapsed + (Uint32)(1000 / WindowConstants::FPS_LIMIT))) {
				handleMouseEventsDeleteScene();
				timeElapsed = SDL_GetTicks();
			}
		}
	}

	/*****************************************************************************************************************************/

	void BlockEditor::listenMouseEvents(bool inMainScene, int grid_x, int grid_y)
	{
		int fps_lock_timer = SDL_GetTicks();
		int msg_box_timer, msg_box_fadeout;
		msg_box_timer = msg_box_fadeout = fps_lock_timer;

		if (m_message_box != nullptr) {
			m_message_box->display();
		}

		while (m_pressed_button == nullptr)
		{
			if (SDL_GetTicks() > (fps_lock_timer + (Uint32)(1000 / WindowConstants::FPS_LIMIT))) {
				if (inMainScene) {
					handleMouseEventsMainScene();
				}
				else {
					handleMouseEventsCreationScene(grid_x, grid_y);
				}
				fps_lock_timer = SDL_GetTicks();
			}

			if (m_message_box != nullptr && SDL_GetTicks() > (Uint32)(msg_box_timer + BlockEditorConstants::TIME_BEFORE_FADE_OUT) && SDL_GetTicks() > (Uint32)(msg_box_fadeout + 50)) {
				if (!m_message_box->fadeOut()) {
					m_message_box->erase();
					m_message_box.reset();
				}
				msg_box_fadeout = SDL_GetTicks();
			}
		}
	}

	/*****************************************************************************************************************************/

	void BlockEditor::createNewBlock() const
	{
		// Determine the width and height of the created block //
		int width, height;
		int topmost_row = GridConstants::HEIGHT_CELLS;
		int undermost_row = 0;
		int leftmost_column = GridConstants::WIDTH_CELLS;
		int rightmost_column = 0;

		for (int r = 0; r < m_grid_size; r++) {
			for (int c = 0; c < m_grid_size; c++) {
				if (m_block_data.at(c + r * m_grid_size)->filled && topmost_row > r)
					topmost_row = r;
				if (m_block_data.at(c + r * m_grid_size)->filled && undermost_row < r)
					undermost_row = r;
				if (m_block_data.at(c + r * m_grid_size)->filled && leftmost_column > c)
					leftmost_column = c;
				if (m_block_data.at(c + r * m_grid_size)->filled && rightmost_column < c)
					rightmost_column = c;
			}
		}

		width = rightmost_column - leftmost_column + 1;
		height = undermost_row - topmost_row + 1;

		// Create block and save it in the saved file //
		std::vector<bool> block_data;
		std::ofstream save_file(PathConstants::SAVED_FILE_NAME, std::ofstream::out | std::ofstream::app);

		save_file << std::endl << '[';
		for (int i = 0; i < m_grid_size * m_grid_size; i++) {
			block_data.push_back(m_block_data[i]->filled);
			save_file << m_block_data[i]->filled;
		}
		save_file << ']';
		save_file.close();

		// Look for a free block type
		int block_type = 0;
		for (auto block : Block::alltypesblocks) {
			if (block_type != block->getType()) {
				break;
			}
			block_type++;
		}

		std::shared_ptr<Block> block(new Block(block_type, width, height, block_data));
		block->init();
		Block::alltypesblocks.push_back(block);
	}

	/*****************************************************************************************************************************/

	void BlockEditor::deleteBlock()
	{
		if (Block::alltypesblocks.size() > GameConstants::BLOCKS_NUMBER_MINIMUM)
		{
			// Remove the block from the list of existing blocks
			int block_pos = 0;	// block position in the vector containing all blocks types

			for (auto block : Block::alltypesblocks) {
				if (block.get() == m_pressed_button->getBlock())
					break;
				block_pos++;
			}

			Block::alltypesblocks.at(block_pos).reset();
			Block::alltypesblocks.erase(Block::alltypesblocks.begin() + block_pos);

			// Delete selected block from the saved file //
			std::ofstream saved_file("tmp", std::ofstream::out | std::ofstream::trunc);
			std::ifstream old_saved_file(PathConstants::SAVED_FILE_NAME);

			std::string line;
			int cpt_line = 0;

			while (std::getline(old_saved_file, line))
			{
				if (cpt_line != block_pos)
				{
					if (cpt_line != 0 && (block_pos != 0 || cpt_line != 1))
						saved_file << std::endl;
					saved_file << line;
				}
				cpt_line++;
			}

			saved_file.close();
			old_saved_file.close();

			// Delete old saved file
			std::remove(PathConstants::SAVED_FILE_NAME.c_str());
			if (std::ifstream(PathConstants::SAVED_FILE_NAME)) {
				std::cerr << "An error occured during the file deletion" << std::endl;
				exit(EXIT_FAILURE);
			}

			// Replace old saved file with the update one
			if (std::rename("tmp", PathConstants::SAVED_FILE_NAME.c_str())) {
				std::cerr << "An error occured during the file renaming" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		else {
			std::cout << "you need at least 6 blocks to play" << std::endl;

			SDL_Rect rect = { WindowConstants::WIDTH / 2 - 953 / 2, 400, 953, 113 };
			m_message_box = std::make_unique<MessageBox>(rect, BlockEditorConstants::ERROR_MIN_BLOCKS_TEXT);
		}
		
	}

	/*****************************************************************************************************************************/

	bool BlockEditor::canCreateBlock()
	{
		if (Block::alltypesblocks.size() >= GameConstants::BLOCKS_NUMBER_MAXIMUM) {
			std::cout << "you can't create more blocks : delete an existing block to create a new one" << std::endl;

			SDL_Rect rect = { WindowConstants::WIDTH / 2 - 953 / 2, 10, 953, 113 };
			m_message_box = std::make_unique<MessageBox>(rect, BlockEditorConstants::ERROR_MAX_BLOCKS_TEXT);

			return false;
		}

		// Check if the block is too small //
		int nb_squares = 0;
		bool bound = false;

		for (int r = 0; r < m_grid_size; r++) {
			for (int c = 0; c < m_grid_size; c++) {

				if (m_block_data[c + m_grid_size * r]->filled) {
					nb_squares++;
				}
			}
		}

		if (nb_squares < 3) {
			std::cout << "your block is too small : it must contains at least 3 squares" << std::endl;

			SDL_Rect rect = { WindowConstants::WIDTH / 2 - 953 / 2, 10, 953, 113 };
			m_message_box = std::make_unique<MessageBox>(rect, BlockEditorConstants::ERROR_BLOCK_SIZE);

			return false;
		}

		// Check if there is only one block //
		addCellsNeighbors();

		int i = 0;
		while (!m_block_data[i]->filled)	// find the first filled cell in the grid
			i++;	
		checkBlockShape(*m_block_data[i]);	// visit every neighbors of this cells and the neighbors of her neighbors, etc.

		for (i; i < m_block_data.size(); i++) {
			if (m_block_data[i]->filled && !m_block_data[i]->visited) {					// if a cell is filled but has not been visited, it means it's not bound to 
				std::cout << "there must be only one block in the grid" << std::endl;	// the first cell filled in the grid and so there is more than one block in the grid

				SDL_Rect rect = { WindowConstants::WIDTH / 2 - 953 / 2, 10, 953, 113 };
				m_message_box = std::make_unique<MessageBox>(rect, BlockEditorConstants::ERROR_MULTIPLE_BLOCKS);

				return false;												
			}
		}	

		return true;
	}

	/*****************************************************************************************************************************/

	void BlockEditor::checkBlockShape(Cell &cell)
	{
		if (!cell.visited && cell.filled) {
			cell.visited = true;

			for (Cell *c : cell.neighbors) {
				checkBlockShape(*c);
			}
		}
	}

	/*****************************************************************************************************************************/

	void BlockEditor::handleMouseEventsMainScene()
	{
		// Event handler
		SDL_Event e;

		int mouse_x, mouse_y;

		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// If the mouse moved //
			if (e.type == SDL_MOUSEMOTION) {
				mouse_x = e.motion.x;
				mouse_y = e.motion.y;

				for (Button *button : m_buttons)
				{
					// If a button was updated, break the loop
					if (button->handleButtonHovering(e))
						break;
				}
			}

			// If the player left clicked, check if it was on a button //
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
				mouse_x = e.motion.x;
				mouse_y = e.motion.y;

				for (auto button : m_buttons)
				{
					if (mouse_x >= button->getHoveredRect().x && mouse_x <= button->getHoveredRect().x + button->getHoveredRect().w
						&& mouse_y >= button->getHoveredRect().y && mouse_y <= button->getHoveredRect().y + button->getHoveredRect().h)
					{
						if (button->isHovered()) {
							m_pressed_button = button;
							break;
						}
					}
				}
			}

			// If the user requests quit, close the window
			else if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				m_pressed_button = m_buttons.back();
			}
		}
	}

	/*****************************************************************************************************************************/

	void BlockEditor::handleMouseEventsCreationScene(int grid_x, int grid_y)
	{
		SDL_Renderer *renderer = GameWindow::renderer;

		// Event handler
		SDL_Event e;

		int mouse_x, mouse_y, square_width;

		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// If the mouse moved //
			if (e.type == SDL_MOUSEMOTION) {
				mouse_x = e.motion.x;
				mouse_y = e.motion.y;
				square_width = BlockEditorConstants::GRID_CELL_PIXELS;

				// Grid
				if (mouse_x > grid_x && mouse_x < grid_x + square_width * m_grid_size
					&& mouse_y > grid_y && mouse_y < grid_y + square_width * m_grid_size)
				{
					int square_row = (int)((mouse_y - grid_y) / square_width);
					int square_column = (int)((mouse_x - grid_x) / square_width);

					SDL_Rect rect = { grid_x + square_column * square_width + 1, grid_y + square_row * square_width + 1, square_width - 1 , square_width - 1 };

					// If it's a different square, highlight the new one and unhighlight the old one
					if (hovered_square_row != square_row || hovered_square_column != square_column)
					{
						if (!m_block_data.at(square_column + m_grid_size * square_row)->filled) {
							SDL_SetRenderDrawColor(renderer, BlockEditorConstants::HOVERED_CELL_R, BlockEditorConstants::HOVERED_CELL_G
								,BlockEditorConstants::HOVERED_CELL_B, BlockEditorConstants::HOVERED_CELL_A);
							SDL_RenderFillRect(renderer, &rect);
						}

						if (hovered_square_row != -1 && !m_block_data.at(hovered_square_column + m_grid_size * hovered_square_row)->filled) {
							SDL_SetRenderDrawColor(renderer, GridConstants::BACKGROUND_R, GridConstants::BACKGROUND_G, GridConstants::BACKGROUND_B, GridConstants::BACKGROUND_A);
							rect = { grid_x + hovered_square_column * square_width + 1, grid_y + hovered_square_row * square_width + 1, square_width - 1 , square_width - 1 };
							SDL_RenderFillRect(renderer, &rect);
						}

						hovered_square_row = square_row;
						hovered_square_column = square_column;
						SDL_RenderPresent(renderer);
					}
				}

				// If out of the grid, unhighlight the highlighted square
				else if (hovered_square_row != -1 && !m_block_data.at(hovered_square_column + m_grid_size * hovered_square_row)->filled)
				{
					SDL_Rect rect = { grid_x + hovered_square_column * square_width + 1, grid_y + hovered_square_row * square_width + 1, square_width - 1 , square_width - 1 };
					SDL_SetRenderDrawColor(renderer, GridConstants::BACKGROUND_R, GridConstants::BACKGROUND_G, GridConstants::BACKGROUND_B, GridConstants::BACKGROUND_A);
					SDL_RenderFillRect(renderer, &rect);
					SDL_RenderPresent(renderer);

					// There is no hovered square
					hovered_square_row = -1;	
					hovered_square_column = -1;
				}
				
				// Buttons
				for (Button *button : m_buttons)
				{
					// If a button was updated, break the loop
					if (button->handleButtonHovering(e))
						break;
				}
			}

			// If the player left clicked, check if it was on a button or on the grid //
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				mouse_x = e.motion.x;
				mouse_y = e.motion.y;
				square_width = BlockEditorConstants::GRID_CELL_PIXELS;

				if (e.button.button == SDL_BUTTON_LEFT)
				{
					// If the player clicked on the grid
					if (mouse_x > grid_x && mouse_x < grid_x + square_width * m_grid_size
						&& mouse_y > grid_y && mouse_y < grid_y + square_width * m_grid_size)
					{
						// Highlight the clicked square
						int square_row = (int) ((mouse_y - grid_y) / square_width);
						int square_column = (int) ((mouse_x - grid_x) / square_width);

						if (!m_block_data[square_column + m_grid_size * square_row]->filled) {
							mouse_x = grid_x + square_column * square_width + 1;
							mouse_y = grid_y + square_row * square_width + 1;

							m_block_data[square_column + m_grid_size * square_row]->filled = true;

							SDL_SetRenderDrawColor(renderer, BlockEditorConstants::CLICKED_CELL_R, BlockEditorConstants::CLICKED_CELL_G
								, BlockEditorConstants::CLICKED_CELL_B, BlockEditorConstants::CLICKED_CELL_A);
							SDL_Rect rect = { mouse_x, mouse_y, square_width - 1 , square_width - 1 };
							SDL_RenderFillRect(renderer, &rect);
							SDL_RenderPresent(renderer);
						}
					}

					// If the player cliked on a button
					for (auto button : m_buttons)
					{
						if (mouse_x >= button->getHoveredRect().x && mouse_x <= button->getHoveredRect().x + button->getHoveredRect().w
							&& mouse_y >= button->getHoveredRect().y && mouse_y <= button->getHoveredRect().y + button->getHoveredRect().h)
						{
							if (button->isHovered()) {
								m_pressed_button = button;
								break;
							}
						}
					}
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					// Unhighlight the clicked square
					if (mouse_x > grid_x && mouse_x < grid_x + square_width * m_grid_size
						&& mouse_y > grid_y && mouse_y < grid_y + square_width * m_grid_size)
					{
						int square_row = (int) ((mouse_y - grid_y) / square_width);
						int square_column = (int) ((mouse_x - grid_x) / square_width);

						if (m_block_data[square_column + m_grid_size * square_row]->filled) {
							mouse_x = grid_x + square_column * square_width + 1;
							mouse_y = grid_y + square_row * square_width + 1;

							m_block_data[square_column + m_grid_size * square_row]->filled = false;

							SDL_SetRenderDrawColor(renderer, GridConstants::BACKGROUND_R, GridConstants::BACKGROUND_G, GridConstants::BACKGROUND_B, GridConstants::BACKGROUND_A);
							SDL_Rect rect = { mouse_x, mouse_y, square_width - 1 , square_width - 1 };
							SDL_RenderFillRect(renderer, &rect);
							SDL_RenderPresent(renderer);
						}
					}
				}
			}

			// If the user requests quit, close the window
			else if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				m_pressed_button = m_buttons.back();
			}
		}
	}

	/*****************************************************************************************************************************/

	void BlockEditor::handleMouseEventsDeleteScene()
	{
		// Event handler
		SDL_Event e;

		int mouse_x, mouse_y;

		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// If the mouse moved //
			if (e.type == SDL_MOUSEMOTION) {
				mouse_x = e.motion.x;
				mouse_y = e.motion.y;

				for (Button *button : m_buttons)
				{
					// If a button was updated, break the loop
					if (button->handleButtonHovering(e))
						break;
				}
			}

			// If the player left clicked, check if it was on a button //
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
				mouse_x = e.motion.x;
				mouse_y = e.motion.y;

				for (auto button : m_buttons)
				{
					if (mouse_x >= button->getHoveredRect().x && mouse_x <= button->getHoveredRect().x + button->getHoveredRect().w
						&& mouse_y >= button->getHoveredRect().y && mouse_y <= button->getHoveredRect().y + button->getHoveredRect().h)
					{
						if (button->isHovered()) {
							m_pressed_button = button;
							break;
						}
					}
				}
			}

			// If the user requests quit, close the window
			else if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				m_pressed_button = m_buttons.back();
			}
		}
	}

	/*****************************************************************************************************************************/

	void BlockEditor::addCellsNeighbors()
	{
		for (int r = 0; r < m_grid_size; r++)
		{
			for (int c = 0; c < m_grid_size; c++)
			{
				if (c < m_grid_size - 1)
					m_block_data[c + r * m_grid_size]->neighbors.push_back(m_block_data[c + 1 + r * m_grid_size]);
				if (c >= 1)
					m_block_data[c + r * m_grid_size]->neighbors.push_back(m_block_data[c - 1 + r * m_grid_size]);
				if (r < m_grid_size - 1)
					m_block_data[c + r * m_grid_size]->neighbors.push_back(m_block_data[c + (r + 1) * m_grid_size]);
				if (r >= 1)
					m_block_data[c + r * m_grid_size]->neighbors.push_back(m_block_data[c + (r - 1) * m_grid_size]);
			}
		}
	}

	/*****************************************************************************************************************************/

	void BlockEditor::resetBlockData()
	{
		for (Cell *cell : m_block_data) {
			cell->filled = false;
			cell->visited = false;
			cell->neighbors.clear();
		}
	}

	/*****************************************************************************************************************************/
	/*****************************************************************************************************************************/

	// Getter
	Button *BlockEditor::getPressedButton() const { return m_pressed_button; }
}
