#include "noob.h"

#define RLBUILD "-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"
#define CFLAGS "-Wpedantic -Wextra -Werror"

int main(int argc, const char **argv) {
  noob_rebuild_yourself(argc, argv);

	noob_ensure_dir("bin/res");

	noob_run("cc "CFLAGS" src/tru-masker.c src/tinyfiledialogs.c -o bin/tru-masker "RLBUILD);
	noob_run("cp -rf res bin/");
	
  if (noob_has_flag(argc, argv, "run")) {
		noob_run("./bin/tru-masker");
	}

	return 0;
}
