#include "Advertiser.h"
using namespace std;

Advertiser::Advertiser() {}

Advertiser::Advertiser(string name, string keyword, double bid)
{
	_name = name;
	addKeyword(keyword, bid);
	_totalCharge = 0;
}

Advertiser::~Advertiser() {}

void Advertiser::addKeyword(string keyword, double bid)
{
	_keyToBid.insert(pair<string, double>(keyword, bid));
}

void Advertiser::addToTotal(double bid)
{
	_totalCharge += bid;
}

bool Advertiser::operator==(Advertiser & rhs)
{
	return (_name == rhs._name);
}

double Advertiser::getTotalCharge()
{
	return _totalCharge;
}

double Advertiser::getBid(string key)
{
	map<string, double>::iterator it = _keyToBid.find(key);
	if(it != _keyToBid.end())
	{
		return it->second;
	}
	else
	{
		return 0.0;
	}
}

string Advertiser::getName()
{
	return _name;
}
