#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <limits>

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
	//things the player cannot change, and or can be changed by external things
	int level = 1;
	int gold = 0;
	int experience = 0;
	int experienceToNext = 100;
	int playerHealth = 0;
	int playerMaxHealth = 0;
	unsigned int defense = 0;
	unsigned short temporaryDefense = 0;
	unsigned int magicDefense = 0;
	unsigned short spellsAvailable = 0;
	unsigned short maximumSpellsAvailable = 0;



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
			<< "Spell slots available: " << spellsAvailable << std::endl;
	}
	void pickAttributes() {

		//first show available points to distribute, also make a constructor that auto assigns stats basically.



	}
	void updateCharacterStats() {

		playerMaxHealth = 10 + endurance * 3;
		defense = 0 + endurance * 2 + strength * 1;
		temporaryDefense = defense;
		magicDefense = faith * 2 + wisdom * 1 + luck * 1;
		maximumSpellsAvailable = (faith / 6 + wisdom / 3); // adds up the two variables and rounds down
		spellsAvailable = maximumSpellsAvailable;
	}
	void takeDamage(int damage) {
		std::cout << std::endl << "You took " << damage << " damage!" << std::endl;
		playerHealth -= damage;

	}
	character(short stat1, short stat2, short stat3, short stat4, short stat5, short stat6) {
		endurance = stat1;
		strength = stat2;
		dexterity = stat3;
		wisdom = stat4;
		luck = stat5;
		faith = stat6;
		updateCharacterStats();
		playerHealth = playerMaxHealth;

	};


	friend class combatHandler;
	friend class spells;
};

class spells {

public:
	char id = '0';
	int baseDamage = 0;
	float attributeDamageModifier = 0;
	short cost = 0;
	char scalingAttribute = '0'; // scalingAttribute of 0 is wisdom, 1 is faith, 2 is both
	std::string spellName = "default";
	std::vector<char> spellsKnown;
	

	spells() {

		id = 0;
		baseDamage = 3;
		attributeDamageModifier = 1.2f;
		spellName = "Zaqp";
		scalingAttribute = '0';

	}
	
	char displaySpellMenu() {

		if (spellsKnown.empty()) {

			std::cout << "You don't know any spells. \n";
			return 'f';
		}
		else {
			int i = 1;
			std::cout << "-----Spells----- \n";
			for (char spellId : spellsKnown) {
				
				id = spellId;
				updateSpell(spellId);
				std::cout << i << ": " << spellName << " | Cost: " << cost << std::endl;
				i++;

			}
			std::cout << "---------------- \n";
			if (battling) {

				std::cout << "Type the number of the spell you'd like to cast, or anything else to cancel casting. \n";

				std::cin >> input; 


				if (std::cin.fail() || input < 1 || input > spellsKnown.size() ) {
					std::cout << "Spell casting cancelled. \n";
					std::cin.clear();
					std::cin.ignore(10000, '\n');
					return 'f';
				}
				return spellsKnown[input - 1];

			}

		}


		return 'f';

	};

	void grantPlayerSpell(char spellId) {


		spellsKnown.push_back(spellId);

	}

	void updateSpell(char SpellId) {

		
		switch (SpellId) {


		case 0:
			
			baseDamage = 3;
			attributeDamageModifier = 1.4f;
			spellName = "Zap";
			scalingAttribute = '0';
			cost = 1;

			break;
		case 1:
			
			baseDamage = 6;
			attributeDamageModifier = 1.1f;
			spellName = "Light Ray";
			scalingAttribute = '1';
			cost = 1;

			break;
		default:
			std::cout << "Hello, i'm not a spell!"; //this will display this humorous text if some how
			// you input a wrong value when updating the spell
			break;



		}


	}
	int calculateSpellDamage(character& player) {
		// this code decides the spell damage based off the scaling types. 0 for wisdom, 1 for faith, 2 for both.
		int finalDamage = 0;
		switch (scalingAttribute) {


		case '0':
			finalDamage = baseDamage + (player.wisdom * attributeDamageModifier);
			
			return finalDamage;
			break;
		case '1':

			finalDamage = baseDamage + (player.faith * attributeDamageModifier);

			return finalDamage;
			break;

		case '2':

			finalDamage = baseDamage + ((player.faith + player.wisdom) * attributeDamageModifier);

			return finalDamage;
			break;

		default:

			std::cout << "\n Something went wrong.. \n";
			return 0;
			break;



		}
	}
};



class enemy {

private:

	int enemyMaxHealth = 0;
	int enemyHealth = 0;
	int defense = 0;
	int attack = 0;
	unsigned short goldDropped = 0;
	unsigned short experienceWorth = 0;
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
		if (battling)  {
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

	}

};

class combatHandler {

private:
	
	int intensity = 0; // I think this will increase as the battle goes on, maybe as a multiplier to all damage done, to avoid battles taking too long.
	std::string battleArena = ""; // This will likely have some sort of impact, maybe giving status effects. Remember, keep the scope of the project smaller for now.
	bool playerTurn = true;
	character& player;
	enemy& opponent;
	spells& spell;

public:
	
	combatHandler(character& p, enemy& e, spells& s) : player(p), opponent(e), spell(s) {
		std::cout << std::endl << "BATTLE START!" << std::endl;
	};

	bool fleeLogic() {
		int fleeValue = rand() % 301 + player.luck * 3 + player.dexterity * 2;

		if (fleeValue > 100 + (sqrt(player.luck*(player.dexterity+2)) * 4) + intensity*4){
			std::cout << "You fled successfully.\n";
			battling = false;
		 return true; 
		}
		else {
			std::cout << "You couldn't flee!\n";
		 return false;
		}
		
		

	}

	void battlegroundInfo() {

		std::cout << "\nIntensity: " << intensity;


	}

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

			
			std::cin >> input; // this actually takes the input, everything below describes what happens.
			

			if (std::cin.fail()) {

				
				std::cout << "Invalid input! Please enter a valid number." << std::endl;
				
				std::cin.clear();
				std::cin.ignore(10000, '\n');
				continue;

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
				intensity++;
				turnsElapsed++;

				break;
			}
			case 2:
				std::cout << " \n You brace for impact.. \n";
				player.temporaryDefense = player.defense;
				player.defense += player.defense / 3;
				intensity++;
				turnsElapsed++;
				break;
			case 3:
			{
				char spellUsed = spell.displaySpellMenu();
				if (spellUsed == 'f') break;  //f is the default 
				spell.updateSpell(spellUsed);

				if (spell.cost <= player.spellsAvailable) {
					int spellDamage = spell.calculateSpellDamage(player);
					opponent.takeDamage(spellDamage);
					player.spellsAvailable -= spell.cost; 
					intensity++;
					turnsElapsed++;
				}
				else {
					std::cout << "Insufficient spell power available";
				}
				


				break;
			}
			case 4:

				//open items menu
				break;
			case 5:

				opponent.displayStatsTest();
				player.displayStats();
				battlegroundInfo();
				break;
			case 6:

				fleeLogic();
				intensity++;
				turnsElapsed++;
				break;
			default:
				std::cout << "number out of range, try again";
				break;

			
				}
			if (opponent.enemyHealth <= 0) {

				battling = false;
				std::cout << "\n you win!";
			}
			if (turnsElapsed % 2 == 1) opponent.basicEnemyAi(player);
			}

			if (player.playerHealth <= 0) {

				battling = false;
				std::cout << "\n you lose!";
			}
			player.defense = player.temporaryDefense; //reset defense if you defended
			
		}
	}



};

int main() {

	srand(static_cast<unsigned int>(time(NULL)));
	enemy test;
	spells spelltest;
	spelltest.grantPlayerSpell(0);
	character test1(3,3,3,3,3,3);
	test.enemySet(10, 10, 1, 12, 10, 10, "beebie");
	combatHandler combat1(test1, test, spelltest);
	combat1.initiateCombat();
	return 0;
}