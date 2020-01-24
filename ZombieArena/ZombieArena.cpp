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
	IntRect arena; // End Setup
	
	// Main Loop
	while (window.isOpen()) {

		/******************************
		 *   Handle Input             *
		 ******************************/
		Event event;
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
	}

	return 0;
}