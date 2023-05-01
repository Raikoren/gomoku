#include "game.hpp"

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Permet de check les leaks https://www.youtube.com/watch?v=v8jQqAZk0zY
	Game game;

	game.run();
	return 0;
}