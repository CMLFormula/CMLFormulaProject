#include "PeriodicTable.h"
#include <algorithm>
#include <sstream>

using std::map;
using std::pair;
using std::make_pair;
using std::vector;
using std::string;

PeriodicTable* PeriodicTable::instance = nullptr;

PeriodicTable::PeriodicTable()
{
	vector<int> v1;
	v1.push_back(1);

	valencies.insert(make_pair("Li",v1));
	valencies.insert(make_pair("Na",v1));
	valencies.insert(make_pair("K",v1));
	valencies.insert(make_pair("Rb",v1));
	valencies.insert(make_pair("Cs",v1));
	valencies.insert(make_pair("Fr",v1));
	valencies.insert(make_pair("F",v1));

	vector<int> v2;
	v2.push_back(2);

	valencies.insert(make_pair("Be",v2));
	valencies.insert(make_pair("Mg",v2));
	valencies.insert(make_pair("Ca",v2));
	valencies.insert(make_pair("Sr",v2));
	valencies.insert(make_pair("Ba",v2));
	valencies.insert(make_pair("Ra",v2));

	vector<int> v3;
	v3.push_back(3);

	valencies.insert(make_pair("B",v3));
	valencies.insert(make_pair("Al",v3));
	valencies.insert(make_pair("Ga",v3));
	valencies.insert(make_pair("In",v3));

	vector<int> v13;
	v13.push_back(1);
	v13.push_back(3);

	valencies.insert(make_pair("Tl",v13));

	vector<int> v4;
	v4.push_back(4);

	valencies.insert(make_pair("C",v4));
	valencies.insert(make_pair("Si",v4));
	valencies.insert(make_pair("Ge",v4));

	vector<int> v24;
	v24.push_back(2);
	v24.push_back(4);

	valencies.insert(make_pair("Pb",v24));
	valencies.insert(make_pair("Sn",v24));
	valencies.insert(make_pair("O",v24));

	vector<int> v35;
	v35.push_back(3);
	v35.push_back(5);

	valencies.insert(make_pair("N",v35));
	valencies.insert(make_pair("P",v35));
	valencies.insert(make_pair("As",v35));
	valencies.insert(make_pair("Sb",v35));
	valencies.insert(make_pair("Bi",v35));
	
	vector<int> v246;
	v246.push_back(2);
	v246.push_back(4);
	v246.push_back(6);
	
	valencies.insert(make_pair("S",v246));
	valencies.insert(make_pair("Se",v246));
	valencies.insert(make_pair("Te",v246));
	valencies.insert(make_pair("Po",v246));
	
	vector<int> v1357;
	v1357.push_back(1);
	v1357.push_back(3);
	v1357.push_back(5);
	v1357.push_back(7);
	
	valencies.insert(make_pair("Cl",v1357));
	valencies.insert(make_pair("Br",v1357));
	valencies.insert(make_pair("I",v1357));
	valencies.insert(make_pair("At",v1357));


	numbers.insert(make_pair("H",1));
	numbers.insert(make_pair("He",2));
	numbers.insert(make_pair("Li",3));
	numbers.insert(make_pair("Be",4));
	numbers.insert(make_pair("B",5));
	numbers.insert(make_pair("C",6));
	numbers.insert(make_pair("N",7));
	numbers.insert(make_pair("O",8));
	numbers.insert(make_pair("F",9));
	numbers.insert(make_pair("Ne",10));
	numbers.insert(make_pair("Na",11));
	numbers.insert(make_pair("Mg",12));
	numbers.insert(make_pair("Al",13));
	numbers.insert(make_pair("Si",14));
	numbers.insert(make_pair("P",15));
	numbers.insert(make_pair("S",16));
	numbers.insert(make_pair("Cl",17));
	numbers.insert(make_pair("Ar",18));
	numbers.insert(make_pair("K",19));
	numbers.insert(make_pair("Ca",20));
	numbers.insert(make_pair("Sc",21));
	numbers.insert(make_pair("Ti",22));
	numbers.insert(make_pair("V",23));
	numbers.insert(make_pair("Cr",24));
	numbers.insert(make_pair("Mn",25));
	numbers.insert(make_pair("Fe",26));
	numbers.insert(make_pair("Co",27));
	numbers.insert(make_pair("Ni",28));
	numbers.insert(make_pair("Cu",29));
	numbers.insert(make_pair("Zn",30));
	numbers.insert(make_pair("Ga",31));
	numbers.insert(make_pair("Ge",32));
	numbers.insert(make_pair("As",33));
	numbers.insert(make_pair("Se",34));
	numbers.insert(make_pair("Br",35));
	numbers.insert(make_pair("Kr",36));
	numbers.insert(make_pair("Rb",37));
	numbers.insert(make_pair("Sr",38));
	numbers.insert(make_pair("Y",39));
	numbers.insert(make_pair("Zr",40));
	numbers.insert(make_pair("Nb",41));
	numbers.insert(make_pair("Mo",42));
	numbers.insert(make_pair("Tc",43));
	numbers.insert(make_pair("Ru",44));
	numbers.insert(make_pair("Rh",45));
	numbers.insert(make_pair("Pd",46));
	numbers.insert(make_pair("Ag",47));
	numbers.insert(make_pair("Cd",48));
	numbers.insert(make_pair("In",49));
	numbers.insert(make_pair("Sn",50));
	numbers.insert(make_pair("Sb",51));
	numbers.insert(make_pair("Te",52));
	numbers.insert(make_pair("I",53));
	numbers.insert(make_pair("Xe",54));
	numbers.insert(make_pair("Cs",55));
	numbers.insert(make_pair("Ba",56));
	numbers.insert(make_pair("La",57));
	numbers.insert(make_pair("Ce",58));
	numbers.insert(make_pair("Pr",59));
	numbers.insert(make_pair("Nd",60));
	numbers.insert(make_pair("Pm",61));
	numbers.insert(make_pair("Sm",62));
	numbers.insert(make_pair("Eu",63));
	numbers.insert(make_pair("Gd",64));
	numbers.insert(make_pair("Tb",65));
	numbers.insert(make_pair("Dy",66));
	numbers.insert(make_pair("Ho",67));
	numbers.insert(make_pair("Er",68));
	numbers.insert(make_pair("Tm",69));
	numbers.insert(make_pair("Yb",70));
	numbers.insert(make_pair("Lu",71));
	numbers.insert(make_pair("Hf",72));
	numbers.insert(make_pair("Ta",73));
	numbers.insert(make_pair("W",74));
	numbers.insert(make_pair("Re",75));
	numbers.insert(make_pair("Os",76));
	numbers.insert(make_pair("Ir",77));
	numbers.insert(make_pair("Pt",78));
	numbers.insert(make_pair("Au",79));
	numbers.insert(make_pair("Hg",80));
	numbers.insert(make_pair("Tl",81));
	numbers.insert(make_pair("Pb",82));
	numbers.insert(make_pair("Bi",83));
	numbers.insert(make_pair("Po",84));
	numbers.insert(make_pair("At",85));
	numbers.insert(make_pair("Rn",86));
	numbers.insert(make_pair("Fr",87));
	numbers.insert(make_pair("Ra",88));
	numbers.insert(make_pair("Ac",89));
	numbers.insert(make_pair("Th",90));
	numbers.insert(make_pair("Pa",91));
	numbers.insert(make_pair("U",92));
	numbers.insert(make_pair("Np",93));
	numbers.insert(make_pair("Pu",94));
	numbers.insert(make_pair("Am",95));
	numbers.insert(make_pair("Cm",96));
	numbers.insert(make_pair("Bk",97));
	numbers.insert(make_pair("Cf",98));
	numbers.insert(make_pair("Es",99));
	numbers.insert(make_pair("FM",100));
	numbers.insert(make_pair("Md",101));
	numbers.insert(make_pair("No",102));
	numbers.insert(make_pair("Lr",103));
	numbers.insert(make_pair("Rf",104));
	numbers.insert(make_pair("Db",105));
	numbers.insert(make_pair("Sg",106));
	numbers.insert(make_pair("Bh",107));
	numbers.insert(make_pair("Hs",108));
	numbers.insert(make_pair("Mt",109));
}

int PeriodicTable::getNumber(const std::string& element)
{
	const PeriodicTable& inst = PeriodicTable::getInstance();
	auto it = inst.numbers.find(element);
	if (it == inst.numbers.end())
		return -1;
	return it->second;
}

const PeriodicTable& PeriodicTable::getInstance()
{
	if (PeriodicTable::instance == nullptr)
	{
		PeriodicTable::instance = new PeriodicTable();
	}

	return *PeriodicTable::instance;
}

vector<int> PeriodicTable::getValence(const string& element)
{
	const PeriodicTable& inst = PeriodicTable::getInstance();
	return inst.valencies.find(element)->second;
}