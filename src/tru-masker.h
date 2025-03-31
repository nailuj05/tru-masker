#include <stdlib.h>
#include <raylib.h>
#include "tinyfiledialogs.h"
#include "circlefs.h"
#include "rectfs.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int si = 0;
Shader shader;

Color bg = {0,0,0,200};

int GetNextShader(int force) {
	if(IsKeyPressed(KEY_SPACE) || force) {
		si = !si;
		if (si == 0) {
			shader = LoadShaderFromMemory(0, (const char*)res_circle_fs);
		} else {
			shader = LoadShaderFromMemory(0, (const char*)res_rect_fs);
		}
		return 1;
	}
	return 0;
}

void DrawBackground(Shader grid, Vector2 res) {
		ClearBackground(BLACK);

		// Grid
		BeginShaderMode(grid);
		DrawRectangle(0,0,res.x, res.y, WHITE);
		EndShaderMode();
}

int DrawTopBar(Vector2 res) {
	GuiDrawRectangle((Rectangle){0,0, res.x, 40}, 1, bg, bg);
	GuiLabel((Rectangle){res.x / 2 -75,0,150,40}, "TruMasker");
	
	if(GuiButton((Rectangle){10,0,100,40}, "Open")) {
		return 1;
	}
	if(GuiButton((Rectangle){120,0,100,40}, "Save")) {
		return 2;
	}
	return 0;
}

void DrawControlPanel(float* size, float* sharp) {
	GuiDrawRectangle((Rectangle){10, 50, 360, 91}, 1, bg, bg);
	
	Rectangle r1 = {160, 60, 200, 30};
	Rectangle r2 = {160, 100, 200, 30};
	GuiSlider(r1, "Size: ", "", size, 0.0f, 1.0f);
	GuiSlider(r2, "Shapness: ", "", sharp, 0.05f, 15.0f);
}

void SaveImage(const char* file, RenderTexture2D* target) {
	Image image = LoadImageFromTexture(target->texture);
	ImageFlipVertical(&image);
	ExportImage(image, file);
}

const char* OpenFileDialog(const char* name, int open) {
	const char *filterPatterns[] = { "*.png" };
	const char *filePath = NULL;

	if (open)
		filePath = tinyfd_openFileDialog(name, "", 1, filterPatterns, "png", 0);
	else
		filePath = tinyfd_saveFileDialog(name, "output.png", 1, filterPatterns, "png");
	
	return filePath;
}
