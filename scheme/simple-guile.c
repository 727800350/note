/* simple-guile.c --- Start Guile from C.
 * When execute, it produces a complete Guile interpreter
 * gcc simple-guile.c -lguile
*/
#include <libguile.h>
static void inner_main (void *closure, int argc, char **argv) {
	/* preparation */
	scm_shell (argc, argv);
	/* after exit */
}

int main (int argc, char **argv) {
	scm_boot_guile (argc, argv, inner_main, 0);
	return 0; /* never reached, see inner_main */
}
