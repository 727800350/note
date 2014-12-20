#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_key(gpointer key){
  printf("freeing key: %d %p\n", *(int *)key, key);
  free(key);
}

void free_value(gpointer value){
  printf("freeing value: %s %p\n", (char *)value, value);
  free(value);
}

int main(void){
  unsigned int *key;
  char *val;
  GHashTable *hash_table
  = g_hash_table_new_full(g_int_hash,  /* Hash function  */
                          g_int_equal, /* Comparator     */

//   = g_hash_table_new_full(g_int64_hash, 
//                           g_int64_equal, 

//   = g_hash_table_new_full(g_direct_hash,
//                           g_direct_equal,

//   = g_hash_table_new_full(g_direct_hash, 
//                           g_int_equal,
                          free_key,   /* Key destructor */
                          free_value);  /* Val destructor */

/* 通过用不同的hash函数来测试, 发现及时uint 表示的数据有些不再int范围内, 
 * 但是由于两个同样的很大的uint转换为负数的int时, 指针所指向的值也是一样的, 所以是可以用int作为hash函数, 
 * 但是前提是数据全部是正的数据, 即要保持unit ~ int 是一一对应的关系
 * 
 * 但是使用direct就不行了, 因为虽然指针所指向的值一样, 但是由于指针不一样, 所以不能用作comparator
 *
 * 使用int64也是可以的
 */

  /* Insert 'k_1' */
  key = malloc(sizeof(unsigned int)); *key = G_MAXINT + 1;
  printf("%u %p\n", *key, key);
  val = strdup("v_1");
  printf("%s %p\n", val, val);

  printf("inserting\n");
  g_hash_table_insert(hash_table, key, val);
  printf("insert finished\n\n");

  /* Insert 'k_1' again using new strings */
  key = malloc(sizeof(unsigned int)); *key = G_MAXINT + 1;
  printf("%u %p\n", *key, key);
  val = strdup("new_v_1");
  printf("%s %p\n", val, val);

  printf("inserting\n");
  g_hash_table_insert(hash_table, key, val);
  printf("insert finished\n");

  g_hash_table_unref(hash_table);

  return 0;
}

/***
[eric@fedora glib]$ ./a.out
2 0x1164e70
v_1 0x1164e90
inserting
insert finished

2 0x1164f00
new_v_1 0x1164f20
inserting
freeing key: 2 0x1164f00
freeing value: v_1 0x1164e90
insert finished
freeing key: 2 0x1164e70
freeing value: new_v_1 0x1164f20

You will see that if you create a 'new' key identical to an existing key, 
and try to insert a corresponding 'new' value, 
then the 'insert' operation automatically destroys 
the 'new' key (because it is the same as the existing one), 
as well as the old value (because it has to be replaced with the new one).
***/
