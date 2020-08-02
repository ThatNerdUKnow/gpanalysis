#pragma once
#include <string>
using namespace std;
class product
{
public:
	string name;
	double price;
	double capacity;
	double demand;
	double cost;
	
	
	product(string givenName, double givenPrice)
	{
		name = givenName;
		price = givenPrice;
		
	}

	
	

	product()
	{}
	
	void setCost(double material, double production)
	{
		cost = material + production;
	}
};

