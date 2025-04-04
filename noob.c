#include "noob.h"

#define RLBUILD "-Ilib/linux/include -Llib -Llib/linux/lib/libraylib.a -lraylib -lGL -lm -lpthread -ldl -lrt -lX11"
#define WIN_RLBUILD "-Ilib/win/include -Llib/win/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lole32 -luuid -lcomdlg32 -Wno-error=cast-function-type"
#define CFLAGS "-Wpedantic -Wextra -Werror -Ilib"

int main(int argc, const char **argv) {
  noob_rebuild_yourself(argc, argv);

	noob_help(argc, argv, "usage: ./noob <win> <run>",
						"\tbuilds the main executable (linux by default)",
						"\tadding the \"win\" flag will produce a windows executable when requirements are met",
						"\tadding the \"run\" flag will execute the binary (on linux only)");
	
  noob_ensure_dir("bin");

	int win = noob_has_flag(argc, argv, "win");

	// compile shaders and style into header so the executable doesn't have to load anything dynamically
	noob_run("xxd -i res/grid.fs   > src/gridfs.h");
	noob_run("xxd -i res/circle.fs > src/circlefs.h");
	noob_run("xxd -i res/rect.fs   > src/rectfs.h");
	noob_run("xxd -i res/cyber.rgs > src/cyber.h");
	
  if (win) {
		if (noob_is_outdated("lib/tinyfiledialogs.c", "lib/tfdwin.o")) {
			noob_run("x86_64-w64-mingw32-gcc -c lib/tinyfiledialogs.c -o lib/tfdwin.o");
		}
		
    noob_run("x86_64-w64-mingw32-gcc "CFLAGS" src/tru-masker.c lib/tfdwin.o -o bin/tru-masker.exe "WIN_RLBUILD);
  }
	else {
		if (noob_is_outdated("lib/tinyfiledialogs.c", "lib/tfdlinux.o")) {
			noob_run("gcc -c lib/tinyfiledialogs.c -o lib/tfdlinux.o");
		}
		
    noob_run("cc "CFLAGS" src/tru-masker.c lib/tfdlinux.o -o bin/tru-masker "RLBUILD);
  }

	// running is not supported (assumes you are on linux) - wine should work fine though
  if (noob_has_flag(argc, argv, "run") && !win) {
		noob_run("./bin/tru-masker");
  }

  return 0;
}
