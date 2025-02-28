#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <thread>

int input = 0;
bool battling = true;

class character {

private:
	//things the player can change
	std::string name = "John Placeholder";
	unsigned short endurance = 1;
	unsigned short strength = 1;
	unsigned short dexterity = 1;
	unsigned short wisdom = 1;
	unsigned short luck = 1;
	unsigned short faith = 1;
public:
	//things the player cannot change
	int playerHealth = 0;
	int playerMaxHealth = 0;
	int defense = 0;
	int magicDefense = 0;
	unsigned short spellsAvailable = 0;
	

	void displayStats() {

		std::cout << "Player Name: " << name << std::endl;
		std::cout << "Max Health: " << playerMaxHealth << std::endl;
		std::cout << "Current Health: " << playerHealth << std::endl;
		std::cout << "Defense: " << defense << std::endl;
		std::cout << "Magic Defense: " << magicDefense << std::endl;
		std::cout << "--------Stats--------" << std::endl;
		std::cout << "Endurance: " << endurance << std::endl
			<< "Strength: " << strength << std::endl
			<< "Dexterity: " << dexterity << std::endl
			<< "Wisdom: " << wisdom << std::endl
			<< "Luck: " << luck << std::endl
			<< "Faith: " << faith << std::endl;
	}

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
		std::cout << std::endl << "Enemy took " << damage << " damage!" << std::endl;
		enemyHealth -= damage;

	}

	void displayStatsTest() {

		std::cout << "Enemy Name: " << name << std::endl;
		std::cout << "Max Health: " << enemyMaxHealth << std::endl;
		std::cout << "Current Health: " << enemyHealth << std::endl;
		std::cout << "Defense: " << defense << std::endl;
		std::cout << "Attack: " << attack << std::endl;
		std::cout << "Gold Dropped: " << goldDropped << std::endl;
		std::cout << "Experience Worth: " << experienceWorth << std::endl << std:: endl;

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
		while (battling)  {
			if(turnsElapsed % 2 == 0){
			std::cout 
				<< std::endl << "1) Attack"
				<< std::endl << "2) Defend"
				<< std::endl << "3) Spells"
				<< std::endl << "4) Items"
				<< std::endl << "5) Check"
				<< std::endl << "6) Flee" << std::endl;

			std::cin >> input;

			if (std::cin.fail()) {
				std::cout << "Invalid input! Please enter a valid number." << std::endl;

				std::cin.clear();
				return;

			}

			switch (input) {

			case 1: 

				int baseDamage = player.strength;
				int damage = baseDamage - opponent.defense;
				if (damage < 1) damage = 1;

				double critChance = 3 + player.luck + player.faith / 3;
				if (critChance > 93) critChance = 93;

				double rolledNumber = rand() % 100;
				bool criticalHit = (critChance > rolledNumber);

				std::cout << std::endl << "Roll: " << rolledNumber;

				if (criticalHit) {
					damage *= 2;
					std::cout << "Critical Hit! ";
				}
				opponent.takeDamage(damage);
				turnsElapsed++;
			
				  break;
			case 2:

				//defend
				turnsElapsed++;
				break;
			case 3:

				//open spell menu
				break;
			case 4:

				//open items menu
				break;
			case 5:

				opponent.displayStatsTest();
				player.displayStats();
				break;
			case 6:

				//flee encounter
				turnsElapsed++;
				break;
			default:
				std::cout << "number out of range, try again";
				break;

			
				}
			}
		}
	}



};

int main() {

	srand(static_cast<unsigned int>(time(NULL)));
	enemy test;
	character test1;
	test.enemySet(10, 10, 10, 10, 10, 10, "beebie");
	combatHandler combat1(test1, test);
	combat1.initiateCombat();
	return 0;
}