#pragma once
#pragma once

#include <SFML/graphics.hpp>
#include <vector>

using std::vector;
using std::shared_ptr;

struct Collision {
	shared_ptr<sf::Sprite> obj1;
	shared_ptr<sf::Sprite> obj2;
};

class CollisionEngine {
public:

	CollisionEngine() {}
	~CollisionEngine() {
		/*for (int i = 0; i < friendlyColliders.size(); i++) {
			friendly
		}*/
	}

	void addEnemy(shared_ptr<sf::Sprite> object) {
		enemyColliders.push_back(object);
	}

	void addFriendly(shared_ptr<sf::Sprite> object) {
		friendlyColliders.push_back(object);
	}

	void updatePositions() {
		resetToBegining();
	}
	void resetToBegining() {
		currentEnemy = 0;
		cycledEnemies = false;
		currentFriendly = 0;
		cycledFriendlies = false;
		if (currentCollision != nullptr) {
			delete currentCollision;
			currentCollision = nullptr;
		}
	}

	Collision* getNextCollision() {
		bool foundCollision = false;
		for (int i = currentFriendly; i < friendlyColliders.size() && !cycledFriendlies && !foundCollision; i++) {
			shared_ptr<sf::Sprite> friendly = friendlyColliders.at(i);
			for (int j = currentEnemy; j < enemyColliders.size() && !foundCollision; j++) {
				if (checkCollision(friendly, enemyColliders.at(j))) {
					setCurrentCollision(friendly, enemyColliders.at(j));
					currentFriendly = i;
					currentEnemy = j + 1;
					foundCollision = true;
				}
			}
		}
		if (foundCollision) {
			return currentCollision;
		}
		cycledFriendlies = true;
		return nullptr;
	}

	void setCurrentCollision(shared_ptr<sf::Sprite>& obj1, shared_ptr<sf::Sprite>& obj2) {
		if (currentCollision != nullptr) {
			delete currentCollision;
		}
		currentCollision = new Collision{ obj1, obj2 };
	}

	bool checkCollision(shared_ptr<sf::Sprite>& obj1, shared_ptr<sf::Sprite>& obj2) {
		return obj1->getGlobalBounds().findIntersection(obj2->getGlobalBounds()).has_value();
	}
private:
	vector<shared_ptr<sf::Sprite>> enemyColliders;
	int currentEnemy = 0;
	bool cycledEnemies = false;
	vector<shared_ptr<sf::Sprite>> friendlyColliders;
	int currentFriendly = 0;
	bool cycledFriendlies = false;
	Collision* currentCollision = nullptr;
};

