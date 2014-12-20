#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_data (gpointer data){
  printf ("freeing: %s %p\n", (char *) data, data);
  free (data);
}

int main (void){
  char *key, *val;
  GHashTable *hash_table
  = g_hash_table_new_full (g_str_hash,  /* Hash function  */
                           g_str_equal, /* Comparator     */
                           free_data,   /* Key destructor */
                           free_data);  /* Val destructor */
  /* Insert 'k_1' */
  key = strdup("k_1");
  printf ("%s %p\n", key, key);
  val = strdup("v_1");
  printf ("%s %p\n", val, val);

  printf ("inserting\n");
  g_hash_table_insert (hash_table, key, val);
  printf ("insert finished\n\n");

  /* Insert 'k_1' again using new strings */
  key = strdup("k_1");
  printf ("%s %p\n", key, key);
  val = strdup("new_v_1");
  printf ("%s %p\n", val, val);

  printf ("inserting\n");
  g_hash_table_insert (hash_table, key, val);
  printf ("insert finished\n");

  g_hash_table_unref (hash_table);

  return 0;
}

/***
[eric@fedora glib]$ ./a.out
k_1 0x1829e70
v_1 0x1829e90
inserting
insert finished

k_1 0x1829f00
new_v_1 0x1829f20
inserting
freeing: k_1 0x1829f00
freeing: v_1 0x1829e90
insert finished
freeing: k_1 0x1829e70
freeing: new_v_1 0x1829f20

You will see that if you create a 'new' key identical to an existing key, 
and try to insert a corresponding 'new' value, 
then the 'insert' operation automatically destroys 
the 'new' key (because it is the same as the existing one), 
as well as the old value (because it has to be replaced with the new one).
***/
