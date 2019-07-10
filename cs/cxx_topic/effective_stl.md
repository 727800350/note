# container 的种类
- standard STL sequence containers: vector, string, deque, and list
- standard STL associative containers: set, multiset, map and multimap
- standard STL unordered associative containers: `unordered_set`, `unordered_map`
- nonstandard sequence containers: slist and rope. slist is a singly linked list, and rope is essentially a heavy-duty string
- nonstandard associative containers: `hash_set, hash_multiset, hash_map, and hash_multimap`

- `vector<char>` as a replacement for string
- several standard non-STL containers, including arrays, bitset, valarray, stack, queue, and `priority_queue` 

Contiguous-memory containers store their elements in one or more (dynamically allocated) chunks of memory, each chunk holding more than one container element.
If a new element is inserted or an existing element is erased, other elements in the same memory chunk have to be shifted up or down to make room for the new element or
to fill the space formerly occupied by the erased element. This kind of movement affects both performance and exception safety.
The standard contiguous-memory containers are vector, string, and deque.

Node-based containers store only a single element per chunk of (dynamically allocated) memory.
Insertion or erasure of a container element affects only pointers to nodes, not the contents of the nodes themselves, so element values need not be moved when something is inserted or erased.

In general, insertions or erasures on contiguous-memory containers may invalidate all iterators, pointers, and ref- erences into the container.
While insertions and erasures on node-based containers never invalidate iterators, pointers, or references (unless they point to an element you are erasing).
That is why the form of erase taking an iterator returns a new iterator when invoked on a sequence container, but it returns nothing when invoked on an associative container.

In the presence of inheritance, of course, copying leads to slicing.
That is, if you create a container of base class objects and you try to insert derived class objects into it,
the derivedness of objects will be removed as the objects are copied(via the base class copy contructor) into the container.
子类是可以作为base class 的copy constructor 的参数的. 所以这种情况下就需要用到pointer 或者 smart pointers.

empty is a contant-time operation for all standard containers, but for some list implementations, size takes linear time.
So we should call empty instead of checking size() against zero.

# range operations
prefer range member functions to their single-element counterparts.
Almost all uses of copy where the destination range is specified using an insert iterator should be replaced with calls to range member functions.

- given two vectors, v1 and v2, what's the easiest way to make v1's contents be the same as the second half of v2's?
  ```C++
  // first one is better
  v1.assign(v2.begin() + v2.size() / 2, v2.end());

  v1.clear();
  std::copy(v2.begin() + v2.size() / 2; v2.end(), std::back_inserter(v1));

  v1.clear();
  for (auto&& it = v2.begin() + v2.size() / 2; it != v2.end(); ++it) {
    v1.push_back(*it);
  }
  ```
- insert half v2 into v1
  ```C++
  v1.insert(v1.end(), v2.begin() + v2.size() / 2, v2.end());

  std::copy(v2.begin() + v2.size() / 2; v2.end(), std::back_inserter(v1));
  ```

memmber functions supports ranges

- range construnction. All standard containers offer a constructor of this form: `container::container(InputIterator begin, InputIterator end)`
- range insertion.
  - All standard sequence containers offer this form of insert: `void container::insert(iterator position, InputIterator begin, InputIterator end)`
  - Associative containers use their comparison function to determiner where elements go, so they use `void container::insert(InputIterator begin, InputIterator end)`
- range erasure. Every standard container offers a range form of erase, but return type differs
  - sequence containers: `iterator container::erase(iterator begin, iterator end)`
  - associative containsers: `void container::erase(iterator begin, iterator end)`
- range assignment. All standard sequence containers offer a range form of assign: `void container::assign(InputIterator begin, InputIterator end)`

# erasing options
Suppose you have a standard STL container, c, that holds ints. `Container<int> c;`
and you'd like to get rid of all the objects in c with the value 1963.

- for contiguous-memory container(std::vector, std::deque, std::string), the best approach is the erase-remove idiom:
  - `c.erase(std::remove(c.begin(), c.end(), 1963), c.end())`
  - `c.erase(std::remove_if(c.begin(), c.end(), badValue), c.end())`, badVaue is a predicate
- for std::list, erase-remove idiom is ok, but list member function is more efficient, `c.remove(1963)` and `c.remove_if(badValue)`
- for associative container(std::set, std::map), `c.erase(1963)`, 
  ```C++
  // for predicate, we must write a loop
  for (auto&& it = c.begin(); it != c.end();) {
    if (badValue(*it)) {
      // we pass it's old value to erase, but we also increment it itself before erase begins executing, that is exactly what we want
      // 在C++ 11 之后, 可以用 it = c.erase(it)
      // note: for contiguous-memory container, invoking erase invalidates all iterators beyond the erased element, in our case, that includes all ietrators beyond it, so c.erase(it++) does not work
      c.erase(it++);
    }
    else {
      ++it;
    }
  }
  ```

```C++
template<class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value);
```
Because the only way to eliminate an element from a container is to invoke a member function on that container,
and because remove cannot know the container holding the elements on which it is operating, it is not possible for remove to eliminate elements from a container.
That explains the otherwise baffling observation that removeing elements from a container never changes the number of elements in the container.

Very briefly, remove moves elements in the range it's given until all the "unremoved" elements are at the front of the range (in the same relative order they were in originally).
It returns an iterator pointing one past the last "unremoved" element. This return value is the "new logical end" of the range.

remove doesn't change the order of the elements in a range so that all the "removed" ones are at the end, it arranges for all the "unremoved" values to be at the beginning.
```
before: 1(v.begin()), 2, 3, 99, 5, 99, 7, 8, 9, 99, v.end()
after auto it = remove(c.begin(), c.end(), 99): 1(v.begin()), 2, 3, 5, 7, 8, 9, 8(it), 9, 99, v.end()
```
As you can see, two of the "99" values that used to exist in v are no longer there, while one "99" remains.
In general, after calling remove, the values removed from the range may or may not continue to exist in the range. Most people find this surprising, but why?
You asked remove to get rid of some values, so it did. You didn't ask it to put the removed values in a special place where you could get at them later, so it didn't.
What's the problem? (If you don't want to lose any values, you should probably be calling partition instead of remove.
remove's behavior sounds spiteful, but it's simply a fallout of the way the algorithm operates.
Internally, remove walks down the range, overwriting values that are to be "removed" with later values that are to be retained.
The overwriting is accomplished by making assignments to the elements holding the values to be overwritten.

# use reserve to avoid unncessary reallocations
- `resize(size_t n)`: forces the container to change to n the number of elements it holds. After the call to resize, size will return n.
  If n is smaller than the current size, elements at the end will be destroyed.
  if n is larger than the current size, new default-constructed elements will added to the end of the container.
  if n is larger than the current capacity, a reallocation will take place before the elements are added.
- `reserve(size_t n)`: forces the container to change its capacity to at least n.

# pass vector and string data to legacy C APIs
- sizeof(string) depends on the implementation of string
- for non-empty vector v, `&v[0]` is a pointer to the data in v that can be viewed as an array

The approach to getting a pointer to container data that works for vectors isn't reliable for strings, because
(1) the data for strings are not guaranteed to be stored in contiguous memory, and
(2) the internal representation of a string is not guaranteed to end with a null character.
This explains the existence of the string member function `c_str`, which returns a pointer to the value of the string in a form designed for C.

If you have a vector that you'd like to initialize with elements from a C API,
you can take advantage of the underlying layout compatibility of vectors and arrays by passing to the API the storage for the vector's elements:
```C++
// C API: this function takes a pointer to an array of at most arraySize doubles and writes data to it.
// It returns the number of doubles written, which is never more than maxNumDoubles.
size_t fillArray(double* pArray, size_t arraySize);
vector<double> vd(maxNumDoubles); // create a vector whose size is maxNumDoubles
vd.resize(fillArray(&vd[0], vd.size()));
```
This technique works only for vectors, because only vectors are guaranteed to have the same underlying memory layout as arrays.
If you want to initialize a string with data from a C API, however, you can do it easily enough.
Just have the API put the data into a `vector<char>`, then copy the data from the vector to the string:
```C++
size_t fillString(char* pArray, size_t arraySize);
vector<char> vc(maxNumChars); // create a vector whose size is maxNumChars
size_t charsWritten = fillString(&vc[0], vc.size()); // have fillString write
string s(vc.begin(), vc.begin() + charsWritten);
```

# use the swap trick to trim excess capacity
```C++
std::vector<int> v;
// suppose v.capacity() >> v.size()
std::vector<int>(v).swap(v);
```
vector's copy constructor allocates only as much memory as is needed for the elements being copied.
so the created temporary vector has no excess capacity.

The same trick is applicable to string
```C++
std::string s;
std::string(s).swap(s);
```

C++ 11 offers `std::string::shrink_to_fit` and `std::vector::shrink_to_fit` to do the same thing, swap trick is no longer needed.

```C++
std::vector<int>().swap(v);  // clear v and minimize its capacity
std::string().swap(s);  // clear s and minimize its capacity
```

# understand the difference between equality and equivalence
the std::find algorithm and set's insert member function are representative of many functions that must determine whether two values are the same. Yet they do it in defferent ways,

- `std::find`'s definition of the "the same" is equality, which is based on the `operator==`.
- `std::set::insert` and `std::set::find`'s definition of "the same" is equivalence, which is usually based on operators.

Equivalence is based on the relative ordering of object values in a sorted range.
Two objects x and y have equivalent values with respect to the sort order used by an associative container c if neither precedes the other in c's sort order. That is:
```C++
!(x < y) && !(y < x)  // suppose we are using the default comparion function <
```
In the genearal case, the comparison function for an associative container isn't `operator<`, it is a user-defined predicate.
Every standard associative container makes its sorting predicate available through its `key_comp` member function,
so two objects x and y have equivalent values with respect to an associative containers c's soring criterion if the following evalutes to true:
```C++
!c.key_comp()(x, y) && !c.key_comp()(y, x)
```

Each of the set template's three parameters is a type, so we can not pass a function as comparison type.
set doesn't want a function, it wants a type that it can internally instantiate to create a function.

Always have comparison functions return false for equal values.

Equal values are, by definition, not equivalent! Equal values never precede one another!

# avoid in-place key modification in set and multiset
Like all the standard associative containers, set and multiset keep elements in sorted order, and the proper behavior of these containers id dependent on their remaining sorted.
If you change the key of an elemnent, the new element might not be in the corrent location.

The elements in an object of type `std::map<K, V>` or `std::multimap<K, V>` are `std::pair<const K, V>`.
But the elements in an object of type `std::set<K>` or `std::multiset<K>` are `K`.
虽然我们认为两者应该都是一样, 但是标准委员会就是这样认为的.

For std::set and std::multiset, we could use `const_cast<K&>(*iterator)` to access the modification member functions.
But for std::map and std::multimap, const cast won't work, because the first component of `std::pair<const K, V>` is defined to be const, meaning that
that attemps to cast away its constness are undefined.
TODO: 用 std::map 测试之后是ok 的, 应该跟实现有关.

