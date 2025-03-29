#include <stdlib.h>
#include <raylib.h>
#include "../noob.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "tinyfiledialogs.h"

#define FONTSIZE 30

Vector2 res = {1000, 1000};

size_t si = 0;
Shader shader;
int last_modified = 0;
char *shaders[] = {"res/circle.fs", "res/rect.fs", NULL };

int MainApp(const char*);
int GetNextShader();
void LoadDroppedTex();
void DoUI(float*, float*);
void SaveImage(RenderTexture2D *);

int main(int argc, char** argv) {
	if (argc == 0) exit(1);
	_noob_set_wdir(argv[0]);

	// Open Image Prolog
	const char *filterPatterns[] = { "*.png", "*.jpg" };
	const char *filePath = NULL;

	while(filePath == NULL) {
		filePath = tinyfd_openFileDialog("Open an image", "", 2, filterPatterns, "png/jpeg", 0);
	}

	MainApp(filePath);
}

int MainApp(const char *imagePath) {
	SetTargetFPS(60);
	InitWindow(res.x, res.y, "tru-masker");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	GuiLoadStyle("res/cyber.rgs");
	GuiSetStyle(DEFAULT, TEXT_SIZE, FONTSIZE);

	// Load Texture and prepare Render Texture
	Texture2D texture = LoadTexture(imagePath);
	float size = 0.25f, sharp = 2.0f;
	int tres_loc, sharp_loc, size_loc;
	float scale = 1.0;

	Vector2 tres = (Vector2){ texture.width, texture.height };
	
	RenderTexture2D target = LoadRenderTexture(texture.width, texture.height);
	
	// Load and prepare Shaders
	shader = LoadShader(0, shaders[si]);
	Shader grid = LoadShader(0, "res/grid.fs");

	tres_loc = GetShaderLocation(shader, "tres");
	size_loc = GetShaderLocation(shader, "size");
	sharp_loc = GetShaderLocation(shader, "sharp");
	
	while (!WindowShouldClose()) {
		GetNextShader();

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
		BeginDrawing();
		ClearBackground(BLACK);

		// Grid
		BeginShaderMode(grid);
		DrawRectangle(0,0,res.x, res.y, WHITE);
		EndShaderMode();

		// Render to RenderTexture
		BeginTextureMode(target);
		BeginShaderMode(shader);
		DrawTexture(texture, 0, 0, WHITE);
		EndShaderMode();
		EndTextureMode();
		
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

		DoUI(&size, &sharp);
		SaveImage(&target);
		
		EndDrawing();
	}

	UnloadShader(shader);
	UnloadShader(grid);
	UnloadRenderTexture(target);
	CloseWindow();
	return 0;
}

int GetNextShader() {
	if(IsKeyPressed(KEY_SPACE)) {
		si = shaders[si + 1] == NULL ? 0 : si + 1;
		shader = LoadShader(0, shaders[si]);
		return 1;
	}
	return 0;
}

void DoUI(float *size, float *sharp) {
	GuiDrawRectangle((Rectangle){10, 10, 360, 91}, 1, BLACK, BLACK);
	
	Rectangle r1 = {160, 20, 200, 30};
	Rectangle r2 = {160, 60, 200, 30};
	GuiSlider(r1, "Size: ", "", size, 0.0f, 0.75f);
	GuiSlider(r2, "Shapness: ", "", sharp, 0.1f, 20.0f);
}

void SaveImage(RenderTexture2D *target) {
	if (IsKeyPressed(KEY_S)) {
		Image image = LoadImageFromTexture(target->texture);
		ExportImage(image, "output.png");
	}
}
