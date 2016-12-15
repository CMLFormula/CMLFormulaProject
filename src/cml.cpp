#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <tuple>
#include <algorithm>
#include <set>
#include <queue>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "cml.h"
#include "PeriodicTable.h"

using boost::property_tree::ptree;
using std::map;
using std::set;
using std::queue;
using std::string;
using std::pair;
using std::vector;
using std::ostringstream;
using std::logic_error;

class Atom
{
	string id;
	string element;
	int valence; // 0 is it must be deduced from bonds number and standard valences
	int bondsCnt;   // to accumulate number of explicit bonds
	int charge;
	int hydroCount;
	int radicalCount;
public:
	Atom(const string& id, const string& element, int valence = -1, int charge = 0, int hydroCount = -1, int radicalCount = 0) 
		: id(id), element(element), valence(valence), bondsCnt(0), charge(charge), hydroCount(hydroCount), radicalCount(radicalCount)
	{
	}

	void addBonds(int order)
	{
		bondsCnt += order;

		if (valence != -1 && bondsCnt > valence)
		{
			ostringstream oss;
			oss << "Atom with id '" << id << "' has more bounds than valence.";
			throw logic_error(oss.str());
		}
	}

	// if valence == 0 then deduce it from bonds and standard valences for that element
	void correct_valence()
	{
		std::cout << "Error";
		if (this->valence == 0)
			this->valence = PeriodicTable::getCeilValence(this->element, this->bondsCnt);
	}

	int getValence() const
	{
		if (this->valence == -1)
			return 0;
		return this->valence;
	}

	int getHydrogenCountHydrogen() const
	{
		if (this->bondsCnt == 0 
			&& this->charge == 0 
			&& this->getValence() == 0
			&& this->radicalCount == 0) 
		{
			return 1;
		}
		return 0;
	}

	int getHydrogenCountMetal() const 
	{
		return 0;
	}

	int getSuitableValence(int valence) const 
	{
		vector<int> val = PeriodicTable::getValence(this->element);
		int defVal = val[val.size() - 1];
		for (int i = 0; i < val.size(); ++i)
		{
			if (val[i] >= valence)
			{
				defVal = val[i];
				break;
			}
		}
		return defVal;
	}

	int getHydrogenCountGroup14() const
	{
		int valence = this->valence;
		if (valence == -1) 
		{
			valence = this->bondsCnt - abs(this->charge) + this->radicalCount;
		}
		int defVal = getSuitableValence(valence);
		return std::max(0, defVal - valence);
	}

	int getHydrogenCountNonMetal() const
	{
		int valence = this->valence;
		if (valence == -1) 
		{
			valence = this->bondsCnt - this->charge + this->radicalCount;
		}
		int defVal = getSuitableValence(valence);
		return std::max(0, defVal - valence);
	}

	int getHydrogenCountTin() const 
	{
		int valence = this->valence;
		if (valence == -1) 
		{
			valence = this->bondsCnt - abs(this->charge) + this->radicalCount;
		}
		int defVal = getSuitableValence(valence);
		if (this->charge != 0) 
		{
			defVal = 4;
		}
		return std::max(0, defVal - valence);
	}

	int getHydrogenCountBoron() const 
	{
		int valence = 3;
		int hc = valence - this->charge - this->bondsCnt - this->radicalCount;
		return std::max(0, hc);
	}

	int getImplicitHydrogenCnt() const {
		if (hydroCount != -1)
			return hydroCount;
		int number = PeriodicTable::getNumber(this->element);
		int var1 = valence - bondsCnt + charge;
		int var2;
		switch(number)
		{
			case 1:
				var2 = getHydrogenCountHydrogen();
				break;
			case 3:
			case 11:
			case 19:
			case 37:
			case 55:
			case 87: //group 1
			case 4:
			case 12:
			case 20:
			case 38:
			case 56:
			case 88: //group 2
			case 13:
			case 31:
			case 49:
			case 41: //group 13 but Boron
			case 82:
			case 83: //Bi and Pb
				var2 = getHydrogenCountMetal();
				break;
			case 6:
			case 14:
			case 32:
			case 51:      //C, Si, Ge, Sb
				var2 = getHydrogenCountGroup14();
				break;
			case 50:       //Sn
				var2 = getHydrogenCountTin();
				break;
			case 7:
			case 8:
			case 9:
			case 15:
			case 16:
			case 17:      //N, O, F, P, S, Cl
			case 33:
			case 34:
			case 35:
			case 53:
			case 85:       //As, Se, Br, I, At
				var2 = getHydrogenCountNonMetal();
				break;
			case 5:
				var2 = getHydrogenCountBoron();
				break;
			default:
				var2 = 0;
		}
		//if (var1 != var2)
			//std::cout << this->element << " " << var1 << " " << var2 << std::endl;
		return var2;
	}

	string getElement() const { return element; }
};

class Atoms
{
	map<string, Atom> atoms; // atom id => Atom

	// atom id => list of bound atom ids
	map<string, vector<string> > graph;

	void addAtom(const string& id, const string& element, int valence = -1, int charge = 0, int hydroCount = -1, int radicalCount = 0)
	{
		if (atoms.find(id) != atoms.end())
		{
			ostringstream oss;
			oss << "Atom with id '" << id << "' already exists.";
			throw logic_error(oss.str());
		}
		atoms.insert(make_pair(id, Atom(id, element, valence, charge, hydroCount, radicalCount)));
		graph.insert(make_pair(id, vector<string>()));
	}

	void addBond(const string& id1, const string& id2, int order)
	{
		graph[id1].push_back(id2);
		graph[id2].push_back(id1);
		Atom& a1 = atoms.find(id1)->second;
		Atom& a2 = atoms.find(id2)->second;
		a1.addBonds(order);
		a2.addBonds(order);
	}

	vector<vector<string>> get_connected_components() const
	{
		vector<vector<string> > connected_components;
		set<string> visited;
		
		for (std::map<string, vector<string>>::const_iterator it = graph.cbegin(); it != graph.cend(); ++it)
		{
			const string& currentAtomID = it->first;
			
			if (visited.find(currentAtomID) == visited.end())
			{
				vector<string> component;
				queue<string> q;
				q.push(currentAtomID);
				visited.insert(currentAtomID);
				component.push_back(currentAtomID);

				while (!q.empty())
				{
					string neighborID = q.front();
					q.pop();
					const vector<string>& adjList = graph.find(neighborID)->second;

					for (std::vector<string>::const_iterator adjIt = adjList.cbegin(); adjIt != adjList.cend(); ++adjIt)
					{
						if (visited.find(*adjIt) == visited.end())
						{
							visited.insert(*adjIt);
							q.push(*adjIt);
							component.push_back(*adjIt);
						}
					}
				}

				connected_components.push_back(component);
			}
		}

		return connected_components;
	}

	static string getFormula(const map<string, int>& molecule)
	{
		std::ostringstream oss;
		std::map<string, int>::const_iterator tC, tH;
		tH = molecule.end();
		tC = molecule.find("C");
		if(tC != molecule.end())
		{
			const string& element = tC->first;
			int cnt = tC->second;
			
			oss << element;

			if (cnt > 1)
				oss << cnt;
			
			tH = molecule.find("H");
			
			const string& element1 = tH->first;
			cnt = tH->second;
			
			oss << element1;

			if (cnt > 1)
				oss << cnt;
		}
		for (std::map<string, int>::const_iterator el = molecule.begin(); el != molecule.end(); ++el)
		{
			if(el == tC || el == tH)
				continue;
			const string& element = el->first;
			int cnt = el->second;

			oss << element;

			if (cnt > 1)
				oss << cnt;
		}

		return oss.str();
	}

	// vector of molecules, where molecule = { element => element count }
	vector<map<string, int>> getMolecules() const
	{
		vector<map<string, int>> molecules;

		vector<vector<string>> connected_components = this->get_connected_components();
		for (std::vector<vector<string>>::const_iterator component = connected_components.begin(); component != connected_components.end(); ++component)
		{
			map<string, int> molecule;
			int implicitHydrogenCnt = 0;
			for (std::vector<string>::const_iterator id = (*component).begin(); id != (*component).end(); ++id)
			{
				const Atom& a = atoms.find(*id)->second;
				molecule[a.getElement()]++;
				implicitHydrogenCnt += a.getImplicitHydrogenCnt();
			}

			if (implicitHydrogenCnt)
				molecule["H"] += implicitHydrogenCnt;

			molecules.push_back(molecule);
		}

		return molecules;
	}


public:

	ptree find_tag(ptree& pt, string tag)
	{
		for (ptree::iterator it = pt.begin(); it != pt.end(); ++it)
		{
			if (it->first == tag)
			{
				return it->second;
			}
			ptree tmp = find_tag(it->second, tag);
			if (!tmp.empty())
			{
				return tmp;
			}
		}
		return ptree(); 
	}

	void readFromCML(const string& filename)
	{
		ptree pt;
		try
		{
			read_xml(filename, pt);
		}
		catch(std::exception &e)
		{
			throw string("Error: no file exists");
		}
		//std::ifstream chem_atom ("table.txt");
		//std::ifstream exmp ("ketcher (4).cml");
		//if(!chem_atom.is_open())
			//throw string("Error: source file problem");
		//vector<string> atom_tab;
		//while(true)
		//{
			//if(chem_atom.eof())
				//break;
			//string str;
			//getline(chem_atom, str, '\n');
			//atom_tab.push_back(str);
		//}
		ptree molecule_pt;
		try
		{
			molecule_pt = find_tag(pt, "molecule");
			if (molecule_pt.empty())
			{
				throw std::exception();
			}
			ptree atomArray_pt = find_tag(molecule_pt, "atomArray");
			if (atomArray_pt.empty())
			{
				throw std::exception();
			}
			//BOOST_FOREACH(ptree::value_type &i, pt.get_child("cml.molecule.atomArray"))
			BOOST_FOREACH(ptree::value_type &i, atomArray_pt.get_child(""))
			{
				std::string name = i.first;
				ptree& sub_pt = i.second;
				int valence = -1;
				int charge = 0;
				int hydroCount = -1;

				if (name == "atom")
				{
					string id = sub_pt.get<string>("<xmlattr>.id");
					string elementType = sub_pt.get<string>("<xmlattr>.elementType");
				
					if(PeriodicTable::getNumber(elementType) == -1)
					{
						throw string("Error: non chemical atom in file");
					}

					boost::optional<int> mrvValence = sub_pt.get_optional<int>("<xmlattr>.mrvValence");
				
					if (mrvValence)
					{
						valence = mrvValence.value();
					}

					boost::optional<int> formalcharge = sub_pt.get_optional<int>("<xmlattr>.formalCharge");
				
					if (formalcharge)
					{
						charge = formalcharge.value();
					}

					boost::optional<int> hydrogenCount = sub_pt.get_optional<int>("<xmlattr>.hydrogenCount");
				
					if (hydrogenCount)
					{
						hydroCount = hydrogenCount.value();
					}

					boost::optional<string> radicalCount = sub_pt.get_optional<string>("<xmlattr>.radical");
					int radCount = 0;
					if (radicalCount)
					{
						if (radicalCount.value() == "divalent1")
						{
							radCount = 2;
						}
						else
						{
							std::cout << "Error: unknown radical count\n";
							throw std::exception();
						}
					}
					this->addAtom(id, elementType, valence, charge, hydroCount, radCount);
				}
			}
		}
		catch(std::exception& e)
		{
			throw string("Error: No interesting data");
		}
		try
		{
			ptree bondArray_pt = find_tag(molecule_pt, "bondArray");
			if (bondArray_pt.empty())
			{
				throw std::exception();
			}
			BOOST_FOREACH(ptree::value_type &i, bondArray_pt.get_child(""))
			{
				std::string name = i.first;
				ptree& sub_pt = i.second;

				if (name == "bond")
				{
					string atomRefs2 = sub_pt.get<string>("<xmlattr>.atomRefs2");
					int order = sub_pt.get<int>("<xmlattr>.order");

					vector<string> ids;
					boost::split(ids, atomRefs2, boost::is_any_of(" \t\n"));
					if (ids.size() != 2)
					{
						ostringstream oss;
						oss << "Wrong bond '" << atomRefs2 << "'.";
						throw logic_error(oss.str());
					}
					this->addBond(ids.at(0), ids.at(1), order);
				}
			}
		}
		catch(std::exception& e)
		{ 
		}

	}

	void correct_valences()
	{
		for (std::map<string, Atom>::iterator id_atom = atoms.begin(); id_atom != atoms.end(); ++id_atom)
		{
			id_atom->second.correct_valence();
		}
	}
	
	string getFormula()
	{
		vector<map<string, int>> molecules = this->getMolecules();

		vector<string> molecules_str;
		for (std::vector<map<string, int>>::const_iterator molecule = molecules.begin(); molecule != molecules.end(); ++molecule)
		{
			molecules_str.push_back(getFormula(*molecule));
		}

		string formula = "";
		for (unsigned i = 0; i < molecules_str.size(); ++i)
		{
			if (i > 0)
				formula += ".";
			formula += molecules_str[i];
		}
		return formula;
	}
};

string getFormulaFromCML(const string& xmlfile)
{
	Atoms atoms;
	atoms.readFromCML(xmlfile);
	//atoms.correct_valences();
	return atoms.getFormula();
}
