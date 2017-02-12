#include <iostream>
#include <queue>
#include <utility> // using std::pair

int test_simple(){
	std::priority_queue<int> pq;
// 	same as: std::priority_queue<int, std::vector<int>, std::less<int>> pq;

	pq.push(30);
	pq.push(100);
	pq.push(25);
	pq.push(40);

	std::cout << "Popping out elements from simple" << std::endl;
	while (!pq.empty()){
		std::cout << pq.top() << std::endl;
		pq.pop();
	}
	std::cout << std::endl;

	return 0;
}

// initialize the pq from a int array
int test_single(){
	int ints[]= {10, 60, 50, 20};
	std::priority_queue<int, std::vector<int>, std::greater<int>> pq(ints, ints + sizeof(ints) / sizeof(int));

	pq.push(30);

	std::cout << "Popping out elements from single" << std::endl;
	while (!pq.empty()){
		std::cout << pq.top() << std::endl;
		pq.pop();
	}
	std::cout << std::endl;

	return 0;
}

class Compare{
private:
	int index;
	bool desc;
public:
	Compare(const int index = 1, const bool desc = true){
		this->index = index;
		this->desc = desc;
	}

	bool operator()(std::pair<int, int> &lv, std::pair<int, int> &rv){
		if(index == 1){
			if(desc){
				// first descending order, default
				return lv.first <= rv.first;
			}
			else{
				// first ascending order
				return lv.first > rv.first;
			}
		}
		else if(index == 2){
			if(desc){
				// second descending order
				return lv.second <= rv.second;
			}
			else{
				// second ascending order
				return lv.second > rv.second;
			}
		}
		else{
			std::cerr << "index " << index << " out of range" << std::endl;
			exit(-1);
		}
	}
};

// use pair<int, int> as the base elem
int test_pair(){
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, Compare> pq(Compare(1, false));

	pq.push(std::make_pair(10, 5));
	pq.push(std::make_pair(8, 6));
	pq.push(std::make_pair(9, 4));

	std::cout << "Popping out elements from pair" << std::endl;
	while (!pq.empty()){
		std::cout << pq.top().first << " " << pq.top().second << std::endl;
		pq.pop();
	}
	std::cout << std::endl;

	return 0;
}

int main(){
	test_simple();

	test_single();

	test_pair();

	return 0;
}

