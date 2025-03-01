#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <cmath>

int input = 0;
bool battling = true;
double rolledNumber;
int diceRoll;
int turnsElapsed = 0;

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
	int level = 1;
	int experience = 0;
	int experienceToNext = 100;
	int playerHealth = 0;
	int playerMaxHealth = 0;
	unsigned int defense = 0;
	unsigned int magicDefense = 0;
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
			<< "Faith: " << faith << std::endl
			<< "Spells slots available: " << spellsAvailable << std::endl;
	}
	void pickAttributes() {

		//first show available points to distribute, also make a constructor that auto assigns stats basically.



	}
	void updateCharacterStats(){

		playerMaxHealth = 10 + endurance * 3;
		defense = 0 + endurance * 2 + strength * 1;
		magicDefense = faith * 2 + wisdom * 1 + luck * 1;
		spellsAvailable = (faith / 6 + wisdom / 3); // adds up the two variables and rounds down
	}
	void takeDamage(int damage) {
		std::cout << std::endl << "You took " << damage << " damage!" << std::endl;
		playerHealth -= damage;

	}
	character(short stat1, short stat2, short stat3,short stat4, short stat5, short stat6) {
		endurance = stat1;
		strength = stat2;
		dexterity = stat3;
		wisdom = stat4;
		luck = stat5;
		faith = stat6;
		updateCharacterStats();
		playerHealth = 10 + endurance * 3;

	};
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

	void basicEnemyAi(character& player){

		diceRoll = rand() % 4;
		switch (diceRoll) {

		case 0: {
			int finalDamage = attack - player.defense;
			if (finalDamage < 1) finalDamage = 1;
			player.takeDamage(finalDamage);
			turnsElapsed++;
			break;
		}
		case 1:
			//defend
			std::cout << "Enemy defends, but it doesn't do anything";
			turnsElapsed++;
			break;
		case 2:
			std::cout << "enemy also does nothing";
			turnsElapsed++;
			//do nothing
			break;
		case 3:
			std::cout << "enemy does nothing";
			//also do nothing
			turnsElapsed++;
			break;

		}

	}

};

class combatHandler {

private:
	
	int intensity = 0; // I think this will increase as the battle goes on, maybe as a multiplier to all damage done, to avoid battles taking too long.
	std::string battleArena = ""; // This will likely have some sort of impact, maybe giving status effects. Remember, keep the scope of the project smaller for now.
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

			case 1: {  //these brackets are necessary to avoid errors! idrk why, has to do with scope I think
				//i might move this code to the player class
				int baseDamage = player.strength;
				int damage = baseDamage - opponent.defense;
				if (damage < 1) damage = 1;

				double critChance = 3 + player.luck + player.faith / 3;
				if (critChance > 93) critChance = 93;

				rolledNumber = rand() % 100;
				bool criticalHit = (critChance > rolledNumber);

				std::cout << std::endl << "Roll: " << rolledNumber;

				if (criticalHit) {
					damage *= 2;
					std::cout << "Critical Hit! ";
				}
				opponent.takeDamage(damage);
				turnsElapsed++;

				break;
			}
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
			if (turnsElapsed % 2 == 1) opponent.basicEnemyAi(player);
			}
		}
	}



};

int main() {

	srand(static_cast<unsigned int>(time(NULL)));
	enemy test;
	character test1(3,3,3,3,3,3);
	test.enemySet(10, 10, 1, 10, 10, 10, "beebie");
	combatHandler combat1(test1, test);
	combat1.initiateCombat();
	return 0;
}