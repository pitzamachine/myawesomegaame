#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <limits>

int intensity = 0;
int input = 0;
bool battling = true;
double rolledNumber;
int diceRoll;
int turnsElapsed = 0;
int roundsPassed = 0;

bool handleInputFailure(const std::string& errorMessage) {
	if (std::cin.fail()) {
		std::cout << errorMessage << std::endl;
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		return true; // Indicates failure occurred
	}
	return false; // No failure
}

void setColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

class items {

private:

	char id = '0'; 
	std::string itemName = "default";
	int itemHealingPower = 0;
	int itemDamage = 0;
	char scalingType = '0'; // 0 static , 1 intensity, 2 atribute scaling, 3 for both
	float intensityScaleFactor = 0.0f; //item intensity scaling
	std::string attributeScale = "Strength"; //replace with scaling attribute
	float attributeScalingFactor = 0.0f;  //item attribute scaling
	int itemCost = 0;
	std::vector<char> itemsHeld;
	
	
public:

	void grantPlayerItem(char itemId) {


		itemsHeld.push_back(itemId);

	}

	void updateItem(char itemId) {


		switch (itemId) {


		case 0:
			itemName = "Rock";
			itemDamage = 12;
			scalingType = 1;
			intensityScaleFactor = 0.05f;
			itemHealingPower = 0;
			itemCost = 3;
			break;
		case 1:
			itemName = "Throwing Dagger";
			itemDamage = 7;
			scalingType = 2;
			attributeScalingFactor = 0.16f;
			attributeScale = "Dexterity";
			itemHealingPower = 0;
			itemCost = 5;

			break;
		case 2:
			itemName = "Dynamite";
			itemDamage = 50;
			scalingType = 0;
			itemCost = 10;
			itemHealingPower = 0;
			break;
		case 3:
			itemName = "Dynamite+";
			itemDamage = 65;
			scalingType = 1;
			intensityScaleFactor = 0.04f;
			itemHealingPower = 0;
			itemCost = 20;


			break;
		case 4:
			itemName = "Healing Potion-";
			itemHealingPower = 15;
			itemCost = 6;
			scalingType = 0;
			itemDamage = 0;

			break;

		case 5:
			itemName = "Healing Potion";
			itemHealingPower = 35;
			itemCost = 10;
			scalingType = 0;
			itemDamage = 0;

			break;

		case 6:

			itemName = "Healing Potion+";
			itemHealingPower = 65;
			itemCost = 18;
			scalingType = 0;
			itemDamage = 0;

			break;
		default:
			std::cout << "Hello, the item you're trying to access doesn't exist! oops!";
			break;



		}


	} // this holds the data for items

	char displayItemMenu() {

		if (itemsHeld.empty()) {

			std::cout << "You don't have any items. \n";
			return 'f';
		}
		else {
			int i = 1;
			std::cout << "-----Items----- \n";
			for (char itemId : itemsHeld) {

				id = itemId;
				updateItem(itemId);
				std::cout << i << ": " << itemName << std::endl;
				i++;

			}
			std::cout << "---------------\n";
			if (battling) {

				std::cout << "Type the number of the item you'd like to use, or anything else to cancel. \n";

				std::cin >> input;


				if (std::cin.fail() || input < 1 || input > itemsHeld.size()) {
					std::cout << "Item use cancelled. \n";
					std::cin.clear();
					std::cin.ignore(10000, '\n');
					return 'f';
				}
				return itemsHeld[input - 1];

			}

		}


		return 'f';

	};

	int useItem(int itemUsed) {

		int finalDamage = 0;
		int healingApplied = itemHealingPower;

		switch (scalingType) {
		case '0': {
			finalDamage = itemDamage;
			break;
		}
		case '1':

		case '2':

		case '3':

		default:
			break;


		}
		return finalDamage;
	}

};

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
	int attributePoints = 15;
	int level = 1;
	int gold = 0;
	int experience = 0;
	int experienceToNext = 40;
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
		int attributeAssigner = 0;
		int pointsToAssign = 0;
		while (attributePoints > 0) {
			std::cout << "Distribute your attribute points by typing 1 through 6, you have " << attributePoints << " left \n";
			std::cout << "1) Endurance: " << endurance << std::endl
				<< "2) Strength: " << strength << std::endl
				<< "3) Dexterity: " << dexterity << std::endl
				<< "4) Wisdom: " << wisdom << std::endl
				<< "5) Luck: " << luck << std::endl
				<< "6) Faith: " << faith << std::endl;


			std::cin >> attributeAssigner;

			if (handleInputFailure("\nInvalid input! Please enter a valid number between 1 and 6.")){
				continue;
		}
			if (attributeAssigner < 1 || attributeAssigner > 6) {
				std::cout << "Number out of range, try again.\n";
				continue;
			}

			std::cout << "How many points would you like to assign? (Remaining: " << attributePoints << ")\n";
			std::cin >> pointsToAssign;

			if (handleInputFailure("\nInvalid input! Please enter a valid number.")) {
				continue;
			}
			if (attributePoints < pointsToAssign || pointsToAssign <= 0) {
				std::cout << "You can't assign that many points.\n";
				continue;
			}

			switch (attributeAssigner) {

			case 1: {  //keep these brackets

				endurance += pointsToAssign;
				attributePoints -= pointsToAssign;
				std::cout << pointsToAssign << " points assigned to Endurance. New value: " << endurance << "\n";
				break;
			}
			case 2:
				strength += pointsToAssign;
				attributePoints -= pointsToAssign;
				std::cout << pointsToAssign << " points assigned to Strength. New value: " << strength << "\n";

				break;
			case 3:
			{
				dexterity += pointsToAssign;
				attributePoints -= pointsToAssign;
				std::cout << pointsToAssign << " points assigned to dexterity. New value: " << dexterity << "\n";


				break;
			}
			case 4:
			{
				wisdom += pointsToAssign;
				attributePoints -= pointsToAssign;
				std::cout << pointsToAssign << " points assigned to wisdom. New value: " << wisdom << "\n";

				break;
			}
			case 5:
				luck += pointsToAssign;
				attributePoints -= pointsToAssign;
				std::cout << pointsToAssign << " points assigned to Luck. New value: " << luck << "\n";
				
				break;
			case 6:

				faith += pointsToAssign;
				attributePoints -= pointsToAssign;
				std::cout << pointsToAssign << " points assigned to Faith. New value: " << faith << "\n";
				break;
			default:
				std::cout << "Number out of range, try again \n";
				break;


			}
			updateCharacterStats();
			playerHealth = playerMaxHealth;
		}
		std::cout << "Attribute points assigned.\n";
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
	int calculateMeleeDamage(int enemyDefense) {
		int baseDamage = static_cast<int>(strength *(1 + intensity * 0.33));
		int damage = baseDamage - enemyDefense;
		if (damage < 1) damage = 1;
		
		return damage;
	}

	bool criticalCalculator() {

		double critChance = 3 + luck + faith / 3;  // determine crit chance
		if (critChance > 93) critChance = 93;
		rolledNumber = rand() % 100;
		bool criticalHit = (critChance > rolledNumber);
		std::cout << std::endl << "Roll: " << rolledNumber;
		return criticalHit;
	}

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
		player.spellsAvailable -= cost;
		int finalDamage = 0;
		
		switch (scalingAttribute) {


		case '0':
			finalDamage = baseDamage + static_cast<int>((player.wisdom * attributeDamageModifier));
			finalDamage *= static_cast<int>((1 + intensity * 0.33));
			return finalDamage;
			break;
		case '1':

			finalDamage = baseDamage + static_cast<int>((player.faith * attributeDamageModifier));
			finalDamage *= static_cast<int>((1 + intensity * 0.33));
			return finalDamage;
			break;

		case '2':

			finalDamage = baseDamage + static_cast<int>(((player.faith + player.wisdom) * attributeDamageModifier));
			finalDamage *= static_cast<int>((1 + intensity * 0.33));
			return finalDamage;
			break;

		default:

			std::cout << "\n Something went wrong.. \n";
			return 0;
			



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

	int enemyGetDefense() {
		return defense;
	}

	void basicEnemyAi(character& player){
		if (battling)  {
			diceRoll = rand() % 4;
			switch (diceRoll) {

			case 0: {
				int finalDamage = (attack + intensity/3) - player.defense;
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
				std::cout << "The enemy  turns up the heat! Intensity + 3";
				intensity += 3;
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
	
	std::string battleArena = ""; // This will likely have some sort of impact, maybe giving status effects. Remember, keep the scope of the project smaller for now.
	bool playerTurn = true;
	character& player;
	enemy& opponent;
	spells& spell;
	items& item;

	bool intensity10 = false;
	bool intensity20 = false;
	bool intensity30 = false;

public:
	
	combatHandler(character& p, enemy& e, spells& s, items& i) : player(p), opponent(e), spell(s), item(i) {
		std::cout << std::endl << "BATTLE START!" << std::endl;
	};
	void battleRewards() {
		if (!battling && opponent.enemyHealth <= 0) {
			player.experience += opponent.experienceWorth;
			player.gold += opponent.goldDropped;
			setColor(6);
			std::cout << "\nYou gained " << opponent.goldDropped << " gold and " << opponent.experienceWorth << " experience!\n";
			setColor(7);
		}


	}
	
	void levelUpHandler() {
		int originalLevel = player.level;
		while (player.experience >= player.experienceToNext) {
			setColor(2);
			std::cout << "You leveled up to level " << player.level << ".\n";
			setColor(7);
			player.experience -= player.experienceToNext;
			player.level++;
			player.experienceToNext *= 1.33;
			player.attributePoints += 3;

		}
		if(player.level > originalLevel)
			player.pickAttributes();
	}

	void winLogic() {

		if (opponent.enemyHealth <= 0) {

			battling = false;
			std::cout << "\n you win!";
		}
		if (player.playerHealth <= 0) {

			battling = false;
			std::cout << "\n you lose!";
		}
	}

	void displayIntensity() {
		setColor(6);
		std::cout << "\nIntensity:" << intensity << std::endl;
		setColor(7);
	}

	void displayIntensityFlavorText() {

		if (intensity >= 10 && !intensity10) {
			setColor(6);
			std::cout << "\n---The air around you feels warmer---";
			setColor(7);
			intensity10 = true;
		}
		else if (intensity >= 20 && !intensity20) {
			setColor(4);
			std::cout << "\n---The atmosphere grows further hostile, your vision blurs---";
			setColor(7);
			intensity20 = true;
		}
		else if (intensity >= 30 && !intensity30) {
			setColor(12);
			std::cout << "\n---The air is thick with pressure. There is no escape.---";
			setColor(7);
			intensity30 = true;

		}
	}

	bool fleeLogic() {
		float intensityMultiplier = 1;
		if (intensity <= 10) intensityMultiplier = 0.6f;
		else intensityMultiplier = 1;
		double result = 6.2 * intensity * intensityMultiplier; // result is double
		int finalIntensity = static_cast<int>(result); // explicit cast to int
		int fleeValue = rand() % 301 + player.luck * 3 + player.dexterity * 2;
		int fleeNumberNeeded = static_cast<int>(100 + (sqrt(player.luck * (player.dexterity + 2)) * 4) + finalIntensity);
		if (fleeValue > fleeNumberNeeded){
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


			std::cin >> input; 
			

			if (std::cin.fail()) {

				
				std::cout << "Invalid input! Please enter a valid number." << std::endl;
				
				std::cin.clear();
				std::cin.ignore(10000, '\n');
				continue;

			}

			switch (input) {

			case 1: {  //keep these brackets
				
				int damage = player.calculateMeleeDamage(opponent.defense);
				
				bool criticalHit = player.criticalCalculator();

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
				std::cout << " \nYou brace for impact.. \n";
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
					
					intensity++;
					turnsElapsed++;
				}
				else {
					std::cout << "Insufficient spell power available";
				}
				


				break;
			}
			case 4:
			{
				int itemUsed = item.displayItemMenu();
				item.useItem(itemUsed);
				
				break;
			}
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
			
			winLogic();

			if (turnsElapsed % 2 == 1) opponent.basicEnemyAi(player);
			}

			winLogic();
			battleRewards();
			levelUpHandler();

			player.defense = player.temporaryDefense; //reset defense if you defended
			
			if(battling)		{
				displayIntensity();
				displayIntensityFlavorText();
			}

			
			}
		}
	};



int main() {

	srand(static_cast<unsigned int>(time(NULL)));
	enemy test;
	items testitem;
	spells spelltest;
	testitem.grantPlayerItem(2);
	spelltest.grantPlayerSpell(0);
	character test1(0,0,0,0,0,0);
	test1.pickAttributes();
	test.enemySet(20, 20, 1, 12, 10, 1700, "beebie");
	combatHandler combat1(test1, test, spelltest, testitem);
	combat1.initiateCombat();
	return 0;
}