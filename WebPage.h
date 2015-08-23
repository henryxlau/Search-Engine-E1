#ifndef WEBPAGE_H_
#define WEBPAGE_H_
// required headers go here
#include <string>
#include <vector>
#include "Set.h"

using namespace std;

class WebPage {
public:
	WebPage();    // constructor that creates an empty page
	WebPage(string filename);
	/* constructor that initializes the page from the given file.
	Should throw an exception if the file does not exist
	or has other problems. */
	~WebPage();   // destructor

	Set<string> allWords() const;
	/* Returns a set containing all individual words on the web page. */

	friend ostream & operator<< (ostream & os, const WebPage & page);
	/* Declares an operator we can use to print the web page. */

	Set<string> allOutgoingLinks() const;
	/* Returns "pointers" to all webpages that this page has links to.
	As discussed above, this could be as a set or via an iterator,
	and it could be as actual pointers, or as strings,
	or possibly other. */

	Set<string> allIncomingLinks() const;
	/* Returns "pointers" to all webpages that link to this page.
	Same consideration as previous function. */

	string filename() const;
	/* Returns the filename from which this page was read. */

	/* Based on which overall architecture you choose, you may
	also need/want some or all of the following functions. */

	void addIncomingLink(string start);
	/* Adds a link from start to the current page
	in the current page's storage. */

	void addOutgoingLink(string target);
	/* Adds a link from the current page to the target
	in the current page's storage. */

private:
	// you get to decide what goes here.
	vector<string> lines;
	Set<string> distinctWords;
	
	Set<string> outLinks;	//outLinks and inLinks store the string filename of the target WebPage
	Set<string> inLinks; 	//(since it might refer to a page that hasn't been created yet)
	string fname;

	void addDistinctWord(string &line, int start, int length);
};

#endif
