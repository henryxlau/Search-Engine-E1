#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>

using namespace std;

bool find(list<string> &mylist, string item)
{
	bool found = false;

	list<string>::iterator it = mylist.begin();
	while((it != mylist.end()) && !found)
	{
		if (*it == item)
		{
			found = true;
		}
		it++;
	}

	return found;
}


void processFile(string filename, list<string> &filelist)
{
	ifstream input;
	string line;
	string word;

	if (!find(filelist, filename))
	{
		filelist.push_back(filename);

		input.open(filename.c_str());
		if(input.is_open())
		{
			while(getline(input,line))
			{
				//find link (if there is one)
				size_t brackets = line.find("](");

				while(brackets != line.npos)	//if "](" is present, need to check if theres a link to pull out
       	     	{
					size_t openBrack = line.find_last_of("[",brackets);
					size_t closeParen = line.find_first_of(")",brackets);

					if((openBrack != line.npos) && (closeParen != line.npos))
					{
						//save link
						string linkTarget = line.substr(brackets + 2, closeParen - (brackets + 2));	//is +2 to skip past the ](

						processFile(linkTarget, filelist);

						//remove brackets and target filename
						line.replace(brackets, (closeParen - brackets + 1), "");
						line.replace(openBrack, 1, "");
					}

					// anymore markdown on this line?
					brackets = line.find("](");
				}
			}
			input.close();
		}
		else
		{
			cout << "unable to open file: " << filename << endl;
		}
	}
}


void processSeedFile(char* seedFile, char *outputFilename)
{
	ifstream seedfile;
	stringstream ss;
	string line;
	string fn;
	ofstream outfile;
	list<string> filelist;

	seedfile.open(seedFile);
	if (seedfile.is_open())
	{
		while(getline(seedfile, line))
		{
			if (line.size() > 0)
			{
				ss.clear();
				ss.str(line);
				ss >> fn;

				processFile(fn, filelist);
			}
		}

		seedfile.close();

		// write filelist to file
		outfile.open(outputFilename);
		if (outfile.is_open())
		{
			list<string>::iterator it = filelist.begin();
			while(it != filelist.end())
			{
				outfile << *it << endl;

				it++;
			}				

			outfile.close();
		}
		else
		{
			cout << "unable to open outputfile: " << outputFilename << endl;
		}
	}
	else
	{
		cout << "unable to open seedfile: " << seedFile << endl;
	}
}


int main(int argc, char *argv[])
{
	if (argc == 3)
	{
		processSeedFile(argv[1], argv[2]);
	}
	else
	{
		cout << "usage: " << argv[0] << " seedFile outfileFile" << endl;
	}
}


