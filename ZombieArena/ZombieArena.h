#pragma once
#include "Zombie.h"

using namespace sf;

int createBackground(VertexArray& rVA, IntRect Arena);
Zombie* createHorde(int numZombies, IntRect arena);