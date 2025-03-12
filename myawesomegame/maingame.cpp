#define NOMINMAX
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
#include <iomanip>


int intensity = 0;
int input = 0;
bool battling = true;
double rolledNumber;
int diceRoll;
int enemyRoll;
int turnsElapsed = 0;
int roundsPassed = 0;
int roundsNeeded = 2;

//bonus stats
unsigned short bonusHealth = 0;
unsigned short bonusDefense = 0;
unsigned short bonusIntensity = 0;
unsigned short bonusCritChance = 0;
unsigned short bonusMeleeDamage = 0;
unsigned short bonusSpellDamage = 0;
unsigned short bonusRecycle = 0;
unsigned short bonusItemDamage = 0;
unsigned short bonusFreeItemTurnChance = 0;
unsigned short bonusSpellSlots = 0;
std::string playerSpeciality = "fuck";



std::string drawHealthBar(int currentHP, int maxHP) { 
	int segments = std::round((static_cast<float>(currentHP) / maxHP) * 9);
	segments = std::max(0, std::min(9, segments));
	std::string bar = "[";
	for (int i = 0; i < 9; i++) {
		bar += (i < segments) ? "#" : "_";
	}
	bar += "]";
	return bar;
}

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
	{"Fear Monger", {90,12,12,40, 150,'3', 6}},
	{ "Fat Dragon", {180,25,25,100, 250,'4', 9}},
	{ "Penultimate Knight", {220,34,22,120, 350,'3', 10}}
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
	unsigned short temporarySpellSlots = 0;

	void setName(std::string yourname) {

		name = yourname;
	}

	int getEndurance() {
		return endurance;
	}
	int getDexterity() {
		return dexterity;
	}
	int getFaith() {
		return faith;
	}
	int getWisdom() {
		return wisdom;
	}
	int getLuck() {
		return luck;
	}
	int getStrength() {
		return strength;
	}

	void grantGold(int amount) {
		gold += amount;
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
			if (playerHealth < playerMaxHealth)
				playerHealth += playerMaxHealth - playerHealth;
			else
				std::cout << "\nYou preserve your bonus health.";
		}
		clear();
		
		std::cout << "Attribute points assigned.\n";
	}
	void updateCharacterStats() {

		playerMaxHealth = 10 + bonusHealth + 3*(level-1) + endurance * 3;
		defense = 1 + endurance * 2 + strength * 1 + (level*0.5) + bonusDefense;
		temporaryDefense = defense;
		magicDefense = faith * 3 + wisdom * 1 + luck * 1;
		maximumSpellsAvailable = (faith / 4.5f + wisdom / 2.7f) + bonusSpellSlots; // adds up the 3 variables and rounds down
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
		float baseDamage = static_cast<int>((strength *(1 + intensity * 0.1))-static_cast<float>(enemyDefense/1.4f));		
		int minimumDamage = 1 + static_cast<int>(strength / 4.0f + faith/10.0f + wisdom/10.0f + luck/10.0f + dexterity/10.0f);
		int damage = baseDamage * calcDamageVariance();
		if (damage < minimumDamage) damage = minimumDamage;
		
		return damage + bonusMeleeDamage;
	}

	bool criticalCalculator() {

		double critChance = 3 + luck + faith / 3 + dexterity/4 + bonusCritChance;  // determine crit chance
		if (critChance > 93) critChance = 93;
		rolledNumber = rand() % 100;
		bool criticalHit = (critChance > rolledNumber);
		std::cout << std::endl << "Roll: " << rolledNumber << "(<" << critChance << " needed) \n";
		return criticalHit;
	}

	friend class combatHandler;
	friend class spells;
	friend class items;
	friend class statuseffects;
};

class spells {

public:
	char id = '0';
	int baseDamage = 0;
	float attributePowerModifier = 0; 
	short spellSlotCost = 0;
	short spellPrice = 0;
	int baseHealing = 0;
	int healthCost = 0;
	std::string statusEffect = ""; 
	float statusApplyChance = 0.05;
	char scalingAttribute = '0'; // scalingAttribute of 0 is wisdom, 1 is faith, 2 is both, 3 is neither, 4 is strength, 5 is dex?
	std::string spellName = "default";
	short intensityChange = 0;
	std::vector<char> spellsKnown;
	

	spells() {

		id = 0;
		baseDamage = 3;
		attributePowerModifier = 1.2f;
		spellName = "Zaqp";
		scalingAttribute = '0';

	}
	
	struct spellEffect {
		int damage = 0;
		int healing = 0;
		int selfDamage = 0;
		unsigned short intensityChanger = 0;
	};
	
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
				spellEffect effect = calculateSpellPower(player, true);
				std::cout << std::setw(2) << i << ": " << std::setw(15) << spellName
					<< " | Cost: " << std::setw(2) << spellSlotCost
					<< " | Damage: " << std::setw(3) << effect.damage
					<< " | Healing: " << std::setw(3) << effect.healing
					<< " | Self Damage: " << std::setw(3) << effect.selfDamage 
				<< " | Intensity Change: " << std::setw(3) << intensityChange << std::endl;
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
		healthCost = 0;
		intensityChange = 0;
		switch (SpellId) {


		case 0:
			baseHealing = 0;
			baseDamage = 4;
			attributePowerModifier = 1.6f;
			spellName = "Zap"; // wisdom
			scalingAttribute = '0';
			spellSlotCost = 1;
			spellPrice = 20;
			break;
		case 1:
			baseHealing = 0;
			baseDamage = 5;
			attributePowerModifier = 1.32f;
			spellName = "Light Ray"; // faith
			scalingAttribute = '1';
			spellSlotCost = 1;
			spellPrice = 25;

			break;

		case 2:
			baseHealing = 0;
			baseDamage = 15;
			spellName = "Hailstorm";
			attributePowerModifier = 1.38f;
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
			baseHealing = 13;
			baseDamage = 0;
			spellName = "Minor Heal";
			attributePowerModifier = 0.98f;
			scalingAttribute = '1'; // faith
			spellSlotCost = 1;
			spellPrice = 35;
			break;

		case 5:
			baseDamage = 3; // this spell is really weak, it does comprable damage to lifetap
			spellName = "Spark";
			attributePowerModifier = 0.33f;
			scalingAttribute = '2'; // wis faith
			spellSlotCost = 1;
			spellPrice = 5;
			baseHealing = 0;

			break;

		case 6:
			baseHealing = 22;
			baseDamage = 0;
			spellName = "Heal";
			attributePowerModifier = 1.35f;
			scalingAttribute = '1'; // faith
			spellSlotCost = 2;
			spellPrice = 65;
			break;

		case 7:
			baseHealing = 3;
			baseDamage = 9;
			spellName = "Unholy Siphon";
			attributePowerModifier = 1.28f;
			scalingAttribute = '1'; // faith
			spellSlotCost = 2;
			spellPrice = 55;
			break;
		case 8:
			baseHealing = 5;
			baseDamage = 5;
			spellName = "Lifetap";
			attributePowerModifier = 0.10f;
			scalingAttribute = '2'; // faith wis
			spellSlotCost = 1;
			spellPrice = 15;
			break;
		case 9:
			baseHealing = 0;
			baseDamage = 11;
			spellName = "Thunderbolt";
			attributePowerModifier = 1.48f;
			scalingAttribute = '0'; // wisdom
			spellSlotCost = 2;
			spellPrice = 45;
			break;

		case 10:
			baseHealing = 0;
			baseDamage = 8;
			spellName = "Crash Wave";
			attributePowerModifier = 0.65f;
			scalingAttribute = '4'; // strength
			spellSlotCost = 1;
			spellPrice = 20;
			break;
		case 11:
			baseHealing = 0;
			healthCost = 6;
			baseDamage = 12;
			spellName = "High Sacrifice";
			attributePowerModifier = 1.05f;
			scalingAttribute = '7'; // end faith
			spellSlotCost = 2;
			spellPrice = 45;
			break;
		case 12:
			baseHealing = 0;
			healthCost = 0;
			baseDamage = 2;
			spellName = "Splitting Shot";
			attributePowerModifier = 1.85f;
			scalingAttribute = '6'; // dex luck
			spellSlotCost = 2;
			spellPrice = 25;
			break;
		case 13:
			baseHealing = 0;
			healthCost = 9;
			baseDamage = 15;
			spellName = "Blind Rage";
			attributePowerModifier = 0.65f;
			scalingAttribute = '4'; // strength
			spellSlotCost = 1;
			spellPrice = 25;
			break;
		case 14:
			baseHealing = 5;
			healthCost = 10;
			baseDamage = 0;
			spellName = "Monk's Healing";
			attributePowerModifier = 1.85f;
			scalingAttribute = '7'; // end faith
			spellSlotCost = 1;
			spellPrice = 12;
			break;

		case 15:
			baseHealing = 0;
			healthCost = 0;
			baseDamage = 4;
			spellName = "Thrown Coin";
			attributePowerModifier = 0.35f;
			scalingAttribute = '8'; // luck
			spellSlotCost = 1;
			spellPrice = 20;
			break;

		case 16:
			baseHealing = 0;
			healthCost = 0;
			baseDamage = 4;
			spellName = "Special Dagger";
			attributePowerModifier = 1.15f;
			scalingAttribute = '5'; // dex 
			spellSlotCost = 1;
			spellPrice = 20;
			break;

		case 17:
			baseHealing = 0;
			healthCost = 3;
			baseDamage = 35;
			spellName = "Fist of Fire";
			attributePowerModifier = 1.35f;
			scalingAttribute = '4'; // strength
			spellSlotCost = 3;
			spellPrice = 50;
			intensityChange = 4;
			break;
		case 18:
			baseHealing = 0;
			baseDamage = 3;
			attributePowerModifier = 1.45f;
			spellName = "Frozen Gust"; // wisdom
			scalingAttribute = '0';
			spellSlotCost = 1;
			spellPrice = 25;
			intensityChange = -2;
			break;
		case 19:
			baseHealing = 0;
			baseDamage = 20;
			attributePowerModifier = 1.95f;
			spellName = "Inferno"; // wisdom
			scalingAttribute = '0';
			spellSlotCost = 3;
			spellPrice = 75;
			intensityChange = 5;
			break;
		case 20:
			baseHealing = 0;
			baseDamage = 3;
			healthCost = 5;
			attributePowerModifier = 1.1f;
			spellName = "Blood Dart"; 
			scalingAttribute = '5'; //dex scaling
			spellSlotCost = 0;
			spellPrice = 15;
			intensityChange = 0;
			break;


		default:
			std::cout << "Hello, i'm not a spell!"; //this will display this humorous text if some how
			// i input a wrong value when updating the spell or some garbage
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

		case '4':
			modifier = ((player.strength * attributePowerModifier));
			break;
		case '5':
			modifier = ((player.dexterity * attributePowerModifier));
			break;

		case '6':
			modifier = (((player.dexterity + player.luck) * attributePowerModifier));
			break;
		case '7':
			modifier = (((player.endurance + player.faith/2) * attributePowerModifier));
			break;

		case '8':
			modifier = (player.luck * attributePowerModifier);
			rolledNumber = 10 + rand() % 290;
			modifier *= rolledNumber / 100.0f;
			
			break;

		default:

			break;
		}
		return modifier;
	}

	spellEffect calculateSpellPower(character& player, bool display) {

		if (intensityChange != 0 && !display) {
			int newIntensity = intensity + intensityChange;
			if (newIntensity < 0) {
				std::cout << "\nThings can't get any calmer.\n";
				intensity = 0;
			}
			else
			{
				intensity = newIntensity;
				if (intensityChange > 0) {
					std::cout << "\nThe world grows more restless\n";
				}
				else if (intensityChange < 0) {
					std::cout << "\nThe world calms..\n";
				}
			}

		}


		spellEffect effect;
		
		int finalDamage = 0;
		if (id == 14) {
			effect.selfDamage = player.playerMaxHealth/10;
		}
		else {
			effect.selfDamage = healthCost;

		}
		
		if (baseHealing > 0) {
			int finalHealing = baseHealing + powerModifier(player);
			effect.healing = finalHealing;
		}

		int originalPlayerHealth = player.playerHealth;

		/*if (healthCost > 0 && player.playerHealth <= healthCost) {
			if (!display) {
				std::cout << "\nYou lack the life force to cast this spell!\n";
				return 0;
			}
			if (display) return 0; // Return base value for display if HP too low
		}*/
	
			
	
					
		if (baseDamage > 0) {
			switch (scalingAttribute) {

			case '0':
				finalDamage = baseDamage + powerModifier(player);
				finalDamage *= static_cast<float>((1 + intensity * 0.1));
				effect.damage = finalDamage + bonusSpellDamage;
				
				break;
			case '1':

				finalDamage = baseDamage * powerModifier(player);
				finalDamage *= static_cast<float>((1 + intensity * 0.172));
				effect.damage = finalDamage;
				
				break;

			case '2':

				finalDamage = baseDamage + powerModifier(player);
				finalDamage *= static_cast<float>((1 + intensity * 0.1));
				effect.damage = finalDamage;
			
				break;

			case '3':

				finalDamage = baseDamage;
				finalDamage *= static_cast<float>((1.2 + intensity * 0.11));
				effect.damage = finalDamage;
				
				break;

			case '4':

				finalDamage = baseDamage + powerModifier(player);
				finalDamage *= static_cast<float>((1.1 + intensity * 0.08));
				effect.damage = finalDamage;
				
				break;

			case '5':

				finalDamage = baseDamage + powerModifier(player);
				finalDamage *= static_cast<float>((1.0 + intensity * 0.12));
				effect.damage = finalDamage;
				
				break;

			case '8':
				//pure luck spells
				finalDamage = baseDamage * powerModifier(player);
				finalDamage *= static_cast<float>((0.3 + intensity * 0.22));
				effect.damage = finalDamage;

				break;

			default:

				finalDamage = baseDamage + powerModifier(player);
				finalDamage *= static_cast<float>((1 + intensity * 0.1));
				effect.damage = finalDamage;
				
				break;


			}

	}
					
			

			return effect;
		
		
		
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
			tempDefense = defense;

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
		int minimumDamage = (1 + attack / 6);
		int finalDamage = static_cast<int>(baseDamage * variance);
		if (finalDamage<minimumDamage) finalDamage = minimumDamage;
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
	friend character;
	character& player;
	enemy& opponent;


	statuseffects(character& p, enemy& e) : player(p), opponent(e) {}

	void grantStatusEffect(character& player,enemy& opponent,bool onPlayer, char statusId){
		statusEffect newEffect;
		switch (statusId) {
		case '0':
			newEffect.name = "Strength Boost";
			newEffect.actionName = "Feeling Strong";
			newEffect.statusId = '0';
			newEffect.type = 'b';
			newEffect.strengthValue = 3;
			newEffect.duration = 3;
			newEffect.buffTarget = "strength";
			break;

		default:
			break;
		}
		if (onPlayer) {
			playerStatuses.push_back(newEffect);
		}
		else {
			enemyStatuses.push_back(newEffect);
		}
		//dealing with vectors
	//if on player true
		//statuspushback.player
	//if on player flase
		//statuspushback.enemy

	}
	/*
	void tickStatus(character& player, enemy& opponent) {
		if (turnsElapsed % 2 == 0) { 
			for (auto it = playerStatuses.begin(); it != playerStatuses.end();) {
				if (it->type == 'b' && it->buffTarget == "strength") {
					std::cout << "\n" << it->actionName << ": +" << it->strengthValue << " strength!";
					player.strength += it->strengthValue; 
				}
				it->duration--;
				if (it->duration <= 0) {
					if (it->type == 'b' && it->buffTarget == "strength") player.strength -= it->strengthValue; // Revert
					it = playerStatuses.erase(it);
				}
				else ++it;
			}
		}
			//for(statuses in player.statusEffects )
				//do status effects on player
				//duration - 1
		// if turnsElapsed % 2 = 1
			//for(statuses in opponent.statusEffects )
				//do status effects on enemy
				//duration - 1
	}
	*/
	

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
	float itemVariance = 1.0f;
	int itemBecomes = 0;
	char statusEffectIdGiven = 0;
	bool hasStatus = false;
	bool itemTransforms = false;
	bool canRefill = false;
	friend class shop;
	int input = 0;

public:
	statuseffects& status;

	items(statuseffects& st) : status(st) {}

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
		else if (attribute == "luck") {
			return player.luck;
		}

		// Return a default value if the attribute doesn't exist
		static int defaultAttr = 0;
		return defaultAttr;
	}

	void grantPlayerItem(char itemId) {


		itemsHeld.push_back(itemId);

	}

	void updateItem(char itemId){
		itemHealingPower = 0;
		hasStatus = false;
		statusEffectIdGiven = 0;
		itemVariance = 1.0f;
		itemQuantity = 1;
		spellSlotsRestored = 0;
		intensityChange = 0;
		itemTransforms = false;
		canRefill = false;
		switch (itemId) {


		case -1:
			itemName = "Bowl";
			itemDamage = 5;
			scalingType = "attribute";
			itemType = "damage";
			attributeScale = "dexterity";
			attributeScalingFactor = 0.1;
			intensityScaleFactor = 0;
			itemHealingPower = 0;
			canRefill = true;
			itemBecomes = 19; //bowl of soup
			itemCost = 3;
			break;

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
			itemHealingPower = 7;
			itemCost = 35;
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
			itemCost = 20;
			scalingType = "attribute";
			attributeScale = "strength";
			itemType = "damage";
			itemDamage = 10;
			attributeScalingFactor = 0.20f;

			break;

		case 10:

			itemName = "Prayer Bead";
			itemHealingPower = 0;
			itemCost = 6;
			scalingType = "attribute";
			attributeScale = "faith";
			itemType = "damage";
			itemQuantity = 2;
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

		case 16:
			itemName = "Wonder Ball";
			itemDamage = 5;
			scalingType = "intensity";
			itemType = "damage";
			intensityScaleFactor = 0.07f;
			itemHealingPower = 5;
			intensityChange = 5;
			itemCost = 20;
			break;
		case 17:
			itemName = "Wonder Ball-";
			itemDamage = 2;
			scalingType = "intensity";
			itemType = "damage";
			intensityScaleFactor = 0.03f;
			itemHealingPower = 7;
			intensityChange = 2;
			itemCost = 12;
			break;
		case 18:
			itemName = "Peculiar Object";
			itemDamage = 1;
			itemQuantity = 3;
			scalingType = "both";
			itemType = "damage";
			itemVariance = 3;
			intensityScaleFactor = 0.02f;
			attributeScalingFactor = 0.3f;
			itemHealingPower = 0;
			attributeScale = "luck";
			intensityChange = 0;
			itemCost = 10;
			break;
		case 19:
			itemName = "Bowl of soup";
			itemDamage = 0;
			itemQuantity = 1;;
			scalingType = "both";
			itemType = "healing";
			itemVariance = 0;
			intensityScaleFactor = 0.0f;
			attributeScalingFactor = 0.0f;
			itemHealingPower = 10;
			attributeScale = "static";
			itemBecomes = -1;
			intensityChange = 0;
			itemTransforms = true;
			itemCost = 9;
			break;
		case 20:
			itemName = "Bowl of Strength Potion";
			itemDamage = 0;
			scalingType = "attribute";
			itemType = "buff";
			statusEffectIdGiven = 0;
			attributeScalingFactor = 0.0f;
			intensityScaleFactor = 0.0f;
			itemHealingPower = 0;
			intensityChange = 0;
			itemBecomes = -1;
			itemTransforms = true;
			itemCost = 16;

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
				std::cout << i << ": " << itemName << "\n";
				if (itemType == "damage")
					std::cout << "Base Damage : " << itemDamage << "\n";
				if (itemType == "healing")
					std::cout << "Base Healing : " << itemHealingPower << "\n";
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
			int newIntensity = intensity + intensityChange;
			if (newIntensity < 0) {
				std::cout << "\nThings can't get any calmer.\n";
				intensity = 0;
			}
			else
			{
				intensity = newIntensity;
				if (intensityChange > 0) {
					std::cout << "\nThe world grows more restless\n";
				}
				else if (intensityChange < 0) {
					std::cout << "\nThe world calms..\n";
				}
			}

		}

		if (itemType == "healing" && player.playerHealth < player.playerMaxHealth) {
			float totalHealing = itemHealingPower * (1 + player.endurance / 25);
			player.playerHealth += totalHealing; // you should replace this with a player.heal function
			std::cout << "\nYou gain " << totalHealing << " Health Points!\n";
			

			if (spellSlotsRestored > 0) {
				player.spellsAvailable += spellSlotsRestored;
				if (player.spellsAvailable > player.maximumSpellsAvailable) {
					std::cout << "\nSome of the ethereal mana fades away";
					player.spellsAvailable = player.maximumSpellsAvailable;
				}
			}
			itemsHeld.erase(itemsHeld.begin() + (input - 1));
			if (player.playerHealth > player.playerMaxHealth) {
				std::cout << "\nBut some of it was wasted..";
				player.playerHealth = player.playerMaxHealth;
			}
			

		}
		if (itemType == "buff") {
			//status.grantStatusEffect(player, opponent, true, 0);
			itemsHeld.erase(itemsHeld.begin() + (input - 1));
		}
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
				finalItemDamage = itemDamage * (1 + ((attribute * attributeScalingFactor) + (intensity * intensityScaleFactor)));
				itemsHeld.erase(itemsHeld.begin() + (input - 1));
				break;
			default:
				itemsHeld.erase(itemsHeld.begin() + (input - 1));
				break;

			}
			if (itemVariance != 1.0) {

				finalItemDamage *= rand() % static_cast<int>((itemVariance + player.luck / 2));
			}
			std::cout << "Your item does " << finalItemDamage << " Damage!";

			opponent.takeDamage(finalItemDamage + bonusItemDamage);
			if (id == 1 || id == 8 || id == 14) {
			label_name: //i've never used go to, but I was too lazy to make a while loop. Have fun throwing a ton of daggers.

				float multiDaggerChance = player.dexterity / 1.8;
				if (multiDaggerChance > 24) multiDaggerChance = 24;
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
		if (itemTransforms) {
			itemsHeld.push_back(itemBecomes);
			updateItem(itemBecomes);
			std::cout << "\nYou get a " << itemName << "!";
		}
		float chanceFreeAction = player.luck / 2 + bonusFreeItemTurnChance;
		if (chanceFreeAction >= 20)chanceFreeAction = 20;
		rolledNumber = rand() % 100;
		if (chanceFreeAction > rolledNumber) {
			setColor(6);
			std::cout << "\nYou got lucky, and can act again!";
			setColor(7);
		}
		else {
			turnsElapsed++;
		}
		
	}

};

class shop {

private:
	int objectsAvailable = 4;
	int recycleCount = 0;
	int maxRecycleCount = 2;
	std::string shopType = "items";
	bool itemsGenerated = false;
	bool spellsGenerated = false;
	bool doneShopping = false;
	bool shopUpgraded = false;
	std::vector<char> availableSpells = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 , 13, 14, 15, 16, 17, 18, 19, 20 };
	std::vector<char> spellsInShop;

public:

	spells& shopSpell;
	items& shopItem;
	character& player;
	enemy& opponent;
	friend items;
	friend spells;
	friend character;
	std::vector<char> itemsInShop;
	shop(spells& s, items& i, character& c, enemy& e) : shopSpell(s), shopItem(i), player(c), opponent(e){


	}

	void healChoiceMenu() {
		int healingChoice;
		int healingPrice = ((player.playerMaxHealth - player.playerHealth) / 3);
		int spellSlotPrice = 4 * 1 * (player.level * 0.8);
		if (spellSlotPrice > 20) spellSlotPrice = 20;
		short tempSlotPrice = (10 * player.temporarySpellSlots) + 15;
		int fullSpellSlotPrice = spellSlotPrice * (player.maximumSpellsAvailable - player.spellsAvailable);

		std::cout << "-----Healing-----\n" <<
			"1) Heal 3 HP: 1 Gold\n" <<
			"2) Full Heal: " << healingPrice << " Gold\n" <<
			"3) One spell slot: " << spellSlotPrice << " Gold\n" <<
			"4) Full spell slots: " << fullSpellSlotPrice << " Gold\n" <<
			"5) Temporary spell slots: " << tempSlotPrice << " Gold\n"
			<< "Health: " << player.playerHealth << "/" << player.playerMaxHealth << "\n" 
			<< "Spell Slots: " <<player.spellsAvailable << "/" << player.maximumSpellsAvailable << "\n"
			<< "Temporary Slots: " <<player.temporarySpellSlots << "\n" 
			<< "\nChoose a number 1-5\n";
			
		std::cin >> healingChoice;

		handleInputFailure("\nInvalid input, healing cancelled.");
		if (healingChoice > 5 || healingChoice < 1) {
			std::cout << "\nChoose a number 1-5";
		}
		else {

			switch(healingChoice){

			case 1:
				if (player.playerHealth < player.playerMaxHealth && player.gold > 0) {

					player.playerHealth += 3;
					if (player.playerHealth > player.playerMaxHealth) player.playerHealth = player.playerMaxHealth;
					player.gold--;

				}
				else {
					std::cout << "\nHealing unsuccessful. (Full health, or not enough gold)\n";
				}
				break;
			case 2:
				if (player.playerHealth < player.playerMaxHealth && player.gold >= healingPrice) {

					player.playerHealth = player.playerMaxHealth;
					
					player.gold -= healingPrice;

				}
				else {
					std::cout << "\nHealing unsuccessful. (Full health, or not enough gold)";
				}
				break;
			case 3:
				if (player.spellsAvailable < player.maximumSpellsAvailable && player.gold >= spellSlotPrice) {

					player.spellsAvailable++;

					player.gold -= spellSlotPrice;

				}
				else {
					std::cout << "\nChanneling unsuccessful. (Full spell slots, or not enough gold)";
				}
				break;
			case 4:
				if (player.spellsAvailable < player.maximumSpellsAvailable && player.gold >= fullSpellSlotPrice) {

					player.spellsAvailable = player.maximumSpellsAvailable;

					player.gold -= fullSpellSlotPrice;

				}
				else {
					std::cout << "\nChanneling unsuccessful. (Full spell slots, or not enough gold)";
				}
				break;
				
			case 5: {
			
				if (player.gold >= tempSlotPrice) {

					player.gold -= tempSlotPrice;
					player.temporarySpellSlots++;
					std::cout << "\nYou gain a temporary spell slot to use in the next 2 battles.";

				}
				else {

					std::cout << "\nYou don't have enough to buy a temporary spell slot!";
				}

				break;

			}
				

			}

		}


	}

	void itemShopMenu(items& shopItem) {
		
		std::cout << "-----Buy Items-----\n";
		
		for (int i = 1; i <= objectsAvailable; i++) { 
				
			int randomItemId = rand() % 20; //generates a random item that has to be updated manually in accordance with total items in game, think about making an unordered map
			//bool isPair = (rand() % 100 < 20);
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

	void leaveShop() {
		std::cout << "\nBye-bye!\n";
		recycleCount = 0;
		doneShopping = true;
		itemsGenerated = false;
		spellsGenerated = false;
		itemsInShop.clear();
		spellsInShop.clear();

	}

	void shopRefresh() {

		itemsGenerated = false;
		spellsGenerated = false;
		itemsInShop.clear();
		spellsInShop.clear();
	}

	void itemManage() {
		
		int manageChoice;
		int actionChoice;
		bool itemManaged = false;
		maxRecycleCount = 2 + player.getLuck() / 5 + bonusRecycle;
		
		shopItem.displayItemMenu(player, opponent);
		if (shopItem.itemsHeld.empty()) {
			std::cout << "\nNo items to manage!\n";
			return;
		}

		std::cout << "\nWhich item do you wish to manage? (1-" << shopItem.itemsHeld.size() << ")\n";
		std::cin >> manageChoice;

		
		if (handleInputFailure("\nInvalid input, managing cancelled.") ||
			manageChoice < 1 || manageChoice > shopItem.itemsHeld.size()) {
			std::cout << "\nChoose an item within your inventory (1-" << shopItem.itemsHeld.size() << ").\n";
			return;
		}

		
		char selectedItem = shopItem.itemsHeld[manageChoice - 1];
		shopItem.updateItem(selectedItem);

		
		while (!itemManaged) {
			std::cout << "\n" << shopItem.itemName << "\nWhat do you wish to do with this item?"
				<< "\nRecycles used: " << recycleCount << "/" << maxRecycleCount << "\n"
				<< "\n1) Throw away"
				<< "\n2) Recycle"
				<< "\n3) Refill (only for containers) (" << shopItem.itemCost << " gold)"
				<< "\n4) Keep\n";

			std::cin >> actionChoice;

			if (handleInputFailure("\nInvalid input.")) {
				std::cout << "\nPlease enter a number 1-3.\n";
				continue;
			}

			if (actionChoice < 1 || actionChoice > 4) {
				std::cout << "\nChoose one of the following options (1-3).\n";
				continue;
			}

			switch (actionChoice) {
			case 1: 
				shopItem.itemsHeld.erase(shopItem.itemsHeld.begin() + (manageChoice - 1));
				std::cout << "\n" << shopItem.itemName << " tossed.\n";
				itemManaged = true;
				break;
			case 2: {
				int recycleCost = (5 + recycleCount * 2) - bonusRecycle;
				if (recycleCount < maxRecycleCount) {
					if (player.gold >= recycleCost) {
						player.gold -= recycleCost;
						recycleCount++;
						int randomItemId = rand() % 16;

						std::cout << "\nYou recycled your " << shopItem.itemName << "...\n";
						shopItem.itemsHeld.erase(shopItem.itemsHeld.begin() + (manageChoice - 1));
						shopItem.itemsHeld.push_back(randomItemId);
						shopItem.updateItem(randomItemId);
						std::cout << "\nand got a " << shopItem.itemName << " for " << recycleCost << " gold!\n";
					}
					else {
						std::cout << "You need " << recycleCost << " gold to recycle an item";
					}
					itemManaged = true;
					break;
				}
				else
				{
					std::cout << "\nYou've already recycled twice this visit!";
						itemManaged = true;
					break;
				}
			}
			case 3:
				if (player.gold >= shopItem.itemCost && shopItem.canRefill) {

					player.gold -= 3;
					std::cout << "\nYou refil your " << shopItem.itemName;
					shopItem.itemsHeld.push_back(shopItem.itemBecomes);
					shopItem.updateItem(shopItem.itemBecomes);
					shopItem.itemsHeld.erase(shopItem.itemsHeld.begin() + (manageChoice - 1));
					std::cout << ", it's now a " << shopItem.itemName << ".";

				}
				else {

					std::cout << "\nYou can't refill that silly!";

				}
				itemManaged = true;
				break;

			case 4: 
				std::cout << "\n" << shopItem.itemName << " kept.\n";
				itemManaged = true;
				break;
			}
		}


	}



	void spellShopMenu() {
		std::cout << "-----Buy Spells-----\n";
		
		int availableCount = 0;
		for (char spell : availableSpells) {
			if (std::find(shopSpell.spellsKnown.begin(), shopSpell.spellsKnown.end(), spell) == shopSpell.spellsKnown.end() &&
				std::find(spellsInShop.begin(), spellsInShop.end(), spell) == spellsInShop.end()) {
				availableCount++;
			}
		}

		if(!spellsGenerated) while (spellsInShop.size() < std::min(3, availableCount)) {

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
					spellsInShop.erase(spellsInShop.begin() + (purchaseChoice - 1));
				}
				else {
					std::cout << "Not enough gold.\n";
				}
			}
		}
		else
		{
			std::cout << "\nThere are no more spells to learn!";
		}
	}

	void shopMenu() {

		int shopChoice;
		doneShopping = false;
		player.temporarySpellSlots = 0; //reset temp spell slots, they only last 2 battles.
		setColor(4);
		std::cout <<  "--Welcome to the shop!--\n";
		setColor(7);
		while (!doneShopping && player.playerHealth > 0) {
			int healPrice = ((player.playerMaxHealth - player.playerHealth) / 3 + 4 * (player.maximumSpellsAvailable - player.spellsAvailable)) * (player.level*0.8);
			std::cout
				<< "\nYou have " << player.gold << " Gold."
				<< "\nHP: " << drawHealthBar(player.playerHealth,player.playerMaxHealth)
				<< "\n1) Items"
				<< "\n2) Spells"
				<< "\n3) Healer" 
				<< "\n4) Check"
				<< "\n5) Refresh (8 Gold)"
				<< "\n6) Manage Items"
				<< "\n7) Leave shop\n";

			
			

			std::cin >> shopChoice;

			if (std::cin.fail()) { 
				std::cin.clear();
				std::cin.ignore(10000, '\n');
				std::cout << "Invalid input! Choose a number 1-7.\n";
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
				break;
			}

				
			case 4:
				player.displayStats();
				break;
			case 5:
				if (player.gold >= 8) {
					player.gold -= 8;
					shopRefresh();
					std::cout << "\nShop refreshed.";
				}
				else {
					std::cout << "\nYou need 8 gold to refresh the shop";
				}

				break;
			case 6:
				itemManage();
				break;
			case 7:
				leaveShop();
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
	statuseffects& status;

	bool intensity10 = false;
	bool intensity20 = false;
	bool intensity30 = false;

public:
	
	combatHandler(character& p, enemy& e, spells& s, items& i, shop& sh, statuseffects& st) : player(p), opponent(e), spell(s), item(i), shops(sh), status(st) {
		std::cout << std::endl << "BATTLE START!" << std::endl;
	};
	void battleRewards() {
		if (!battling && opponent.enemyHealth <= 0) {
			player.experience += opponent.experienceWorth;
			int finalGold = opponent.goldDropped * (1.15 - (intensity/(85+player.endurance))*1.15);
			if (finalGold < 0) finalGold = 1;
			player.gold += finalGold;
			setColor(6);
			std::cout << "\nYou gained " << opponent.goldDropped << " + ("<< (finalGold-opponent.goldDropped) <<" due to intensity)" <<" gold and " << opponent.experienceWorth << " experience!\n";
			setColor(7);
			if (player.getDexterity() >= 10) {

				rolledNumber = rand() % 5;
					if (rolledNumber == 3) {
						item.grantPlayerItem(1);
						std::cout << "\nThe enemy drops a throwing dagger.\n";

					}
			}
		}


	}
	
	void levelUpHandler() {
		int originalLevel = player.level;
		while (player.experience >= player.experienceToNext) {
			player.level++;
			setColor(2);
			std::cout << "You leveled up to level " << player.level << ".\n";
			setColor(7);
			player.experience -= player.experienceToNext;
			
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
		int fleebonus = 0;
		float intensityMultiplier = 1;
		if (intensity <= 10) intensityMultiplier = 0.6f;
		if (intensity < 2) fleebonus = 50;
		else intensityMultiplier = 1;
		double result = 6.2 * intensity * intensityMultiplier; // result is double
		int finalIntensity = static_cast<int>(result); // explicit cast to int
		int fleeValue = rand() % 301 + player.luck * 3 + player.dexterity * 2 + fleebonus;
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

		randomHealth = rand() % (28 + player.level * 9) + 6;
		randomAttack = rand() % (3 + player.level * 4) + 1;
		randomDefense = rand() % (12 + player.level * 3) + 1;
		randomXP = (randomHealth + randomAttack + randomDefense)/1.5;
		randomGold = (rand() % 8 + player.luck / 2) + player.level * 2 + randomXP/20 ;
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

			//status.tickStatus(player, opponent);

			if(turnsElapsed % 2 == 0){

				
				std::cout
					<< "\nEnemy Health: "<< drawHealthBar(opponent.enemyHealth,opponent.enemyMaxHealth) << " " << opponent.enemyHealth << " / " << opponent.enemyMaxHealth
					<< "\nEnemy Attack: "<< opponent.enemyCalculateMeleeDamage(player)
					<< "\nYour Health: " << drawHealthBar(player.playerHealth, player.playerMaxHealth) << " " << player.playerHealth << " / " << player.playerMaxHealth
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
				std::cout << "\Temporary Slots: " << player.temporarySpellSlots << "\n";
				char spellUsed = spell.displaySpellMenu(player);
				spell.updateSpell(spellUsed);
				spells::spellEffect effect = spell.calculateSpellPower(player, false);
				if (spellUsed == 'f') break;  //f is the default 
			

				if (spell.spellSlotCost <= player.spellsAvailable || spell.spellSlotCost <= player.temporarySpellSlots) {
					if (player.playerHealth > spell.healthCost) {

						

						if (effect.selfDamage > 0) {
							player.playerHealth -= spell.healthCost;
							std::cout << "\nYou take " << spell.healthCost << " damage due to " << spell.spellName;
							intensity--; //neutral intensity on healing spells
						}

							if (effect.damage > 0) {
								opponent.takeDamage(effect.damage);
							}
							if (effect.healing > 0) {
								int originalHealth = player.playerHealth;
								player.playerHealth += effect.healing;
								if (player.playerHealth > player.playerMaxHealth) {
									player.playerHealth = player.playerMaxHealth;
									std::cout << "\nYou heal for " << (player.playerHealth - originalHealth) << " health.";
								}	
							}
							
							
							if (player.spellsAvailable < spell.spellSlotCost) {

								player.temporarySpellSlots -= spell.spellSlotCost;
							}
							else {
								player.spellsAvailable -= spell.spellSlotCost;

							}
						intensity++;
						turnsElapsed++;

					}
					else {
						std::cout << "You lack the life force to cast this spell (" << spell.healthCost << ")";
					}

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

void startingBonus(character& player, items& item, spells& spell) {

		if (player.getDexterity() >= 5) {
			item.grantPlayerItem(1);
			item.grantPlayerItem(1);
			item.grantPlayerItem(1);
			
			std::cout << "\nYour dexterity grants you a free trio of throwing daggers.\n";

		}
		 
		if ((player.getDexterity() + player.getFaith()) >= 11 && player.getFaith() >= 4 && player.getDexterity() >= 3) {
			item.grantPlayerItem(8);
			item.grantPlayerItem(8);
			spell.grantPlayerSpell(20);
			bonusCritChance = 1;
			bonusIntensity = 1;
			std::cout << "\nYour religion instills in you an odd obsession with blood.\n";

		} 
		if (player.getStrength() >= 8) {
			spell.grantPlayerSpell(10);
			bonusHealth += 1;
			bonusCritChance = 1;
			bonusMeleeDamage = 1;
			std::cout << "\nYou strength lets you send forth a damaging wave of energy, given the correct circumstances.\n";

		}
		if (player.getStrength() >= 3 && player.getEndurance() >= 3 && player.getDexterity() >= 3 && player.getLuck() >= 3) {
			spell.grantPlayerSpell(18);
			bonusHealth += 2;
			bonusCritChance = 2;
			bonusMeleeDamage = 1;
			bonusRecycle = 1;
			bonusItemDamage = 1;
			player.grantGold(4);
			item.grantPlayerItem(0);
			item.grantPlayerItem(1);
			std::cout << "\nYou're well rounded, but somewhat bland..\n";

		}

		if (player.getLuck() == 15) {
			item.grantPlayerItem(-1);
			item.grantPlayerItem(-1);
			item.grantPlayerItem(-1);
			item.grantPlayerItem(rand() % 8);
			bonusIntensity = 2;
			bonusCritChance = 6;
			bonusItemDamage = 1;
			std::cout << "\nYou're just a really lucky person, maybe you're here because you're lucky, or maybe your luck is coming to an end.\n"
				<< "Though, hard to say why you have 3 bowls.. I guess it was just luck";
			player.grantGold(15);

		}
		if (player.getEndurance() > 11) {
			//spell.grantPlayerSpell(14);
			std::cout << "\nYou are a hulking mass, capable of nudging off all but the strongest of attacks...\n";
			bonusHealth += 6;
			bonusCritChance = 1;
		}

		if ((player.getFaith() + player.getWisdom()) >= 10) {
			
			spell.grantPlayerSpell(8);
			std::cout << "\nYour arcane knowledge grants you insight on a spell!";

		}
		if ((player.getLuck() + player.getDexterity()) >= 12 && player.getDexterity() > 3 && player.getLuck() > 3) {

			item.grantPlayerItem(1);
			item.grantPlayerItem(8);
			item.grantPlayerItem(14);
			item.grantPlayerItem(18);
			player.grantGold(10);
			bonusCritChance = 2;
			bonusItemDamage = 1;
			std::cout << "\nYour unique attributes offer you some useful goods..";

		}


		if (player.getEndurance() >= 6) {
			item.grantPlayerItem(5);
			item.grantPlayerItem(19);
			bonusHealth += 3;
			std::cout << "\nYou grew up healthier than most, you left on your adventure with a bowl of soup and a potion!\n";

		}
		if (player.getFaith() >= 5) {
			item.grantPlayerItem(10);
			item.grantPlayerItem(10);
			item.grantPlayerItem(10);
			std::cout << "\nYou find a broken cross necklace, and take some of the beads off of it.\n";
			spell.grantPlayerSpell(4);

		}
		if (player.getLuck() >= 8) {
			item.grantPlayerItem(18);
			item.grantPlayerItem(18);
			item.grantPlayerItem(18);
			item.grantPlayerItem(rand() % 20);
			std::cout << "\nYou've always been lucky, but will your luck persist?\n";
			player.grantGold(15);

		}

		if (player.getWisdom() >= 5) {

			std::cout << "\nUsing your intellect, you realize you can shoot sparks from your hands.\n";

			spell.grantPlayerSpell(5);

		}
	}

	void pickName(character& player) {

		std::string newName;
		std::cout << "\nWhat is your name?\n";
		std::getline(std::cin, newName);
		player.setName(newName);

	}
	void pickSpeciality() {
		bool specialityPicked = false;
		int specialityChosen = '0';
		while (!specialityPicked) {

			std::cout << "\nPick a speciality: "
				<< "\n1) Magic"
				<< "\n2) Items"
				<< "\n3) Melee"
				<< "\n4) Defense\n";
			std::cin >> specialityChosen;
			

			if(handleInputFailure("\Invalid Input, enter a number from 1-4")) {
				continue;
			}
			if (specialityChosen > 4 || specialityChosen < 1) {
				std::cout << "\Invalid Input, enter a number from 1-4";
				continue;
			}

			switch (specialityChosen){
			
			case 1: // magic
				bonusSpellSlots++;
				bonusSpellDamage = 2;
				playerSpeciality = "Magic";
				specialityPicked = true;
				break;
			case 2: // items
				bonusItemDamage = 2;
				bonusFreeItemTurnChance = 5;
				playerSpeciality = "Items";
				specialityPicked = true;
				break;
			case 3: // melee
				bonusMeleeDamage = 2;
				bonusCritChance = 3;
				bonusDefense = 2;
				playerSpeciality = "Melee";
				specialityPicked = true;
				break;
			case 4: // defense
				bonusDefense = 5;
				bonusHealth = 4;
				playerSpeciality = "Defense";
				specialityPicked = true;
				break;
			}
			
		}
		
	}



int main() {
	startMenu();
	
	srand(static_cast<unsigned int>(time(NULL)));

	
	spells spelltest;
	
	character test1(0, 0, 0, 0, 0, 0);
	pickName(test1);

	
	pickSpeciality();
	test1.pickAttributes();
	//clear();


	enemy test;
	statuseffects teststatus(test1, test);

	items testitem(teststatus);
	startingBonus(test1, testitem, spelltest);
	test.enemySet(20, 20, 1, 4, 10, 20, "beebie", test1);
	shop shoptest(spelltest, testitem, test1, test);
	combatHandler combat1(test1, test, spelltest, testitem, shoptest, teststatus);

	while (test1.playerHealth > 0) {
		battling = true;
		intensity = bonusIntensity;
		turnsElapsed = 0;
		combat1.initiateCombat();
		roundsPassed++;
		if (roundsPassed >= roundsNeeded) {
			roundsPassed = 0;
			shoptest.shopMenu();
		}
	}
	
	
}