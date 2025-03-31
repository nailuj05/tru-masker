#include <raylib.h>
#include "tru-masker.h"
#include "gridfs.h"
#include "cyber.h"

#ifdef __linux__
#include "../noob.h"
#endif

#define FONTSIZE 30

Vector2 tres, res = {1000, 1000};
Texture texture = {0};
RenderTexture2D target = {0};
int texLoaded = 0;

void PrepareTextures(const char* file) {
	texture = LoadTexture(file);
	texLoaded = 1;

	tres = (Vector2){ texture.width, texture.height };	
	target = LoadRenderTexture(texture.width, texture.height);
}

int main(int argc, const char** argv) {
	if (argc == 0) exit(1);
	#ifdef __linux__
	_noob_set_wdir(argv[0]);
	#endif

	SetTargetFPS(60);
	InitWindow(res.x, res.y, "tru-masker");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	
	GuiLoadStyleFromMemory((const unsigned char*)res_cyber_rgs, res_cyber_rgs_len);
	GuiSetStyle(DEFAULT, TEXT_SIZE, FONTSIZE);

	// Load and prepare Shaders
	float size = 0.25f, sharp = 2.0f;
	int tres_loc, sharp_loc, size_loc;
	float scale = 1.0;

	GetNextShader(1);
	Shader grid = LoadShaderFromMemory(0, (const char*)res_grid_fs);

	tres_loc = GetShaderLocation(shader, "tres");
	size_loc = GetShaderLocation(shader, "size");
	sharp_loc = GetShaderLocation(shader, "sharp");
	
	while (!WindowShouldClose()) {
		GetNextShader(0);

		// Scaling
		scale += GetMouseWheelMove() * 0.02f;
		if (scale < 0.1f) scale = 0.1f;

		// Resolution and resizing
		res.x = GetScreenWidth();
		res.y = GetScreenHeight();
		SetWindowSize(res.x, res.y);

		// Shader values
		SetShaderValue(shader, tres_loc, &tres, SHADER_UNIFORM_VEC2);
		SetShaderValue(shader, size_loc, &size, SHADER_UNIFORM_FLOAT);
		SetShaderValue(shader, sharp_loc, &sharp, SHADER_UNIFORM_FLOAT);
		
		// Draw Shaders

		if (!texLoaded) goto defer;
	
		// Render to RenderTexture
		BeginTextureMode(target);
		BeginShaderMode(shader);
		ClearBackground(BLANK);
		DrawTexture(texture, 0, 0, WHITE);
		EndShaderMode();
		EndTextureMode();

		BeginDrawing();
		DrawBackground(grid, res);
		
		float scaledWidth = target.texture.width * scale;
		float scaledHeight = target.texture.height * scale;
		float posX = (res.x - scaledWidth) / 2;
		float posY = (res.y - scaledHeight) / 2;
 
		DrawTexturePro(
							 target.texture,
							 (Rectangle){0, 0, target.texture.width, -target.texture.height},
							 (Rectangle){posX, posY, scaledWidth, scaledHeight},
							 (Vector2){0, 0}, 0, WHITE
									 );

		defer:
		DrawControlPanel( &size, &sharp);

		switch (DrawTopBar(res)) {
		case 1: {
			const char* file = OpenFileDialog("Open Image", 1);
			if (file)
				PrepareTextures(file);
			break;
		}
		case 2: {
			const char* file = OpenFileDialog("Save Image", 0);
			if (file)
				SaveImage(file, &target);
			break;
		}
		}
		
		EndDrawing();
	}

	UnloadShader(shader);
	UnloadShader(grid);
	UnloadTexture(texture);
	UnloadRenderTexture(target);
	CloseWindow();
	return 0;
}

