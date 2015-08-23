#ifndef SET_H_
#define SET_H_
// necessary includes can go here
//#include "List.h"
#include <set>

using namespace std;

template <class T>
class Set : public set<T>
{
public:
	Set();                     // constructor for an empty set

	bool contains(const T & item) const;
	/* Returns true if the set contains the item, and false otherwise. */

	Set<T> setIntersection(const Set<T> & other) const;
	/* Returns the intersection of the current set with other.
	That is, returns the set of all items that are both in this
	and in other. */

	Set<T> setUnion(const Set<T> & other) const;
	/* Returns the union of the current set with other.
	That is, returns the set of all items that are in this set
	or in other (or both).
	The resulting set should not contain duplicates. */

};

#include "SetImpl.h"
#endif 