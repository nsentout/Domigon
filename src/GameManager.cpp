#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include <SDL_image.h>

#include "GameWindow.h"

namespace Domigon
{
	GameManager::GameManager()
		: m_grid(nullptr), m_currentBlock(nullptr), m_original_currentBlock(nullptr), m_random(nullptr),
		m_can_hold_block(true), m_cleared_lines_nb(0), m_game_over(false), m_holdBlock(nullptr), m_level(0),
		m_refresh_followingblocks(false), m_refresh_grid(false), m_refresh_holdblock(false), m_refresh_level(false),
		m_refresh_score(false), m_score(0), m_speed(0.0f)
	{}

	GameManager::~GameManager() {
		delete m_grid;
		delete m_currentBlock;
		delete m_original_currentBlock;
		delete m_random;

		if (m_holdBlock != nullptr)
			delete m_holdBlock;
	}

	/*****************************************************************************************************************************/

	bool GameManager::initBlocks()
	{
		// Read saved file //
		std::string line;
		std::vector<bool> block_data;
		int data_mat_size, cpt_square, i;
		char data;

		std::ifstream save_file(PathConstants::SAVED_FILE_NAME);

		// Read line
		while (std::getline(save_file, line))
		{
			// Check file format
			if (line == "") {
				std::cerr << "saved file format incorrect : blank line" << std::endl;
				exit(EXIT_FAILURE);
			}

			// Check file format
			if (line.at(0) != '[') {
				std::cerr << "saved file format incorrect : [ missing" << std::endl;
				exit(EXIT_FAILURE);
			}

			// Check file format
			if (line.at(line.size() - 1) != ']') {
				std::cerr << "saved file format incorrect : ] missing" << std::endl;
				exit(EXIT_FAILURE);
			}

			cpt_square = 0;
			i = 1;

			while (line.at(i) != ']') {
				data = line.at(i);

				// Check the block data only contains 0 and 1
				if (data != '0' && data != '1') {
					std::cerr << "saved file format incorrect : number other than 0 or 1 present" << std::endl;
					exit(EXIT_FAILURE);
				}

				cpt_square++;

				// Stop reading the file if the block is too big
				if (cpt_square > GameConstants::MAX_BLOCK_SIZE * GameConstants::MAX_BLOCK_SIZE) {
					std::cerr << "saved file format incorrect : block size too big" << std::endl;
					exit(EXIT_FAILURE);
				}

				block_data.push_back(data == '1');
				i++;
			}

			// Check if the block's size is between 2 and 4
			data_mat_size = (int)pow(cpt_square, 0.5);
			if (data_mat_size < GameConstants::MIN_BLOCK_SIZE || data_mat_size > GameConstants::MAX_BLOCK_SIZE) {
				std::cerr << "saved file format incorrect : block size incorrect" << std::endl;
				exit(EXIT_FAILURE);
			}

			// Determine the width and height of the block //
			int width, height;
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

			width = rightmost_column - leftmost_column + 1;
			height = undermost_row - topmost_row + 1;

			// Create block //
			std::shared_ptr<Block> block(new Block(static_cast<int>(Block::alltypesblocks.size()), width, height, block_data));
			block->init();
			Block::alltypesblocks.push_back(block);
			block_data.clear();
		}

		save_file.close();

		return true;
	}

	/*****************************************************************************************************************************/

	void GameManager::newGame()
	{
		//If it's not the first game, delete the previous one
		if (m_currentBlock != nullptr) {
			delete m_grid;
			delete m_currentBlock;
			delete m_original_currentBlock;
			delete m_random;

			if (m_holdBlock != nullptr) {
				delete m_holdBlock;
				m_holdBlock = nullptr;
			}

			while (!m_followingblocks.empty()) {
				m_followingblocks.front().reset();
				m_followingblocks.pop();
			}
		}

		// Create a new grid
		m_grid = new Grid();
		m_grid->init();

		// Create a queue of blocks storing the following blocks
		m_random = new Random(0, static_cast<int>(Block::alltypesblocks.size()) - 1);
		int r = m_random->rand();

		for (int i = 0; i < GameConstants::NB_FOLLOWING_BLOCKS; i++) {
			std::shared_ptr<Block> block(new Block(*Block::alltypesblocks[r]));
			m_followingblocks.push(block);
			r = m_random->rand();
		}

		// Init the current block and add it to the game's blocks
		m_currentBlock = new Block(*Block::alltypesblocks[r]);
		m_original_currentBlock = new Block();
		*m_original_currentBlock = *m_currentBlock;

		// Init grid
		m_grid->addBlock(*m_currentBlock);

		// Allow the player to hold a block
		m_can_hold_block = true;
		
		// Refresh window
		m_refresh_grid = true;
		m_refresh_followingblocks = true;

		// Reset some values
		m_game_over = false;
		m_score = 0;
		m_level = 1;
		m_cleared_lines_nb = 0;
		m_speed = GameConstants::SPEED;

		// Start timers
		m_timer = std::chrono::high_resolution_clock::now();
		m_timer2 = std::chrono::high_resolution_clock::now();
	}

	/*****************************************************************************************************************************/

	void GameManager::insertNextBlock()
	{
		// Change the current block with the first one in the following blocks queue //
		delete m_currentBlock;
		m_currentBlock = new Block(*m_followingblocks.front());

		// Add new block and check if the game is over //
		if (!m_grid->addBlock(*m_currentBlock)) {
			m_game_over = true;
			return;
		}

		m_followingblocks.pop();

		// Update the unrotated current block
		*m_original_currentBlock = *m_currentBlock;

		// Add a new block to the following blocks queue
		int r = m_random->rand();
		m_followingblocks.push(Block::alltypesblocks[r]);
	}

	/*****************************************************************************************************************************/

	void GameManager::handleGameSpeed()
	{
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(m_timer2 - m_timer);

		if (time_span.count() >= m_speed) {
			m_timer = m_timer2;
			handleMoveDown();
			return;
		}

		m_timer2 = std::chrono::high_resolution_clock::now();
	}

	/*****************************************************************************************************************************/

	void GameManager::handleMoveDown()
	{
		if (!m_grid->moveBlock(*m_currentBlock, SDLK_DOWN)) {
			m_grid->placeBlock(*m_currentBlock);

			int completed_lines_nb = m_grid->completedLinesNumber(*m_currentBlock);

			if (completed_lines_nb > 0) {
				m_grid->deleteCompletedLines();
				m_cleared_lines_nb += completed_lines_nb;
				m_refresh_score = true;
				
				switch (completed_lines_nb) {
				case 1:
					m_score += GameConstants::SCORE_PER_1_LINE_CLEARED * m_level;
					break;
				case 2:
					m_score += GameConstants::SCORE_PER_2_LINES_CLEARED * m_level;
					break;
				case 3:
					m_score += GameConstants::SCORE_PER_3_LINES_CLEARED * m_level;
					break;
				case 4:
					m_score += GameConstants::SCORE_PER_4_LINES_CLEARED * m_level;
					break;
				default:
					std::cerr << "can't clear more than 4 lines at one" << std::endl;
					exit(EXIT_FAILURE);
				}

				if (m_level < 10 && m_cleared_lines_nb >= GameConstants::CLEARED_LINES_REQUIRED_LEVEL_CHANGE) {
					m_refresh_level = true;
					m_level++;
					m_speed -= 0.1f;
				}
			}

			insertNextBlock();
			m_can_hold_block = true;
			m_refresh_followingblocks = true;
		}
		m_refresh_grid = true;
	}

	/*****************************************************************************************************************************/

	void GameManager::holdBlock()
	{
		// If there is no hold block
		if (m_holdBlock == nullptr)
		{
			m_holdBlock = new Block(m_original_currentBlock->getType(), m_original_currentBlock->getWidth(), m_original_currentBlock->getHeight()
				, m_original_currentBlock->getData());
			m_holdBlock->init();

			m_grid->eraseBlock(*m_currentBlock);
			insertNextBlock();
			m_refresh_holdblock = true;
			m_refresh_followingblocks = true;
		}

		// If there already is a hold block
		else if (m_can_hold_block)
		{
			m_grid->eraseBlock(*m_currentBlock);

			// Switch the hold block and the current block //
			*m_currentBlock = *m_holdBlock;
			delete m_holdBlock;

			m_holdBlock = new Block(m_original_currentBlock->getType(), m_original_currentBlock->getWidth(), m_original_currentBlock->getHeight()
				, m_original_currentBlock->getData());
			m_holdBlock->init();

			m_grid->addBlock(*m_currentBlock);
			m_refresh_holdblock = true;
		}
		else
		{
			std::cout << "can't hold another block, place the current one first" << std::endl;
		}

		// Prevent the player to hold a block until he placed the current block
		m_can_hold_block = false;
	}

	/*****************************************************************************************************************************/

	void GameManager::handleInputEvents(SDL_Event &e, bool &quit)
	{
		switch (e.key.keysym.sym) {

		case SDLK_LEFT:
			std::cout << "moving left ..." << std::endl;
			if (m_grid->moveBlock(*m_currentBlock, SDLK_LEFT))
				m_refresh_grid = true;

			break;

		case SDLK_RIGHT:
			std::cout << "moving right ..." << std::endl;
			if (m_grid->moveBlock(*m_currentBlock, SDLK_RIGHT))
				m_refresh_grid = true;

			break;

		case SDLK_DOWN:
			std::cout << "moving down ..." << std::endl;
			handleMoveDown();

			break;

		case SDLK_UP:
			std::cout << "rotating ..." << std::endl;
			if (m_grid->rotateBlock(*m_currentBlock)) {
				m_grid->updateBlock(*m_currentBlock);
				m_refresh_grid = true;
			}

			break;

		case SDLK_SPACE:
			std::cout << "holding block ..." << std::endl;
			holdBlock();
			m_refresh_grid = true;

			break;

		case SDLK_ESCAPE:
			quit = true;
			break;

		// DEBUG
		/*
		default:
			if (e.key.keysym.sym > SDLK_0 && e.key.keysym.sym <= SDLK_0 + Block::alltypesblocks.size()) {
				std::cout << "force new block" << std::endl;
				m_grid->placeBlock(*m_currentBlock);
				m_currentBlock = new Block(*Block::alltypesblocks[e.key.keysym.sym - SDLK_1]);
				m_grid->addBlock(*m_currentBlock);
				m_refresh_grid = true;
			}
			else if (e.key.keysym.sym == SDLK_k && m_level < 10) {
				std::cout << "force change level" << std::endl;
				m_level++;
				m_speed -= 0.1f;
				m_refresh_level = true;
			}
		*/
		}
		//m_grid->displayTxtGrid();
	}

	/*****************************************************************************************************************************/
	/*****************************************************************************************************************************/

	// Getters
	Grid* GameManager::getGrid() const { return m_grid; }
	Block* GameManager::getCurrentBlock() const { return m_currentBlock; }
	std::queue<std::shared_ptr<Block>> GameManager::getFollowingblocks() const { return m_followingblocks; }
	Block* GameManager::getHoldBlock() const { return m_holdBlock; }
	bool GameManager::needGridRefresh() const { return m_refresh_grid; }
	bool GameManager::needFollowingblocksRefresh() const { return m_refresh_followingblocks; }
	bool GameManager::needHoldblockRefresh() const { return m_refresh_holdblock; }
	bool GameManager::needScoreRefresh() const { return m_refresh_score; }
	bool GameManager::needLevelRefresh() const { return m_refresh_level; }
	int GameManager::getLevel() const { return m_level; }
	int GameManager::getScore() const { return m_score; }
	bool GameManager::isGameOver() const { return m_game_over; }
	std::chrono::high_resolution_clock::time_point GameManager::getTimer() const { return m_timer; }

	// Setter
	void GameManager::setGridRefresh(bool refresh) { m_refresh_grid = refresh; }
	void GameManager::setFollowingblocksRefresh(bool refresh) { m_refresh_followingblocks = refresh; }
	void GameManager::setHoldblockRefresh(bool refresh) { m_refresh_holdblock = refresh; }
	void GameManager::setScoreRefresh(bool refresh) { m_refresh_score = refresh; }
	void GameManager::setLevelRefresh(bool refresh) { m_refresh_level = refresh; }

	/*****************************************************************************************************************************/
	/*****************************************************************************************************************************/
}

#include "../include/Menu.h"
#include "../include/BlockEditor.h"
#include "../include/ResourceManager.h"
//#include <vld.h>	//check memory leak

using namespace Domigon;

int main(int argc, char* args[])
{
	// Initialize the TTF engine
	if (TTF_Init() == -1) {
		std::cerr << "TTF could not initialize! TTF error: " << TTF_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Initialize SDL video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL video could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Initialize SDL audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cerr << "SDL audio could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Initialize SDL_image
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(flags) != flags) {
		std::cerr << "IMG_Init: Failed to init required jpg and png support!" << std::endl;
		std::cerr << "IMG error: " << IMG_GetError() << std::endl;;
	}

	// Load sounds resources
	ResourceManager::Instance().loadResources();

	GameManager *gameManager = new GameManager();
	GameWindow *gameWindow = new GameWindow();

	gameWindow->setGameManager(gameManager);


	// Initialize blocks
	if (!gameManager->initBlocks()) {
		std::cerr << "An error occured during the initialization of the blocks!" << std::endl;
		return EXIT_FAILURE;
	}

	// If the creation of the window succeeded
	if (gameWindow->create())
	{
		bool close_window = false;

		while (!close_window)
		{
			// Display main menu //
			Menu *main_menu = new Menu();
			main_menu->createMainMenu();
			main_menu->display();

			// If the player clicked on a button, exit the menu and proceed
			std::string pressed_button = main_menu->getPressedButton()->getText();
			delete main_menu;

			// Game loop //
			if (pressed_button == MenuConstants::PLAY_BUTTON)
			{
				gameManager->newGame();

				gameWindow->firstDraw();
				gameWindow->draw();

				//ResourceManager::Instance().playSound(GAME_MUSIC);

				bool quit = false;
				SDL_Event e;			// event handler

				int timeElapsed = SDL_GetTicks();

				while (!quit)
				{
					if (SDL_GetTicks() > (timeElapsed + (Uint32)(1000 / WindowConstants::FPS_LIMIT)))
					{
						// Handle events on queue
						while (SDL_PollEvent(&e) != 0)
						{
							// User requests quit
							if (e.type == SDL_QUIT) {
								quit = true;	//TODO: cache le jeu et affiche un bouton reprendre et retourner au menu (pause in-game)
							}

							// Handle input events
							if (e.type == SDL_KEYDOWN) {
								gameManager->handleInputEvents(e, quit);
							}
						}

						// Moves the block down according to the game speed
						gameManager->handleGameSpeed();

						// Handle game over
						if (gameManager->isGameOver()) {
							std::cout << "GAME OVER" << std::endl;	//TODO: message "GAME OVER"
							break;
						}

						// Draw all the blocks
						gameWindow->draw();

						// Update time elapsed
						timeElapsed = SDL_GetTicks();
					}
				}
			}

			else if (pressed_button == MenuConstants::EDIT_BLOCKS_BUTTON)
			{
				BlockEditor *block_editor = new BlockEditor();
				block_editor->display();
				delete block_editor;	// if back button is pressed
			}

			else if (pressed_button == MenuConstants::EXIT_BUTTON)
			{
				close_window = true;
			}

			else
			{
				std::cerr << "Unexpected error in main menu!!" << std::endl;
			}
		}
	}

	else {
		std::cerr << "An error occured during the creation of the window" << std::endl;
	}

	delete gameWindow;
	delete gameManager;

	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}