#include <iostream>
#include <Windows.h>
#include <stdlib.h>

class Cell
{
public:
    Cell();
    int x;
    int y;

private:
};

Cell::Cell()
{
    x = {};
    y = {};
}

class Fruit : public Cell
{
public:
    Fruit();
    Fruit(int width, int heigth);
    void Spawn(int width, int heigth);
};
Fruit::Fruit()
{
    x = 0;
    y = 0;
}
Fruit::Fruit(int width, int heigth)
{
    x = 0;
    y = 0;
    Spawn(width, heigth);
}
void Fruit::Spawn(int width, int heigth)
{
    std::srand(time(0));
    x = std::rand() % width;
    y = std::rand() % heigth;
    printf("xD:%d, yD:%d", x, y);
}

class SnakeCell : public Cell
{
public:
    SnakeCell();
    SnakeCell *next{};
    SnakeCell *previos{};
    void InsertAfter(SnakeCell *cell);
    void InsertBefore(SnakeCell *cell);
};

SnakeCell::SnakeCell()
{
    x = 0;
    y = 0;
}

void SnakeCell::InsertAfter(SnakeCell *cell)
{
    if (this->next)
        this->next->previos = cell;
    cell->next = this->next;
    this->next = cell;
    cell->previos = this;
}
void SnakeCell::InsertBefore(SnakeCell *cell)
{
    if (this->previos)
        this->previos->next = cell;
    cell->previos = this->previos;
    this->previos = cell;
    cell->next = this;
}

enum class SnakeDirections
{
    LEFT,
    UP,
    RIGHT,
    DOWN
};

class Snake
{
private:
    int length;
    SnakeDirections direction;

public:
    Snake();
    SnakeCell head;
    SnakeCell *GetTail();
    void SetDirection(SnakeDirections dir);
    void Move(int width, int height);
    void Grow();
    int GetLength() const;
    bool Collide();
};

Snake::Snake()
{
    head = {};
    length = 1;
    direction = SnakeDirections::RIGHT;
}

bool Snake::Collide()
{
    if (length > 1)
    {
        SnakeCell *cell = head.next;
        for (int i = 0; i < length; i++)
        {
            if (head.x == cell->x && head.y == cell->y)
                return true;
            else
            {
                if (cell->next)
                    cell = cell->next;
            }
        }
    }
    return false;
}

SnakeCell *Snake::GetTail()
{
    SnakeCell *tail = &head;
    for (int i = 0; i < length; i++)
    {
        if (tail->next)
            tail = tail->next;
    }
    return tail;
}
int Snake::GetLength() const
{
    return length;
}
void Snake::SetDirection(SnakeDirections dir)
{
    if ((int)dir % 2 != (int)direction % 2)
        direction = dir;
}

void Snake::Grow()
{
    SnakeCell *tail = GetTail();
    SnakeCell *newCell = new SnakeCell{};
    tail->InsertAfter(newCell);
    newCell->x = tail->x;
    newCell->y = tail->y;
    length++;
}

void Snake::Move(int width, int height)
{
    SnakeCell *cell = GetTail();
    for (int i = 0; i < length; i++)
    {
        if (cell->previos)
        {
            cell->x = cell->previos->x;
            cell->y = cell->previos->y;
            cell = cell->previos;
        }
    }
    switch (direction)
    {
    case SnakeDirections::LEFT:
        head.x = (width + head.x - 1) % width;
        break;
    case SnakeDirections::RIGHT:
        head.x = (width + head.x + 1) % width;
        break;
    case SnakeDirections::UP:
        head.y = (height + head.y - 1) % height;
        break;
    case SnakeDirections::DOWN:
        head.y = (height + head.y + 1) % height;
        break;
    default:
        break;
    }
}

class SnakeGame
{
public:
    bool end;
    int width, heigth;
    SnakeGame();
    Snake snake;
    Fruit fruit;
    void GameLoop();
    void KeyPress();
    void Draw();
    void Destroy();
    void PickFruit();
};

SnakeGame::SnakeGame()
{
    width = 15;
    heigth = 15;
    fruit = {};
    fruit.Spawn(width, heigth);
}

void SnakeGame::PickFruit()
{
    if (fruit.x == snake.head.x && fruit.y == snake.head.y)
    {
        snake.Grow();
        bool place = false;
        int i = 0;
        do
        {
            fruit.Spawn(width, heigth);
            place = true;
            SnakeCell *cell = &snake.head;
            for (int n = 0; n < snake.GetLength(); n++)
            {
                if (cell->x == fruit.x && cell->y == fruit.y)
                {
                    place = false;
                    break;
                }
                if (cell->next)
                    cell = cell->next;
            }
            i++;
        } while (!place && i < width * heigth);
    }
}

void SnakeGame::Draw()
{
    system("cls");
    for (int i = 0; i < heigth; i++)
    {
        for (int j = 0; j < width; j++)
        {
            bool empty = true;
            SnakeCell *cell = &snake.head;
            for (int n = 0; n < snake.GetLength(); n++)
            {
                if (cell->x == j && cell->y == i)
                {
                    if (cell == &snake.head)
                        std::cout << "YY";
                    else
                        std::cout << "[]";
                    empty = false;
                    break;
                }
                if (cell->next)
                    cell = cell->next;
            }
            if (empty)
            {
                if (fruit.x == j && fruit.y == i)
                {
                    std::cout << "oO";
                }
                else
                    std::cout << "..";
            }
        }
        std::cout << std::endl;
    }
}
void SnakeGame::GameLoop()
{
    while (!end)
    {
        Sleep(100);
        KeyPress();
        snake.Move(width, heigth);
        if (snake.Collide())
            end = true;
        PickFruit();
        Draw();
        printf("Score: %d\n", snake.GetLength());
    }
}
void SnakeGame::KeyPress()
{
    if (GetKeyState(VK_LEFT) & 0x8000)
    {
        snake.SetDirection(SnakeDirections::LEFT);
    }
    else if (GetKeyState(VK_RIGHT) & 0x8000)
    {
        snake.SetDirection(SnakeDirections::RIGHT);
    }
    else if (GetKeyState(VK_UP) & 0x8000)
    {
        snake.SetDirection(SnakeDirections::UP);
    }
    else if (GetKeyState(VK_DOWN) & 0x8000)
    {
        snake.SetDirection(SnakeDirections::DOWN);
    }
    else if (GetKeyState(VK_ESCAPE) & 0x8000)
    {
        end = true;
    }
}

void SnakeGame::Destroy()
{
    SnakeCell *cell = &snake.head;
    for (int i = 0; i < snake.GetLength(); i++)
    {
        SnakeCell *temp;
        if (cell->next)
        {
            temp = cell->next;
        }
        if (cell != &snake.head)
            delete cell;
        cell = temp;
    }
}