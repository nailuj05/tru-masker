#include "noob.h"

#define RLBUILD "-Ilib/linux/include -Llib/linux/lib lib/linux/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11"
#define WIN_RLBUILD "-Ilib/win/include -Llib/win/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lole32 -luuid -lcomdlg32 -Wno-error=cast-function-type"
#define CFLAGS "-Wpedantic -Wextra -Werror"

int main(int argc, const char **argv) {
  noob_rebuild_yourself(argc, argv);

  noob_ensure_dir("bin");

	int win = noob_has_flag(argc, argv, "win");

	// compile shaders and style into executable
	noob_run("xxd -i res/grid.fs   > src/gridfs.h");
	noob_run("xxd -i res/circle.fs > src/circlefs.h");
	noob_run("xxd -i res/rect.fs   > src/rectfs.h");
	noob_run("xxd -i res/cyber.rgs > src/cyber.h");
	
  if (win) {
    noob_run("x86_64-w64-mingw32-gcc "CFLAGS" src/tru-masker.c src/tinyfiledialogs.c -o bin/tru-masker.exe "WIN_RLBUILD);
  } else {
    noob_run("cc "CFLAGS" src/tru-masker.c src/tinyfiledialogs.c -o bin/tru-masker "RLBUILD);
  }

  if (noob_has_flag(argc, argv, "run")) {
		noob_run("./bin/tru-masker");
  }

  return 0;
}
