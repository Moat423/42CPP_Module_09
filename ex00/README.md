### Comparator

Making a comparator for classes or types that don't have one enables you to use some STL algorithms on it.

[making a comparator](https://www.geeksforgeeks.org/cpp/comparator-in-cpp/)

	bool tmComparatorLess::operator()(const std::tm &a, const std::tm &b) const

using this her for the map as the comparator:

	std::map<std::tm, float, tmComparatorLess> _m;

or i just avoid this completely, and convert it to time_t (which is seconds since epoch) and store that as a key in the map. That seems smarter.
