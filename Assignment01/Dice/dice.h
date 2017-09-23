#include <vector>
using namespace std;


class Dice
{
private:
	int numTracker[6];
public:
	int numGenerator();
	vector<int> Dice::howManyDice(int howManyDice);
	int* getNumTracker();
};