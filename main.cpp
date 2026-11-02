#include<iostream>
#include <vector>
#include <cstddef>
#include <limits>
#include <cassert>
#include "Random.h"

constexpr int g_consoleLines{ 25 };

class Direction
{
public:
    enum Type
    {
        up,
        left,
        down,
        right,
        maxDirections,
    };

    Direction(Type type)
        : m_type{ type }
    {}

    Type getType() const
    {
        return m_type;
    }

    Direction operator-() const
    {
        switch (m_type)
        {
        case up:
            return Direction{ down };
        case left:
            return Direction{ right };
        case down:
            return Direction{ up };
        case right:
            return Direction{ left };
        default:
            break;
        }

        assert(0 && "Unsupported direction was passed!"); // 0 makes the assertion evaluate to false
        return Direction{ up };
    }

    static Direction getRandomDirection()
    {
        Type random{ static_cast<Type>(Random::get(0, Type::maxDirections - 1)) };
        return Direction{ random };
    }

private:
    Type m_type{};
};

std::ostream& operator<<(std::ostream& out, const Direction& direction)
{
    switch (direction.getType())
    {
    case Direction::up:
        return out << "up";
    case Direction::down:
        return out << "down";
    case Direction::left:
        return out << "left";
    case Direction::right:
        return out << "right";
    default:
        break;
    }
    assert(0 && "Unsupported direction was passed!");
    return out << "unknown direction";
}

struct Point
{
    int row{};
    int column{};

    Point getAdjacentPoint(const Direction direction) const
    {
        switch (direction.getType())
        {
        case Direction::up:
            return { row - 1, column};
        case Direction::down:
            return { row + 1, column };
        case Direction::left:
            return { row, column - 1 };
        case Direction::right:
            return { row, column + 1 };
        default:
            break;
        }
        assert(0 && "Unsupported direction was passed!");
        return *this;
    }

    friend bool operator==(const Point& point1, const Point& point2);

    friend bool operator!=(const Point& point1, const Point& point2);
};

bool operator==(const Point& point1, const Point& point2)
{
    return point1.row == point2.row && point1.column == point2.column;
}

bool operator!=(const Point& point1, const Point& point2)
{
    return point1.row != point2.row || point1.column != point2.column;
}

namespace Input
{
    bool isValidCommand(char c)
    {
        return c == 'w'
            || c == 'a'
            || c == 's'
            || c == 'd'
            || c == 'q';
    }

    char getCharacter()
    {
        char ch{};
        std::cin >> ch;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return ch;
    }

    char getUserInput()
    {
        char ch{};
        while (!isValidCommand(ch))
        {
            ch = getCharacter();
        }
        return ch;
    }

    Direction commandToDirection(char command)
    {
        switch (command)
        {
        case 'w':
            return Direction{ Direction::up };
        case 'a':
            return Direction{ Direction::left };
        case 's':
            return Direction{ Direction::down };
        case 'd':
            return Direction{ Direction::right };
        }

        assert(0 && "Invalid command was passed!");
        return Direction{ Direction::up };
    }
}

class Tile
{
public:
    Tile() = default;
    explicit Tile(int num)
        : m_displayNumber{ num }
    {}

    bool isEmpty() const
    {
        return m_displayNumber == 0;
    }

    int getNum() const
    {
        return m_displayNumber;
    }

private:
    int m_displayNumber{};
};

std::ostream& operator<<(std::ostream& out, const Tile& tile)
{
    if (tile.getNum() == 0)
    {
        out << "    ";
    }
    else if (tile.getNum() > 9)
    {
        out << " " << tile.getNum() << " ";
    }
    else if (tile.getNum() > 0)
    {
        out << "  " << tile.getNum() << " ";
    }
    return out;
}

class Board
{
public:
    Board() = default;

    friend std::ostream& operator<<(std::ostream& out, const Board& board);

    Point getEmptyPoint()
    {
        for (std::size_t i{ 0 }; i < s_size; ++i)
        {
            for (std::size_t j{ 0 }; j < s_size; ++j)
            {
                if (m_tiles[i][j].isEmpty())
                {
                    return { static_cast<int>(i), static_cast<int>(j) };
                }
            }
        }
        return { 67, 67 };
    }

    bool checkValidPosition(Point point)
    {
        if (point.row >= 0 && point.row <= 3 && point.column >= 0 && point.column <= 3)
        {
            return true;
        }
        return false;
    }

    void moveTile(Direction dir)
    {
        Point emptyTilePoint{ getEmptyPoint() };
        Tile& emptyTile{ m_tiles[emptyTilePoint.row][emptyTilePoint.column]};

        Point adjacentTilePoint{ emptyTilePoint.getAdjacentPoint(-dir) };
        Tile& adjacentTile{m_tiles[adjacentTilePoint.row][adjacentTilePoint.column]};

        bool valid{ checkValidPosition(adjacentTilePoint) };
        if (!valid)
        {
            return;
        }

        Tile temp{};
        temp = emptyTile;
        emptyTile = adjacentTile;
        adjacentTile = temp;
    }

private:
    static constexpr int s_size{ 4 };
    Tile m_tiles[s_size][s_size]{
        Tile{1}, Tile{2}, Tile{3}, Tile{4},
        Tile{5}, Tile{6}, Tile{7}, Tile{8},
        Tile{9}, Tile{10}, Tile{11}, Tile{12},
        Tile{13}, Tile{14}, Tile{15}, Tile{0}
    };
};

std::ostream& operator<<(std::ostream& out, const Board& board)
{
    for (int i{ 0 }; i < g_consoleLines; ++i)
    {
        std::cout << '\n';
    }

    for (std::size_t i{ 0 }; i < board.s_size; ++i)
    {
        for (std::size_t j{ 0 }; j < board.s_size; ++j)
        {
            out << board.m_tiles[i][j];
        }
        std::cout << '\n';
    }
    return out;
}

int main()
{
    Board board{};
    std::cout << board;

    for (int i{ 0 }; i < 4; ++i)
    {
        std::cout << "Generating a random direction... " << Direction::getRandomDirection() << '\n';
    }
    std::cout << '\n';

    std::cout << "Enter a command: \n";

    while (true)
    {
        char command{ Input::getUserInput() };

        if (command == 'q')
        {
            std::cout << "\n\nBye!\n\n";
            break;
        }

        Direction dir{ Input::commandToDirection(command) };
        std::cout << "You entered direction: " << dir << '\n';
        board.moveTile(dir);
        std::cout << board;
    }

    return 0;
}