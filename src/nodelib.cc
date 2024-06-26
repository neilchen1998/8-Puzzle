#include <iostream> // std::cout
#include <span> // std::span
#include <algorithm> // std::ranges::find

#include "node/nodelib.hpp"
#include "constants/constantslib.hpp"
#include "math/mathlib.hpp"

template class Node<constants::EIGHT_PUZZLE_SIZE>;
template class Node<constants::FIFTEEN_PUZZLE_SIZE>;

template <int GameType>
Node<GameType>::Node(std::vector<int> input) : state(input), depth(0), inversion(0)
{
    hashValue = hash_range(input);
    posX = std::ranges::find(input, constants::EMPTY) - input.begin();
    CalculateManhattanDistance();
}

template <int GameType>
Node<GameType>::Node(std::vector<int> input, int posX, int depth) : state(input), posX(posX), depth(depth), inversion(0)
{
    hashValue = hash_range(input);
    CalculateManhattanDistance();
}

template <int GameType>
std::vector<int> Node<GameType>::AvailableMoves() const
{
    std::vector<int> ret;

    auto xDv = std::div(posX, GameType);
    int xRow = xDv.quot;
    int xCol = xDv.rem;

    // constants::RIGHT
    if (xCol + 1 < GameType)
    {
        ret.push_back(constants::RIGHT);
    }

    // constants::UP
    if (xRow >= 1)
    {
        ret.push_back(constants::UP);
    }

    // constants::LEFT
    if (xCol >= 1)
    {
        ret.push_back(constants::LEFT);
    }

    // constants::DOWN
    if (xRow + 1 < GameType)
    {
        ret.push_back(constants::DOWN);
    }

    return ret;
}

template <int GameType>
std::vector<Node<GameType>> Node<GameType>::GetChildrenNodes() const
{
    std::vector<Node> children;

    std::vector<int> moves = AvailableMoves();

    for(const int& move : moves)
    {
        auto [childState, childPosX] = GetNextState(move);
        children.emplace_back(childState, childPosX, depth + 1);
    }

    return children;
}

template <int GameType>
std::tuple<std::vector<int>, int> Node<GameType>::GetNextState(int dir) const
{
    std::vector<int> newState = state;
    int newPosX = posX;

    // swaps the piece
    switch (dir)
    {
    case constants::LEFT:
        std::swap(newState[newPosX], newState[newPosX - 1]);
        newPosX -= 1;
        break;

    case constants::RIGHT:
        std::swap(newState[newPosX], newState[newPosX + 1]);
        newPosX += 1;
        break;

    case constants::UP:
        std::swap(newState[newPosX], newState[newPosX - GameType]);
        newPosX -= GameType;
        break;

    case constants::DOWN:
        std::swap(newState[newPosX], newState[newPosX + GameType]);
        newPosX += GameType;
        break;

    default:
        break;
    }

    return std::tuple<std::vector<int>, int>{newState, newPosX};
}

template <int GameType>
int Node<GameType>::GetManhattanDistance() const
{
    return manhattanDistance;
}

template <int GameType>
std::size_t Node<GameType>::GetHashValue() const
{
    return hashValue;
}

template <>
void Node<constants::EIGHT_PUZZLE_SIZE>::Print() const
{
    std::size_t cnt = 0;
    for (const auto& ele : state)
    {
        // prints "x" if the value if equals to "constants::EMPTY"
        if (ele != constants::EMPTY)
        {
            std::cout << (int)ele << " ";
        }
        else
        {
            std::cout << "x ";
        }

        // increments the value of cnt and then be checked
        if (++cnt % constants::EIGHT_PUZZLE_SIZE == 0)
        {
            std::cout << "\n";
        }
    }
}

template <>
void Node<constants::FIFTEEN_PUZZLE_SIZE>::Print() const
{
    std::size_t cnt = 0;
    for (const auto& ele : state)
    {
        // prints "x" if the value if equals to "constants::EMPTY"
        if (ele < 10)
        {
            std::cout << " " << (int)ele << " ";
        }
        else if (ele != constants::EMPTY)
        {
            std::cout << (int)ele << " ";
        }
        else
        {
            std::cout << " x ";
        }

        // increments the value of cnt and then be checked
        if (++cnt % constants::FIFTEEN_PUZZLE_SIZE == 0)
        {
            std::cout << "\n";
        }
    }
}

template <int GameType>
bool Node<GameType>::operator==(const Node &rhs) const
{
    return hashValue == rhs.GetHashValue();
}

template <int GameType>
bool Node<GameType>::operator!=(const Node &rhs) const
{
    return hashValue != rhs.GetHashValue();
}

template <int GameType>
bool Node<GameType>::IsSolved() const
{
    return (manhattanDistance == 0) ? true : false;
}

template <int GameType>
void Node<GameType>::CalculateManhattanDistance()
{
    // initializes the total Manhattan distance
    manhattanDistance = 0;

    // finds the distance of each element
    auto itr = state.cbegin();
    while (itr != state.cend())
    {
        // checks if the element is not EMPTY
        if (*itr != constants::EMPTY)
        {
            // finds the goal of the element
            auto goalDv = std::div(*itr - 1, GameType);
            int goalRow = goalDv.quot;
            int goalCol = goalDv.rem;

            // finds the current coordinate of the element
            auto curDv = std::div(static_cast<int>(std::distance(state.cbegin(), itr)), GameType);
            int curRow = curDv.quot;
            int curCol = curDv.rem;

            // finds the Manhattan distance of the two
            manhattanDistance += std::abs(goalCol - curCol) + std::abs(goalRow - curRow);
        }

        ++itr;
    }
}

template <int GameType>
void Node<GameType>::CalculateInversion()
{
    // loop through the entire vector
    auto right = state.cbegin();
    while (right != state.cend())
    {
        // disregard 'empty' element
        if (*right != constants::EMPTY)
        {
            // count the number of inversions between the first element to the current one
            inversion += std::ranges::count_if(state.cbegin(), right, [&](int i)
            {
                // only cares when the element is greater than the current one and disregard 'empty' element
                return i > *right && i != constants::EMPTY;
            });
        }

        ++right;
    }
}

template <int GameType>
int Node<GameType>::GetDepth() const
{
    return depth;
}

template <int GameType>
int Node<GameType>::GetInversion()
{
    CalculateInversion();
    return inversion;
}

template <int GameType>
void Node<GameType>::UpdateDepth(int newDepth)
{
    depth = newDepth;
}

template <int GameType>
int Node<GameType>::GetTotalCost() const
{
    return GetManhattanDistance() + GetDepth();
}

template <>
int Node<constants::FIFTEEN_PUZZLE_SIZE>::GetTotalCost() const
{
    int ret = (GetManhattanDistance() + GetDepth());

    return ret;
}

template <>
bool Node<constants::EIGHT_PUZZLE_SIZE>::Insolvable()
{
    return (GetInversion() % 2) ? true : false;
}

/// @brief Checks whether the given puzzle is insolvable or not
/// @return the puzzle is insolvable
template <>
bool Node<constants::FIFTEEN_PUZZLE_SIZE>::Insolvable()
{
    // source: https://www.geeksforgeeks.org/check-instance-15-puzzle-solvable/#

    // whether the number of the inversion is even
    bool condition1 = GetInversion() % 2;

    // whether the empty puzzle is located in the an even row (counted from the bottom)
    int ptr = std::ranges::find(state, constants::EMPTY) - state.begin();
    int curRow = std::div(ptr, constants::FIFTEEN_PUZZLE_SIZE).quot;
    bool condition2 = (constants::EMPTY - curRow) % 2;

    return condition1 ^ condition2;
}

template <int GameType>
int Node<GameType>::FindUndoMove(const Node<GameType>& parent)
{
    // first: the iterator that points to the first argument
    // second: the iterator that points to the second argument
    std::pair<std::vector<int>::iterator, std::vector<int>::const_iterator> ptrs;

    // finds the first mismatch
    ptrs = std::mismatch(state.begin(), state.end(), parent.state.begin());
    int posFirst = std::distance(state.begin(), ptrs.first);

    // finds the second mismatch
    ++ptrs.first, ++ptrs.second;
    ptrs = std::mismatch(ptrs.first, state.end(), ptrs.second);
    int posSecond = std::distance(state.begin(), ptrs.first);

    int diff = (*ptrs.first == constants::EMPTY) ? (posSecond - posFirst) : (posFirst - posSecond);

    int ret;
    switch (diff)
    {
    case -1:
        ret = constants::LEFT;
        break;

    case 1:
        ret = constants::RIGHT;
        break;

    case -GameType:
        ret = constants::UP;
        break;

    case GameType:
        ret = constants::DOWN;
        break;

    default:
        ret = -1;
        break;
    }

    return ret;
}
