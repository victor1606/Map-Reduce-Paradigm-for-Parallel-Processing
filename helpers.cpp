#include <string>
#include <cmath>

#include "tema1.h"

// Used to sort vector< pair<string, int> > based on
// the second element of the pair
bool comparator(pair<string, int> &x, pair<string, int> &y)
{
	if (x.second < y.second) {
		return 1;
	} else {
		return 0;
	}
}

// Uses binary search to find the bases more efficiently
vector<int> perfect_power_fct(int nr) {
	int exp = 2;
	vector<int> exp_vect;
	
	// 2 ^ MAX_INT_EXPONENT = MAX_INT
	while (exp < MAX_INT_EXPONENT) {
		if (pow(2, exp) <= nr) {
			int right, left = 2;

			for (right = 2; pow(right, exp) <= nr; right = 2 * right) {}

			while (right - left > 1) {
				int middle = (left + right) / 2;

				if (pow(middle, exp) > nr) {
					right = middle;
				} else {
					left = middle;
				}
			}

			if (nr == pow(left, exp)) {
				exp_vect.push_back(exp);
			}

			exp++;
		} else {
			return exp_vect;
		}
	}
	return exp_vect;
}
