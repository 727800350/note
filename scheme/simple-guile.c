/* simple-guile.c --- Start Guile from C.
 * When execute, it produces a complete Guile interpreter
 * gcc simple-guile.c -lguile
*/
#include <stdlib.h>
#include <libguile.h>

static SCM my_hostname (void){
	char *s = getenv ("HOSTNAME");
	if (s == NULL)
		return SCM_BOOL_F;
	else
		return scm_from_locale_string (s);
}

static void inner_main (void *closure, int argc, char **argv) {
	scm_c_define_gsubr ("my-hostname", 0, 0, 0, my_hostname);
	/* preparation */
	scm_shell (argc, argv);
	/* after exit */
}

int main (int argc, char **argv) {
	scm_boot_guile (argc, argv, inner_main, 0);
	return 0; /* never reached, see inner_main */
}
