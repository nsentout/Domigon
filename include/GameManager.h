#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <queue>
#include <chrono>

#include <SDL_ttf.h>

#include "Random.h"
#include "Grid.h"

namespace Domigon
{
	/* This class manages the core of the game. It handles the inputs to move and rotate the blocks, change the levels when needed */
	/* , manages the score, the hold and following blocks and tell the GameWindow when to refresh some areas. */
	class GameManager {
	public:
		// Constructor
		GameManager();

		// Destructor
		~GameManager();

		/*****************************************************************************************************************************/

		// Initialize the blocks from the corresponding saved file
		bool initBlocks();

		// Initialize a new game
		void newGame();

		// Manage inputs
		void handleInputEvents(SDL_Event &e, bool &quit);

		// Reset timer and move the block down every <GameConstants::SPEED> seconds
		void handleGameSpeed();

		/*****************************************************************************************************************************/

		// Returns the grid
		Grid *getGrid() const;

		// Returns the block the player is currently controlling
		Block *getCurrentBlock() const;

		// Returns the queue of following blocks
		std::queue<std::shared_ptr<Block>> getFollowingblocks() const;

		// Returns the hold block
		Block *getHoldBlock() const;

		// Returns the current level
		int getLevel() const;

		// Returns the score
		int getScore() const;

		// Indicate whether the game is over
		bool isGameOver() const;

		// Returns the timer of the game
		std::chrono::high_resolution_clock::time_point getTimer() const;

		// Indicate whether the grid needs to be refreshed or not
		bool needGridRefresh() const;

		// Indicate whether the following blocks needs to be refreshed or not
		bool needFollowingblocksRefresh() const;

		// Indicate whether the hold block needs to be refreshed or not
		bool needHoldblockRefresh() const;

		// Indicate whether the score needs to be refreshed or not
		bool needScoreRefresh() const;

		// Indicate whether the level needs to be refreshed or not
		bool needLevelRefresh() const;

		// Set the refresh state of the grid
		void setGridRefresh(bool refresh);

		// Set the refresh state of the following blocks
		void setFollowingblocksRefresh(bool refresh);

		// Set the refresh state of the hold block
		void setHoldblockRefresh(bool refresh);

		// Set the refresh state of the score
		void setScoreRefresh(bool refresh);

		// Set the refresh state of the level
		void setLevelRefresh(bool refresh);

		/*****************************************************************************************************************************/

	private:
		// Grid
		Grid *m_grid;

		// Block controlled by the player
		Block *m_currentBlock;

		// Original block controlled by the player (without rotations)
		Block *m_original_currentBlock;

		// Following blocks 
		std::queue<std::shared_ptr<Block>> m_followingblocks;

		// Hold block
		Block *m_holdBlock;

		// Falling speed of the blocks
		float m_speed;

		// Current level
		int m_level;

		// Player score
		int m_score;

		// Number of lines cleared by the player in the current level;
		int m_cleared_lines_nb;

		// Indicate whether the player can hold a block or not
		bool m_can_hold_block;

		// Indicate whether the game is over
		bool m_game_over;

		// Game timer
		std::chrono::high_resolution_clock::time_point m_timer;
		std::chrono::high_resolution_clock::time_point m_timer2;

		// Object that generates random numbers
		Random *m_random;

		// Indicate whether the grid needs to be refreshed or not
		bool m_refresh_grid;

		// Indicate whether the following blocks needs to be refreshed or not
		bool m_refresh_followingblocks;

		// Indicate whether the hold block needs to be refreshed or not
		bool m_refresh_holdblock;

		// Indicate whether the score needs to be refreshed or not
		bool m_refresh_score;

		// Indicate whether the level needs to be refreshed or not
		bool m_refresh_level;

		/*****************************************************************************************************************************/

		// Moves current block down, check for completed lines and if the game is over
		void handleMoveDown();

		// Hold current block or insert the current hold block in the grid
		void holdBlock();

		// Insert the next block in the followingblocks list in the grid
		void insertNextBlock();
	};
}

#endif