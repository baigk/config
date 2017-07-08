#include <iostream>
#include <tuple>

using namespace std;

int maini(const int i) {
	auto a = make_tuple("123","345");

	cout << get<i>(a) << endl;

	return 0;
}

int main() {
	maini(1);
	return 0;
}

