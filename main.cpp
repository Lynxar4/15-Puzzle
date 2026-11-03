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
    int x{};
    int y{};

    Point getAdjacentPoint(const Direction direction) const
    {
        switch (direction.getType())
        {
        case Direction::up:
            return { x, y - 1 };
        case Direction::down:
            return { x, y + 1 };
        case Direction::left:
            return { x - 1, y };
        case Direction::right:
            return { x + 1, y };
        default:
            break;
        }
        assert(0 && "Unsupported direction was passed!");
        return *this;
    }

    friend bool operator==(const Point& point1, const Point& point2)
    {
        return point1.x == point2.x && point1.y == point2.y;
    }

    friend bool operator!=(const Point& point1, const Point& point2)
    {
        return !(point1 == point2);
    }
};

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
    friend bool isComplete(const Board& board);

    Point getEmptyPoint() const
    {
        for (int y{ 0 }; y < s_size; ++y)
        {
            for (int x{ 0 }; x < s_size; ++x)
            {
                if (m_tiles[y][x].isEmpty())
                {
                    return {x, y};
                }
            }
        }
        assert(0 && "There is no empty tiles in this board!");
        return { -1, -1 };
    }

    static bool isValidPosition(Point point)
    {
        return point.x >= 0 && point.x < s_size
            && point.y >= 0 && point.y < s_size;
    }

    void swapTiles(Point pt1, Point pt2)
    {
        std::swap(m_tiles[pt1.y][pt1.x], m_tiles[pt2.y][pt2.x]);
    }

    bool moveTile(Direction dir)
    {
        Point emptyTilePoint{ getEmptyPoint() };
        Point adjacentTilePoint{ emptyTilePoint.getAdjacentPoint(-dir) };

        if (!isValidPosition(adjacentTilePoint))
        {
            return false;
        }

        swapTiles(emptyTilePoint, adjacentTilePoint);
        return true;
    }

    void randomize()
    {
        for (int i{ 0 }; i < 1000;)
        {
            if (moveTile(Direction::getRandomDirection()))
            {
                ++i;
            }
        }
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
        out << '\n';
    }
    return out;
}

bool isComplete(const Board& board)
{
    bool complete{ true };
    int counter{ 1 };

    for (int i{ 0 }; i < board.s_size; ++i)
    {
        for (int j{ 0 }; j < board.s_size; ++j)
        {
            if (counter == 16)
            {
                counter = 0;
            }
            if (board.m_tiles[i][j].getNum() != counter)
            {
                return false;
            }
            ++counter;
        }
    }

    return true;
}

int main()
{
    Board board{};
    board.randomize();
    std::cout << board;

    while (true)
    {
        char command{ Input::getUserInput() };

        if (command == 'q')
        {
            std::cout << "\n\nWow you couldn't solve a simple puzzle.\n\n";
            break;
        }

        Direction dir{ Input::commandToDirection(command) };
        bool tileSwapped{ board.moveTile(dir) };
        if (tileSwapped)
        {
            std::cout << board;
        }
        if (isComplete(board))
        {
            std::cout << "You won!\n";
            break;
        }
    }

    return 0;
}