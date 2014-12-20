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
  int *key;
  char *val;
  GHashTable *hash_table
  = g_hash_table_new_full(g_int_hash,  /* Hash function  */
                          g_str_equal, /* Comparator     */
                          free_key,   /* Key destructor */
                          free_value);  /* Val destructor */
  /* Insert 'k_1' */
  key = malloc(sizeof(int)); *key = 2;
  printf("%d %p\n", *key, key);
  val = strdup("v_1");
  printf("%s %p\n", val, val);

  printf("inserting\n");
  g_hash_table_insert(hash_table, key, val);
  printf("insert finished\n\n");

  /* Insert 'k_1' again using new strings */
  key = malloc(sizeof(int)); *key = 2;
  printf("%d %p\n", *key, key);
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
