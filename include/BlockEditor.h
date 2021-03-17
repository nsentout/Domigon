#ifndef BLOCKEDITOR_H
#define BLOCKEDITOR_H

#include <array>
#include <list>

#include "Button.h"

#include "MessageBox.h"

namespace Domigon
{
	/* Represents a cell in the grid from the block creation scene. */
	struct Cell
	{
		std::list<Cell*> neighbors;
		bool filled;
		bool visited;
		Cell() : neighbors(), filled(false), visited(false) {}
	};


	/* Represents the interface that allows the player to create a new block to add into the game and to remove an existing one. */
	class BlockEditor
	{
	public:
		// Constructor
		BlockEditor();

		// Destructor
		~BlockEditor();

		/*****************************************************************************************************************************/

		// Main loop of the block editor
		void display();

		// Delete elements from the previous scene. Function to call whenever we switch scenes
		void deletePreviousScene();

		// Create main scene buttons
		void createMainScene();

		// Create "create blocks" scene buttons
		void createBlockCreationScene();

		// Create "delete blocks" scene buttons
		void createDeleteBlockScene();

		// Display the main scene of the block editor showing the existing blocks
		void displayMainScene();

		// Display the scene to create new blocks
		void displayBlockCreationScene();

		// Display the scene to delete existing blocks
		void displayDeleteBlockScene();
		
		// Listen mouse events in the current scene
		void listenMouseEvents(bool inMainScene, int grid_x=-1, int grid_y=-1);

		/*****************************************************************************************************************************/

		// Returns the button pressed by the player
		Button *getPressedButton() const;

		/*****************************************************************************************************************************/

	private:
		// New block data
		std::array<Cell*, GameConstants::MAX_BLOCK_SIZE * GameConstants::MAX_BLOCK_SIZE> m_block_data;

		// Edition grid size (in number of squares)
		int m_grid_size;

		// Buttons of the current scene
		std::list<Button*> m_buttons;

		// Button pressed by the player
		Button *m_pressed_button;

		// Font
		TTF_Font *m_font;

		// Font when button is hovered
		TTF_Font *m_hovered_font;

		// Message box appearing when the player does something forbidden
		std::unique_ptr<MessageBox> m_message_box;


		// Hovered square in the block creation scene
		int hovered_square_row;
		int hovered_square_column;

		/*****************************************************************************************************************************/

		// Create a new block described by <m_block_data>
		void createNewBlock() const;

		// Delete the block selected by the player
		void deleteBlock();

		// Check if the block shaped by the player is correct and if the number of blocks didn't reach the limit
		bool canCreateBlock();

		// Check if there is only one block in the grid
		void checkBlockShape(Cell &cell);

		// Add all neighbors to all the cells of the grid
		void addCellsNeighbors();

		// Reset the grid representing the block that the user is creating
		void resetBlockData();

		// Handle mouse motion and mouse clicks events in the main scene
		void handleMouseEventsMainScene();

		// Handle mouse motion and mouse clicks events in the creation scene
		void handleMouseEventsCreationScene(int grid_x, int grid_y);

		// Handle mouse motion and mouse clicks events in the delete scene
		void handleMouseEventsDeleteScene();
	};
}

#endif

