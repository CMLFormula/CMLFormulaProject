#include <map>
#include <string>
#include <vector>

// Singlton class
class PeriodicTable
{
	//element => vector of valencies in increasing order
	std::map<std::string, std::vector<int> > valencies;
	std::map<std::string, int > numbers;

	static PeriodicTable* instance;

	PeriodicTable(); // private constructor
	static const PeriodicTable& getInstance();

public:

	static int getCeilValence(const std::string& element, int lowerValence);
	static std::vector<int> getValence(const std::string& element);
	static int getNumber(const std::string& element);
};
