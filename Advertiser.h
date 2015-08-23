#ifndef ADVERTISER_H
#define ADVERTISER_H

#include <string>
#include <map>

using namespace std;

class Advertiser
{
public:
	Advertiser();
	Advertiser(string name, string keyword, double bid);
	~Advertiser();
	void addToTotal(double bid);
	void addKeyword(string keyword, double bid);
	bool operator==(Advertiser & rhs);
	double getTotalCharge();
	double getBid(string key);
	string getName();

private:
	string _name;
	map<string, double> _keyToBid;
	double _totalCharge;
};

#endif //ADVERTISER_H
