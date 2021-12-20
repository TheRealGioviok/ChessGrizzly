// The uci.h file is the main header file for the uci.cpp file.
// It contains the function used to interface the engine with the chess GUI.
// The implementation is heavily based on Code Monkey King's chess engine (BBC Chess Engine @ https://github.com/maksimKorzh)
// The use of this file is permitted under the Do What The Fuck You Want To Public License.
// Please note that this code is written for windows64, and is not compatible with other platforms (planning to add support for linux64 when engine reaches ~2700 elo).

#include "Move.h"
#include "Position.h"
#include "Game.h"
#include "evaluation.h"

extern int quit;

/**
 * @brief The uciLoop function is the main function of the engine.
 * It is called by the main function of the engine.
 * It is responsible for interfacing the engine with the GUI.
 * @param game The game object.
 * @note This function writes to the standard output, which is then redirected to the GUI.
 */
void uciLoop(Game* game);

/**
 * @brief The executeCommand function reads a string and executes the command(s) contained in the string.
 * @param game The game object.
 * @param command The command string.
 * @return The return value is the return value of the command.
 */
int executeCommand(Game* game, char* command);

/**
 * @brief The positionCommand function handles the position command.
 * @param game The game object.
 * @param command The command string.
 * @return The return value is the return value of the command.
 */
int positionCommand(Game* game, char* command);

/**
 * @brief The goCommand function handles the go command.
 * @param game The game object.
 * @param command The command string.
 * @return The return value is the return value of the command.
 * @note This function is responsible for the main search loop.
 */
int goCommand(Game* game, char* command);

/**
 * @brief The inputWaiting function waits for input from the GUI. The code is based on the code from Code Monkey King's BBC chess engine which is based on the code from VICE by BlueFeverSoftware.
 * @return The return value is the length of the input string (0 means no input).
 * @note This function is responsible for async communication with the GUI.
 */
int inputWaiting();

/**
 * @brief The readInput function reads the input from the GUI. The code is based on the code from Code Monkey King's BBC chess engine which is based on the code from VICE by BlueFeverSoftware.
 * It is responsible for async communication with the GUI during the main search loop. It is not responsible for command execution (except for the stop command).
 * @param game 
 */
void readInput(Game *game);

/**
 * @brief The communicate function is responsible for mid-search communication with the GUI.
 * @param game The game object.
 */
void communicate(Game *game);