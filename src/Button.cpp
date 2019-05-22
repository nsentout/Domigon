#include <iostream>
#include <string>

#include "Button.h"
#include "constants.h"
#include "ResourceManager.h"

namespace Domigon
{

	Button::Button(std::string text, SDL_Rect rect, TTF_Font *font, TTF_Font *hovered_font)
		: m_text(text), m_text_rect(rect), m_font(font), m_hovered_font(hovered_font), m_block(nullptr)
	{
		
	}

	Button::Button(Block *block, int x, int y)
		: m_block(block)
	{
		int data_mat_size = (int)pow(m_block->getData().size(), 0.5);
		m_img_rect = { x, y, data_mat_size * BlockEditorConstants::CELL_PIXELS + 1, data_mat_size * BlockEditorConstants::CELL_PIXELS + 1 };
		m_img_hovered_rect = { x - (data_mat_size * BlockEditorConstants::HOVERED_CELL_PIXELS - m_img_rect.w) / 2
			, y - (data_mat_size * BlockEditorConstants::HOVERED_CELL_PIXELS - m_img_rect.h) / 2
			, data_mat_size * BlockEditorConstants::HOVERED_CELL_PIXELS + 2, data_mat_size * BlockEditorConstants::HOVERED_CELL_PIXELS + 2 };
	}

	Button::~Button()
	{}

	/*****************************************************************************************************************************/

	void Button::init()
	{
		// Default button //
		if (m_block == nullptr)
		{
			int text_width, text_height;
			TTF_SizeText(m_hovered_font, m_text.c_str(), &text_width, &text_height);

			m_text_hovered_rect = { (m_text_rect.x - (text_width - m_text_rect.w) / 2) , (m_text_rect.y - (text_height - m_text_rect.h) / 2),
				text_width , text_height };

			m_img_rect = { (int)(m_text_rect.x - MenuConstants::BUTTON_ENLARGEMENT / 2), (int)(m_text_rect.y - MenuConstants::BUTTON_ENLARGEMENT / 2)
				, (int)(m_text_rect.w + MenuConstants::BUTTON_ENLARGEMENT), (int)(m_text_rect.h + MenuConstants::BUTTON_ENLARGEMENT) };

			m_img_hovered_rect = { (m_text_rect.x - (text_width - m_text_rect.w) / 2) - MenuConstants::BUTTON_HOVERED_ENLARGEMENT / 2
				, (m_text_rect.y - (text_height - m_text_rect.h) / 2) - MenuConstants::BUTTON_HOVERED_ENLARGEMENT / 2
				, text_width + MenuConstants::BUTTON_HOVERED_ENLARGEMENT, text_height + MenuConstants::BUTTON_HOVERED_ENLARGEMENT };

			int w, h;
			SDL_QueryTexture(GameWindow::background, NULL, NULL, &w, &h);	// get background size

			float scale_w = (float)w / WindowConstants::WIDTH;
			float scale_h = (float)h / WindowConstants::HEIGHT;

			m_refresh_rect = { (int)(m_img_hovered_rect.x * scale_w), (int)(m_img_hovered_rect.y * scale_h), (int)(m_img_hovered_rect.w * scale_w), (int)(m_img_hovered_rect.h * scale_h) };
		}

		// Block button
		else
		{
			int w, h;
			SDL_QueryTexture(GameWindow::background, NULL, NULL, &w, &h);	// get background size

			float scale_w = (float)w / WindowConstants::WIDTH;
			float scale_h = (float)h / WindowConstants::HEIGHT;

			m_refresh_rect = { (int)(m_img_hovered_rect.x * scale_w), (int)(m_img_hovered_rect.y * scale_h), (int)(m_img_hovered_rect.w * scale_w), (int)(m_img_hovered_rect.h * scale_h) };
		}
	}

	/*****************************************************************************************************************************/

	bool Button::handleButtonHovering(SDL_Event e)
	{
		int mouse_x = e.motion.x;
		int mouse_y = e.motion.y;

		// If a button is hovered, enlarge it
		if (!m_hovered)
		{
			if (mouse_x > m_img_rect.x && mouse_x < m_img_rect.x + m_img_rect.w
				&& mouse_y > m_img_rect.y && mouse_y < m_img_rect.y + m_img_rect.h)
			{
				drawEnlarged();
				m_hovered = true;
				SDL_RenderPresent(GameWindow::renderer);

				ResourceManager::Instance().playSound(MOUSEHOVER_BUTTON_SOUND_2);

				return true;
			}
		}
		// If the button was hovered and is not anymore, put it back to the default size
		else
		{
			if (mouse_x < m_img_hovered_rect.x || mouse_x > m_img_hovered_rect.x + m_img_hovered_rect.w
				|| mouse_y < m_img_hovered_rect.y || mouse_y > m_img_hovered_rect.y + m_img_hovered_rect.h)
			{
				draw();
				m_hovered = false;
				SDL_RenderPresent(GameWindow::renderer);
				return true;
			}
		}
		return false;
	}

	/*****************************************************************************************************************************/

	void Button::drawEnlarged() const
	{
		// Draw default button //
		if (m_block == nullptr)
		{
			// Clean the non hovered button
			SDL_RenderCopy(GameWindow::renderer, GameWindow::button, NULL, &m_img_hovered_rect);

			// Draw text
			Text::displayText(m_text_hovered_rect, *m_hovered_font, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }, m_text.c_str());
		}

		// Draw block button //
		else
		{
			// Clear the non hovered button //
			SDL_RenderCopy(GameWindow::renderer, GameWindow::background, &m_refresh_rect, &m_img_hovered_rect);

			// Draw block //
			int blocktype = m_block->getType();
			int data_mat_size = (int)pow(m_block->getData().size(), 0.5);
			int grid_square_pix = BlockEditorConstants::HOVERED_CELL_PIXELS;
			SDL_Rect dest;
			int x(m_img_hovered_rect.x), block_x(x);
			int y(m_img_hovered_rect.y), block_y(y);

			SDL_SetRenderDrawColor(GameWindow::renderer, GameWindow::blocktype_color[blocktype].r, GameWindow::blocktype_color[blocktype].g,
				GameWindow::blocktype_color[blocktype].b, GameWindow::blocktype_color[blocktype].a);

			for (int row = 0; row < data_mat_size; row++) {
				for (int column = 0; column < data_mat_size; column++) {

					if (m_block->getData().at(column + data_mat_size * row) == true)
					{
						dest = { x, y, grid_square_pix, grid_square_pix };
						SDL_RenderFillRect(GameWindow::renderer, &dest);
					}
					x += grid_square_pix;
				}
				x -= grid_square_pix * data_mat_size;
				y += grid_square_pix;
			}

			// Draw block grid //
			SDL_SetRenderDrawColor(GameWindow::renderer, GridConstants::LINE_R, GridConstants::LINE_G, GridConstants::LINE_B, GridConstants::LINE_A);

			for (int i = 0; i < grid_square_pix * data_mat_size + 1; i += grid_square_pix) {
				SDL_RenderDrawLine(GameWindow::renderer, i + block_x, block_y, i + block_x, block_y + grid_square_pix * data_mat_size);	// vertical
				SDL_RenderDrawLine(GameWindow::renderer, block_x, block_y + i, block_x + grid_square_pix * data_mat_size, block_y + i);	// horizontal
				SDL_RenderDrawLine(GameWindow::renderer, i + block_x + 1, block_y, i + block_x + 1, block_y + grid_square_pix * data_mat_size);	// vertical + 1 pix
				SDL_RenderDrawLine(GameWindow::renderer, block_x, block_y + i + 1, block_x + grid_square_pix * data_mat_size, block_y + i + 1);	// horizontal + 1 pix
			}
		}
	}

	/*****************************************************************************************************************************/

	void Button::draw() const
	{
		// Draw default button //
		if (m_block == nullptr) {
			// Clean the hovered button
			SDL_RenderCopy(GameWindow::renderer, GameWindow::background, &m_refresh_rect, &m_img_hovered_rect);

			// Draw the non hovered button
			SDL_RenderCopy(GameWindow::renderer, GameWindow::button, NULL, &m_img_rect);

			// Draw text
			Text::displayText(m_text_rect, *m_font, { MenuConstants::FONT_R,  MenuConstants::FONT_G,  MenuConstants::FONT_B }, m_text.c_str());
		}
		
		// Draw block //
		else {
			// Clear the hovered block //
			SDL_RenderCopy(GameWindow::renderer, GameWindow::background, &m_refresh_rect, &m_img_hovered_rect);

			// Draw block //
			int blocktype = m_block->getType();
			int data_mat_size = (int)pow(m_block->getData().size(), 0.5);
			int grid_square_pix = BlockEditorConstants::CELL_PIXELS;
			SDL_Rect dest;
			int x(m_img_rect.x), block_x(x);
			int y(m_img_rect.y), block_y(y);

			SDL_SetRenderDrawColor(GameWindow::renderer, GameWindow::blocktype_color[blocktype].r, GameWindow::blocktype_color[blocktype].g,
				GameWindow::blocktype_color[blocktype].b, GameWindow::blocktype_color[blocktype].a);

			for (int row = 0; row < data_mat_size; row++) {
				for (int column = 0; column < data_mat_size; column++) {

					if (m_block->getData().at(column + data_mat_size * row) == true)
					{
						dest = { x, y, grid_square_pix, grid_square_pix };
						SDL_RenderFillRect(GameWindow::renderer, &dest);
					}
					x += grid_square_pix;
				}
				x -= grid_square_pix * data_mat_size;
				y += grid_square_pix;
			}

			// Draw block grid //
			SDL_SetRenderDrawColor(GameWindow::renderer, GridConstants::LINE_R, GridConstants::LINE_G, GridConstants::LINE_B, GridConstants::LINE_A);

			for (int i = 0; i < grid_square_pix * data_mat_size + 1; i += grid_square_pix) {
				SDL_RenderDrawLine(GameWindow::renderer, i + block_x, block_y, i + block_x, block_y + grid_square_pix * data_mat_size);	// vertical
				SDL_RenderDrawLine(GameWindow::renderer, block_x, block_y + i, block_x + grid_square_pix * data_mat_size, block_y + i);	// horizontal
				SDL_RenderDrawLine(GameWindow::renderer, i + block_x + 1, block_y, i + block_x + 1, block_y + grid_square_pix * data_mat_size);	// vertical + 1 pix
				SDL_RenderDrawLine(GameWindow::renderer, block_x, block_y + i + 1, block_x + grid_square_pix * data_mat_size, block_y + i + 1);	// horizontal + 1 pix
			}
		}
	}

	/*****************************************************************************************************************************/

	// Getters
	std::string Button::getText() const { return m_text; }
	SDL_Rect Button::getRect() const { return m_img_rect;  }
	SDL_Rect Button::getHoveredRect() const { return m_img_hovered_rect;  }
	Block *Button::getBlock() const { return m_block; }
	bool Button::isHovered() const { return m_hovered; }

	// Setter
	void Button::setHovered(bool is_hovered) { m_hovered = is_hovered; }
}
