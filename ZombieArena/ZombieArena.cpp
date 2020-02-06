#include "stdafx.h"
#include <SFML/Graphics.hpp>

#include "ZombieArena.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"

using namespace sf;

int main() {
	// Setup
	TextureHolder holder;

	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	srand((int)time(0));

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

	VertexArray background;
	Texture textureBackground = TextureHolder::GetTexture(
		"graphics/background_sheet.png");

	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	Bullet bullets[Bullet::MAX_BULLETS];
	int currentBullet = 0;
	int bulletSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	Time lastPressed;
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
					//Reloading
					if (event.key.code == Keyboard::R) {
						if (bulletSpare >= clipSize) {
							// Fixed so we dont have to throw away extras
							int bulletsToFill = clipSize - bulletsInClip;
							bulletsInClip = clipSize;
							bulletSpare -= bulletsToFill;
						}
						else if (bulletSpare > 0) {
							bulletsInClip = bulletSpare;
							bulletSpare = 0;
						}
						else {
							// Out of ammo
						}
					}
					// End Reloading
				}
			}
		}	
		// End event polling loop

		// Quitting the game
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); }

		// Handle input while playing
		if (state == State::PLAYING) {
			// WASD movement
			if (Keyboard::isKeyPressed(Keyboard::W)) { player.moveUp(); }
				else { player.stopUp(); }
			if (Keyboard::isKeyPressed(Keyboard::A)) { player.moveLeft(); }
				else { player.stopLeft(); }
			if (Keyboard::isKeyPressed(Keyboard::S)) { player.moveDown(); }
				else { player.stopDown(); }
			if (Keyboard::isKeyPressed(Keyboard::D)) { player.moveRight(); }
				else { player.stopRight(); }
			// End WASD

			// Shooting
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (gameTimeTotal.asMilliseconds() 
					- lastPressed.asMilliseconds() > 1000 / fireRate 
					&& bulletsInClip > 0) {

					bullets[currentBullet].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;
					if (currentBullet > 99) { currentBullet = 0; }
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}
			}
			// End Shooting
		}
		// End input

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

				int tileSize = createBackground(background, arena);

				player.spawn(arena, resolution, tileSize);
				numZombies = 10;

				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

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

			// Update each zombie
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			// Update each bullet
			for (int i = 0; i < Bullet::MAX_BULLETS; i++) {
				if (bullets[i].isInFlight()) {
					bullets[i].update(dtAsSeconds);
				}
			}
		}


		/******************************
		 *   DRAW THE SCENE           *
		 ******************************/
		if (state == State::PLAYING) {
			window.clear();
			window.setView(mainView);
			window.draw(background, &textureBackground);

			window.draw(player.getSprite());

			// Zombies
			for (int i = 0; i < numZombies; i++) { 
				window.draw(zombies[i].getSprite()); 
			}

			// Bullets
			for (int i = 0; i < Bullet::MAX_BULLETS; i++) {
				if (bullets[i].isInFlight()) {
					window.draw(bullets[i].getShape());
				}
			}

		}

		if (state == State::LEVELING_UP) {

		}

		if (state == State::PAUSED) {

		}

		if (state == State::GAME_OVER) {

		}

		window.display();
	}
	// End Main loop

	delete[] zombies;
	return 0;
}