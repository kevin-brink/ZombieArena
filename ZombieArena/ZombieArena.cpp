#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;

int main() {
	// Setup
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	State state = State::GAME_OVER;

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(
		VideoMode(resolution.x, resolution.y),
		"Zombie Arena", Style::Fullscreen);

	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	Clock clock;
	Time gameTimeTotal;

	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	Player player;
	IntRect arena;

	srand((int)time(0));
	// End Setup
	

	// Main Loop
	while (window.isOpen()) {

		/******************************
		 *   HANDLE INPUT             *
		 ******************************/
		Event event;
		// Event Polling
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {

				// Pause while playing
				if (event.key.code == Keyboard::Return
					&& state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return
					&& state == State::PAUSED)
				{
					state = State::PLAYING;
					clock.restart();
				}

				// Start a new game from gameover
				else if (event.key.code == Keyboard::Return
					&& state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING) {

				}
			}
		}	
		// End event polling loop

		// Quitting the game
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); }

		// Handle WASD while playing
		if (state == State::PLAYING) {
			if (Keyboard::isKeyPressed(Keyboard::W)) { player.moveUp(); }
				else { player.stopUp(); }
			if (Keyboard::isKeyPressed(Keyboard::A)) { player.moveLeft(); }
				else { player.stopLeft(); }
			if (Keyboard::isKeyPressed(Keyboard::S)) { player.moveDown(); }
				else { player.stopDown(); }
			if (Keyboard::isKeyPressed(Keyboard::D)) { player.moveRight(); }
				else { player.stopRight(); }
		}
		// End WASD

		// Handle leveling up
		if (state == State::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1) { state = State::PLAYING; }
			if (event.key.code == Keyboard::Num2) { state = State::PLAYING; }
			if (event.key.code == Keyboard::Num3) { state = State::PLAYING; }
			if (event.key.code == Keyboard::Num4) { state = State::PLAYING; }
			if (event.key.code == Keyboard::Num5) { state = State::PLAYING; }
			if (event.key.code == Keyboard::Num6) { state = State::PLAYING; }

			if (state == State::PLAYING) {
				// Prepare the level
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				int tileSize = 50;

				player.spawn(arena, resolution, tileSize);
				clock.restart();
			}
		}
		// End leveling up


		/******************************
		 *   UPDATE THE FRAME         *
		 ******************************/
		if (state == State::PLAYING) {
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);

			player.update(dtAsSeconds, Mouse::getPosition());
			Vector2f playerPosition(player.getCenter());
			mainView.setCenter(player.getCenter());
		}


		/******************************
		 *   DRAW THE SCENE           *
		 ******************************/
		if (state == State::PLAYING) {
			window.clear();
			window.setView(mainView);
			window.draw(player.getSprite());
		}

		if (state == State::LEVELING_UP) {

		}

		if (state == State::PAUSED) {

		}

		if (state == State::GAME_OVER) {

		}

		window.display();
	}

	return 0;
}