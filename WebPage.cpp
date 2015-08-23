#include <iostream>
#include <string>
#include <fstream>

#include "Set.h"
#include "WebPage.h"

using namespace std;

WebPage::WebPage()
{
	
}

WebPage::WebPage(string filename)
{
	ifstream input;
	string line;
	string word;

	input.open(filename.c_str());
	if(input.is_open())
	{
		fname = filename;

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
					addOutgoingLink(linkTarget);

					//remove brackets and target filename
					line.replace(brackets, (closeParen - brackets + 1), "");
					line.replace(openBrack, 1, "");
				}
				brackets = line.find("](");
			}

			lines.insert(lines.end(), line);
			
			//pulling out distinct word
			int lineLength = line.length();
			int wordStart = -1;
			int wordLength = 0;

			for(int i = 0; i < lineLength; i++)
			{
				if(isalnum(line[i]))	//if the ith char in currLine is alphanumeric
				{
					if(wordStart == -1)	//if this is beginning of a word
					{
						wordStart = i;
					}
						
					wordLength++;
				}
				else
				{
					if(wordLength > 0)	//if there is actually a word, add it to distinctWords and clear the string (otherwise do nothing)
					{
						addDistinctWord(line, wordStart, wordLength);
						
						//reset for next word
						wordStart = -1;
						wordLength = 0;
					}
				}
			}
			
			//if end of line is a word, it needs to be added
			if(wordLength > 0)	//if there is actually a word, add it to distinctWords
			{
				addDistinctWord(line, wordStart, wordLength);
			}
		}
	}
	else
	{
		throw "Cannot open file to create WebPage\n";
	}

	input.close();
}

WebPage::~WebPage()		//empty distinctWords and lines
{
	distinctWords.clear();

	lines.clear();

	outLinks.clear();
	inLinks.clear();
}

Set<string> WebPage::allWords() const
{
	return distinctWords;
}

ostream & operator<<(ostream & os, const WebPage & page)
{
	int numLines = page.lines.size();
	for(int i = 0; i < numLines; i++)
	{
		os << page.lines[i] << "\n";
	}

	return os;
}

/* Returns "pointers" to all webpages that this page has links to.
As discussed above, this could be as a set or via an iterator,
and it could be as actual pointers, or as strings,
or possibly other. */
Set<string> WebPage::allOutgoingLinks() const
{
	return outLinks;
}

/* Returns "pointers" to all webpages that link to this page.
Same consideration as previous function. */
Set<string> WebPage::allIncomingLinks() const
{
	return inLinks;
}

/* Returns the filename from which this page was read. */
string WebPage::filename() const
{
	return fname;
}

/* Adds a link from start to the current page
in the current page's storage. */
void WebPage::addIncomingLink(string source)
{
	/*if(inLinks.find(source) == inLinks.end())
	{
		inLinks.push_back(source);
	}*/
	bool find=false;

	for(Set<string>::iterator it = inLinks.begin(); it != inLinks.end(); it++)
	{
		string fname = *it;
		if(fname == source)
		{
			find = true;
			break;
		}
	}

	if (find == false) 
	{
		inLinks.insert(source);
	}   
}

/* Adds a link from the current page to the target
in the current page's storage. */
void WebPage::addOutgoingLink(string target)
{
	/*if(outLinks.find(target) == outLinks.end())
	{
		outLinks.push_back(target);
	}*/
	bool find=false;
	for (Set<string>::iterator it = outLinks.begin(); it != outLinks.end(); it++)
	{
		string fname = *it;
		if (fname == target)
		{
			find = true;
			break;
		}
	}

	if (find==false) 
	{
		outLinks.insert(target);
	} 
}

void WebPage::addDistinctWord(string &line, int start, int length)
{
	//pull those letters out for the word
	string word = line.substr(start, length);

	for(unsigned int i = 0; i < word.length(); i++)	//convert to lowercase
	{
		word[i] = tolower(word[i]);
	}

	
	if(!distinctWords.contains(word))	//this word hasn't been found yet
	{
		distinctWords.insert(word);
	}
}
