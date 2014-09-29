#include <stdio.h>
#include <glib.h>

void print(GSList *gl){
	GSList *p = NULL;
	p = gl;
	while((p = g_slist_next(p)) != NULL){
		printf("%s\n", p->data);
	}
}
int main(){
	GSList *gl = g_slist_alloc();
	gl = g_slist_append(gl, "first");
	gl = g_slist_append(gl, "second");
	print(gl);

	GSList *gl2 = g_slist_alloc();
	gl2 = g_slist_prepend(gl2, "first");
	gl2 = g_slist_prepend(gl2, "second");
	gl2 = g_slist_reverse(gl2);
	print(gl2);
	return 0;
}
