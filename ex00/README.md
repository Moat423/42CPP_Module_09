### Comparator

Making a comparator for classes or types that don't have one enables you to use some STL algorithms on it.

[making a comparator](https://www.geeksforgeeks.org/cpp/comparator-in-cpp/)

	bool tmComparatorLess::operator()(const std::tm &a, const std::tm &b) const

using this her for the map as the comparator:

	std::map<std::tm, float, tmComparatorLess> _m;

or i just avoid this completely, and convert it to time_t (which is seconds since epoch) and store that as a key in the map. That seems smarter.

```C++
void	BitcoinExchange::writeOutput(std::tm time, float value)
{
	char	timeString[11];
	float	result;
	time_t	inputTime = (std::mktime(&time));
	std::map<std::time_t, float>::iterator	it;
	strftime(timeString, sizeof(timeString), "%Y-%m-%d", &time);
	it = _m.upper_bound(inputTime);
	if (it == _m.begin())
	{
		std::cerr << "Error: no exchange rate available for " << timeString << std::endl;
		return ;
	}
	--it;
	result = value * (it->second);
	std::cout << timeString << " => " << value << " = " << result << std::endl;
}
```
a map is something were it's really easy to get the biggest element.
it is a balanced binary tree like such:
      5
     / \
    3   8
   / \ / \
  1  4 6  9

and therefore it implements functions, that search for the biggest or the biggest element before the value.

lower_bound(k): Returns an iterator to the first element not less than key k (i.e., ≥ k)
that means the returned value is bigger or equal.
upper_bound(k): Returns an iterator to the first element greater than key k (i.e., > k)
that means the returned value is bigger exclusively.

so to simplify my program logic I use upper bound and then go one back if possible and i should be at the one that is lower, or equal.
