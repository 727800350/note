# Templates
## Function templates
```C++
#include <iostream>
using namespace std;

template <class T>
T GetMax (T a, T b){
	T result;
	result = (a > b)? a : b;
	return (result);
}

int main(){
	cout << GetMax<int>(5, 3) << endl;
	cout << GetMax<long>(6, 8) << endl;

	return 0;
}
```

