#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class base
{
	public:
		int i{80};
};

class derived : public base
{
	public:
		derived(int m) : j(m){cout << m << endl;} 
		int j{100};
		derived(const derived &c) { cout << "mm" << c.j << endl;}
};

using PtrBase = std::shared_ptr<base>;
using Ptrderived = std::shared_ptr<derived>;

int value = 1;
PtrBase createObj() {
	auto t = make_shared<derived>(value++);
	return t;
}

std::vector<PtrBase> getObj() {
	std::vector<PtrBase> p;

	for (int i = 0; i< 6; i++){
	    auto m = createObj();

		p.push_back(m);
	}

	return p;
}

int main() {
	std::vector<PtrBase> p = getObj();

	for (auto & i : p){
		cout << i->i << endl;
	}

	return 0;
}

