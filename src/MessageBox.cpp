#include "MessageBox.h"
#include "GameWindow.h"

namespace Domigon
{
	MessageBox::MessageBox(const SDL_Rect dstrect, const std::string msg) : m_dstrect(dstrect), m_msg(msg)
	{}

	MessageBox::~MessageBox()
	{
		TTF_CloseFont(m_font);
	}

	/*****************************************************************************************************************************/

	void MessageBox::display()
	{
		int w, h;
		SDL_QueryTexture(GameWindow::background, NULL, NULL, &w, &h);

		float scale_w = (float)w / WindowConstants::WIDTH;
		float scale_h = (float)h / WindowConstants::HEIGHT;

		m_srcrect = { (int)(m_dstrect.x * scale_w), (int)(m_dstrect.y * scale_h), (int)(m_dstrect.w * scale_w), (int)(m_dstrect.h * scale_h) };
		m_alpha = 255;

		SDL_SetTextureBlendMode(GameWindow::error_panel, SDL_BLENDMODE_BLEND);
		SDL_RenderCopy(GameWindow::renderer, GameWindow::error_panel, NULL, &m_dstrect);

		m_font = TTF_OpenFont(PathConstants::TITLE_FONT_PATH.c_str(), 15);

		int text_width, text_height;
		TTF_SizeText(m_font, m_msg.c_str(), &text_width, &text_height);

		m_msgrect = { m_dstrect.x + m_dstrect.w / 8, m_dstrect.y + m_dstrect.h / 2 - text_height / 2, text_width, text_height };
		Text::displayText(m_msgrect, *m_font, { MenuConstants::FONT_R, MenuConstants::FONT_G, MenuConstants::FONT_B }, m_msg.c_str(), m_alpha);

		SDL_RenderPresent(GameWindow::renderer);
	}

	/*****************************************************************************************************************************/

	bool MessageBox::fadeOut()
	{
		if (m_alpha - 5 <= 0)
			return false;

		m_alpha -= 5;
		SDL_SetTextureAlphaMod(GameWindow::error_panel, m_alpha);

		SDL_RenderCopy(GameWindow::renderer, GameWindow::background, &m_srcrect, &m_dstrect);
		SDL_RenderCopy(GameWindow::renderer, GameWindow::error_panel, NULL, &m_dstrect);

		Text::displayText(m_msgrect, *m_font, { MenuConstants::FONT_R, MenuConstants::FONT_G, MenuConstants::FONT_B }, m_msg.c_str(), m_alpha);

		SDL_RenderPresent(GameWindow::renderer);

		return true;
	}

	/*****************************************************************************************************************************/

	void MessageBox::erase()
	{
		SDL_RenderCopy(GameWindow::renderer, GameWindow::background, &m_srcrect, &m_dstrect);
		SDL_RenderPresent(GameWindow::renderer);
		SDL_SetTextureAlphaMod(GameWindow::error_panel, 255);
	}
}
