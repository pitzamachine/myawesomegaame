#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <thread>
int input = 0;
int turnsElapsed = 0;

class character {

private:
	//things the player can change
	std::string name = "";
	int endurance = 0;
	int strength = 0;
	int dexterity = 0;
	int wisdom = 0;
	int luck = 0;
	int faith = 0;
public:
	//things the player cannot change
	int playerHealth = 0;
	int playerMaxHealth = 0;
	int defense = 0;
	int magicDefense = 0;
	int spellsAvailable = 0;

	friend class combatHandler;
};

class enemy {

private:

	int enemyMaxHealth = 0;
	int enemyHealth = 0;
	int defense = 0;
	int attack = 0;
	int goldDropped = 0;
	int experienceWorth = 0;
	std::string name = " ";

public:

	friend class combatHandler;

	enemy(){
		
		std::cout << "enemy encountered" << std::endl;
		
	}

	void enemySet(int a, int b, int c, int d, int e, int f, std:: string g) {

		enemyMaxHealth = a;
		 enemyHealth = b;
		 defense = c;
		 attack = d;
		 goldDropped = e;
		 experienceWorth = f;
		name = g;

	};

	void takeDamage(int damage){
		std::cout << "Enemy took " << damage << " damage!" << std::endl;
		enemyHealth -= damage;

	}

	void displayStatsTest() {

		std::cout << "Enemy Name: " << name << std::endl;
		std::cout << "Max Health: " << enemyMaxHealth << std::endl;
		std::cout << "Current Health: " << enemyHealth << std::endl;
		std::cout << "Defense: " << defense << std::endl;
		std::cout << "Attack: " << attack << std::endl;
		std::cout << "Gold Dropped: " << goldDropped << std::endl;
		std::cout << "Experience Worth: " << experienceWorth << std::endl;

	}

};

class combatHandler {

private:
	int turnsElapsed = 0;
	int intensity = 0;
	std::string battleArena = "";
	bool playerTurn = true;
	character& player;
	enemy& opponent;

public:
	
	combatHandler(character& p, enemy& e) : player(p), opponent(e) {
		std::cout << std::endl << "BATTLE START!" << std::endl;
	};

	void initiateCombat() {
		std::cout << "Battle started, make your first move" << std::endl << "1) Attack" << std::endl << "2)Defend" << std::endl << "3) Spells" << std::endl << "4) Items" << std::endl << "5) Flee";
		
		std::cin >> input;

		if (std::cin.fail()) {
			std::cout << "Invalid input! Please enter a valid number." << std::endl;
			
			std::cin.clear();
			return;
	
		}

		switch (input) {

		case 1: {

			int baseDamage = player.strength;
			int damage = baseDamage - opponent.defense;
			if (damage < 1) damage = 1;

			int critChance = 3 + player.luck;
			if (critChance > 93) critChance = 93;

			int rolledNumber = rand() % 100;
			bool criticalHit = (critChance > rolledNumber);

			if (criticalHit) {
				damage *= 2;
				std::cout << "Critical Hit! ";
			}
		}
			break;
		case 2:
			//defend
			break;
		case 3:
			//open spell menu
			break;
		case 4:
			//open items menu
			break;
		case 5:
			//flee encounter
			break;
		default:
			std::cout << "number out of range, try again";
				break;


		}
	}



};

int main() {

	srand(time(NULL));
	enemy test;
	character test1;
	
	test.enemySet(10, 10, 10, 10, 10, 10, "beebie");
	test.displayStatsTest();
	test.takeDamage(3);
	test.displayStatsTest();
	combatHandler combat1(test1, test);
	combat1.initiateCombat();

}