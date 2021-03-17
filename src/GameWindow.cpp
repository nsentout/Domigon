#include <iostream>
#include <string>

#include <SDL_image.h>

#include "GameWindow.h"
#include "Text.h"

using namespace std::chrono;

namespace Domigon
{
	SDL_Renderer *GameWindow::renderer;
	std::map<int, SDL_Color> GameWindow::blocktype_color;
	SDL_Texture *GameWindow::background;
	SDL_Texture *GameWindow::button;
	SDL_Texture *GameWindow::big_panel;
	SDL_Texture *GameWindow::small_panel;
	SDL_Texture *GameWindow::frame;
	SDL_Texture *GameWindow::error_panel;

	/*****************************************************************************************************************************/

	GameWindow::GameWindow() : m_font(nullptr), m_gamemanager(nullptr), m_width(0), m_height(0), m_hold_rect(), m_level_rect(), m_next_rect(), m_score_rect(), m_window(nullptr)
	{}

	GameWindow::~GameWindow() {
		SDL_DestroyTexture(background);
		SDL_DestroyWindow(m_window);
		SDL_DestroyRenderer(renderer);
		TTF_CloseFont(m_font);
	}

	/*****************************************************************************************************************************/

	bool GameWindow::create()
	{
		// Create window
		m_window = SDL_CreateWindow("DOMIGON", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowConstants::WIDTH, WindowConstants::HEIGHT
			, SDL_WINDOW_SHOWN);

		if (m_window != nullptr)
		{
			m_width = WindowConstants::WIDTH;
			m_height = WindowConstants::HEIGHT;

			// Set the scaling interpolation algorithm for the renderer (1 = linear interpolation)
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

			// Create renderer for the window
			renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (renderer == nullptr) {
				std::cerr << "Renderer could not be created! SDL Error:" << SDL_GetError() << std::endl;
				SDL_DestroyWindow(m_window);
			}

			else {
				// Initialize the color for each type of block
				blocktype_color[0] =	{ 0		, 255	, 255	, 255 };	// light blue
				blocktype_color[1] =	{ 255	, 255	, 0		, 255 };	// yellow
				blocktype_color[2] =	{ 153	, 0		, 255	, 255 };	// purple
				blocktype_color[3] =	{ 0		, 255	, 0		, 255 };	// green
				blocktype_color[4] =	{ 255	, 0		, 0		, 255 };	// red
				blocktype_color[5] =	{ 255	, 153	, 51	, 255 };	// orange
				blocktype_color[6] =	{ 0		, 0		, 255	, 255 };	// blue
				blocktype_color[7] =	{ 255	, 153	, 204	, 255 };	// rose
				blocktype_color[8] =	{ 0		, 0		, 102	, 255 };	// dark blue
				blocktype_color[9] =	{ 153	, 0		, 51	, 255 };	// dark red
				blocktype_color[10] =	{ 102	, 102	, 153	, 255 };	// purple-blue
				blocktype_color[11] =	{ 204	, 102	, 0		, 255 };	// rust
				blocktype_color[12] =	{ 102	, 153	, 153	, 255 };	// dirty water

				// Initialize textures
				background = IMG_LoadTexture(renderer, PathConstants::BACKGROUND_SPRITE_PATH.c_str());
				if (background == NULL) {
					std::cerr << "IMG_LoadTexture: cannot load background texture" << std::endl;
					std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
					exit(EXIT_FAILURE);
				}
				
				button = IMG_LoadTexture(renderer, PathConstants::BUTTON_SPRITE_PATH.c_str());
				if (button == NULL) {
					std::cerr << "IMG_LoadTexture: cannot load button texture" << std::endl;
					std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
					exit(EXIT_FAILURE);
				}

				big_panel = IMG_LoadTexture(renderer, PathConstants::BIG_PANEL_SPRITE_PATH.c_str());
				if (big_panel == NULL) {
					std::cerr << "IMG_LoadTexture: cannot load big panel texture" << std::endl;
					std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
					exit(EXIT_FAILURE);
				}

				small_panel = IMG_LoadTexture(renderer, PathConstants::SMALL_PANEL_SPRITE_PATH.c_str());
				if (small_panel == NULL) {
					std::cerr << "IMG_LoadTexture: cannot load small panel texture" << std::endl;
					std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
					exit(EXIT_FAILURE);
				}

				frame = IMG_LoadTexture(renderer, PathConstants::TITLE_PANEL_SPRITE_PATH.c_str());
				if (frame == NULL) {
					std::cerr << "IMG_LoadTexture: cannot load frame texture" << std::endl;
					std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
					exit(EXIT_FAILURE);
				}

				error_panel = IMG_LoadTexture(renderer, PathConstants::MSG_SPRITE_PATH.c_str());
				if (error_panel == NULL) {
					std::cerr << "IMG_LoadTexture: cannot load bubble messagge texture" << std::endl;
					std::cerr << "IMG_LoadTexture: " << IMG_GetError() << std::endl;
					exit(EXIT_FAILURE);
				}

				// Create font //
				const char *font_path = PathConstants::FONT_PATH.c_str();
				m_font = TTF_OpenFont(font_path, InGameUIConstants::TEXT_FONT_SIZE);

				if (m_font == nullptr) {
					std::cerr << "Font could not be loaded! TTF error: " << TTF_GetError() << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		}
		else
		{
			std::cerr << "Window could not be created! SDL Error:" << SDL_GetError() << std::endl;
		}

		return m_window != nullptr && renderer != nullptr;
	}

	/*****************************************************************************************************************************/

	void GameWindow::draw()
	{
		if (m_gamemanager->needGridRefresh())
		{
			drawGrid();
			m_gamemanager->setGridRefresh(false);
		}

		if (m_gamemanager->needFollowingblocksRefresh())
		{
			drawNextBlocks();
			m_gamemanager->setFollowingblocksRefresh(false);
		}

		if (m_gamemanager->needHoldblockRefresh())
		{
			drawHoldBlock();
			m_gamemanager->setHoldblockRefresh(false);
		}

		if (m_gamemanager->needScoreRefresh())
		{
			drawScore();
			m_gamemanager->setScoreRefresh(false);
		}

		if (m_gamemanager->needLevelRefresh())
		{
			drawLevel();
			m_gamemanager->setLevelRefresh(false);
		}

		SDL_RenderPresent(renderer);
	}

	/*****************************************************************************************************************************/

	void GameWindow::drawGrid()
	{
		int grid_w_pix = GridConstants::WIDTH_PIXELS;
		int grid_h_pix = GridConstants::HEIGHT_PIXELS;
		int grid_square_pix = GridConstants::CELL_PIXELS;

		int grid_x = GridConstants::X;
		int grid_y = GridConstants::Y;

		// Clean the grid //
		SDL_Rect refresh_rect = { GridConstants::X, GridConstants::Y, GridConstants::WIDTH_PIXELS, GridConstants::HEIGHT_PIXELS };
		SDL_SetRenderDrawColor(renderer, GridConstants::BACKGROUND_R, GridConstants::BACKGROUND_G, GridConstants::BACKGROUND_B, GridConstants::BACKGROUND_A);
		SDL_RenderFillRect(renderer, &refresh_rect);

		// Draw blocks //	
		SDL_Rect dest;
		int x(0), y(0);
		Uint8 r, g, b, a;

		for (int i = 0; i < m_gamemanager->getGrid()->getGrid().size(); i++) {

			int blocktype = m_gamemanager->getGrid()->getGrid()[i];

			// If the block is the current block controlled by the player, adapt his color
			if (blocktype == CURRENT_BLOCK)
				blocktype = m_gamemanager->getCurrentBlock()->getType();

			// If the square of the grid is not empty, fill it with the corresponding color
			if (blocktype != EMPTY) {
				x = grid_x + grid_square_pix * (i - GridConstants::WIDTH_CELLS * (i / GridConstants::WIDTH_CELLS));
				y = grid_y + grid_square_pix * (i / GridConstants::WIDTH_CELLS);

				r = blocktype_color[blocktype].r;
				g = blocktype_color[blocktype].g;
				b = blocktype_color[blocktype].b;
				a = blocktype_color[blocktype].a;

				dest = { x, y, grid_square_pix, grid_square_pix };

				SDL_SetRenderDrawColor(renderer, r, g, b, a);
				SDL_RenderFillRect(renderer, &dest);
			}
		}

		// Draw grid //
		SDL_SetRenderDrawColor(renderer, GridConstants::LINE_R, GridConstants::LINE_G, GridConstants::LINE_B, GridConstants::LINE_A);

		// Vertical
		for (int i = 0; i < grid_w_pix + grid_square_pix; i += grid_square_pix) {
			// Draw vertical lines centered
			SDL_RenderDrawLine(renderer, i + grid_x, grid_y, i + grid_x, grid_y + grid_h_pix);
		}

		// Horizontal
		for (int i = 0; i < grid_h_pix + grid_square_pix; i += grid_square_pix) {
			// Draw horizontal lines centered
			SDL_RenderDrawLine(renderer, grid_x, i, grid_x + grid_w_pix, i);
		}
	}

	/*****************************************************************************************************************************/

	void GameWindow::drawNextBlocks()
	{
		auto followingblocks = m_gamemanager->getFollowingblocks();
		Block *block;
		int blocktype;
		int data_mat_size;
		SDL_Rect dest;

		int grid_square_pix = GridConstants::CELL_PIXELS;
		int x, tmp_x;
		int y = InGameUIConstants::NEXT_BLOCKS_Y + 100;

		// Clean the following blocks //
		int w, h;
		SDL_QueryTexture(big_panel, NULL, NULL, &w, &h);
		SDL_Rect srcrect = { 0, 24, w, h };
		SDL_Rect dstrect = { m_next_rect.x, m_next_rect.y + InGameUIConstants::HEIGHT_PANEL_HEADER, m_next_rect.w, m_next_rect.h };

		SDL_RenderCopy(renderer, big_panel, &srcrect, &dstrect);

		while (!followingblocks.empty())
		{
			block = followingblocks.front().get();
			data_mat_size = (int)pow(block->getData().size(), 0.5);

			if (data_mat_size == 2)
				tmp_x = x = m_next_rect.x + m_next_rect.w / 2 - grid_square_pix;
			else if (data_mat_size == 3)
				tmp_x = x = m_next_rect.x + m_next_rect.w / 2 - (int)(grid_square_pix * 1.5);
			else
				tmp_x = x = m_next_rect.x + m_next_rect.w / 2 - grid_square_pix * 2;

			blocktype = block->getType();

			for (int row = 0; row < data_mat_size; row++) {
				for (int column = 0; column < data_mat_size; column++) {

					if (block->getData().at(column + data_mat_size * row) == true) {

						SDL_SetRenderDrawColor(renderer, blocktype_color[blocktype].r, blocktype_color[blocktype].g,
							blocktype_color[blocktype].b, blocktype_color[blocktype].a);
						dest = { x, y, grid_square_pix, grid_square_pix };
						SDL_RenderFillRect(renderer, &dest);

						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
						SDL_RenderDrawLine(renderer, x, y, x + grid_square_pix - 1, y);	// top
						SDL_RenderDrawLine(renderer, x, y + grid_square_pix, x + grid_square_pix - 1, y + grid_square_pix); // bottom
						SDL_RenderDrawLine(renderer, x, y, x, y + grid_square_pix);	// left
						SDL_RenderDrawLine(renderer, x + grid_square_pix, y, x + grid_square_pix, y + grid_square_pix);	// right
					}
					x += grid_square_pix;
				}
				x = tmp_x;
				y += grid_square_pix;
			}
			x = tmp_x;
			y += grid_square_pix;

			followingblocks.pop();
		}
	}

	/*****************************************************************************************************************************/

	void GameWindow::drawHoldBlock()
	{
		Block *block;
		int data_mat_size;
		SDL_Rect dest;
		int grid_square_pix = GridConstants::CELL_PIXELS;

		// Clean the old hold block //
		int w, h;
		SDL_QueryTexture(small_panel, NULL, NULL, &w, &h);
		SDL_Rect srcrect = { 0, 24, w, h };
		SDL_Rect dstrect = { m_hold_rect.x, m_hold_rect.y + InGameUIConstants::HEIGHT_PANEL_HEADER, m_hold_rect.w, m_hold_rect.h - InGameUIConstants::HEIGHT_PANEL_HEADER };

		SDL_RenderCopy(renderer, small_panel, &srcrect, &dstrect);

		// Draw the new hold block //
		block = m_gamemanager->getHoldBlock();
		data_mat_size = (int)pow(block->getData().size(), 0.5);

		int x, tmp_x;
		int y;

		if (block->getHeight() == 2)
			y = InGameUIConstants::HOLD_BLOCK_Y + (int)(grid_square_pix * 2.5);
		else if (block->getHeight() == 3)
			y = InGameUIConstants::HOLD_BLOCK_Y + (int)(grid_square_pix * 2.15);
		else
			y = InGameUIConstants::HOLD_BLOCK_Y + (int)(grid_square_pix * 1.65);

		if (data_mat_size == 2)
			tmp_x = x = m_hold_rect.x + m_hold_rect.w / 2 - grid_square_pix;
		else if (data_mat_size == 3)
			tmp_x = x = m_hold_rect.x + m_hold_rect.w / 2 - (int)(grid_square_pix * 1.5);
		else
			tmp_x = x = m_hold_rect.x + m_hold_rect.w / 2 - grid_square_pix * 2;

		int blocktype = block->getType();

		for (int row = 0; row < data_mat_size; row++) {
			for (int column = 0; column < data_mat_size; column++) {

				if (block->getData().at(column + data_mat_size * row) == true)
				{
					SDL_SetRenderDrawColor(renderer, blocktype_color[blocktype].r, blocktype_color[blocktype].g,
						blocktype_color[blocktype].b, blocktype_color[blocktype].a);
					dest = { x, y, grid_square_pix, grid_square_pix };
					SDL_RenderFillRect(renderer, &dest);	

					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderDrawLine(renderer, x, y, x + grid_square_pix - 1, y);	// top
					SDL_RenderDrawLine(renderer, x, y + grid_square_pix, x + grid_square_pix - 1, y + grid_square_pix); // bottom
					SDL_RenderDrawLine(renderer, x, y, x, y + grid_square_pix);	// left
					SDL_RenderDrawLine(renderer, x + grid_square_pix, y, x + grid_square_pix, y + grid_square_pix);	// right
				}
				x += grid_square_pix;
			}
			x = tmp_x;
			y += grid_square_pix;
		}
	}

	/*****************************************************************************************************************************/

	void GameWindow::drawScore()
	{
		int text_width, text_height;
		std::string score_string = std::to_string(m_gamemanager->getScore());
		const char *score_txt = score_string.c_str();
		TTF_SizeText(m_font, score_txt, &text_width, &text_height);

		SDL_Rect refresh_rect = { m_score_rect.x + m_score_rect.w / 2 - text_width / 2, m_score_rect.y + InGameUIConstants::HEIGHT_PANEL_HEADER + text_height / 2, text_width, text_height };

		// Draw score
		Text::displayText(refresh_rect, *m_font, { 0, 0, 0 }, score_txt, { 214, 221, 231 });
	}

	/*****************************************************************************************************************************/

	void GameWindow::drawLevel()
	{
		int text_width, text_height;
		std::string level_string = std::to_string(m_gamemanager->getLevel());
		const char *level_txt = level_string.c_str();
		TTF_SizeText(m_font, level_txt, &text_width, &text_height);

		SDL_Rect refresh_rect = { m_level_rect.x + m_level_rect.w / 2 - text_width / 2, m_level_rect.y + InGameUIConstants::HEIGHT_PANEL_HEADER + text_height / 2, text_width, text_height };

		// Draw level
		Text::displayText(refresh_rect, *m_font, { 0, 0, 0 }, level_txt, { 214, 221, 231 });
	}

	/*****************************************************************************************************************************/

	void GameWindow::firstDraw()
	{
		// Draw hold panel //
		int text_width, text_height;
		TTF_SizeText(m_font, InGameUIConstants::HOLD_BLOCK_TEXT.c_str(), &text_width, &text_height);

		int grid_square_pix = GridConstants::CELL_PIXELS;
		int w, h;
		SDL_QueryTexture(small_panel, NULL, NULL, &w, &h);

		m_hold_rect = { GridConstants::X / 2 - (grid_square_pix * GameConstants::MAX_BLOCK_SIZE + grid_square_pix) / 2, 25, grid_square_pix * GameConstants::MAX_BLOCK_SIZE + grid_square_pix, InGameUIConstants::HEIGHT_PANEL_HEADER + grid_square_pix * GameConstants::MAX_BLOCK_SIZE + grid_square_pix / 2 };
		
		SDL_Rect srcrect =  { 0, 24, w, h };
		SDL_Rect dstrect = { m_hold_rect.x, m_hold_rect.y + InGameUIConstants::HEIGHT_PANEL_HEADER, m_hold_rect.w, m_hold_rect.h - InGameUIConstants::HEIGHT_PANEL_HEADER };

		SDL_RenderCopy(renderer, small_panel, &srcrect, &dstrect);

		// Draw hold header //
		srcrect = { 0, 0, w, 24 };
		dstrect = { m_hold_rect.x, m_hold_rect.y, m_hold_rect.w, InGameUIConstants::HEIGHT_PANEL_HEADER };
		SDL_RenderCopy(renderer, small_panel, &srcrect, &dstrect);

		// Draw hold text //
		Text::displayText({ m_hold_rect.x + m_hold_rect.w / 2 - text_width / 2, InGameUIConstants::HOLD_BLOCK_Y, text_width, text_height }
			, *m_font, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }, InGameUIConstants::HOLD_BLOCK_TEXT.c_str());


		// Draw next header //
		TTF_SizeText(m_font, InGameUIConstants::NEXT_BLOCK_TEXT.c_str(), &text_width, &text_height);
		m_next_rect = { (GridConstants::X + GridConstants::WIDTH_PIXELS + (WindowConstants::WIDTH - (GridConstants::X + GridConstants::WIDTH_PIXELS)) / 2) - (grid_square_pix * GameConstants::MAX_BLOCK_SIZE + grid_square_pix) / 2
			, 25, grid_square_pix * GameConstants::MAX_BLOCK_SIZE + grid_square_pix, grid_square_pix * 14 + grid_square_pix };
		
		dstrect = { m_next_rect.x, m_next_rect.y, m_next_rect.w, InGameUIConstants::HEIGHT_PANEL_HEADER };

		SDL_RenderCopy(renderer, big_panel, &srcrect, &dstrect);

		// Draw next text //
		Text::displayText({ m_next_rect.x + m_next_rect.w / 2 - text_width / 2, InGameUIConstants::NEXT_BLOCKS_Y, text_width, text_height }
			, *m_font, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }, InGameUIConstants::NEXT_BLOCK_TEXT.c_str());


		// Draw score header //
		TTF_SizeText(m_font, InGameUIConstants::SCORE_TEXT.c_str(), &text_width, &text_height);

		m_score_rect = { m_hold_rect.x, WindowConstants::HEIGHT / 2, m_hold_rect.w, (int)(m_hold_rect.h * 0.65) };
		dstrect = { m_score_rect.x, m_score_rect.y, m_score_rect.w, InGameUIConstants::HEIGHT_PANEL_HEADER };

		SDL_RenderCopy(renderer, small_panel, &srcrect, &dstrect);

		// Draw score panel //
		dstrect = { m_score_rect.x, m_score_rect.y + InGameUIConstants::HEIGHT_PANEL_HEADER , m_score_rect.w, m_score_rect.h - InGameUIConstants::HEIGHT_PANEL_HEADER };
		srcrect = { 0, 24, w, h };
		
		SDL_RenderCopy(renderer, small_panel, &srcrect, &dstrect);

		// Draw score text //
		dstrect = { m_score_rect.x + m_score_rect.w / 2 - text_width / 2, m_score_rect.y + text_height / 2, text_width, text_height };
		Text::displayText(dstrect, *m_font, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }
			, InGameUIConstants::SCORE_TEXT.c_str());

		// Draw score //
		std::string score_string = std::to_string(m_gamemanager->getScore());
		const char *score_txt = score_string.c_str();
		TTF_SizeText(m_font, score_txt, &text_width, &text_height);

		dstrect = { m_score_rect.x + m_score_rect.w / 2 - text_width / 2, m_score_rect.y + InGameUIConstants::HEIGHT_PANEL_HEADER + text_height / 2, text_width, text_height };

		Text::displayText(dstrect, *m_font, { 0, 0, 0 }, score_txt);


		// Draw level header //
		TTF_SizeText(m_font, InGameUIConstants::LEVEL_TEXT.c_str(), &text_width, &text_height);

		m_level_rect = { m_score_rect.x, m_score_rect.y + m_score_rect.h + 50, m_score_rect.w, m_score_rect.h };
		srcrect = { 0, 0, w, 24 };
		dstrect = { m_level_rect.x, m_level_rect.y, m_level_rect.w, InGameUIConstants::HEIGHT_PANEL_HEADER };

		SDL_RenderCopy(renderer, small_panel, &srcrect, &dstrect);

		// Draw level panel //
		srcrect = { 0, 24, w, h };
		dstrect.y += dstrect.h;

		SDL_RenderCopy(renderer, small_panel, &srcrect, &dstrect);

		// Draw level text //
		dstrect = { m_level_rect.x + m_level_rect.w / 2 - text_width / 2, m_level_rect.y + text_height / 2, text_width, text_height };
		Text::displayText(dstrect, *m_font, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }
			, InGameUIConstants::LEVEL_TEXT.c_str());

		// Draw level //
		std::string level_string = std::to_string(m_gamemanager->getLevel());
		const char *level_txt = level_string.c_str();
		TTF_SizeText(m_font, level_txt, &text_width, &text_height);

		dstrect = { m_level_rect.x + m_level_rect.w / 2 - text_width / 2, m_level_rect.y + InGameUIConstants::HEIGHT_PANEL_HEADER + text_height / 2, text_width, text_height };

		Text::displayText(dstrect, *m_font, { 0, 0, 0 }, level_txt);


		SDL_RenderPresent(renderer);
	}

	/*****************************************************************************************************************************/
	/*****************************************************************************************************************************/

	// Getters
	SDL_Window* GameWindow::getWindow() const { return m_window; }
	SDL_Renderer* GameWindow::getRenderer() const { return renderer; }
	int GameWindow::getWidth() const { return m_width; }
	int GameWindow::getHeight()	const { return m_height; }
	// Setter
	void GameWindow::setGameManager(GameManager *gamemanager) { m_gamemanager = gamemanager; }
}
