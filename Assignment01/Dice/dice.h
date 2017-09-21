


class Dice
{
private:
	int numRolls;
	int numTracker[6];
	int numOfDice;
	int numOfArmies;
public:
	Dice();
	void numOfArmies();
	void numGenerator();
	int getNumRolls();
	int getNumTracker();
	int getNumOfDice();
	int getNumOfArmies();
	void setNumRolls(int numRolls);
	void setNumTracker(int index);
	void setNumOfDice(int numOfDice);
	void setNumOfArmies(int numOfArmies);
};