#include<iostream>
#include <vector>
#include <cstddef>
#include <limits>

constexpr int g_consoleLines{ 25 };

namespace Input
{
    bool isValidCommand(char c)
    {
        return c == 'w' || c == 'a' || c == 's' || c == 'd' || c == 'q';
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

private:
    static constexpr int s_size{ 4 };
    Tile m_tiles[s_size][s_size]{
        Tile{1}, Tile{2}, Tile{3}, Tile{4},
        Tile{5}, Tile{6}, Tile{7}, Tile{8},
        Tile{9}, Tile{10}, Tile{11}, Tile{12},
        Tile{13}, Tile{14}, Tile{15}
    };
};

std::ostream& operator<<(std::ostream& out, const Board& board)
{
    for (int i{ 0 }; i < g_consoleLines; ++i)
    {
        std::cout << '\n';
    }

    for (std::size_t i{0}; i < board.s_size; ++i)
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

    while (true)
    {
        char command{ Input::getUserInput() };
        std::cout << "Valid command: " << command << '\n';
        if (command == 'q')
        {
            std::cout << "\n\nBye!\n\n";
            break;
        }
    }

    return 0;
}