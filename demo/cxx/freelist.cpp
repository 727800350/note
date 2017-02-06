#include <iostream>
#include <climits>
#include <cstdlib>
#include <csignal>

using namespace std;

#define FREELIST_MAX_ALLOC	0x1000000
#define FREELIST_INITIAL_ALLOC	16

class freelist{
private:
	unsigned int allocsz; // size of the element in freelist
	unsigned int nalloc; // the capacity
	unsigned int navail; // available
	void **free_entries;
public:
	freelist(int size){
		allocsz = size;
		nalloc = 0;
		navail = 0;
		cout << "element size = " << allocsz << " Bytes" << endl;
		free_entries = NULL;
	}
	int grow();
	void *get();
	void put(void *p);
};

int freelist::grow(){
	size_t i, oldnalloc, need;
	void *p;

	cout << __func__ << ": nalloc = " << nalloc <<", navail = " << navail << endl;

	/* Sanity check */
	if (nalloc > FREELIST_MAX_ALLOC)
		return -1;

	oldnalloc = nalloc;
	if (nalloc == 0)
		nalloc = FREELIST_INITIAL_ALLOC;
	else
		nalloc <<= 1;
	if (nalloc > FREELIST_MAX_ALLOC)
		nalloc = FREELIST_MAX_ALLOC;

	/* Check for integer overflow */
	if(UINT_MAX / nalloc < allocsz || UINT_MAX / nalloc < sizeof(*free_entries)) {
		cout << __func__ << ": integer overflow" << endl;
 resize_fail:
		nalloc = oldnalloc;
		return -1;
	}

	/* Allocate freelist - max size of nalloc */
	need = nalloc * sizeof(*free_entries);
	if ((p = realloc(free_entries, need)) == NULL) {
		cout << __func__  << ": realloc(" << need << ") failed" << endl;
		goto resize_fail;
	}

	/* Allocate the entries */
	free_entries = (void **)p;
	need = (nalloc - oldnalloc) * allocsz;
	if ((p = malloc(need)) == NULL) {
		cout << __func__  << ": malloc(" << need << ") failed" << endl;
		goto resize_fail;
	}

	navail = nalloc - oldnalloc;
	for (i = 0; i < navail; i++)
		free_entries[i] = (u_char *)p + (i * allocsz);
	for (i = navail; i < nalloc; i++)
		free_entries[i] = NULL;

	cout << __func__ << ": done, nalloc = " << nalloc <<", navail = " << navail << endl;

	return 0;
}

void *freelist::get(){
	cout << __func__ << ": nalloc = " << nalloc <<", navail = " << navail << endl;

	void *r;
	if(navail == 0) {
		if(grow() == -1)
			return NULL;
	}

	/* Sanity check */
	if(navail == 0 || navail > FREELIST_MAX_ALLOC || free_entries[navail - 1] == NULL) {
		cout << __func__  << ": invalid navail" << endl;
		raise(SIGSEGV);
	}

	navail--;
	r = free_entries[navail];
	free_entries[navail] = NULL;

	cout << __func__ << ": done, nalloc = " << nalloc <<", navail = " << navail << endl;
	return r;
}

void freelist::put(void *p){
	cout << __func__ << p << endl;
	cout << __func__ << ": nalloc = " << nalloc <<", navail = " << navail << endl;

	/* Sanity check */
	if(navail >= nalloc) {
		cout << __func__  << ": freelist navail >= nalloc" << endl;
		raise(SIGSEGV);
	}
	if (free_entries[navail] != NULL){
		cout << __func__ << ": free_entries[" << (unsigned long )navail << "] != NULL" << endl;
		raise(SIGSEGV);
	}

	free_entries[navail] = p;
	navail++;

	cout << __func__ << ": done, nalloc = " << nalloc <<", navail = " << navail << endl;
}

int main(){
	freelist *fl = new freelist(4);
	cout << "get 1" << endl;
	void *p = fl->get();
	cout << endl;

	cout << "get 2" << endl;
	void *p2 = fl->get();
	cout << endl;

	cout << "put 1" << endl;
	fl->put(p);
	cout << endl;

	cout << "put 2" << endl;
	fl->put(p2);
	return 0;
}
