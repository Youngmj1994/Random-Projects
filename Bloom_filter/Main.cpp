/*
Optimum number of hash functions: The number of hash functions k must be a positive integer. If m is size of bit array and n is number of elements to be inserted, then k can be calculated as :
k= ({m}/{n}) ln 2

Probability of False positivity: Let m be the size of bit array, k be the number of hash functions and n be the number of expected elements to be inserted in the filter, then the probability of false positive p can be calculated as:
P= (1-[1-{1}/{m}]^{kn})^k

finding out how many spaces of m we need, with P being your acceptable level of possibility of false positivity, which will probably .0000001
m = ceil((n * log(p)) / log(1 / pow(2, log(2))));


1. get the number of elements from the database you want to search. 
2. set the optimum amount of bits for the bit array.
3. compute the optimum number of hashes that are required for a minimum level of false positivity
ADD AN ELEMENT TO THE BIT ARRAY
4. somehow with the hashes put a one in the place where the single data point from the database which correspondes with the bit array, should be multiple spots in the bit array
5. turn the bit array at those spots to 1s instead of 0s so that you indicate that that element is probably there. if the element is already 1 thats ok. 
QUERY THE BIT ARRAY
6. take the data point that is being searched for and hash it the correct amount of times and get the positions that corrspond to the number, and if each of those spots are 1's there is a good
probability that it is in the database. 


k is the amount of times each individual piece of data has to be hashed, so like x is the piece of data and it has to be hashed 3 times so k = 3
h1(x) = 4
h2(x) = 1
h3(x) = 5
so bitarray with m=8 [0,1,0,0,1,1,0,0]
also, if you wanted to know if a data point is in there, you check each hashes point, and if even one of the points is 0 then it is definetly not in there.

*/

#include <vector>
#include <math.h>
#include <string>
#include <iostream>
class BloomFilter
{
public:
	BloomFilter() {};
	BloomFilter(int sizeOfDB, double falsePositiveProb = 0) 
	{
		P = falsePositiveProb == 0 ? acceptableFalsePositivty : falsePositiveProb;
		sizeofbitarrayM = calculateM_sizeofbitarray(sizeOfDB);
		hashFunctionsK = calculateK_NumHashes(sizeofbitarrayM, sizeOfDB);
		bitarray.assign(sizeofbitarrayM, 0);

	};
	~BloomFilter() {};

	/// <summary>
	/// takes pre-hashed data and hashes it k times and flips the corresponding bits in the bitarray
	/// </summary>
	/// <param name="data">pre hashed data that will be added to the bitarray</param>
	void add_bf_element(std::string data)
	{
		hashAddHelper(data, hashFunctionsK);
	}

	bool query_bf_element(std::string data)
	{
		return hashQueryHelper(data, hashFunctionsK);
	}
	


private:

	int sizeofbitarrayM = 0;
	std::vector<bool> bitarray;
	short hashFunctionsK = 1;
	const double acceptableFalsePositivty = 0.0000001;
	double P;


	double calculateM_sizeofbitarray(int n)
	{
		return ceil((n * log(P)) / log(1 / pow(2, log(2))));
	}

	short calculateK_NumHashes(int m, int n)
	{
		return short(round((m / n) * log(2)));
	}

	double calculateP_falsePositivityProb(int k, int m, int n)
	{
		return pow(1 - exp(-k / (m / n)), k);
	}

	static unsigned int FNVHash(std::string str, int seed) 
	{
		const unsigned int fnv_prime = 0x811C9DC5;
		unsigned int hash = 0;
		unsigned int i = 0;
		unsigned int len = str.length();

		for (i = 0; i < len; i++)
		{
			hash *= fnv_prime;
			hash ^= (str[i]);
			hash &= seed;
		}

		return hash;
	}

	void hashAddHelper(std::string str, int k)
	{
		for (int i = 1; i < k + 1; i++)
		{
			srand(i);
			int randnum = rand();
			bitarray.at(FNVHash(str, randnum)) = 1;
		}
	}

	bool hashQueryHelper(std::string str, int k)
	{
		for (int i = 1; i < k + 1; i++)
		{
			srand(i);
			int randnum = rand();
			if (bitarray.at(FNVHash(str, randnum)) == 0)
			{
				return false;
			}
			
		}
		return true;
	}
	
};




int main() 
{
	
	std::vector<std::string> db = { "h","he","hel","hell","hello","hello ", "hello y", "hello yo", "hello you", "hello you!" };
	std::vector<std::string> queryDb = { "h","he","hel","hell2","hello111","hello ", "hello y111", "hello yo", "hello you", "hello you!222" };
	BloomFilter bf(db.size());
	for (auto n : db)
		bf.add_bf_element(n);
	for (auto x : queryDb) 
	{
		if (bf.query_bf_element(x))
			std::cout << "There is a good chance that " << x << " is in the Database.\n";
		else
			std::cout << x << " is definetly not in the Database.\n";
	}
	system("PAUSE");
	return 0;
}