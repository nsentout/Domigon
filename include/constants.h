#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace GameConstants
{
	// Minimum number of blocks present in the game
	const int BLOCKS_NUMBER_MINIMUM = 6;

	// Maximum number of blocks present in the game
	const int BLOCKS_NUMBER_MAXIMUM = 13;

	// Number of following blocks
	const int NB_FOLLOWING_BLOCKS = 3;

	// Number of seconds before the block moves down at level 1
	const float SPEED = 1.0f;

	// Minimum size of a block
	const int MIN_BLOCK_SIZE = 2;

	// Maximum size of a block
	const int MAX_BLOCK_SIZE = 4;

	// Number of lines that needs to be cleared to go to the next level
	const int CLEARED_LINES_REQUIRED_LEVEL_CHANGE = 1;

	// Number of score granted for clearing one line at level 1
	const int SCORE_PER_1_LINE_CLEARED = 40;

	// Number of score granted for clearing two lines at level 1
	const int SCORE_PER_2_LINES_CLEARED = 100;

	// Number of score granted for clearing three lines at level 1
	const int SCORE_PER_3_LINES_CLEARED = 300;

	// Number of score granted for clearing four lines at level 1
	const int SCORE_PER_4_LINES_CLEARED = 1200;
}

namespace WindowConstants
{
	// Window dimensions
	const int WIDTH = 1200;
	const int HEIGHT = 1000;

	// FPS limit
	const int FPS_LIMIT = 30;
}

namespace GridConstants
{
	// Number of cells composing the grid
	const int WIDTH_CELLS = 10;
	const int HEIGHT_CELLS = 20;

	// Size of a grid's cell in pixels
	const int CELL_PIXELS = 50;

	// Grid's dimension in pixels
	const int WIDTH_PIXELS = WIDTH_CELLS * CELL_PIXELS;
	const int HEIGHT_PIXELS = HEIGHT_CELLS * CELL_PIXELS;

	// Coordinates of the grid
	const int X = WindowConstants::WIDTH / 2 - GridConstants::WIDTH_PIXELS / 2;
	const int Y = 0;

	/*const int R = 31;
	const int G = 75;
	const int B = 187;
	const int A = 255;*/

	// Color of the grid's background
	const int BACKGROUND_R = 0;
	const int BACKGROUND_G = 0;
	const int BACKGROUND_B = 0;
	const int BACKGROUND_A = 255;

	// Color of the grid's lines
	const int LINE_R = 255;
	const int LINE_G = 255;
	const int LINE_B = 255;
	const int LINE_A = 255;
}

namespace InGameUIConstants
{
	// Hold block text
	const std::string HOLD_BLOCK_TEXT = "HOLD";

	// Following blocks text
	const std::string NEXT_BLOCK_TEXT = "NEXT";

	// Score text
	const std::string SCORE_TEXT = "SCORE";

	// Level text
	const std::string LEVEL_TEXT = "LEVEL";

	// Size of the text font
	const int TEXT_FONT_SIZE = 50;

	// Height of the panels header
	const int HEIGHT_PANEL_HEADER = 104;

	// Coordinates of the hold block text
	const int HOLD_BLOCK_Y = GridConstants::Y + 50;

	// Coordinates of the next blocks text
	const int NEXT_BLOCKS_Y = GridConstants::Y + 50;
}

namespace MenuConstants
{
	// Size of the menu title font
	const int TITLE_FONT_SIZE = 100;

	// Size of the button text font
	const int BUTTON_TEXT_FONT_SIZE = 55;

	// Size of the button text font when mouse hovers
	const int BUTTON_TEXT_FONT_SIZE_HOVER = 65;

	// Font color
	const int FONT_R = 230;
	const int FONT_G = 230;
	const int FONT_B = 230;
	const int FONT_A = 255;

	// Ordinate of the first button
	const int Y = 400;

	// Shift in pixels between each button
	const int BUTTON_SHIFT = 50;

	// Number of pixels between the borders of the button image and the button text
	const int BUTTON_ENLARGEMENT = 20;

	// Number of pixels between the borders of the hovered button image and the hovered button text
	const int BUTTON_HOVERED_ENLARGEMENT = 35;

	// Menu title
	const std::string TITLE = "DOMIGON";

	// Play button text
	const std::string PLAY_BUTTON = "PLAY";

	// Edit blocks button text
	const std::string EDIT_BLOCKS_BUTTON = "EDIT BLOCKS";

	// Exit button text
	const std::string EXIT_BUTTON = "EXIT";

	// Resume button text
	const std::string RESUME_BUTTON = "RESUME";

	// Exit to menu button text
	const std::string EXIT_TO_MENU_BUTTON = "EXIT TO MENU";
}

namespace BlockEditorConstants
{
	// Size of the menu title font
	const int TITLE_FONT_SIZE = 80;

	// Size of the button text font
	const int BUTTON_TEXT_FONT_SIZE = 30;

	// Size of the button text font when mouse hover
	const int BUTTON_TEXT_FONT_SIZE_HOVER = 40;

	// Block editor title
	const std::string TITLE = "EXISTING BLOCKS";

	// New block button text
	const std::string NEW_BLOCK_BUTTON = "Create new block";

	// Delete a block button text
	const std::string DELETE_BLOCK_BUTTON = "Delete a block";

	// Back button text
	const std::string BACK_BUTTON = "Back";

	// Create block button text
	const std::string CREATE_BUTTON = "Create";

	// Cancel button text
	const std::string CANCEL_BUTTON = "Cancel";

	// Shape block text
	const std::string SHAPE_BLOCK_TEXT = "Click on squares to shape a block";

	// Block added text
	const std::string ADDED_BLOCK_TEXT = "A new block has been added to the game!";

	// Block size text
	const std::string BLOCK_SIZE_TEXT = "Block size";

	// Delete block text
	const std::string DELETE_BLOCK_TEXT = "Click on the block you want to delete";

	// Error message minimum blocks
	const std::string ERROR_MIN_BLOCKS_TEXT = "Can't delete this block, you need at least 6 blocks to play!";

	// Error message maximum blocks
	const std::string ERROR_MAX_BLOCKS_TEXT = "Can't create more blocks!";

	// Error message block size
	const std::string ERROR_BLOCK_SIZE = "This block is too small!";

	// Error message only one block
	const std::string ERROR_MULTIPLE_BLOCKS = "There must be only one block in the grid!";

	// Size of a grid's cell (in pixels)
	const int GRID_CELL_PIXELS = 150;

	// Hovered cell in block creation scene color
	const int HOVERED_CELL_R = 0;
	const int HOVERED_CELL_G = 127;
	const int HOVERED_CELL_B = 127;
	const int HOVERED_CELL_A = 255;

	// Clicked cell in block creation scene color
	const int CLICKED_CELL_R = 0;
	const int CLICKED_CELL_G = 127;
	const int CLICKED_CELL_B = 200;
	const int CLICKED_CELL_A = 255;

	// Coordinates of the first block displayed in the main scene
	const int EXISTING_BLOCKS_X = 50;
	const int EXISTING_BLOCKS_Y = 210;

	// Size of a cell in the main scene
	const int CELL_PIXELS = (int)(GridConstants::CELL_PIXELS * 0.8);

	// Size of a hovered cell in the main scene
	const int HOVERED_CELL_PIXELS = (int)(GridConstants::CELL_PIXELS * 0.9);

	// Time before error messages start fading out (in ms)
	const int TIME_BEFORE_FADE_OUT = 2000;
}

namespace PathConstants
{
	// Saved file name
	const std::string SAVED_FILE_NAME = "blocks.dgon";

	// Text Font
	const std::string FONT_PATH = "font/arcade_classic_mod.ttf";

	// Title Font
	const std::string TITLE_FONT_PATH = "font/atari.ttf";

	// Title panel
	const std::string TITLE_PANEL_SPRITE_PATH = "img/panel-title.png";
	
	// Background sprite
	const std::string BACKGROUND_SPRITE_PATH = "img/background.jpg";

	// Button sprite
	const std::string BUTTON_SPRITE_PATH = "img/button.png";

	// Big panel sprite
	const std::string BIG_PANEL_SPRITE_PATH = "img/panel-big.png";

	// Small panel sprite
	const std::string SMALL_PANEL_SPRITE_PATH = "img/panel-small.png";

	// Error panel sprite
	const std::string MSG_SPRITE_PATH = "img/panel-error.png";
}

#endif