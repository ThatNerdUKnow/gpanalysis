#pragma once
#include"mill.h"
#include <vector>
#include "product.h"
#include <string>

using namespace std;
// This class tracks balances in revenue, cost and contribution margin daily
class logger
{
public:
	vector<mill> sampleMills;
	vector<double> revenue;
	vector<double> cost;
	vector<double> contributionMargin;
	vector<double> plushREV;
	vector<double> plushCOST;
	vector<double> ssREV;
	vector<double> ssCOST;
	vector<double> ssCM;
	vector<double> plushCM;
	vector<double> ssBacklog;
	vector<double> plushBacklog;

	string name = "";

	double revACC = 0;
	double costACC = 0;
	double contACC = 0;
	double plushACC = 0;
	double ssACC = 0;

	logger(vector<mill> samples)
	{
		sampleMills = samples;
		//name = sampleMills[0].name + " " + sampleMills[1].name + " " + sampleMills[2].name;
		for (mill current : samples)
		{
			name = name + " " + current.name;
		}
	}

	void update(double rev, double costs)
	{
		double contMargin = rev - costs;

		revACC += rev;
		costACC += costs;
		contACC += contMargin;

		revenue.push_back(revACC);
		cost.push_back(costACC);
		contributionMargin.push_back(contACC);
	}

	void update(double ssRev, double ssCosts, double plushRev, double plushCosts)
	{
		ssREV.push_back(ssRev);
		ssCOST.push_back(ssCosts);
		plushREV.push_back(plushRev);
		plushCOST.push_back(plushCosts);


		ssCM.push_back(ssRev - ssCosts);
		plushCM.push_back(plushRev - plushCosts);

		ssBacklog.push_back(ssACC);
		plushBacklog.push_back(plushACC);


		ssACC = 0;
		plushACC = 0;

		double revenue = ssRev + plushRev;
		double costs = ssCosts + plushCosts;

		update(revenue, costs);


	}

	void updateBalances(double ssBalance, double plushBalance)
	{
		plushACC += plushBalance;
		ssACC += ssBalance;
	
	}



	long double totalCM()
	{
		return contACC;
	}

	long double totalREV()
	{
		return revACC;
	}
	long double totalCOST()
	{
		return costACC;
	}
};

