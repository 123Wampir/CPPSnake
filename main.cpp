#include <iostream>
#include <Snake.h>
int main(void)
{
    SnakeGame game{};
    game.GameLoop();
    game.Destroy();
    char a;
    std::cin >> a;
}