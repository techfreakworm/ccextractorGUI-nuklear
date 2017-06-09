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


//#define WINDOW_WIDTH 1200
//#define WINDOW_HEIGHT 800
//#define true 1
//#define false 0
//#define UNUSED(a) (void)a
//#define MIN(a,b) ((a) < (b) ? (a) : (b))
//#define MAX(a,b) ((a) < (b) ? (b) : (a))
//#define LEN(a) (sizeof(a)/sizeof(a)[0])
#include "activity.c"
#include "terminal.c"
#include "preview.c"


/*Trigger command for CLI*/
char command[20];

/*Global Variables for Drag and Drop files*/
char filePath[50][250];
int fileCount;

/* Width and Height of all frames*/
const GLint WIDTH_mainPanelAndWindow = 530, HEIGHT_mainPanelandWindow = 550;
const GLint WIDTH_termORPreviewPanel = 530, HEIGHT_termORPreviewPanel = 100;
const GLint WIDTH_termANDPreviewPanel = 400, HEIGHT_termANDPreviewPanel = 650;
const GLint WIDTH_activityPanel = 400, HEIGHT_activityPanelSolo = 550, HEIGHT_activityPanelDuo = 650;
const GLint WIDTH_mainTermORPreviewWindow = 530, HEIGHT_mainORPreviewTermWindow = 650;
const GLint WIDTH_mainTermANDPreviewWindow = 930, HEIGHT_mainTermAndPreviewWindow = 650;
const GLint WIDTH_mainActivityWindow = 930, HEIGHT_mainActivityWindowSolo = 550, HEIGHT_mainActivityWindowDuo = 650;

/*Tab constants*/
static int tab_screen_height;


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
void draw_file_rectangle_widget(struct nk_context *ctx, struct nk_font *font)
{
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
		nk_draw_text(canvas, space, "Drag and Drop files here for Extraction.", 40, &font->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
	}
	else {
		for (int i = 0; i < fileCount; i++)
		{
			nk_draw_text(canvas, space, filePath[i], strlen(filePath[i]), &font->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
			space.y = space.y + 20;
		}
	}
	
}

/*Rectangle to hold extraction info*/
void draw_info_rectangle_widget(struct nk_context *ctx, struct nk_font *font)
{
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
	nk_draw_text(canvas, space, "Input Type: Auto", 16, &font->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
	space.y = space.y + 20;
	nk_draw_text(canvas, space, "Output Type: Default(.srt)", 26, &font->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
	space.y = space.y + 20;
	nk_draw_text(canvas, space, "Output Path: Default", 20, &font->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
	space.y = space.y + 20;
	nk_draw_text(canvas, space, "Hardsubs Extraction: Yes", 24, &font->handle, nk_rgb(88, 81, 96), nk_rgb(0, 0, 0));
}

int main(void)
{
	//Platform
	static GLFWwindow *win;
	struct nk_context *ctx;
	static const char *ports[] = { "UDP", "TCP" };
	static int selected_port = 0;
	int screenWidth, screenHeight;
	//int winWidth, winHeight;

	//GLFW
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		fprintf(stdout, "GLFW failed to initialise.\n");
	}
	win = glfwCreateWindow(WIDTH_mainPanelAndWindow, HEIGHT_mainPanelandWindow, "CCExtractor", NULL, NULL);
	if (win == NULL)
		printf("Window Could not be created!\n");
	glfwMakeContextCurrent(win);
	glfwSetWindowSizeLimits(win, WIDTH_mainPanelAndWindow, HEIGHT_mainPanelandWindow, WIDTH_mainPanelAndWindow, HEIGHT_mainPanelandWindow);
	glfwSetWindowUserPointer(win, &ctx);
	glfwSetDropCallback(win, drop_callback);

	//GUI

	ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);
	struct nk_font_atlas *font_atlas;
	nk_glfw3_font_stash_begin(&font_atlas);
	struct nk_font *droid = nk_font_atlas_add_from_file(font_atlas, "fonts/Roboto-Regular.ttf", 16, 0);
	nk_glfw3_font_stash_end();
	nk_style_set_font(ctx, &droid->handle);

	//CHECKBOX VALUES
	static int show_terminal_check = nk_false;
	static int show_preview_check = nk_false;
	static int show_activity_check = nk_false;
	static int advanced_mode_check = nk_false;
	static int file_extension_check = nk_true;

	/*Main GUI loop*/
	while (!glfwWindowShouldClose(win))
	{
		//Input
		glfwPollEvents();
		nk_glfw3_new_frame();

		//Popups
		static int show_about_ccx = nk_false;
		static int show_getting_started = nk_false;
		static int show_preferences_network = nk_false;

		//GUI
		if (nk_begin(ctx, "CCExtractor", nk_rect(0, 0, WIDTH_mainPanelAndWindow, HEIGHT_mainPanelandWindow),
			NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND))
		{
			

		//MENUBAR
			nk_menubar_begin(ctx);
			nk_layout_row_begin(ctx, NK_STATIC, 30, 3);
			nk_layout_row_push(ctx, 80);
			if (nk_menu_begin_label(ctx, "Preferences", NK_TEXT_LEFT, nk_vec2(120, 200))) {
				nk_layout_row_dynamic(ctx, 30, 1);
				nk_menu_item_label(ctx, "Reset Defaults", NK_TEXT_LEFT);
				if (nk_menu_item_label(ctx, "Network Settings", NK_TEXT_LEFT))
					show_preferences_network = nk_true;
				nk_menu_end(ctx);
			}
			nk_layout_row_push(ctx, 70);
			if (nk_menu_begin_label(ctx, "Windows", NK_TEXT_LEFT, nk_vec2(120, 200))) {
				nk_layout_row_dynamic(ctx, 30, 1);
				nk_checkbox_label(ctx, "Activity", &show_activity_check);
				nk_checkbox_label(ctx, "Terminal", &show_terminal_check);
				nk_checkbox_label(ctx, "Preview", &show_preview_check);
				nk_menu_end(ctx);
			}
			nk_layout_row_push(ctx, 45);
			if (nk_menu_begin_label(ctx, "Help", NK_TEXT_LEFT, nk_vec2(120, 200)))
			{
				nk_layout_row_dynamic(ctx, 30, 1);
				if (nk_menu_item_label(ctx, "Getting Started", NK_TEXT_LEFT))
					show_getting_started = nk_true;
				if (nk_menu_item_label(ctx, "About CCExtractor", NK_TEXT_LEFT))
					show_about_ccx = nk_true;
				nk_menu_end(ctx);
			}
			
		//Network Settings
			if (show_preferences_network)
			{
				static struct nk_rect s = { 20,100,300,190 };
				if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Network Settings", NK_WINDOW_CLOSABLE, s))
				{
					nk_layout_row_dynamic(ctx, 20, 1);
					nk_label(ctx, "Network Settings will come here!", NK_TEXT_LEFT);
					nk_popup_end(ctx);
				}
				else
					show_preferences_network = nk_false;
			}

		//About CCExtractor Popup
			if (show_about_ccx)
			{
				static struct nk_rect s = { 20,100,300,190 };
				if (nk_popup_begin(ctx, NK_POPUP_STATIC, "About CCExtractor", NK_WINDOW_CLOSABLE, s))
				{
					nk_layout_row_dynamic(ctx, 20, 1);
					nk_label(ctx, "Information about CCExtractor will come here!", NK_TEXT_LEFT);
					nk_popup_end(ctx);
				}
				else
					show_about_ccx = nk_false;
			}

		//Getting Started
			if (show_getting_started)
			{
				static struct nk_rect s = { 20,100,300,190 };
				if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Getting Started", NK_WINDOW_CLOSABLE, s))
				{
					nk_layout_row_dynamic(ctx, 20, 1);
					nk_label(ctx, "Getting Started information about CCX will come here!", NK_TEXT_LEFT);
					nk_popup_end(ctx);
				}
				else
					show_getting_started = nk_false;
			}

			nk_layout_row_end(ctx);
			nk_menubar_end(ctx);
			nk_layout_space_begin(ctx, NK_STATIC, 15, 1);
			nk_layout_space_end(ctx);

			/*TABS TRIGGERED IN ADVANCED MODE FLAG*/
			if (advanced_mode_check)
			{
				static int current_tab = 0;
				struct nk_vec2 item_padding;
				struct nk_rect bounds;
				enum tab_name { MAIN, INPUT, ADV_INPUT, OUTPUT, DECODERS, CREDITS, DEBUG, HDHOMERUN, BURNEDSUBS };
				const char *names[] = { "Main", "Input", "Adavanced Input", "Output", "Decoders", "Credits", "Debug", "HDHomeRun", "BurnedSubs" };
				float id = 0;
				int i;

				nk_style_push_vec2(ctx, &ctx->style.window.spacing, nk_vec2(0, 0));
				nk_style_push_float(ctx, &ctx->style.button.rounding, 0);
				nk_layout_row_begin(ctx, NK_STATIC, 20, 9);
				for (i = 0; i < 9; ++i) {
					/*Make sure button perfectly fits text*/
					const struct nk_user_font *f = ctx->style.font;
					float text_width = f->width(f->userdata, f->height, names[i], nk_strlen(names[i]));
					float widget_width = text_width + 3 * ctx->style.button.padding.x;
					nk_layout_row_push(ctx, widget_width);
					if (current_tab == i) {
						/*Active tab gets highlighted*/
						struct nk_style_item button_color = ctx->style.button.normal;
						ctx->style.button.normal = ctx->style.button.active;
						current_tab = nk_button_label(ctx, names[i]) ? i : current_tab;
						ctx->style.button.normal = button_color;
					}
					else
						current_tab = nk_button_label(ctx, names[i]) ? i : current_tab;
				}
				nk_style_pop_float(ctx);
				/*Body*/

				nk_layout_row_dynamic(ctx, tab_screen_height, 1);
				if (nk_group_begin(ctx, "Notebook", 0))
				{
					nk_style_pop_vec2(ctx);
					switch (current_tab) {
					case MAIN:
						tab_screen_height = 0;
						break;

					case INPUT:
						tab_screen_height = 472;
						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "Advanced Input", NK_TEXT_LEFT);
						break;

					case ADV_INPUT:
						tab_screen_height = 472;
						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "Advanced input -2", NK_TEXT_LEFT);
						break;

					case OUTPUT:
						tab_screen_height = 472;
						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "Output", NK_TEXT_LEFT);
						break;

					case DECODERS:
						tab_screen_height = 472;
						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "Decoders", NK_TEXT_LEFT);
						break;

					case CREDITS:
						tab_screen_height = 472;
						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "Credits", NK_TEXT_LEFT);
						break;

					case DEBUG:
						tab_screen_height = 472;
						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "Debug", NK_TEXT_LEFT);
						break;

					case HDHOMERUN:
						tab_screen_height = 472;
						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "HD Home RUn", NK_TEXT_LEFT);
						break;

					case BURNEDSUBS:
						tab_screen_height = 472;
						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "Burned Subtitles", NK_TEXT_LEFT);
						break;
					}
					nk_group_end(ctx);
				}
				else nk_style_pop_vec2(ctx);
			}

			//ADVANCED MODE FLAG
			static const float ratio_adv_mode[] = { 0.75f, 0.22f, .03f };
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
			draw_file_rectangle_widget(ctx, droid);
			//nk_spacing(ctx, 1);



		//RadioButton 2 along with combobox
			static const float ratio_port[] = { 0.10f,0.20f,0.20f,0.20f,0.20f,0.10f };
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
			draw_info_rectangle_widget(ctx, droid);
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
				strcpy(command, "ccextractorwin ");
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
				show_activity_check = nk_true;
				exit;
			}
			nk_spacing(ctx, 1);

		}
		nk_end(ctx);

		glfwGetWindowSize(win, &screenWidth, &screenHeight);

		if (show_activity_check && show_preview_check && show_terminal_check)
		{
			if (screenWidth != 930 || screenHeight != 650)
				glfwSetWindowSizeLimits(win, 930, 650, 930, 650);
			activity(ctx, 530, 0, 400, 550);
			terminal(ctx, 0, 550, 530, 100);
			preview(ctx, 530, 550, 400, 100);
		}
		if (show_activity_check && show_preview_check && !show_terminal_check)
		{
			if (screenWidth != 930 || screenHeight != 650)
				glfwSetWindowSizeLimits(win, 930, 650, 930, 650);
			activity(ctx, 530, 0, 400, 650);
			preview(ctx, 0, 550, 530, 100);
		}
		if (show_activity_check && !show_preview_check && show_terminal_check)
		{
			if (screenWidth != 930 || screenHeight != 650)
				glfwSetWindowSizeLimits(win, 930, 650, 930, 650);
			activity(ctx, 530, 0, 400, 650);
			terminal(ctx, 0, 550, 530, 100);
		}
		if (show_terminal_check && show_preview_check && !show_activity_check)
		{
			if (screenWidth != 930 || screenHeight != 650)
				glfwSetWindowSizeLimits(win, 930, 650, 930, 650);
			terminal(ctx, 0, 550, 530, 100);
			preview(ctx, 530, 0, 400, 650);
		}
		if (show_activity_check && !show_preview_check && !show_terminal_check)
		{
			if (screenWidth != 930 || screenHeight == 650)
				glfwSetWindowSizeLimits(win, 930, 550, 930, 550);
			activity(ctx, 530, 0, 400, 550);
		}
		if (show_terminal_check && !show_activity_check && !show_preview_check)
		{
			if (screenHeight != 650 || screenWidth == 930)
				glfwSetWindowSizeLimits(win, 530, 650, 530, 650);
			terminal(ctx, 0, 550, 530, 100);
		}
		if (show_preview_check && !show_terminal_check && !show_activity_check)
		{
			if (screenHeight != 650 || screenWidth == 930)
				glfwSetWindowSizeLimits(win, 530, 650, 530, 650);
			preview(ctx, 0, 550, 530, 100);
		}
		if (!show_preview_check && !show_terminal_check && !show_activity_check)
			glfwSetWindowSizeLimits(win, WIDTH_mainPanelAndWindow, HEIGHT_mainPanelandWindow, WIDTH_mainPanelAndWindow, HEIGHT_mainPanelandWindow);


		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		/* IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
		* with blending, scissor, face culling and depth test and defaults everything
		* back into a default state. Make sure to either save and restore or
		* reset your own state after drawing rendering the UI. */
		nk_glfw3_render(NK_ANTI_ALIASING_ON);
		glfwSwapBuffers(win);
	}

	nk_glfw3_shutdown();
	glfwTerminate();
	return 0;
}