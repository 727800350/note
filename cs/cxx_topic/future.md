# [std::future](http://www.cplusplus.com/reference/future/future/future)
std::future提供了一种访问异步操作结果的机制. 从字面意思看, 它表示未来, 通常一个异步操作我们是不能马上就获取操作结果的, 只能在未来某个时候获取.

获取future结果有三种方式:

- `get()`: Returns the value stored in the shared state (or throws its exception) when the shared state is ready.
	If the shared state is not yet ready (i.e., the provider has not yet set its value or exception), the function blocks the calling thread and waits until it is ready.
	一旦被执行过 get, 这个future 就会被置为invalid, 也就是说get 只能调用一次.
- `valid()`: Returns whether the future object is currently associated with a shared state.

- `std::future` 不能被copy, 只能通过 std::ref 或者 std::move 的方式作为参数
- `std::shared_future` 可以被copy, 且可以被get 多次

# [std::promise](http://www.cplusplus.com/reference/future/promise)
A promise is an object that can store a value of type T to be retrieved by a future object (possibly in another thread), offering a synchronization point.

This shared state can be associated to a future object by calling member `get_future`. After the call, both objects share the same shared state:

- The promise object is the asynchronous provider and is expected to set a value for the shared state at some point.
- The future object is an asynchronous return object that can retrieve the value of the shared state, waiting for it to be ready, if necessary.

```C++
int factorial(std::future<int> &future){
	int res = 1;
	int n = future.get();
	for(int i = n; i > 1; --i){
		res *= i;
	}
	return res;
}

std::promise<int> promise;
std::future<int> future = promise.get_future();
std::future<int> fu = std::async(std::launch::async, factorial, std::ref(future));

// do xxx else

promise.set_value(4);
LOG(INFO) << fu.get();
```

如果一个promise 要被多个future 使用, 则需要使用 `std::shared_future`
```C++
int factorial(std::shared_future<int> future){
	int res = 1;
	int n = future.get();
	for(int i = n; i > 1; --i){
		res *= i;
	}
	return res;
}

std::promise<int> promise;
std::future<int> f = promise.get_future();
std::shared_future<int> sf = f.share();
std::vector<std::future<int>> vec;
for(int i = 0; i < 5; ++i){
	std::future<int> future = std::async(std::launch::async, factorial, sf);
	vec.push_back(std::move(future));
}

promise.set_value(4);

for(int i = 0; i < 5; ++i){
	LOG(INFO) << vec[i].get();
}
```

# [std::packaged_task](http://www.cplusplus.com/reference/future/packaged_task/packaged_task)
```C++
std::packaged_task<int(int, int)> task([](int a, int b){ return a - b; });
std::future<int> future = task.get_future();
/* 这里用 std::ref 或者 std::move 都可以
 * 如果用std::move, 则必须在之前先调用get_future(), 因为一旦被std::move, get_future() 就不能用了
 */
std::thread th(std::move(task), 10, 2);
int res = future.get();
LOG(INFO) << "result: " << res; // output 8
th.join();
```

# [std::async](http://www.cplusplus.com/reference/future/async)
std::async大概的工作过程: 先将异步操作用std::packaged_task包装起来, 然后将异步操作的结果放到std::promise中, 这个过程就是创造未来的过程. 外面再通过future.get/wait来获取这个未来的结果.
可以说,std::async帮我们将std::future, std::promise和std::packaged_task 三者结合了起来.

```C++
std::future<int> future = std::async(std::launch::async, [](int x){
	return x + 1;
}, 10);
LOG(INFO) << future.get(); // output 11
```

- `std::launch::async`: 立刻创建一个新的 thread 来执行fun
- `std::launch::deferred`: 在调用future.get() 或者 future.wait() 的时候才创建新的thread 来执行 fun
- fun 可以为A pointer to function, pointer to member, or any kind of move-constructible function object (i.e., an object whose class defines operator(), including closures and function objects)

