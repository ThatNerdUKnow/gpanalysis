#pragma once
#include <string>
#include "product.h"
using namespace std;

class mill
{
public:
	

	
	mill(string Name, double ProductionSpeed, product productToSell,double material, double production)
	{
		name = Name;
		productionSpeed = ProductionSpeed;
		dailyProduction = productionSpeed / 365;
		product = productToSell;
		setCost(material, production);
	}

	void step(int day)
	{
		if (day > modifierTime)
		{
			modifier = 1;
		}
		else if (conversion)
		{
			modifier = 0;
		}
		else if (expansion)
		{
			modifier = .5;
		}
		inventory += dailyProduction * modifier;
	}

	mill()
	{
	
	}
	double inventory = 0;
	double modifier = 1;
	double modifierTime = 0;
	bool conversion = false;
	bool expansion = false;
	double dailyProduction;
	string name;
	product product;
	double productionSpeed;

	
	string getname()
	{
		return name;
	}
private:
	void setCost(double material, double production)
	{
		product.setCost(material, production);
	}

	
};
