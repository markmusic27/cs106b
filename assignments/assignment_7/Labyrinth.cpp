// This file deliniates the isPathToFreedom function including helper methods and tests for this function

#include "set.h"
#include "Labyrinth.h"
using namespace std;

// Indicates what isValidMove will be for later use
bool isValidMove(MazeCell* root, char move);

// Takes a maze and moves and tests whether these moves are in fact valid
bool isPathToFreedom(MazeCell* start, const string& moves) {
    Set<Item> items;
    MazeCell* current = start;

    // Check the starting position for an item
    if (current->whatsHere != Item::NOTHING) {
        items.add(current->whatsHere);
    }

    for (char move : moves) {
        if (!isValidMove(current, move)) {
            return false; // Invalid move detected
        }

        // Move to the new position
        switch (move) {
        case 'N':
            current = current->north;
            break;
        case 'S':
            current = current->south;
            break;
        case 'E':
            current = current->east;
            break;
        case 'W':
            current = current->west;
            break;
        }

        // Collect the item in the new position
        if (current->whatsHere != Item::NOTHING) {
            items.add(current->whatsHere);
        }
    }

    return items.contains(Item::SPELLBOOK) &&
           items.contains(Item::POTION) &&
           items.contains(Item::WAND);
}

// This function checks if the move is valid as to not cause an error with accessing inexistant memory
bool isValidMove(MazeCell* root, char move) {
    if (move == 'N') return root->north != nullptr;
    if (move == 'S') return root->south != nullptr;
    if (move == 'E') return root->east != nullptr;
    if (move == 'W') return root->west != nullptr;
    return false; // Invalid character (though it's guaranteed to be N/S/E/W)
}
/* * * * * * Test Cases Below This Point * * * * * */
#include "GUI/SimpleTest.h"
#include "Demos/MazeGenerator.h"

/* Optional: Add your own custom tests here! */














/* * * * * Provided Tests Below This Point * * * * */

/* Utility function to free all memory allocated for a maze. */
void deleteMaze(const Grid<MazeCell*>& maze) {
    for (auto* elem: maze) {
        delete elem;
    }
    /* Good question to ponder: why don't we write 'delete maze;'
     * rather than what's shown above?
     */
}

PROVIDED_TEST("Checks paths in the sample maze.") {
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN"));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES"));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES"));

    /* These paths don't work, since they don't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW"));
    EXPECT(!isPathToFreedom(maze[2][2], "SWWN"));
    EXPECT(!isPathToFreedom(maze[2][2], "WNNE"));

    /* These paths don't work, since they aren't legal paths. */
    EXPECT(!isPathToFreedom(maze[2][2], "WW"));
    EXPECT(!isPathToFreedom(maze[2][2], "NN"));
    EXPECT(!isPathToFreedom(maze[2][2], "EE"));
    EXPECT(!isPathToFreedom(maze[2][2], "SS"));

    deleteMaze(maze);
}

PROVIDED_TEST("Can't walk through walls.") {
    auto maze = toMaze({"* S *",
                        "     ",
                        "W * P",
                        "     ",
                        "* * *"});

    EXPECT(!isPathToFreedom(maze[1][1], "WNEES"));
    EXPECT(!isPathToFreedom(maze[1][1], "NWSEE"));
    EXPECT(!isPathToFreedom(maze[1][1], "ENWWS"));
    EXPECT(!isPathToFreedom(maze[1][1], "SWNNEES"));

    deleteMaze(maze);
}

PROVIDED_TEST("Works when starting on an item.") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "EE"));
    EXPECT(isPathToFreedom(maze[0][1], "WEE"));
    EXPECT(isPathToFreedom(maze[0][2], "WW"));

    deleteMaze(maze);
}

/* Printer for items. */
ostream& operator<< (ostream& out, Item item) {
    if (item == Item::NOTHING) {
        return out << "Item::NOTHING";
    } else if (item == Item::WAND) {
        return out << "Item::WAND";
    } else if (item == Item::POTION) {
        return out << "Item::POTION";
    } else if (item == Item::SPELLBOOK) {
        return out << "Item::SPELLBOOK";
    } else {
        return out << "<unknown item type>";
    }
}
