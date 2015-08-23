#ifndef SET_IMPL__H_
#define SET_IMPL__H_

//includes

//consstructors/destructor
template <class T>
Set<T>::Set()	// constructor for an empty set
{
	
}

/* Returns true if the set contains the item, and false otherwise. */
template <class T>
bool Set<T>::contains(const T & item) const
{
	if(this->find(item) == this->end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

/* Returns the intersection of the current set with other.
That is, returns the set of all items that are both in this
and in other. */
template <class T>
Set<T> Set<T>::setIntersection(const Set<T> & other) const
{
	Set<T> *newSet = new Set<T>();

	for(typename set<T>::iterator myIter = this->begin(); myIter != this->end(); myIter++)
	{

		if(other.contains(*myIter))
		{
			newSet->insert(*myIter);
		}
	}

	return *newSet;
}

/* Returns the union of the current set with other.
That is, returns the set of all items that are in this set
or in other (or both).
The resulting set should not contain duplicates. */
template <class T>
Set<T> Set<T>::setUnion(const Set<T> & other) const
{
	Set<T> *newSet = new Set<T>();

	for(typename set<T>::iterator myIter = this->begin(); myIter != this->end(); myIter++)
	{
		newSet->insert(*myIter);
	}

	for(typename set<T>::iterator myIter = other.begin(); myIter != other.end(); myIter++)
	{
		if(!newSet->contains(*myIter))
		{
			newSet->insert(*myIter);
		}
	}

	return *newSet;
}

#endif