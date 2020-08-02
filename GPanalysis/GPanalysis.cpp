//#include "product.h"
#include "region.h"
#include "mill.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "logger.h"
#include <fstream>
using namespace std;


map<string, double> shippingCost;
map<string, vector<mill>> mills;

vector<region> regions;
vector<logger> logs;

string keys[3] = { "Oregon","Wisconsin","Arkansas" };

// Declare Products

product plush("Plush", 220);
product softStrong("Strong & Soft", 200);


void setup();
void run();
void flush();

int main()
{
    cout << "INITILIZING" << endl;
    setup();
    cout << "FINISHED INITILIZING" << endl;
    cout << "STARTING SIMULATION" << endl;
    run();
    cout << "FINISHED SIMULATION" << endl;
    cout << "FLUSHING DATA" << endl;
    flush();
}


void setup()
{
    // Fill the map with their key value pairs

    shippingCost["Oregon"] = 0;
    shippingCost["Wisconsin"] = 0;
    shippingCost["Arkansas"] = 0;

    

    // Declare Mills

    mill* current = new mill("Wisconsin",1500000,softStrong,15,25);

    // Insert to mills map
    mills["Wisconsin"].push_back(*current);

    current = new mill("Wisconsin Converted (Plush)", 1400000, plush, 17, 30);
    current->conversion = true;
    current->modifier = 0;
    current->modifierTime = 50;
    mills["Wisconsin"].push_back(*current);

    current = new mill("Oregon", 1100000, softStrong, 17, 35);
    mills["Oregon"].push_back(*current);

    current = new mill("Oregon Converted (Plush)", 100000, plush, 19, 40);
    current->conversion = true;
    current->modifier = 0;
    current->modifierTime = 40;
    mills["Oregon"].push_back(*current);

    current = new mill("Arkansas", 800000, plush, 18, 35);
    mills["Arkansas"].push_back(*current);

    current->expansion = true;
    current->modifier = .5;
    current->modifierTime = 280;
    current->productionSpeed = 100000;
    current->name = "Arkansas Expanded";
    mills["Arkansas"].push_back(*current);

    current = new mill("Arkansas Converted", 900000, softStrong, 16, 35);
    current->modifier = 0;
    current->modifierTime = 30;
    mills["Arkansas"].push_back(*current);

    // Fill regions

    //East

    shippingCost["Oregon"] = 42;
    shippingCost["Wisconsin"] = 32;
    shippingCost["Arkansas"] = 30;
    region* currentRegion = new region("East", 1072.146119, 356.1643836, shippingCost);
    regions.push_back(*currentRegion);
    
    //West 
    shippingCost["Oregon"] = 22;
    shippingCost["Wisconsin"] = 44;
    shippingCost["Arkansas"] = 40;
    currentRegion = new region("West", 526.0273973, 624.6576342, shippingCost);
    regions.push_back(*currentRegion);

    // Midwest
    shippingCost["Oregon"] = 34;
    shippingCost["Wisconsin"] = 26;
    shippingCost["Arkansas"] = 22;
    currentRegion = new region("Midwest", 620.6392694, 69.7716895, shippingCost);
    regions.push_back(*currentRegion);

    // South
    shippingCost["Oregon"] = 43;
    shippingCost["Wisconsin"] = 30;
    shippingCost["Arkansas"] = 20;
    currentRegion = new region("South", 983.0163986, 129.3150685, shippingCost);
    regions.push_back(*currentRegion);

}

void run()
{
    for (mill oregon : mills["Oregon"])
    {
        for (mill wisconsin : mills["Wisconsin"])
        {
            for (mill arkansas : mills["Arkansas"])
            {
                vector<region> currentRegions = regions;
                vector<mill> currentMills;
               
                
                currentMills.push_back(oregon);
                currentMills.push_back(wisconsin);
                currentMills.push_back(arkansas);
                logger log(currentMills);
               // cout << "NOW SAMPLING\t" << currentMills[0].name << " " << currentMills[1].name << " " << currentMills[2].name << endl;
                //logger currentLogs(currentMills);

                double start = 2020.5 * 365;
                double x;

                // This loop cycles until we're done simulating with our test paramaters

                for (double days = 0; days + start < double(2030) * 365; days++)
                {
                    x = days + start;
                    

                    // Update Inventory Counts for the mills and demand counts for the regions
                    for (mill &current : currentMills)
                    {
                        current.step(days);
                    }
                    for (region& current : currentRegions)
                    {
                        // Advance Demand (New day, new orders)
                        current.demandAdvance();
                    }

                    // Function here takes a region and returns it by reference. Easy peasy.
                    // Function ALSO returns a mill with it's updated balance. or something like that
                    //cout << days << endl;

                    double totalSSDemand = 0;
                    double totalPlushDemand = 0;
                    double revenue = 0;
                    double costs = 0;
                    double plushCost=0;
                    double plushRev=0;
                    double ssCost=0;
                    double ssRev=0;
                    for (region& currentRegion : currentRegions)
                    {
                        int ssOptimalINDEX = 0;
                        int plushOptimalINDEX = 0;

                        // Put a loop here as a contingency just in case a region is unable to get their demands met
                        bool match = false;
                        bool ssMatch = false;
                        bool plushMatch = false;
                        bool demandMet;
                        bool matchFound;
                        do
                        {
                            
                            ssMatch = false;
                            plushMatch = false;
                            
                            int plushCount = 0;
                            int ssCount = 0;


                            

                            // Find the optimal mill
                            for (int count = 0; count < currentMills.size(); count++)
                            {
                                // Make a temporary copy of the current mill to save some typing
                                mill workingMill = currentMills[count];
                                mill optimalSS = currentMills[ssOptimalINDEX];
                                mill optimalPlush = currentMills[plushOptimalINDEX];


                                
                                // Find the price+shipping of the current mill
                                double totalCost = 0;

                                if (workingMill.inventory > 0)
                                {
                                    if (workingMill.product.name == softStrong.name)
                                    {
                                        // Find current Mill's total cost and the optmal solution's cost
                                        totalCost = workingMill.product.cost + currentRegion.shippingCost[workingMill.name];

                                        double optimalCost = optimalSS.product.cost + currentRegion.shippingCost[optimalSS.name];

                                        // Compare the two costs and update the match bool and index if necessary

                                        if (totalCost < optimalCost || ssCount == 0)
                                        {
                                            ssOptimalINDEX = count;
                                            ssMatch = true;
                                        }
                                        ssCount++;
                                    }
                                    else if (workingMill.product.name == plush.name)
                                    {
                                        totalCost = workingMill.product.cost + currentRegion.shippingCost[workingMill.name];

                                        double optimalCost = optimalPlush.product.cost + currentRegion.shippingCost[optimalPlush.name];

                                        if (totalCost < optimalCost|| plushCount == 0)
                                        {
                                            plushOptimalINDEX = count;
                                            plushMatch = true;
                                        }
                                        plushCount++;
                                    }
                                }

                            }
                           // cout << match << "\t" << x;
                            // Update our balances and loop if our needs are still not met
                            // TODO make a dummy mill with 0 inventory for when we can't find a match to meet demand


                            //match = ssMatch && plushMatch;


                           
                            double smaller = 0;

                            if (ssMatch)
                            {
                                smaller = min(currentRegion.ssDemand, currentMills[ssOptimalINDEX].inventory);

                                currentRegion.ssDemand -= smaller;
                                currentMills[ssOptimalINDEX].inventory -= smaller;
                                revenue += smaller * softStrong.price;
                                ssRev += smaller * softStrong.price;
                                ssCost += smaller*(currentMills[ssOptimalINDEX].product.cost + currentRegion.shippingCost[currentMills[ssOptimalINDEX].name]);
                                costs += smaller * (currentMills[ssOptimalINDEX].product.cost + currentRegion.shippingCost[currentMills[ssOptimalINDEX].name]);
                            }
                            // Update Revenue and cost for soft and strong

                            if (plushMatch)
                            {
                                smaller = min(currentRegion.plushDemand, currentMills[plushOptimalINDEX].inventory);

                                currentRegion.plushDemand -= smaller;
                                currentMills[plushOptimalINDEX].inventory -= smaller;
                                revenue += smaller * plush.price;
                                costs += smaller * (currentMills[plushOptimalINDEX].product.cost + currentRegion.shippingCost[currentMills[plushOptimalINDEX].name]);

                                plushRev += smaller * softStrong.price;
                                plushCost += smaller * (currentMills[plushOptimalINDEX].product.cost + currentRegion.shippingCost[currentMills[plushOptimalINDEX].name]);
                            }

                           // cout << "SS\t" << currentRegion.ssDemand << "\tPL\t" << currentRegion.plushDemand << endl;
                          
                            totalSSDemand += currentRegion.ssDemand;
                            totalPlushDemand += currentRegion.plushDemand;


                            demandMet = !(currentRegion.ssDemand > 0 || currentRegion.plushDemand > 0);
                            matchFound = (ssMatch && plushMatch);
                       } while (!(demandMet == matchFound));

                       log.updateBalances(currentRegion.ssDemand, currentRegion.plushDemand);
                    }
                
                    log.update(ssRev, ssCost, plushRev, plushCost);
                   //log.update(revenue, costs);

                   // cout << "SS\t" << totalSSDemand << "\tPL\t" << totalPlushDemand << endl;
                }

                logs.push_back(log);
                    
                }
            }
        }
    }


void flush()
{
    ofstream output;
    output.open("GPdata.csv");


    output << "Name,Revenue,Costs,Contribution Margin\n";
    for (logger log : logs)
    {
        long double totalREV = log.totalREV();
        long double totalCOST = log.totalCOST();
        long double totalCM = totalREV - totalCOST;
       // cout << fixed << log.name << "," << totalREV << "," << totalCOST << "," << totalCM << endl;
        output << fixed << log.name << "," << totalREV << "," << totalCOST << "," << totalCM << endl;

        output << "REVENUE,";
        for (double revenue : log.revenue)
        {
            output << revenue << ",";
        }
        output << endl;

        output << "COSTS,";
        for (double cost : log.cost)
        {
            output << cost << ",";
        }
        output << endl;

        output << "CONTRIBUTION MARGIN,";
        for (double contMar : log.contributionMargin)
        {
            output << contMar << ",";
        }
        output << endl;

        output << "Strong & Soft" << endl;

        output << "Unfulfilled Orders:,";

        for (double balance : log.ssBacklog)
        {
            output << balance << ",";
        }
        output << endl << ",";

        for (double ssRev : log.ssREV)
        {
            output << ssRev << ",";
        }
        output << endl << " ,";

        for (double sscost : log.ssCOST)
        {
            output << sscost << ",";
        }
        output << endl << " ,";

        for (double cm : log.ssCM)
        {
            output << cm << ",";
        }
        output << endl;

        output << "Plush" << endl;
        
        output << "Unfulfilled Orders:,";

        for (double balance : log.plushBacklog)
        {
            output << balance << ",";
        }
        output << endl << ",";

        for (double plushRev : log.plushREV)
        {
            output << plushRev << ",";
        }
        output << endl << " ,";

        for (double plushcost : log.plushCOST)
        {
            output << plushcost << ",";
        }
        output << endl << " ,";

        for (double plCM : log.plushCM)
        {
            output << plCM << ",";
        }
        output << endl << endl;

    }

    output.close();
}