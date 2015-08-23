#include <iostream>
#include <QApplication>

#include "SearchEngine.h"

using namespace std;

int main(int argc, char *argv[])
{	
	QApplication app(argc, argv);

	if(argc > 3)
	{
		try
		{
			char* webpages = argv[1];
			char* advertisers = argv[2];
			char* bill = argv[3];
			SearchEngine *me = new SearchEngine(0,webpages,advertisers, bill);
			me->show();
			return app.exec();
		}
		catch(const char* message)
		{
			cout << message << "\n";
		}
	}
	else
	{
		cout << "Not enough arguments found\n";
		return -1;
	}


}
