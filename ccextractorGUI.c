/* nuklear - v1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION
#include "nuklear_lib/nuklear.h"
#include "nuklear_lib/nuklear_glfw_gl2.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
//#define true 1
//#define false 0
//#define UNUSED(a) (void)a
//#define MIN(a,b) ((a) < (b) ? (a) : (b))
//#define MAX(a,b) ((a) < (b) ? (b) : (a))
//#define LEN(a) (sizeof(a)/sizeof(a)[0])

/*Trigger command for CLI*/
char command[20];

/*Global Variables for Drag and Drop files*/
char filePath[50][250];
int fileCount;

static void error_callback(int e, const char *d)
{
	printf("Error %d: %s\n", e, d);
}

void drop_callback(GLFWwindow* window, int count, const char **paths)
{
	fileCount = count;
	int i;
	printf("Number of selected paths:%d\n", count);
	for (i = 0; i < count; i++)
	{
		strcpy(filePath[i], paths[i]);
	}
}


/*Rectangle to hold file names*/
void draw_file_rectangle_widget(struct nk_context *ctx)
{
	struct nk_font_atlas *font_atlas;
	nk_glfw3_font_stash_begin(&font_atlas);
	struct nk_font *droid = nk_font_atlas_add_from_file(font_atlas, "fonts/DroidSans.ttf", 14, 0);
	nk_glfw3_font_stash_end();

	struct nk_command_buffer *canvas;
	struct nk_input *input = &ctx->input;
	canvas = nk_window_get_canvas(ctx);

	struct nk_rect space;
	enum nk_widget_layout_states state;
	state = nk_widget(&space, ctx);
	if (!state) return;

	/*if (state != NK_WIDGET_ROM)
	update_your_widget_by_user_input(...);*/
	nk_fill_rect(canvas, space, 5, nk_rgb(88, 81, 96));
	if (!strcmp(filePath[0], "\0")) {
		space.y = space.y + (space.h / 2) -10;
		space.x = space.x + 90;
		nk_draw_text(canvas, space, "Drag and Drop files here for Extraction.", 40, &droid->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
	}
	else {
		for (int i = 0; i < fileCount; i++)
		{
			nk_draw_text(canvas, space, filePath[i], strlen(filePath[i]), &droid->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
			space.y = space.y + 20;
		}
	}
	
}

/*Rectangle to hold extraction info*/
void draw_info_rectangle_widget(struct nk_context *ctx)
{
	struct nk_font_atlas *font_atlas;
	nk_glfw3_font_stash_begin(&font_atlas);
	struct nk_font *droid = nk_font_atlas_add_from_file(font_atlas, "fonts/DroidSans.ttf", 14, 0);
	nk_glfw3_font_stash_end();

	struct nk_command_buffer *canvas;
	struct nk_input *input = &ctx->input;
	canvas = nk_window_get_canvas(ctx);

	struct nk_rect space;
	enum nk_widget_layout_states state;
	state = nk_widget(&space, ctx);
	if (!state) return;

	/*if (state != NK_WIDGET_ROM)
	update_your_widget_by_user_input(...);*/
	nk_fill_rect(canvas, space, 5, nk_rgb(88, 81, 96));
	space.x = space.x + 3;
	nk_draw_text(canvas, space, "Input Type: Auto", 16, &droid->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
	space.y = space.y + 20;
	nk_draw_text(canvas, space, "Output Type: Default(.srt)", 26, &droid->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
	space.y = space.y + 20;
	nk_draw_text(canvas, space, "Output Path: Default", 20, &droid->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
	space.y = space.y + 20;
	nk_draw_text(canvas, space, "Hardsubs Extraction: Yes", 24, &droid->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
}

int main(void)
{
	//Platform
	static GLFWwindow *win;
	int width = 530, height = 550;
	struct nk_context *ctx;
	struct nk_color background;
	static const char *ports[] = { "UDP", "TCP" };
	static int selected_port = 0;

	//GLFW
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		fprintf(stdout, "GLFW failed to initialise.\n");
	}
	win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CCExtractor", NULL, NULL);
	glfwMakeContextCurrent(win);
	glfwSetWindowSize(win, width, height);
	glfwSetWindowSizeLimits(win, width, height, width, height);
	glfwSetDropCallback(win, drop_callback);

	//GUI
	static int advanced_mode_check = 1;
	static int file_extension_check = 1;
	ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);
	{
		struct nk_font_atlas *font_atlas;
		nk_glfw3_font_stash_begin(&font_atlas);
		struct nk_font *droid = nk_font_atlas_add_from_file(font_atlas, "fonts/DroidSans.ttf", 14, 0);
		nk_glfw3_font_stash_end();
		nk_style_load_all_cursors(ctx, font_atlas->cursors);
		nk_style_set_font(ctx, &droid->handle);
	}
	background = nk_rgb(0, 0, 0);

	/*Main GUI loop*/
	while (!glfwWindowShouldClose(win))
	{
		//Input
		glfwPollEvents();
		nk_glfw3_new_frame();

		


		//GUI
		if (nk_begin(ctx, "CCExtractor", nk_rect(0, 0, 530, 550),
			NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BACKGROUND))
		{
			// nk_menubar_begin(ctx);
			// nk_layout_row_dynamic(ctx, 31, 3);
			// // nk_menu_begin(ctx, win, "pref", 1, nk_rect(0, 0, 30, 10),nk_vec2(0,0));
			// nk_menu_begin_text(ctx, "Preferences", 11, NK_TEXT_LEFT, nk_vec2(0, 0));
			// nk_menu_begin_text(ctx, "Windows", 7, NK_TEXT_LEFT, nk_vec2(0, 0));
			// nk_menu_begin_text(ctx, "About", 5, NK_TEXT_LEFT, nk_vec2(0, 0));

			// // nk_menu_begin_label(ctx, "Preferences", NK_TEXT_LEFT, nk_vec2(0, 0));
			// // nk_menu_close(ctx);
			// nk_menubar_end(ctx);

		//MENUBAR
			nk_menubar_begin(ctx);
			nk_layout_row_begin(ctx, NK_STATIC, 30, 3);
			nk_layout_row_push(ctx, 80);
			if (nk_menu_begin_label(ctx, "Preferences", NK_TEXT_LEFT, nk_vec2(120, 200))) {
				nk_layout_row_dynamic(ctx, 30, 1);
				nk_menu_item_label(ctx, "Reset Defaults", NK_TEXT_LEFT);
				nk_menu_item_label(ctx, "Network Settings", NK_TEXT_LEFT);
				nk_menu_end(ctx);
			}
			nk_layout_row_push(ctx, 70);
			if (nk_menu_begin_label(ctx, "Windows", NK_TEXT_LEFT, nk_vec2(120, 200))) {
				nk_layout_row_dynamic(ctx, 30, 1);
				nk_menu_item_label(ctx, "Activity", NK_TEXT_LEFT);
				nk_menu_item_label(ctx, "Terminal", NK_TEXT_LEFT);
				nk_menu_item_label(ctx, "Progress", NK_TEXT_LEFT);
				nk_menu_end(ctx);
			}
			nk_layout_row_push(ctx, 45);
			if (nk_menu_begin_label(ctx, "Help", NK_TEXT_LEFT, nk_vec2(120, 200))) 
			{
				nk_layout_row_dynamic(ctx, 30, 1);
				if (nk_menu_item_label(ctx, "Getting Started", NK_TEXT_LEFT))
			
					fprintf(stdout, "Getting started selected!");
				nk_menu_item_label(ctx, "About CCExtractor", NK_TEXT_LEFT);
				nk_menu_end(ctx);
			}
			nk_layout_row_end(ctx);
			nk_menubar_end(ctx);
			nk_layout_space_begin(ctx, NK_STATIC, 15, 1);
			nk_layout_space_end(ctx);

		//ADVANCED MODE FLAG
			static const float ratio_adv_mode[] = { 0.77f, 0.20f, .03f };
			nk_layout_row(ctx, NK_DYNAMIC, 20, 3, ratio_adv_mode);
			nk_spacing(ctx, 1);
			if (nk_checkbox_label(ctx, "Advanced Mode", &advanced_mode_check))
			{
				fprintf(stdout, "Advanced mode selected\n");
			}
			//nk_spacing(ctx, 1);

		//RADIO BUTTON 1 
			static const float ratio_button[] = { .10f, .90f };
			enum { PORT, FILES };
			static int op = FILES;
			static int property = 20;
			nk_layout_row(ctx, NK_DYNAMIC, 20, 2, ratio_button);
			nk_spacing(ctx, 1);
			if (nk_option_label(ctx, "Extract from files below:", op == FILES))
				op = FILES;

		//CHECKBOX FOR FILE TYPES
			nk_layout_row(ctx, NK_DYNAMIC, 20, 2, ratio_button);
			nk_spacing(ctx, 1);
			if (nk_checkbox_label(ctx, "Check for common video file extensions", &file_extension_check))
			{
				fprintf(stdout, "Will check for common video extensions with value:%d\n", file_extension_check);
			}

		//RECTANGLE-FILES
			static const float ratio_rect_files[] = { 0.10f,0.80f,0.10f };
			nk_layout_row(ctx, NK_DYNAMIC, 180, 3, ratio_rect_files);
			nk_spacing(ctx, 1);
			draw_file_rectangle_widget(ctx);
			//nk_spacing(ctx, 1);

		

		//RadioButton 2 along with combobox
			static const float ratio_port[]={0.10f,0.20f,0.20f,0.20f,0.20f,0.10f};
			nk_layout_row(ctx, NK_DYNAMIC, 20, 6, ratio_port);
			nk_spacing(ctx, 1);
			if (nk_option_label(ctx, "Extract from", op == PORT))
				op = PORT;
			if (nk_combo_begin_label(ctx, ports[selected_port], nk_vec2(85, 100)))
			{
				nk_layout_row_dynamic(ctx, 20, 1);
				for (int i = 0; i < 2; i++)
					if (nk_combo_item_label(ctx, ports[i], NK_TEXT_LEFT))
						selected_port = i;
				nk_combo_end(ctx);
			}
			nk_label(ctx, " stream, on port:", NK_TEXT_LEFT);

		//RADDIO BUTTON 2, TEXTEDIT FOR ENTERING PORT NUMBER
			static int port_num;
			static int *current = &port_num;
			{
				int len;
				char buffer[10];
				len = snprintf(buffer, 8, "%d", *current);
				nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 8, nk_filter_ascii);
				buffer[len] = 0;
				*current = atoi(buffer);
			}
			nk_layout_space_begin(ctx, NK_STATIC, 10, 1);
			nk_layout_space_end(ctx);

		//Extraction Information
			nk_layout_row_dynamic(ctx, 10, 1);
			nk_text(ctx, "Extraction Info:", 16, NK_TEXT_CENTERED);

		//RECTANGLE-INFO
			static const float ratio_rect_info[] = { 0.10f,0.80f,0.10f };
			nk_layout_row(ctx, NK_DYNAMIC, 75, 3, ratio_rect_info);
			nk_spacing(ctx, 1);
			draw_info_rectangle_widget(ctx);
			//nk_fill_rect(nk_window_get_canvas(ctx), nk_layout_space_bounds(ctx) , 5, nk_rgb(004, 003, 255));
			//nk_spacing(ctx, 1);

			
			nk_layout_space_begin(ctx, NK_STATIC, 10, 1);
			nk_layout_space_end(ctx);
		//PROGRESSBAR
			static nk_size cursor = 0;
			static const float ratio_progress[] = { 0.10f,0.03f,0.57f,0.03f,0.17f,0.10f };
			nk_layout_row(ctx, NK_DYNAMIC, 20, 6, ratio_progress);
			//nk_layout_row_dynamic(ctx, 30, 1);
			//nk_layout_row_push(ctx, 50);
			nk_spacing(ctx, 1);
			nk_spacing(ctx, 1);
			//nk_layout_row_push(ctx, 295);
			nk_progress(ctx, &cursor, 101, nk_true);

		//Extract Button
		//	nk_layout_row_push(ctx, 50);
			//nk_spacing(ctx, 1);
			//nk_layout_row_push(ctx, 100);
			/*static const float extract_ratio[] = { 0.80f, 0.20f };
			nk_layout_row(ctx, NK_DYNAMIC, 30, 2, extract_ratio);
			nk_spacing(ctx, 1);*/
			nk_spacing(ctx, 1);
			if (nk_button_label(ctx, "Extract"))
			{
				strcpy(command, "./ccextractor ");
				strcat(command, filePath[0]);
				system(command);
			}

			//nk_layout_row_end(ctx);
			nk_layout_space_begin(ctx, NK_STATIC, 10, 1);
			nk_layout_space_end(ctx);
		//PROGRESS_DETAILS_BUTTON
			nk_layout_row_dynamic(ctx, 20, 3);
			nk_spacing(ctx, 1);
			if (nk_button_label(ctx, "Progress Details"))
			{
				fprintf(stdout, "Progress Details clicked!\n");
			}
			nk_spacing(ctx, 1);

		}
		nk_end(ctx);

		{
			float bg[4];
			nk_color_fv(bg, background);
			glfwGetWindowSize(win, &width, &height);
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(bg[0], bg[1], bg[2], bg[3]);
			/* IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
			* with blending, scissor, face culling and depth test and defaults everything
			* back into a default state. Make sure to either save and restore or
			* reset your own state after drawing rendering the UI. */
			nk_glfw3_render(NK_ANTI_ALIASING_ON);
			glfwSwapBuffers(win); 
		}
	}

	nk_glfw3_shutdown();
	glfwTerminate();
	return 0;
}
