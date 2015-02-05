#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <apr.h>
#include <apr_pools.h>
#include <apr_hash.h>

void test_string(apr_pool_t *mp, apr_hash_t *h){
	char *r;

	int len = strlen("key");
    apr_hash_set(h, "key", len, "value");
    r = apr_hash_get(h, "key", len);
	printf("%s\n", r);

    apr_hash_set(h, "key", APR_HASH_KEY_STRING, "value");
    r = apr_hash_get(h, "key", APR_HASH_KEY_STRING);
	printf("%s\n", r);

    apr_hash_set(h, "key", APR_HASH_KEY_STRING, "new");
    r = apr_hash_get(h, "key", APR_HASH_KEY_STRING);
	printf("%s\n", r);

    apr_hash_set(h, "key2", APR_HASH_KEY_STRING, "key2");
    r = apr_hash_get(h, "key2", APR_HASH_KEY_STRING);
	printf("%s\n", r);

    apr_hash_index_t *hi;
    void *val, *key;
	int count = 0;
    for (hi = apr_hash_first(mp, h); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, (void*)&key, NULL, &val);
		printf("%s -> %s\n", (char *)key, (char *)val);
        count++;
    }
	printf("elements: %d\n", count);

}

void test_integer(apr_pool_t *mp, apr_hash_t *h){
	char *r;
	int i = 1;
	int *p = &i;
	apr_hash_set(h, p, sizeof(p), "1");
    r = apr_hash_get(h, p,  sizeof(p));
// 	printf("%d -> %s\n",*p, r);

	i = 2;
	apr_hash_set(h, p, sizeof(p), "2");
    r = apr_hash_get(h, p,  sizeof(p));
// 	printf("%d -> %s\n",*p, r);

	apr_hash_set(h, p, sizeof(p), "3");
    r = apr_hash_get(h, p,  sizeof(p));
// 	printf("%d -> %s\n",*p, r);

	apr_hash_set(h, p, sizeof(p), "3");
    r = apr_hash_get(h, p,  sizeof(p));
// 	printf("%d -> %s\n",*p, r);

    apr_hash_index_t *hi;
    void *val, *key;
	int count = 0;
    for (hi = apr_hash_first(mp, h); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, (void*)&key, NULL, &val);
		printf("%d -> %s\n", *(int *)key, (char *)val);
        count++;
    }
	printf("elements: %d\n", count);
}

char* int2str(int *p, char *addr){
	*p = htonl(*p);
	inet_ntop(2, p, addr, sizeof(addr));
// 	printf("%s.\n", addr);
	return strdup(addr);
}

void test_integer_string(apr_pool_t *mp, apr_hash_t *h){
	char *r;
	char *p;
	int i = 1;
	char addr[64];

	p = int2str(&i, addr);
	apr_hash_set(h, p, sizeof(p), "1");
    r = apr_hash_get(h, p,  sizeof(p));
// 	printf("%s -> %s\n",p, r);

	i = 2;
	p = int2str(&i, addr);
	apr_hash_set(h, p, sizeof(p), "2");
    r = apr_hash_get(h, p,  sizeof(p));
// 	printf("%s -> %s\n",p, r);

	i = 3;
	p = int2str(&i, addr);
	apr_hash_set(h, p, sizeof(p), "3");
    r = apr_hash_get(h, p,  sizeof(p));
// 	printf("%s -> %s\n",p, r);

	apr_hash_set(h, p, sizeof(p), "3");
    r = apr_hash_get(h, p,  sizeof(p));

	apr_hash_set(h, p, sizeof(p), "4");
    r = apr_hash_get(h, p,  sizeof(p));

    apr_hash_index_t *hi;
    void *val, *key;
	printf("elements: %d\n", apr_hash_count(h));
    for (hi = apr_hash_first(mp, h); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, (void*)&key, NULL, &val);
		printf("%s -> %s\n", (char *)key, (char *)val);
    }
}

#define size 256
struct dns{
	u_int32_t ip_n;
	char dname[size];
};

void print(apr_pool_t *mp, apr_hash_t *h){
	if(h == NULL){
		printf("hash table is NULL\n");
		return ;
	}
    apr_hash_index_t *hi;
	char *key = NULL;
	struct dns *val = NULL;
	printf("elements: %d\n", apr_hash_count(h));
    for (hi = apr_hash_first(mp, h); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, (const void **)&key, NULL, (void **)&val);
		printf("%s -> {%d, %s}\n", key, val->ip_n, val->dname);
    }
	if(hi == NULL){
		printf("hi is null");
	}
	printf("\n");
}

void clear_hash(apr_pool_t *mp, apr_hash_t *h){
    apr_hash_index_t *hi;
	char *key = NULL;
	struct dns *val = NULL;
	printf("before clear elements: %d\n", apr_hash_count(h));
    for (hi = apr_hash_first(mp, h); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, (const void **)&key, NULL, (void **)&val);
		if(key){
			free(key);
		}
		if(val){
			free(val);
		}
    }
	apr_hash_clear(h);
	h = NULL;
	print(mp, h);
}

char *int2str2(u_int32_t n){
	struct in_addr in;
	in.s_addr = htonl(n);
	return strdup(inet_ntoa(in));
}

void test_dns(apr_pool_t *mp, apr_hash_t *h){
	char *ip_a;
	struct dns *d;
	
	d = (struct dns *)malloc(sizeof(struct dns));
	d->ip_n = 1;
	memcpy(d->dname, "www.baidu.com", sizeof("www.baidu.com"));
	ip_a = int2str2(d->ip_n);
	apr_hash_set(h, ip_a, APR_HASH_KEY_STRING, d);
	print(mp, h);

	d = (struct dns *)malloc(sizeof(struct dns));
	d->ip_n = 2;
	memcpy(d->dname, "www.tencent.com", sizeof("www.tencent.com"));
	ip_a = int2str2(d->ip_n);
	apr_hash_set(h, ip_a, APR_HASH_KEY_STRING, d);
	print(mp, h);

	d = (struct dns *)malloc(sizeof(struct dns));
	d->ip_n = 2;
	memcpy(d->dname, "www.tencent.com", sizeof("www.tencent.com"));
	ip_a = int2str2(d->ip_n);
	apr_hash_set(h, ip_a, APR_HASH_KEY_STRING, d);
	print(mp, h);

	d = (struct dns *)malloc(sizeof(struct dns));
	d->ip_n = 2;
	memcpy(d->dname, "www.google.com", sizeof("www.google.com"));
	ip_a = int2str2(d->ip_n);
	apr_hash_set(h, ip_a, APR_HASH_KEY_STRING, d);
	print(mp, h);

	d = (struct dns *)malloc(sizeof(struct dns));
	d->ip_n = 3;
	memcpy(d->dname, "www.google.com", sizeof("www.google.com"));
	ip_a = int2str2(d->ip_n);
	apr_hash_set(h, ip_a, APR_HASH_KEY_STRING, d);
	print(mp, h);
	
	clear_hash(mp, h);
}

int main(){
	apr_pool_t *mp = NULL;
	apr_initialize();
	apr_pool_create(&mp, NULL);

    apr_hash_t *h = NULL;
    h = apr_hash_make(mp);
	
// 	test_string(mp, h);
// 	test_integer(mp, h);
// 	test_integer_string(mp, h);
	test_dns(mp, h);

	apr_pool_destroy(mp);
	apr_terminate();
	return 0;
}
