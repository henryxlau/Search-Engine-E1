//includes
#include <string>
#include <vector>
#include <map>
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QTextEdit>
#include <QRadioButton>

#include "Set.h"
#include "WebPage.h"
#include "Advertiser.h"

struct pageRankInfo
{
	string filename;
	float pageRank;
	float temp_pageRank;
};

struct RankComp
{
	bool operator()(const pageRankInfo &page1, const pageRankInfo &page2)
	{
		return (page1.pageRank > page2.pageRank);
	}
};

struct FilenameComp
{
	bool operator()(const pageRankInfo &page1, const pageRankInfo &page2)
	{
		return (page1.filename < page2.filename);
	}
};

struct BidComp
{
	bool operator()(pair<string,double> company1, pair<string,double> company2)
	{
		if(company1.second == company2.second)
		{
			return (company1.first < company2.first);
		}
		else
		{
			return (company1.second > company2.second);
		}
	}
};

struct AdAlphaComp
{
	bool operator()(Advertiser *company1, Advertiser *company2)
	{
		return (company1->getName() < company2->getName());
	}
};

class SearchEngine : public QWidget
{
	Q_OBJECT
	
public:
	SearchEngine(QWidget *parent, char* webpages, char* advertisers, char* bill);
	~SearchEngine();

protected:
	void readFile(char* filename);
	string& trim(string &str);
	string& removeSymbols(string &line);
	vector<string>* parseWords(string &input);
	void processSearchWord(string &query);
	void buildWebPageMap();
	void tieIncomingLinks();

	void processResults(Set<int> *indexes);
	void displayResults();

	void rankPages(Set<int> *pageIndexes);
	void rankOnce();

	int findIndexInRankInfoList(string &fname);
	void addFilenameToRankInfoList(string &fname);

	void readAdvertisers(char* filename);
	void closeAll();

	string trimString(string str);
	void billCompanies();

private slots:
	void enterSearch();
	void close();
	void pageSelect(QListWidgetItem * item);
	void searchResultOrder();
	void showAd(QListWidgetItem * item);

private:
	vector<string> *wordList;
	map<string, Set<int>* > wordMap;
	vector<WebPage> allPages;
	map<string, WebPage*> pagesByName;
	vector<pageRankInfo> rankInfo;
	map<string, vector<Advertiser*>* > bidders;
	Set<Advertiser*> adCompanies;
	char* billFile;

	QLineEdit *searchBar;
	QListWidget *resultsList;
	QWidget *newWin;
	QTextEdit *fileContents;
	QListWidget *incomingLinks;
	QListWidget *outgoingLinks;
	QRadioButton *rbuttonByFilename;
	QRadioButton *rbuttonByRank;
	QListWidget *ads;
};
