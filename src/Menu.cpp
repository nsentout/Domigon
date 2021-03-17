#include <iostream>
#include <string>

#include <SDL_image.h>

#include "constants.h"
#include "Menu.h"

namespace Domigon
{
	Menu::Menu() : m_pressed_button(nullptr), m_font(nullptr), m_hovered_font(nullptr)
	{}


	Menu::~Menu() {
		for (auto button : m_buttons)
			delete button;
	}

	/*****************************************************************************************************************************/

	void Menu::createMainMenu()
	{
		// Create font //
		const char *font_path = PathConstants::FONT_PATH.c_str();
		m_font = TTF_OpenFont(font_path, MenuConstants::BUTTON_TEXT_FONT_SIZE);
		m_hovered_font = TTF_OpenFont(font_path, MenuConstants::BUTTON_TEXT_FONT_SIZE_HOVER);

		if (m_font == nullptr || m_hovered_font == nullptr) {
			std::cerr << "Font could not be loaded! TTF error: " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		// Set menu buttons //
		int y = MenuConstants::Y;
		int shift = MenuConstants::BUTTON_SHIFT;
		int text_width, text_height;

		TTF_SizeText(m_font, MenuConstants::PLAY_BUTTON.c_str(), &text_width, &text_height);
		Button *button = new Button(MenuConstants::PLAY_BUTTON, { WindowConstants::WIDTH / 2 - text_width / 2, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);

		y += text_height + shift;
		TTF_SizeText(m_font, MenuConstants::EDIT_BLOCKS_BUTTON.c_str(), &text_width, &text_height);
		button = new Button(MenuConstants::EDIT_BLOCKS_BUTTON, { WindowConstants::WIDTH / 2 - text_width / 2, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);

		y += text_height + shift;
		TTF_SizeText(m_font, MenuConstants::SCORES_BUTTON.c_str(), &text_width, &text_height);
		button = new Button(MenuConstants::SCORES_BUTTON,{ WindowConstants::WIDTH / 2 - text_width / 2, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);

		y += text_height + shift;
		TTF_SizeText(m_font, MenuConstants::EXIT_BUTTON.c_str(), &text_width, &text_height);
		button = new Button(MenuConstants::EXIT_BUTTON,{ WindowConstants::WIDTH / 2 - text_width / 2, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);
	}

	/*****************************************************************************************************************************/

	void Menu::createPauseMenu()
	{
		// Create font //
		const char *font_path = PathConstants::FONT_PATH.c_str();
		m_font = TTF_OpenFont(font_path, MenuConstants::BUTTON_TEXT_FONT_SIZE);
		m_hovered_font = TTF_OpenFont(font_path, MenuConstants::BUTTON_TEXT_FONT_SIZE_HOVER);

		if (m_font == nullptr || m_hovered_font == nullptr) {
			std::cerr << "Font could not be loaded! TTF error: " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}
		
		// Set menu options's rect //
		int y = MenuConstants::Y;
		int shift = MenuConstants::BUTTON_SHIFT;
		int text_width, text_height;

		TTF_SizeText(m_font, MenuConstants::RESUME_BUTTON.c_str(), &text_width, &text_height);
		Button *button = new Button(MenuConstants::RESUME_BUTTON, { WindowConstants::WIDTH / 2 - text_width / 2, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);

		y += text_height + shift;
		TTF_SizeText(m_font, MenuConstants::EXIT_TO_MENU_BUTTON.c_str(), &text_width, &text_height);
		button = new Button(MenuConstants::EXIT_TO_MENU_BUTTON, { WindowConstants::WIDTH / 2 - text_width / 2, y, text_width, text_height }, m_font, m_hovered_font);
		button->init();
		m_buttons.push_back(button);
	}

	/*****************************************************************************************************************************/

	void Menu::display()
	{
		// Clean the window //
		SDL_Rect dstrect = { 0, 0, WindowConstants::WIDTH, WindowConstants::HEIGHT };
		SDL_RenderCopy(GameWindow::renderer, GameWindow::background, NULL, &dstrect);

		// Draw menu title //
		TTF_Font *font = TTF_OpenFont(PathConstants::TITLE_FONT_PATH.c_str(), MenuConstants::TITLE_FONT_SIZE);

		int title_width, title_height;
		TTF_SizeText(font, MenuConstants::TITLE.c_str(), &title_width, &title_height);
		SDL_Rect title_rect = { WindowConstants::WIDTH / 2 - title_width / 2, 100, title_width, title_height };
		SDL_Color font_color = { MenuConstants::FONT_R, MenuConstants::FONT_G, MenuConstants::FONT_B, MenuConstants::FONT_A };

		int enlargement = MenuConstants::BUTTON_HOVERED_ENLARGEMENT * 2;
		dstrect = { title_rect.x - enlargement / 2 - 5, title_rect.y - enlargement / 2 - 5, title_rect.w + enlargement, title_rect.h + enlargement };
		SDL_RenderCopy(GameWindow::renderer, GameWindow::frame, NULL, &dstrect);

		Text::displayText(title_rect, *font, font_color, MenuConstants::TITLE.c_str());
		TTF_CloseFont(font);

		// Draw buttons //
		for (auto button : m_buttons)
			button->draw();

		SDL_RenderPresent(GameWindow::renderer);

		// Menu actions //
		Button *previous_hovered_button = nullptr;
		
		int timeElapsed = SDL_GetTicks();

		// While the player doesn't click on a button
		while (m_pressed_button == nullptr) {
			if (SDL_GetTicks() > (timeElapsed + (Uint32)(1000 / WindowConstants::FPS_LIMIT))) {
				handleMouseEvents();
				timeElapsed = SDL_GetTicks();
			}
		}

		// Close menu fonts
		TTF_CloseFont(m_font);
		TTF_CloseFont(m_hovered_font);

		// Clear the window entirely
		SDL_RenderCopy(GameWindow::renderer, GameWindow::background, NULL, NULL);
	}

	/*****************************************************************************************************************************/

	void Menu::handleMouseEvents()
	{
		// Event handler
		SDL_Event e;

		int mouse_x, mouse_y;
		bool hovering = false;

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
							std::cout << "click on " << button->getText() << std::endl;
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
	/*****************************************************************************************************************************/

	// Getter
	Button* Menu::getPressedButton() const { return m_pressed_button; }


}
