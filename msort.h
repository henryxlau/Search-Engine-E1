#ifndef MSORT_H_
#define MSORT_H_
// necessary includes can go here
#include <vector>

using namespace std;

template <class T, class Comparator>
vector<T> merge(vector<T> &left, vector<T> &right, Comparator comp)
{
	vector<T> result;

	unsigned int leftPos = 0;
	unsigned int rightPos = 0;

	while((leftPos < left.size()) || (rightPos < right.size()))
	{
		if(leftPos == left.size())
		{
			result.push_back(right[rightPos]);
			rightPos++;
		}
		else if(rightPos == right.size())
		{
			result.push_back(left[leftPos]);
			leftPos++;
		}
		else if(comp(left[leftPos], right[rightPos]))
		{
			result.push_back(left[leftPos]);
			leftPos++;
		}
		else
		{
			result.push_back(right[rightPos]);
			rightPos++;
		}
	}

	return result;
}

template <class T, class Comparator>
void mergeSort(vector<T> &pages, Comparator comp)
{
	if(pages.size() == 1)
	{
		return;
	}
	else
	{
		vector<T> left;
		vector<T> right;
		unsigned int mid = pages.size() / 2;

		//fill left
		for(unsigned int i = 0; i < mid; i++)
		{
			left.push_back(pages[i]);
		}

		//fill right
		for(unsigned int i = mid; i < pages.size(); i++)
		{
			right.push_back(pages[i]);
		}

		mergeSort(left, comp);
		mergeSort(right, comp);

		pages = merge(left,right,comp);
	}
}

#endif 