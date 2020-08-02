#pragma once
#include <string>
#include <map>
using namespace std;

struct shippingRate
{
	double shippingRate;
};

class region
{
public:
	string name;
	double ssDemand = 0;
	double plushDemand = 0;

	

	
	map<string, double> shippingCost;

	region(string Name, double strongRate, double ratePlush, map<string, double> shipping)
	{
		name = Name;
		ssRate = strongRate*2;
		plushRate = ratePlush*2;
		shippingCost = shipping;
		demandAdvance();
	}

	void demandAdvance()
	{
		ssDemand += ssRate;
		plushDemand += plushRate;
	}

private:
	double ssRate;
	double plushRate;

};

