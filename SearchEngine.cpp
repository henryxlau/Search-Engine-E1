//includes
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QGridLayout>
#include <QMessageBox>
#include <QString>
#include <QTextEdit>
#include <QLabel>
#include <QRadioButton>
#include <QGroupBox>
#include <QHBoxLayout>

#include "Set.h"
#include "WebPage.h"
#include "Advertiser.h"
#include "SearchEngine.h"
#include "msort.h"

using namespace std;

SearchEngine::SearchEngine(QWidget *parent, char* webpages, char* advertisers, char* bill) : QWidget(parent)
{
	//create primary search window
	setWindowTitle(QString::fromStdString("Michelle and Henry's Magic Search Engine"));
	setMinimumWidth(450);
	searchBar = new QLineEdit();
	QPushButton *btnSearch = new QPushButton("Search");	
	resultsList = new QListWidget;
	QPushButton *btnQuit = new QPushButton("Quit");

	//create toggling buttons for sorting
	QGroupBox *groupBox = new QGroupBox(tr("Sort by:"));

	rbuttonByFilename = new QRadioButton("File name");
	rbuttonByRank = new QRadioButton("Rank");

	rbuttonByRank->setChecked(true);

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(rbuttonByFilename);
	hbox->addWidget(rbuttonByRank);
	groupBox->setLayout(hbox);

	//fill full layout
	QGridLayout *layout = new QGridLayout;
	
	layout->addWidget(searchBar, 0, 0);
	layout->addWidget(btnSearch, 0, 5);
	layout->addWidget(resultsList, 1, 0, 4, 6);
	layout->addWidget(btnQuit, 5, 5);
	layout->addWidget(groupBox, 5, 0);

	setLayout(layout);

	QObject::connect(btnSearch, SIGNAL(clicked()), this, SLOT(enterSearch()));
	QObject::connect(btnQuit, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(searchBar, SIGNAL(returnPressed()), btnSearch, SIGNAL(clicked()));
	QObject::connect(resultsList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(pageSelect(QListWidgetItem *)));
	QObject::connect(rbuttonByRank, SIGNAL(clicked()), this, SLOT(searchResultOrder()));
	QObject::connect(rbuttonByFilename, SIGNAL(clicked()), this, SLOT(searchResultOrder()));

	//set up window displaying a file's contents and links
	newWin = new QWidget();	
	fileContents = new QTextEdit();
	fileContents->setReadOnly(true);
	incomingLinks = new QListWidget;
	outgoingLinks = new QListWidget;
	ads = new QListWidget;
	QLabel *contentLabel = new QLabel("File content");
	QLabel *inLabel = new QLabel("Incoming Links:");
	QLabel *outLabel = new QLabel("Outgoing Links:");
	QLabel *adLabel = new QLabel("Advertisements:");
	QGridLayout *layout2 = new QGridLayout;

	layout2->addWidget(contentLabel, 0, 0, 1, 1);
	layout2->addWidget(fileContents, 1, 0, 6, 1);
	layout2->addWidget(inLabel, 0, 1, 1, 1);
	layout2->addWidget(incomingLinks, 1, 1, 4, 1);
	layout2->addWidget(outLabel, 5, 1, 1, 1);
	layout2->addWidget(outgoingLinks, 6, 1, 4, 1);
	layout2->addWidget(adLabel, 7, 0, 1, 1);	
	layout2->addWidget(ads, 8, 0, 2, 1);

	layout2->setColumnStretch(0, 5);
	layout2->setColumnStretch(1, 2);

	newWin->setLayout(layout2);

	QObject::connect(incomingLinks, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(pageSelect(QListWidgetItem *)));
	QObject::connect(outgoingLinks, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(pageSelect(QListWidgetItem *)));

	//set up ad popup
	QObject::connect(ads, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(showAd(QListWidgetItem *)));

	//store filename for later
	billFile = bill;

	//set up internal stuff to be able to search        
	wordList = new vector<string>;	

	readFile(webpages);
	buildWebPageMap();

	tieIncomingLinks();	
	readAdvertisers(advertisers);
}

SearchEngine::~SearchEngine()
{
	wordMap.clear();
	allPages.clear();
	pagesByName.clear();
	rankInfo.clear();
	adCompanies.clear();
	bidders.clear();
	wordList->clear();
}

void SearchEngine::readFile(char* filename)
{
	ifstream input;
	stringstream ss;
	string line;
	string wpFilename;

	input.open(filename);
	if(input.is_open())
	{
		while(getline(input, line))
		{
			if(line.size() > 0)
			{
				ss.clear();	//empty out ss (in case there were some characters left over from previous loop)
				ss.str(line);
				ss >> wpFilename;

				WebPage *newPage = new WebPage(wpFilename);
				allPages.insert(allPages.end(), *newPage);
			}
			//else do nothing
		}
	}
	else
	{
		throw "Cannot open file";
	}
	input.close();
}

void SearchEngine::readAdvertisers(char* filename)
{
	ifstream input;
	stringstream ss;
	string line;
	int numCompanies;
	string key;
	double bid;
	string company;

	input.open(filename);
	if(input.is_open())
	{
		while(getline(input, line))
		{
			ss.str(line);
			ss >> numCompanies;

			for(int i = 0; i < numCompanies; i++)
			{
				getline(input, line);
				for(unsigned int j = 0; j < line.size(); j++)
				{
					if(line[j] == '\t')
					{
						line[j] = ' ';
					}
				}
				line = trimString(line);

				//whatever is the first word is the keyword
				unsigned int space = line.find(" ");
				key = line.substr(0, space);
				line = trimString(line);

				//make key lowercase
				for(unsigned int j = 0; j < key.size(); j++)
				{
					key[j] = tolower(key[j]);
				}

				//set up for next step
				line = line.substr(space);
				line = trimString(line);

				//next is the bid
				ss.clear();
				ss.str(line);
				ss >> bid;

				//set up for next step
				space = line.find(" ");
				line = line.substr(space);
				line = trimString(line);

				//whatever is left is the company name
				company = line;

				Advertiser *temp = new Advertiser(company, key, bid);
cout << "created Advertiser: name = " << company << ", key = " << key << ", bid = " << bid << endl;
				Advertiser *found = NULL;
				for(Set<Advertiser*>::iterator it_set = adCompanies.begin(); it_set != adCompanies.end(); it_set++)
				{
					if((*it_set)->getName() == temp->getName())
					{
						found = *it_set;
					}
				}
				if(found != NULL)
				{
					found->addKeyword(key, bid);
cout << company << " added key and bid" << endl;
				}
				else
				{
					adCompanies.insert(temp);
cout << company << " is new, was added" << endl;
				}

				map<string, vector<Advertiser*>* >::iterator it_map = bidders.find(key);
				if(it_map == bidders.end())
				{
					vector<Advertiser*> *list = new vector<Advertiser*>;
					list->push_back(temp);
					bidders.insert(pair<string,vector<Advertiser*>*>(key,list));
				}
				else
				{
					vector<Advertiser*> *list = it_map->second;
					list->push_back(temp);
				}
			}
		}
	}
	else
	{
		throw "Cannot open file";
	}
	input.close();
}

string& SearchEngine::trim(string &str)
{
	size_t nonWhite = str.find_first_not_of("\t \n\r");
	if(nonWhite != str.npos)	//if found
	{
		str = str.substr(nonWhite);	//str is now itself without leading whitespace
	}

	nonWhite = str.find_last_not_of("\t \n\r");
	if(nonWhite != str.npos)	//if found
	{
		str = str.substr(0, nonWhite + 1);	//str is now itself without trailing whitespace
	}

	return str;
}

string& SearchEngine::removeSymbols(string &line)
{
	size_t openParenth = line.find("(");
	size_t closeParenth = line.find(")");

	if((openParenth == line.npos) || (closeParenth == line.npos) || (openParenth > closeParenth))	//either missing a parenthesis or they are out of order
	{
		throw "Format error";
	}
	else
	{
		line = line.substr(openParenth + 1, (closeParenth - 1) - openParenth);
		line = trim(line);

		size_t comma = line.find(",");
		while(comma != line.npos)	//will keep looping and replacing "," with " " until there are no more ","
		{
			line.replace(comma, 1, " ");
			comma = line.find(",");
		}

		return line;
	}
}

vector<string>* SearchEngine::parseWords(string &input)
{
	vector<string> *wordlist = new vector<string>();
	string line = input;
	string word;

	//find words that end with "ed", "ing", and "s" to remove before adding to wordlist
	int start1 = 0;
	int end1 = 0;

	for (unsigned int i = 0; i < line.size(); i++)
	{
		if (line[i] == 'e')
		{
			start1 = i;
			i++;
			if (line[i] == 'd')
			{
				i++;
				if (line[i] == isspace(i))
				{
					end1 = i;

					line.erase(line.begin()+start1, line.begin()+end1);
				}
			}
			else
				break;
		}

		if (line[i] == 'i')
		{
			start1 = i;
			i++;
			if (line[i] == 'n')
			{
				i++;
				if (line[i] == 'g')
				{
					i++;
					if (line[i] == isspace(i))
					{
						end1 = i;

						line.erase(line.begin()+start1, line.begin()+end1);
					}
				}
				else
					break;
			}
			else
				break;
		}

		if (line[i] == 's')
		{
			start1 = i;
			i++;
			
			if (line[i] == isspace(i))
			{
				end1 = i;

				line.erase(line.begin()+start1, line.begin()+end1);
			}
			else
				break;
		}
	}
	size_t start = line.find_first_not_of("\t \n\r");
	size_t wordEnd;
	
	while(start != string::npos)
	{
		wordEnd = line.find_first_of("\t \n\r", start + 1);

		if(wordEnd == string::npos)
		{
			wordEnd = line.length();
                        
		}

		word = line.substr(start, wordEnd - start);


		for(unsigned int i = 0; i < word.size(); i++)	//make the word all lowercase
		{
			word[i] = tolower(word[i]);
		}

		wordlist->insert(wordlist->end(), word);
		start = line.find_first_not_of("\t \n\r", wordEnd + 1);
	}

	return wordlist;
}

void SearchEngine::processSearchWord(string &query)
{
	//break into individual words
	//if first word is AND set a flag
	//if first word is OR set diff flag
	//otherwise make it lowercase and save it (in a set?)
	//if multiple words w/o AND or OR tis an error
	//if AND or OR--make sure there is the parenthesis (and then discard it when saving words)
	//if both its an error and quit search
	//if only one word: search like in testDump
	//if AND use setIntersection()
	//if OR use setUnion()

	int start1 = 0;
	int end1 = 0;

	for (unsigned int i = 0; i < query.size(); i++)
	{
		cout << query[i] << endl;
		if (query[i] == 'e')
		{
			start1 = i;
			i++;
			if (query[i] == 'd')
			{
				i++;
				if (query[i] == isspace(i))
				{
					end1 = i;

					query.erase(query.begin()+start1, query.begin()+end1);
				}
				else
					break;
			}
			else
				break;
		}

		if (query[i] == 'i')
		{
			start1 = i;
			i++;
			if (query[i] == 'n')
			{
				i++;
				if (query[i] == 'g')
				{
					i++;
					if (query[i] == isspace(i))
					{
						end1 = i;

						query.erase(query.begin()+start1, query.begin()+end1);
					}
					else
						break;
				}
				else
					break;
			}
			else
				break;
		}

		if (query[i] == 's')
		{
			start1 = i;
			i++;

			if (query[i] == isspace(i))
			{
				end1 = i;

				query.erase(query.begin()+start1, query.begin()+end1);
			}
			else
				break;
		}
	}

	cout << "QUERY: " << query << endl;


	Set<int> *mainIndexes;
	string queryCopy(query);
	map<string, Set<int>* >::iterator mapIter;

	try
	{
		if(query.find("AND") == 0 || query.find("OR") == 0)	//would be in first position since query was just trimmed in main
		{
			queryCopy = removeSymbols(queryCopy);
			wordList = parseWords(queryCopy);

			if(wordList->size() < 2)
			{
				throw "Format error";
			}

			if(query.find("AND") == 0)	//aka if this is an AND search
			{

				string firstWord = (*wordList)[0];
				mapIter = wordMap.find(firstWord);

				if(mapIter == wordMap.end())
				{
					throw "There are no results to be displayed";
				}
				else
				{
					mainIndexes = wordMap[firstWord];

					for(unsigned int i = 1; i < wordList->size(); i++)
					{
						string otherWord = (*wordList)[i];
						if(wordMap.find(otherWord) == wordMap.end())
						{
							throw "There are no results to be displayed";
						}
						Set<int> *otherIndexes = wordMap[otherWord];
						mainIndexes = new Set<int>(mainIndexes->setIntersection(*otherIndexes));
					}
				}
			}
			else	//OR search
			{
				vector<string> goodWords;
				for(unsigned int i = 0; i < wordList->size(); i++)
				{
					string tempWord = (*wordList)[i];

					//word is in wordMap
					if(wordMap.find(tempWord) != wordMap.end())
					{
						mainIndexes = wordMap[tempWord];
						goodWords.insert(goodWords.end(), tempWord);
					}
				}

				//for use in displaying ads later				
				wordList = &goodWords;

				int numWords = goodWords.size();
				if(numWords == 0)
				{
					throw "There are no results to be displayed";
				}
				else if(numWords == 1)
				{
					string firstWord = *goodWords.begin();
					mainIndexes = wordMap[firstWord];
				}
				else
				{
					string firstWord = *goodWords.begin();
					mainIndexes = wordMap[firstWord];

					for(unsigned int i = 1; i < goodWords.size(); i++)
					{
						string otherWord = goodWords[i];
						Set<int> *otherIndexes = wordMap[otherWord];

						mainIndexes = new Set<int>(mainIndexes->setUnion(*otherIndexes));
					}
				}
			}

		}
		else	//single word query
		{
			wordList = parseWords(query);
			if(wordList->size() == 1)
			{
				string firstWord = (*wordList)[0];
				mapIter = wordMap.find(firstWord);
				if(mapIter == wordMap.end())
				{
					throw "There are no results to be displayed";
				}
				mainIndexes = wordMap[firstWord];
			}
			else
			{
				throw "Format error";
			}
		}
		delete wordList;

		processResults(mainIndexes);
	}
	catch(const char* message)
	{
		QMessageBox::warning(NULL,"Error",message);
	}
}

void SearchEngine::buildWebPageMap()
{
	//pagesByName
	for(unsigned int i = 0; i < allPages.size(); i++)
	{
		string fname = allPages[i].filename();
		pagesByName[fname] = &allPages[i];
	}

	//wordMap
	for(unsigned int pageIndex = 0; pageIndex < allPages.size(); pageIndex++)
	{
		Set<string> words = allPages[pageIndex].allWords();
		Set<string>::iterator it = words.begin();

		while(it != words.end())	//while there are still more words on the line
		{
			//this is a new word
			if(wordMap.find(*it) == wordMap.end())
			{
				Set<int> *indexes = new Set<int>();
				indexes->insert(pageIndex);
				wordMap[*it] = indexes;
			}
			else	//this is an existing word
			{
				Set<int> *indexes = wordMap[*it];
				indexes->insert(pageIndex);
			}

			it++;
		}
	}
}

void SearchEngine::tieIncomingLinks()
{
	for(map<string, WebPage*>::iterator it = pagesByName.begin(); it != pagesByName.end(); it++)
	{
		WebPage *currPage = it->second;	//a dereferenced map iterator gives a std::Pair

		Set<string> links_set = currPage->allOutgoingLinks();
		vector<string> links;
		//convert links_set to vector
		for(Set<string>::iterator it_linkSet = links_set.begin(); it_linkSet != links_set.end(); it_linkSet++)
		{
			links.push_back(*it_linkSet);
		}

		for(vector<string>::iterator it_set = links.begin(); it_set != links.end(); it_set++)
		{
			if(pagesByName.find(*it_set) != pagesByName.end())
			{
				WebPage *targetPage = pagesByName[*it_set];
				targetPage->addIncomingLink(currPage->filename());
			}
			else	//not in the map for some reason
			{
				string excepmess = "";
				excepmess += "invalid target page" + *it_set + "\n"; 
				throw excepmess;
			}
		}
	}
}

void SearchEngine::rankPages(Set<int> *pages)
{
	rankInfo.clear();

	//store all pages that satisfy search
	for(Set<int>::iterator it = pages->begin(); it != pages->end(); it++)
	{
		string fname = allPages[*it].filename();
		addFilenameToRankInfoList(fname);
	}

	//expand search to include pages linked to (either direction) to original pages
	for(Set<int>::iterator it = pages->begin(); it != pages->end(); it++)
	{
		string fname = allPages[*it].filename();
		WebPage *page = pagesByName[fname];
		
		Set<string> inLinks = page->allIncomingLinks();
		Set<string> outLinks = page->allOutgoingLinks();		

		//add all pages linking to this particular page
		for(Set<string>::iterator in_it = inLinks.begin(); in_it != inLinks.end(); in_it++)
		{
			string fname = *in_it;
			addFilenameToRankInfoList(fname);
		}
		
		//add all pages linked by this particular page
		for(Set<string>::iterator out_it = outLinks.begin(); out_it != outLinks.end(); out_it++)
		{
			string fname = *out_it;
			addFilenameToRankInfoList(fname);
		}
	}
	
	//initialize all relevant pages' ranks to 1/n
	int expandedCount = rankInfo.size();
	for(int i = 0; i < expandedCount; i++)
	{
		rankInfo[i].pageRank = 1.0 / expandedCount;
		rankInfo[i].temp_pageRank = 0.0;
	}

	//run ranking 30 times
	for(int i = 0; i < 30; i++)
	{
		rankOnce();
	}
}

void SearchEngine::rankOnce()
{
	int resultCount = rankInfo.size();

	for(int i = 0; i < resultCount; i++)
	{
		float outVal;	//for pages that are linked (1-a)
		float allVal;	//for all pages (a)

		WebPage *page = pagesByName[rankInfo[i].filename];

		Set<string> outLinks = page->allOutgoingLinks();
		int outCount = outLinks.size();

		//if there are out links
		if(outCount > 0)
		{
			outVal = ((float) (1 - 0.15) * rankInfo[i].pageRank) / (float) outCount;
			allVal = ((float) 0.15 * rankInfo[i].pageRank) / (float) resultCount;
		}
		else //no out links
		{
			outVal = 0.0;
			allVal = rankInfo[i].pageRank / (float) resultCount;
		}

		for(Set<string>::iterator out_it = outLinks.begin(); out_it != outLinks.end(); out_it++)
		{
			string fname = *out_it;
			int index = findIndexInRankInfoList(fname);

			//if this page is in the rankInfo vector, add outVal to temp rank
			if(index != -1)
			{
				rankInfo[index].temp_pageRank += outVal;
			}
		}

		for(int j = 0; j < resultCount; j++)
		{
			//add allVal to all pages' temp rank
			rankInfo[j].temp_pageRank += allVal;
		}
	}

	//finished going through all once:
	//move new ranks from temp_pageRank to pageRank
	//reset temp_pageRank to 0
	for(int i = 0; i < resultCount; i++)
	{
		rankInfo[i].pageRank = rankInfo[i].temp_pageRank;
		rankInfo[i].temp_pageRank = 0.0;
	}
}

int SearchEngine::findIndexInRankInfoList(string &fname)
{
	bool found = false;
	unsigned int index;

	for(index = 0; !found && (index < rankInfo.size()); index++)
	{
		if(rankInfo[index].filename == fname)
		{
			found = true;
		}
	}

	if(found)
	{
		//is index -1 because it did index++ before checking 
		//loop conditions and quitting
		return index - 1;
	}
	else
	{
		return -1;
	}
}

void SearchEngine::addFilenameToRankInfoList(string &fname)
{
	//if not already in rankInfo
	if(findIndexInRankInfoList(fname) == -1)
	{
		pageRankInfo *item = new pageRankInfo;
		item->filename = fname;
		rankInfo.push_back(*item);
	}
	else
	{
		//intentionally empty
	}
}

void SearchEngine::processResults(Set<int> *indexes)
{
	if(indexes->size() == 0)
	{
		throw "There are no results to be displayed";
	}
	else
	{
		rankPages(indexes);

		displayResults();
	}
}

void SearchEngine::closeAll()
{
	//delete everything in new window
	if(newWin != NULL)
	{
		delete fileContents;
		delete incomingLinks;
		delete outgoingLinks;

		delete ads;
		delete newWin;
	}

	//delete everything in search window
	delete searchBar;
	delete resultsList;
	delete rbuttonByFilename;
	delete rbuttonByRank;

	//print bill to file
	billCompanies();
}

string SearchEngine::trimString(string str)
{
string copy = str;
	char first = copy[0];
	while((first == ' ') || (first == '\t'))
	{
		copy = copy.substr(1);
		first = copy[0];
	}

	char last = copy[copy.size() - 1];
	while((last == ' ') || (last == '\t'))
	{
		copy = copy.substr(0, copy.size() - 1);
		last = copy[copy.size() - 1];
	}

	return copy;
}

void SearchEngine::displayResults()
{
	if(rankInfo.size() > 0)
	{
		RankComp rankComparator;
		FilenameComp filenameComparator;  

		if(rbuttonByFilename->isChecked())
		{
			mergeSort(rankInfo, filenameComparator);
		}
		else
		{
			mergeSort(rankInfo, rankComparator);
		}

		for(unsigned int i = 0; i < rankInfo.size(); i++)
		{
			//add the now sorted things to the QWidget that shows the list of pages
			string fname = rankInfo[i].filename;
			resultsList->addItem(QString::fromStdString(fname));
		}
	}
}	

void SearchEngine::enterSearch()
{
	resultsList->clear();
	QString temp = searchBar->text();
	string query = temp.toStdString();	

	query = trim(query);
	processSearchWord(query);
	
}

void SearchEngine::close()
{
	closeAll();
	exit(0);
}

void SearchEngine::pageSelect(QListWidgetItem * item)
{
cout << "in pageSelect" << endl;
	string fname = (item->text()).toStdString();

	if(pagesByName.find(fname) != pagesByName.end())
	{
cout << "in the if" << endl;
		WebPage *page = pagesByName[fname];

		//change window title
		newWin->setWindowTitle(QString::fromStdString(fname));
cout << "window title set" << endl;
		//fill content area
		fileContents->clear();

		stringstream ss;
		ss << *page;

		string file = ss.str();
		fileContents->setText(QString::fromStdString(file));
cout << "content filled in" << endl;
		//fill incoming link area
		incomingLinks->clear();
		if(!page->allIncomingLinks().empty())
		{
			Set<string> linksIn = page->allIncomingLinks();
			Set<string>::iterator it = linksIn.begin();
			while(it != linksIn.end())
			{
				incomingLinks->addItem(QString::fromStdString(*it));
				it++;
			}
		}
cout << "incoming links done" << endl;
		//fill outgoing link area
		outgoingLinks->clear();
		if(!page->allOutgoingLinks().empty())
		{
			Set<string> linksOut = page->allOutgoingLinks();
			Set<string>::iterator it = linksOut.begin();
			while(it != linksOut.end())
			{
				outgoingLinks->addItem(QString::fromStdString(*it));
				it++;
			}
		}
cout << "outgoing links done" << endl;
		//fill ads area
cout << "begin ad area" << endl;
		ads->clear();
cout << "cleared old ads" << endl;
		if(!adCompanies.empty())
		{
cout << "there are ads potentially there" << endl;
			BidComp bidComp;
			vector<pair<string, double> > *list = new vector<pair<string, double> >;
cout << "created comparator and vector of pairs" << endl;
			//loop thru all words searched for
			for(vector<string>::iterator word_it = wordList->begin(); word_it == wordList->end(); word_it++)
			{
cout << "in loop" << endl;
				string key = (*word_it);
cout << "key = " << key << endl;
				Set<Advertiser*>::iterator it = adCompanies.begin();
cout << "created Set<Advertiser*>::iterator" << endl;
				//go thru all companies
				while(it != adCompanies.end())
				{
					Advertiser *ad = *it;
					//if this company bought an ad on the current keyword
					if(ad->getBid(key) != 0.0)
					{
						pair<string, double> *tempPair = NULL;
						//find if this company already has an ad for this page
						for(vector<pair<string,double> >::iterator it_vec = list->begin(); it_vec != list->end(); it_vec++)
						{
							if(it_vec->first == ad->getName())
							{
cout << "found company" << endl;
								tempPair = &(*it_vec);
							}
						}
						if(tempPair == NULL)
						{
cout << "company didn't already have ad" << endl;
							list->push_back(make_pair(ad->getName(), ad->getBid(key)));
						}
						else 
						{
cout << "company already had ad--figure out higher bid" << endl;
							if(tempPair->second < ad->getBid(key))
							{
cout << "new ad worth more, change stored val" << endl;
								tempPair->second = ad->getBid(key);
							}
						}
						mergeSort(*list,bidComp);
cout << "sorted" << endl;
					}				
					it++;
				}
			}
			list->clear();
			delete [] list;
		}

			ads->clear();
if(!adCompanies.empty())
{
Set<Advertiser*>::iterator it = adCompanies.begin();
while(it != adCompanies.end())
{
ads->addItem(QString::fromStdString((*it)->getName()));
it++;
}
}
	}
	else
	{
		string excepmess = "";
		excepmess += "invalid target page" + fname + "\n"; 
		throw excepmess;
	}
	newWin->show();
}

void SearchEngine::searchResultOrder()
{
	resultsList->clear();
	displayResults();
}

void SearchEngine::billCompanies()
{
	ofstream outfile;
	outfile.open(billFile);

	if(outfile.is_open())
	{
		//copy contents of adCompanies to a vector so it can be sorted
		vector<Advertiser*> *list = new vector<Advertiser*>;
		for(Set<Advertiser*>::iterator it_set = adCompanies.begin(); it_set != adCompanies.end(); it_set++)
		{
			list->push_back(*it_set);
		}

		//sort
		AdAlphaComp adcomp;
		mergeSort(*list,adcomp);

		//print to file in alphabetic order
		for(vector<Advertiser*>::iterator it_vec = list->begin(); it_vec != list->end(); it_vec++)
		{
			string name = (*it_vec)->getName();
			double owed = (*it_vec)->getTotalCharge();

			outfile << name << endl << owed << endl << endl;
		}

		delete [] list;
	
		outfile.close();
	}
	else
	{
		cout << "unable to open billing file " << billFile << endl;
	}
}

void SearchEngine::showAd(QListWidgetItem * item)
{
	string name = (item->text()).toStdString();
	Advertiser comp;

	for(Set<Advertiser*>::iterator it_set = adCompanies.begin(); it_set != adCompanies.end(); it_set++)
	{
		Advertiser *temp = *it_set;
		if(temp->getName() == name)
		{
			comp = *temp;
			break;
		}
	}

	double charge = 0.0;
	for(vector<string>::iterator it = wordList->begin(); it != wordList->end(); it++)
	{
		double temp = comp.getBid(*it);
		if(temp > charge)
		{
			charge = temp;
		}
	}

	comp.addToTotal(charge);
	
	string message = "Thank you for visiting " + name + ".";
	QMessageBox::information(NULL,QString::fromStdString("Advertisement"),QString::fromStdString(message));
}
