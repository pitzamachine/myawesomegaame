#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <limits>
#include <unordered_map>
#include "startgame.h"
#include <algorithm>


int intensity = 0;
int input = 0;
bool battling = true;
double rolledNumber;
int diceRoll;
int enemyRoll;
int turnsElapsed = 0;
int roundsPassed = 0;
int roundsNeeded = 2;

void clear() {
	// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
	std::cout << "\x1B[2J\x1B[H";
	int b = 0;
	while (b < 100, b++) {

		std::cout << "             \n\n\n\n\n";
	}
}

struct EnemyStats {

	int maxHealth;
	int defense;
	int attack;
	unsigned short goldDropped = 0;
	unsigned short exp;
	char aiType;
	unsigned short minLevel = 0;

};

struct statusEffect {

	std::string name;
	std::string actionName;
	char statusId = '0';
	char type = 'h';
	unsigned short strengthValue = 0;
	unsigned short duration = 0;
	std::string buffTarget;


};

const std::unordered_map<std::string, EnemyStats> EnemyStatsMap = {

	{"Snow Golem", {40, 12, 6, 10, 30, '1', 2}},
	{"Little Golem", {30, 10, 6, 8, 30, '1', 1}},
	{"Soul Sucker",{25, 8, 10, 16, 60, '2', 4}},
	{"Living Armor", {80, 14, 6, 30, 100,'1', 5}},
	{"Fear Monger", {90,12,12,40, 150,'3', 6}}
};

float calcDamageVariance() {

	return 0.9f + (rand() % 21) / 100;

}

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


	int getDexterity() {
		return dexterity;
	}
	int getFaith() {
		return faith;
	}
	int getWisdom() {
		return wisdom;
	}
	void displayStats() {

		std::cout << "Player Name: " << name << std::endl
			<< "Level: " << level << "\n"
			<< "Experience: " << experience << "/" << experienceToNext 

			<< "\nGold: " << gold
		 << "\nMax Health: " << playerMaxHealth << std::endl
		 << "Current Health: " << playerHealth << std::endl
		 << "Defense: " << defense << std::endl
		 << "Magic Defense: " << magicDefense << std::endl
		 << "--------Stats--------" << std::endl
		 << "Endurance: " << endurance << std::endl
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
		clear();
		
		std::cout << "Attribute points assigned.\n";
	}
	void updateCharacterStats() {

		playerMaxHealth = 10 + 3*(level-1) + endurance * 3;
		defense = 1 + endurance * 2 + strength * 1 + (level*0.5);
		temporaryDefense = defense;
		magicDefense = faith * 3 + wisdom * 1 + luck * 1;
		maximumSpellsAvailable = (faith / 4.5f + wisdom / 2.7f); // adds up the two variables and rounds down
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
		float baseDamage = static_cast<int>((strength *(1 + intensity * 0.1))-static_cast<float>(enemyDefense/2));		
		int minimumDamage = 1 + static_cast<int>(strength / 4.0f + faith/10.0f + wisdom/10.0f + luck/10.0f + dexterity/10.0f);
		int damage = baseDamage * calcDamageVariance();
		if (damage < minimumDamage) damage = minimumDamage;
		
		return damage;
	}

	bool criticalCalculator() {

		double critChance = 3 + luck + faith / 3;  // determine crit chance
		if (critChance > 93) critChance = 93;
		rolledNumber = rand() % 100;
		bool criticalHit = (critChance > rolledNumber);
		std::cout << std::endl << "Roll: " << rolledNumber << "(<" << critChance << " needed) \n";
		return criticalHit;
	}

	friend class combatHandler;
	friend class spells;
	friend class items;
};

class spells {

public:
	char id = '0';
	int baseDamage = 0;
	float attributePowerModifier = 0; 
	short spellSlotCost = 0;
	short spellPrice = 0;
	int baseHealing = 0;
	char scalingAttribute = '0'; // scalingAttribute of 0 is wisdom, 1 is faith, 2 is both, 3 is neither
	std::string spellName = "default";
	std::vector<char> spellsKnown;
	

	spells() {

		id = 0;
		baseDamage = 3;
		attributePowerModifier = 1.2f;
		spellName = "Zaqp";
		scalingAttribute = '0';

	}
	
	char displaySpellMenu(character& player) {

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
				std::cout << i << ": " << spellName << " | Cost: " << spellSlotCost  << " | Damage: " << calculateSpellPower(player,true) << std::endl;
				i++;

			}
			std::cout << "---------------- \n";
			if (battling) {

				std::cout << "Type the number of the spell you'd like to cast, or anything else to cancel casting. \n";

				std::cin >> input;


				if (std::cin.fail() || input < 1 || input > spellsKnown.size()) {
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
			baseHealing = 0;
			baseDamage = 4;
			attributePowerModifier = 1.6f;
			spellName = "Zap";
			scalingAttribute = '0';
			spellSlotCost = 1;
			spellPrice = 20;
			break;
		case 1:
			baseHealing = 0;
			baseDamage = 5;
			attributePowerModifier = 1.32f;
			spellName = "Light Ray";
			scalingAttribute = '1';
			spellSlotCost = 1;
			spellPrice = 25;

			break;

		case 2:
			baseHealing = 0;
			baseDamage = 15;
			spellName = "Hailstorm";
			attributePowerModifier = 1.4f;
			scalingAttribute = '2'; // both
			spellSlotCost = 2;
			spellPrice = 40;

			break;

		case 3:
			baseHealing = 0;
			baseDamage = 24;
			spellName = "Fireball";
			attributePowerModifier = 1.05f;
			scalingAttribute = '0'; // wisdom
			spellSlotCost = 2;
			spellPrice = 40;

			break;

		case 4:
			baseHealing = 7;
			baseDamage = 0;
			spellName = "Minor Heal";
			attributePowerModifier = 0.9f;
			scalingAttribute = '1'; // faith
			spellSlotCost = 1;
			spellPrice = 35;
			break;

		case 5:
			baseDamage = 3;
			spellName = "Spark";
			attributePowerModifier = 0.33f;
			scalingAttribute = '2';
			spellSlotCost = 1;
			spellPrice = 5;
			baseHealing = 0;

			break;

		case 6:
			baseHealing = 15;
			baseDamage = 0;
			spellName = "Heal";
			attributePowerModifier = 1.25f;
			scalingAttribute = '1'; // faith
			spellSlotCost = 2;
			spellPrice = 65;
			break;

		case 7:
			baseHealing = 3;
			baseDamage = 10;
			spellName = "Unholy Siphon";
			attributePowerModifier = 1.25f;
			scalingAttribute = '1'; // faith
			spellSlotCost = 2;
			spellPrice = 55;
			break;

		default:
			std::cout << "Hello, i'm not a spell!"; //this will display this humorous text if some how
			// you input a wrong value when updating the spell
			break;



		}


	}

	float powerModifier(character& player) {
		float modifier = 0;
		switch (scalingAttribute) {


		case '0':
			modifier = ((player.wisdom * attributePowerModifier));

			break;
		case '1':

			modifier = ((sqrt(player.faith / 1.4) * attributePowerModifier));

			break;
		case '2':

			modifier = (((player.faith + player.wisdom) * attributePowerModifier));

			break;

		default:

			break;
		}
		return modifier;
	}

	int calculateSpellPower(character& player, bool display) {
		// this code decides the spell damage based off the scaling types. 0 for wisdom, 1 for faith, 2 for both.
		if(!display) player.spellsAvailable -= spellSlotCost;
		int finalDamage = 0;
		int finalHealing = baseHealing + powerModifier(player);
		int originalPlayerHealth = player.playerHealth;
		if (!display) {
			if (baseHealing > 0 && player.playerHealth != player.playerMaxHealth) {

			player.playerHealth += finalHealing;

			if (player.playerHealth > player.playerMaxHealth)
				player.playerHealth = player.playerMaxHealth;

			std::cout << "\nYou gain " << player.playerHealth - originalPlayerHealth << " Health Points!\n";
			}
			else {
				if (baseDamage == 0)
					player.spellsAvailable += spellSlotCost;
		}
	}
		switch (scalingAttribute) {

		case '0':
			finalDamage = baseDamage + powerModifier(player);
			finalDamage *= static_cast<int>((1 + intensity * 0.1));
			return finalDamage;
			break;
		case '1':

			finalDamage = baseDamage * powerModifier(player);
			finalDamage *= static_cast<int>((1 + intensity * 0.172));
			return finalDamage;
			break;

		case '2':

			finalDamage = baseDamage + powerModifier(player);
			finalDamage *= static_cast<int>((1 + intensity * 0.1));
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
	int minLevel = 0;
	int tempDefense = 0;
	unsigned short goldDropped = 0;
	unsigned short experienceWorth = 0;
	std::string name = " ";
	char aiType = '0';

public:

	friend class combatHandler;

	friend class items;

	friend class character;

	enemy(){
		
		std::cout << "enemy encountered" << std::endl;
		
	}

	void enemySet(int enemyMaxHp, int enemyHp, int edefense, int eattack, int gold, int exp, std:: string ename, character& player) {
		enemyRoll = rand() % 6;
		if (enemyRoll == 3) {
			auto it = EnemyStatsMap.begin();
			std::advance(it, rand() % EnemyStatsMap.size());
			const EnemyStats& stats = it->second;
			minLevel = it->second.minLevel;
			//if (player.level < minLevel){
				name = it->first;
				enemyMaxHealth = it->second.maxHealth;
				enemyHealth = it->second.maxHealth;
				defense = it->second.defense;
				attack = it->second.attack;
				goldDropped = it->second.goldDropped;
				experienceWorth = it->second.exp;
				aiType = it->second.aiType;
				tempDefense = defense;
			//}
		
		}
		else {
			enemyMaxHealth = enemyMaxHp;
			enemyHealth = enemyHp;
			defense = edefense;
			attack = eattack;
			goldDropped = gold;
			experienceWorth = exp;
			name = ename;

		}


	

	};

	void takeDamage(int damage){
		setColor(3);
		std::cout << std::endl << "Enemy took " << damage << " damage!" << std::endl;
		enemyHealth -= damage;
		setColor(7);
	}

	float enemyCalculateMeleeDamage(character& player) {

		float baseDamage = (attack * (1 + static_cast<float>(intensity) / 8)) - player.defense/3;
		float variance = 0.9f + (rand() % 21) / 100.0;
		int finalDamage = static_cast<int>(baseDamage * variance);
		if (finalDamage< 1) finalDamage = 1;
		return finalDamage;
	}; 

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
			setColor(13);
			switch (diceRoll) {
				
			case 0: {
				
				player.takeDamage(enemyCalculateMeleeDamage(player));
				turnsElapsed++;
				break;
			}
			case 1:
				//defend
				std::cout << "\nEnemy defends, you'll do less melee damage\n";
				tempDefense = defense;
				defense = defense * 1.33;
				turnsElapsed++;
				break;
			case 2:
				std::cout << "\nThe enemy  turns up the heat! Intensity + 3\n";
				intensity += 3;
				turnsElapsed++;
				//do nothing
				break;
			case 3:
				std::cout << "\nEnemy does nothing.\n";
				//also do nothing
				turnsElapsed++;
				break;
			}
			setColor(7);
		}

	}

};

class items {

private:

	char id = '0';
	std::string itemName = "default";
	int itemHealingPower = 0;
	int itemDamage = 0;
	std::string scalingType = "static"; //  static, intensity, attribute, both
	float intensityScaleFactor = 0.0f; //item intensity scaling
	std::string attributeScale = "strength"; //replace with scaling attribute
	float attributeScalingFactor = 0.0f;  //item attribute scaling
	std::string itemType = "healing"; // damage, healing, buff, debuff
	bool hasVariance = false; //determines if the item damage varies from 0.9 to 1.1
	int itemCost = 0;
	int itemQuantity = 1;
	int intensityChange = 0; // + = more intense, - = less intense
	std::vector<char> itemsHeld;
	int spellSlotsRestored = 0;
	friend class shop;
	int input = 0;

public:
	
	const std::unordered_map<std::string, int> scalingTypeMap = {
	{"static", 0},
	{"intensity", 1},
	{"attribute", 2},
	{"both", 3}
	};

	int getScalingTypeCode(const std::string& scalingType) {
		auto it = scalingTypeMap.find(scalingType); // this looks within the map to find the scaling type.
		if (it != scalingTypeMap.end()) {
			return it->second; // this returns the second value of what we were looking through
		}
		return -1; // 
	}

	int getPlayerAttribute(character& player, const std::string& attribute) {
		// Map the string to the corresponding player attribute
		if (attribute == "strength") {
			return player.strength;
		}
		else if (attribute == "dexterity") {
			return player.dexterity;
		}
		else if (attribute == "wisdom") {
			return player.wisdom;
		}

		else if (attribute == "faith") {
			return player.faith;
		}

		// Return a default value if the attribute doesn't exist
		static int defaultAttr = 0;
		return defaultAttr;
	}

	void grantPlayerItem(char itemId) {


		itemsHeld.push_back(itemId);

	}

	void updateItem(char itemId) {
		
		itemQuantity = 1;
		intensityChange = 0;
		switch (itemId) {

			

		case 0:
			itemName = "Rock";
			itemDamage = 12;
			scalingType = "intensity";
			itemType = "damage";
			intensityScaleFactor = 0.05f;
			itemHealingPower = 0;
			itemCost = 4;
			break;
		case 1:
			itemName = "Throwing Dagger";
			itemDamage = 7;
			scalingType = "both";
			itemType = "damage";
			intensityScaleFactor = 0.07f;
			attributeScalingFactor = 0.20f;
			attributeScale = "dexterity";
			itemQuantity = 2;
			itemHealingPower = 0;
			itemCost = 5;

			break;
		case 2:
			itemName = "Dynamite";
			itemDamage = 25;
			scalingType = "static";
			itemType = "damage";
			itemCost = 11;
			itemHealingPower = 0;
			break;
		case 3:
			itemName = "Dynamite+";
			itemDamage = 40;
			scalingType = "intensity";
			intensityScaleFactor = 0.03f;
			itemHealingPower = 0;
			itemType = "damage";
			itemCost = 25;

			break;
		case 4:
			itemName = "Healing Potion-";
			itemHealingPower = 15;
			itemCost = 6;
			scalingType = "static";
			itemType = "healing";
			itemDamage = 0;

			break;

		case 5:
			itemName = "Healing Potion";
			itemHealingPower = 35;
			itemCost = 10;
			scalingType = "static";
			itemType = "healing";
			itemDamage = 0;

			break;

		case 6:

			itemName = "Healing Potion+";
			itemHealingPower = 65;
			itemCost = 15;
			scalingType = "static";
			itemType = "healing";
			itemDamage = 0;

			break;

		case 7:

			itemName = "Manapowder";
			itemHealingPower = 5;
			itemCost = 40;
			scalingType = "static";
			itemType = "healing";
			itemDamage = 0;
			spellSlotsRestored = 1;

			break;

		case 8:

			itemName = "Sanguine Thrownknife";
			itemHealingPower = 4;
			itemCost = 10;
			scalingType = "attribute";
			attributeScale = "dexterity";
			itemType = "damage";
			itemDamage = 10;
			attributeScalingFactor = 0.15f;
			

			break;

		case 9:

			itemName = "Boulder";
			itemHealingPower = 0;
			itemCost = 25;
			scalingType = "attribute";
			attributeScale = "strength";
			itemType = "damage";
			itemDamage = 9;
			attributeScalingFactor = 0.19f;

			break;
			
		case 10:

			itemName = "Prayer Bead";
			itemHealingPower = 0;
			itemCost = 5;
			scalingType = "attribute";
			attributeScale = "faith";
			itemType = "damage";
			itemDamage = 2;
			attributeScalingFactor = 0.50f;

			break;

		case 11:
			itemName = "Crazy Orb";
			itemDamage = 0;
			scalingType = "static";
			itemType = "buff";
			intensityScaleFactor = 0.0f;
			itemHealingPower = 0;
			itemCost = 12;
			intensityChange = 5;
			break;
		case 12:
			itemName = "Calm Feather";
			itemDamage = 0;
			scalingType = "static";
			itemType = "buff";
			intensityScaleFactor = 0.0f;
			itemHealingPower = 0;
			itemCost = 10;
			intensityChange = -5;
			break;
		case 13:
			itemName = "Grand Hammer";
			itemDamage = 30;
			scalingType = "static";
			itemType = "damage";
			intensityScaleFactor = 0.0f;
			itemHealingPower = 0;
			itemCost = 25;
			intensityChange = -7;
			break;
		case 14:
			itemName = "Escalating Dagger";
			itemDamage = 7;
			itemHealingPower = 0;
			attributeScale = "dexterity";
			scalingType = "attribute";
			itemType = "damage";
			itemCost = 8;
			intensityChange = 1;
			attributeScalingFactor = 0.17f;
			break;

		case 15:
			itemName = "Firebomb";
			itemDamage = 15;
			scalingType = "intensity";
			itemType = "damage";
			intensityScaleFactor = 0.05f;
			intensityChange = 2;
			itemCost = 12;
			break;
		default:
			std::cout << "Hello, the item you're trying to access doesn't exist! oops!";
			break;



		}


	} // this holds the data for items //stores all item data

	char displayItemMenu(character& player, enemy& opponent) {

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
				std::cout << i << ": " << itemName <<"\n";
				if (itemType == "damage")
					std::cout << "Base Damage : " << itemDamage << "\n";
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

				char usedItem = itemsHeld[input - 1];
				
				useItem(usedItem, player, opponent);
				intensity++;
				
			}
		}

		return 'f';
	};

	void useItem(char itemUsed, character& player, enemy& opponent) {
		updateItem(itemUsed);

		if (intensityChange != 0) {
			if ((intensity + intensityChange) < 0) {
				std::cout << "\nThings can't get any calmer.\n";
			}else
				intensity += intensityChange;
			std::cout << "\nThe world grows more restless\n";
		}
		if (itemType == "healing" && player.playerHealth < player.playerMaxHealth) {
			player.playerHealth += itemHealingPower; // you should replace this with a player.heal function
			std::cout << "\nYou gain " << itemHealingPower << " Health Points!";
			itemsHeld.erase(itemsHeld.begin() + (input - 1));

			if (spellSlotsRestored > 0) {
				player.spellsAvailable += spellSlotsRestored;
				if (player.spellsAvailable > player.maximumSpellsAvailable) {
					std::cout << "\nSome of the ethereal mana fades away";
					player.spellsAvailable = player.maximumSpellsAvailable;
				}
			}

			if (player.playerHealth > player.playerMaxHealth) {
				std::cout << "\nBut some of it was wasted..";
				player.playerHealth = player.playerMaxHealth;
			}
			

		}
		if (itemType == "buff") itemsHeld.erase(itemsHeld.begin() + (input - 1));
		 if (itemType == "damage") {
			int finalItemDamage = itemDamage;
			int scalingCode = getScalingTypeCode(scalingType);
			int attribute = getPlayerAttribute(player, attributeScale);
			if (itemHealingPower > 0) {
				player.playerHealth += itemHealingPower; // you should replace this with a player.heal function
				std::cout << "\nYou gain " << itemHealingPower << " Health Points!";
			}
			switch (scalingCode) {

			case 1:
				finalItemDamage = itemDamage * (1 + intensity * intensityScaleFactor);
				itemsHeld.erase(itemsHeld.begin() + (input - 1));
				break;
			case 2:
				finalItemDamage = itemDamage * (1 + attribute * attributeScalingFactor);
				itemsHeld.erase(itemsHeld.begin() + (input - 1));
				break;
			case 3:
				finalItemDamage = itemDamage * (1 + ((attribute * attributeScalingFactor)+(intensity*intensityScaleFactor)));
				itemsHeld.erase(itemsHeld.begin() + (input - 1));
				break;
			default:
				itemsHeld.erase(itemsHeld.begin() + (input - 1));
				break;

			}
			std::cout << "Your item does " << finalItemDamage << " Damage!";

			opponent.takeDamage(finalItemDamage);
			if (id == 1 || id == 8 || id == 14) {
			label_name: //i've never used go to, but I was too lazy to make a while loop. Have fun throwing a ton of daggers.

				float multiDaggerChance = player.dexterity / 2.5;
				if (multiDaggerChance > 20) multiDaggerChance = 20;
				rolledNumber = rand() % 101;
				if (multiDaggerChance > rolledNumber) {
					finalItemDamage /= 1.5;
					std::cout << "\nAnother dagger appears, flying into your target!";
					opponent.takeDamage(finalItemDamage);
					switch (id) {

					case 8:
						player.playerHealth += itemHealingPower;
						std::cout << "\nYou gain " << itemHealingPower << " Health Points!";
						break;
					case 14:
						intensity += intensityChange;
						break;
					}

					goto label_name;
				}
			} // multi dagger throwing
					
			
		}
		 turnsElapsed++;
	}

};

class statuseffects {
private:

	std::vector<statusEffect> playerStatuses; 
	std::vector<statusEffect> enemyStatuses;

	std::string defaultName = "Bleed";
	std::string defaultActionName = "Bleeding"; // "You took 3 damage due to : Bleeding"
	char defaultStatusId = '0';
	char defaultType = 'h'; //h for healing over time, d for damage over time, b for buff, f for debuff
	unsigned short defaultStrengthValue = 0;
	unsigned short defaultDuration = 0;
	std::string defaultBuffTarget = "dexterity";



public:
	character& player;
	enemy& opponent;

	void grantStatusEffect(character& player,enemy& opponent,bool onPlayer, char statusId){
		//dealing with vectors
	//if on player true
		//statuspushback.player
	//if on player flase
		//statuspushback.enemy

	}
	 
	void tickStatus() {
		// if turnsElapsed % 2 = 0
			//for(statuses in player.statusEffects )
				//do status effects on player
				//duration - 1
		// if turnsElapsed % 2 = 1
			//for(statuses in opponent.statusEffects )
				//do status effects on enemy
				//duration - 1
	}

	

};

class shop {

private:
	int objectsAvailable = 4;
	std::string shopType = "items";
	bool itemsGenerated = false;
	bool spellsGenerated = false;
	bool doneShopping = false;
	std::vector<char> availableSpells = { 0, 1, 2, 3, 4, 5, 6, 7 };
	std::vector<char> spellsInShop;

public:

	spells& shopSpell;
	items& shopItem;
	character& player;
	friend items;
	friend spells;
	friend character;
	std::vector<char> itemsInShop;
	shop(spells& s, items& i, character& c) : shopSpell(s), shopItem(i), player(c){


	}

	void healChoiceMenu() {
		int healingPrice = ((player.playerMaxHealth - player.playerHealth) / 3);
		int spellSlotPrice =  4 * 1 * (player.level * 0.8);
		if (spellSlotPrice > 20) spellSlotPrice = 20;

		std::cout << "-----Healing-----\n" <<
			"1) Heal 3 HP: 1 Gold\n" <<
			"2) Full Heal: " << healingPrice << " Gold\n" <<
			"3) One spell slot: " << spellSlotPrice << " Gold\n" <<
			"4) Full spell slots: " << spellSlotPrice * (player.maximumSpellsAvailable - player.spellsAvailable) << " Gold\n";
			
	}

	void itemShopMenu(items& shopItem) {
		
		std::cout << "-----Buy Items-----\n";
		
		for (int i = 1; i <= objectsAvailable; i++) { 
				
			int randomItemId = rand() % 16; //generates a random item
			bool isPair = (rand() % 100 < 20);
			if(!itemsGenerated) itemsInShop.push_back(randomItemId); // places it into the shop item vector
		
			}
		itemsGenerated = true;
			
		for (int i = 0; i < itemsInShop.size(); i++) {
			int itemId = itemsInShop[i];
			shopItem.updateItem(itemId); //reads and updates the data

			std::cout << i + 1<< ") " << shopItem.itemQuantity << " " << shopItem.itemName << " | Cost: " << shopItem.itemCost << " gold.\n";

		
		}
		if (!itemsInShop.empty()) {
			std::cout << "What would you like to buy?\n";

			int purchaseChoice;
			std::cin >> purchaseChoice;
			handleInputFailure("Choose one of the displayed numbers.");

			if (purchaseChoice >= 1 && purchaseChoice <= itemsInShop.size()) {

				int selectedItemId = itemsInShop[purchaseChoice - 1];
				shopItem.updateItem(selectedItemId);

				if (player.gold >= shopItem.itemCost) {
					player.gold -= shopItem.itemCost;
					for(int i = 0;i < shopItem.itemQuantity; i++) shopItem.itemsHeld.push_back(selectedItemId);
					
					itemsInShop.erase(itemsInShop.begin() + (purchaseChoice - 1));
					std::cout << "\nItem(s) Purchased";

				}
				else {

					std::cout << "\nYou don't have enough gold.";
				}
			}
		}
		else
		{
			std::cout << "You bought everything!";
		}
	}

	void shopReset() {
		std::cout << "\nBye-bye!\n";
		doneShopping = true;
		itemsGenerated = false;
		spellsGenerated = false;
		itemsInShop.clear();
		spellsInShop.clear();
	}

	void spellShopMenu() {
		std::cout << "-----Buy Spells-----\n";
		
		
		if(!spellsGenerated) while (spellsInShop.size() < 2) {

			int randomSpell = rand() % availableSpells.size(); // gets a random spell from the spells available in the game
			char spellId = availableSpells[randomSpell];

			if (std::find(shopSpell.spellsKnown.begin(), shopSpell.spellsKnown.end(), spellId) == shopSpell.spellsKnown.end() &&
				std::find(spellsInShop.begin(), spellsInShop.end(), spellId) == spellsInShop.end()) {
				spellsInShop.push_back(spellId);
			}
			


		}
		spellsGenerated = true;
		
		for (int i = 0; i < spellsInShop.size(); i++) {

			shopSpell.updateSpell(spellsInShop[i]);
			std::cout << i + 1 << ") " << shopSpell.spellName << " | Cost:" << shopSpell.spellPrice << " gold\n";


		}

		if (!spellsInShop.empty()) {

			std::cout << "What would you like to buy?\n";

			int purchaseChoice;
			std::cin >> purchaseChoice;
			handleInputFailure("Choose one of the displayed numbers.");

			if (purchaseChoice >= 1 && purchaseChoice <= spellsInShop.size()) {

				int selectedSpellId = spellsInShop[purchaseChoice - 1];
				shopSpell.updateSpell(selectedSpellId);

				if (player.gold >= shopSpell.spellPrice) {
					player.gold -= shopSpell.spellPrice;
					shopSpell.grantPlayerSpell(selectedSpellId);
					std::cout << "Spell Purchased! You now know " << shopSpell.spellName << ".\n";
				}
				else {
					std::cout << "Not enough gold.\n";
				}
			}
		}
		else
		{
			std::cout << "You bought everything!";
		}
	}

	void shopMenu() {

		int shopChoice;
		doneShopping = false;
		setColor(4);
		std::cout <<  "--Welcome to the shop!--\n";
		setColor(7);
		while (!doneShopping && player.playerHealth > 0) {
			int healPrice = ((player.playerMaxHealth - player.playerHealth) / 3 + 4 * (player.maximumSpellsAvailable - player.spellsAvailable)) * (player.level*0.8);
			std::cout
				<< "\nYou have " << player.gold << " Gold."
				<< "\n1) Items"
				<< "\n2) Spells"
				<< "\n3) Healer" 
				<< "\n4) Check"
				<< "\n5) Leave shop\n";

			
			

			std::cin >> shopChoice;

			if (std::cin.fail()) { 
				std::cin.clear();
				std::cin.ignore(10000, '\n');
				std::cout << "Invalid input! Choose a number 1-3.\n";
				continue; // i didn't know this but it restarts the loop, I think theres another one which doesn't do that?
			}

			switch (shopChoice) {

			case 1:
				itemShopMenu(shopItem);
				break;
			case 2:
				spellShopMenu();
				break;
			case 3: // handles player healing. All or nothing. I think i'll give you the ability to pick between healing spell slots and hp
			{
				healChoiceMenu();
				//if (player.gold >= healPrice) {
					//player.gold -= healPrice;
					//player.playerHealth = player.playerMaxHealth;
					//player.spellsAvailable = player.maximumSpellsAvailable;
					//setColor(2);
					//std::cout << "\nYou have been healed";
					//setColor(7);
				//}
				//else
				//	std::cout << "\nNot enough gold";
				break;
			}

				
			case 4:
				player.displayStats();
				break;
			case 5:
				shopReset(); // this quits the shop btw
				break;
			default:
				std::cout << "Please enter a number 1-5";
				break;

			};

			
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
	shop& shops;

	bool intensity10 = false;
	bool intensity20 = false;
	bool intensity30 = false;

public:
	
	combatHandler(character& p, enemy& e, spells& s, items& i, shop& sh) : player(p), opponent(e), spell(s), item(i), shops(sh) {
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

	void enemyRandomizer() {
		int randomHealth = 0;
		int randomAttack = 0;
		int randomDefense = 0;
		int randomXP = 0;
		int randomGold = 0;
		std::string randomName = "Alfred";

		randomHealth = rand() % (28 + player.level * 4) + 4;
		randomAttack = rand() % (3 + player.level * 4) + 1;
		randomDefense = rand() % (12 + player.level * 3) + 1;
		randomXP = (randomHealth + randomAttack + randomDefense)/1.5;
		randomGold = rand() % 8 + player.level * 2 + randomXP/20;
		randomName = "alfred " + std::to_string(rand() % 200);

		opponent.enemySet(randomHealth, randomHealth, randomDefense, randomAttack, randomGold, randomXP, randomName,player);
		
	};

	void initiateCombat() {
		setColor(7);
		std::cout << "\n---------Battle Start---------\n";
		setColor(15);
		std::cout << "\nBattle " << roundsPassed + 1 << "/" << roundsNeeded << "\n";
		setColor(7);
		if (enemyRoll == 3)std::cout << "\n Special Encounter: " << opponent.name << ".";
		while (battling)  {

			

			if(turnsElapsed % 2 == 0){

				
				std::cout
					<< "\nEnemy Health: " << opponent.enemyHealth << " / " << opponent.enemyMaxHealth
					<< "\nEnemy Attack: " << opponent.enemyCalculateMeleeDamage(player)
					<< "\nYour Health: " << player.playerHealth << " / " << player.playerMaxHealth
				<< std::endl << "1) Attack" << " | Damage: " << player.calculateMeleeDamage(opponent.defense)
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
				
				std::cout << "\nYou swing at the enemy... \n";
				
				int damage = player.calculateMeleeDamage(opponent.defense);
				
				bool criticalHit = player.criticalCalculator();

				if (criticalHit) {
					damage *= 2;
					setColor(4);
					std::cout << "\nCritical Hit!\n ";
					setColor(7);
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
				std::cout <<"\nSpells Available: "<< player.spellsAvailable << "\n";
				char spellUsed = spell.displaySpellMenu(player);
				if (spellUsed == 'f') break;  //f is the default 
				spell.updateSpell(spellUsed);

				if (spell.spellSlotCost <= player.spellsAvailable) {
					int spellDamage = spell.calculateSpellPower(player, false);
					if (spellDamage > 0)
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
				char itemUsed = item.displayItemMenu(player, opponent);
				
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
			
			opponent.defense = opponent.tempDefense; //also reset enemy defense
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

			enemyRandomizer();
			
		}
			
	};



int main() {
	startMenu();
	srand(static_cast<unsigned int>(time(NULL)));

	items testitem;
	spells spelltest;
	character test1(0,0,0,0,0,0);
	shop shoptest(spelltest, testitem, test1);
	test1.pickAttributes();
	//clear();

	if (test1.getDexterity() >= 5) {
		testitem.grantPlayerItem(1); 
		testitem.grantPlayerItem(1);
		testitem.grantPlayerItem(1);
		std::cout << "\nYour dexterity grants you a free trio of throwing daggers.\n";

	}
	if (test1.getFaith() >= 5) {
		testitem.grantPlayerItem(10);
		testitem.grantPlayerItem(10);
		testitem.grantPlayerItem(10);
		std::cout << "\nYou find a broken cross necklace, and take some of the beads off of it.\n";
		spelltest.grantPlayerSpell(4);

	}

	if (test1.getWisdom() >= 5) {
	
		std::cout << "\nUsing your intellect, you realize you can shoot sparks from your hands.\n";
		spelltest.grantPlayerSpell(5);

	}
	enemy test;
	test.enemySet(20, 20, 1, 4, 10, 20, "beebie", test1);
	combatHandler combat1(test1, test, spelltest, testitem, shoptest);
	while (test1.playerHealth > 0) {
		battling = true;
		intensity = 0;
		turnsElapsed = 0;
		combat1.initiateCombat();
		roundsPassed++;
		if (roundsPassed >= roundsNeeded) {
			roundsPassed = 0;
			shoptest.shopMenu();
		}
	}
	
	
}