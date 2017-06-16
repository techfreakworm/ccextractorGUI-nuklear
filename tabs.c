#ifndef NK_IMPLEMENTATION
#include "nuklear_lib/nuklear.h"
#endif // !NK_IMPLEMENTATION
#include "tabs.h"

/*Initialise data of corresponding tabs*/
void setup_output_tab(struct output_tab *output)
{
	int i;
	//General
	output->type = (char **)malloc(13 * sizeof(char *));
	output->type[0] = "srt";
	output->type[1] = "ass/ssa";
	output->type[2] = "webvtt";
	output->type[3] = "webvtt-full";
	output->type[4] = "sami";
	output->type[5] = "bin";
	output->type[6] = "raw";
	output->type[7] = "dvd-raw";
	output->type[8] = "txt";
	output->type[9] = "ttxt";
	output->type[10] = "smptett";
	output->type[11] = "spupng";
	output->type[12] = "null";
	output->is_delay = nk_false;
	output->delay_sec = 0;
	output->type_select = 0;
	output->is_filename = 0;
	for (int i = 0; i < 260; i++)
		output->filename[i] = '\0';
	//Encoding
	output->encoding = 2;
	output->is_bom = nk_false;
	
	//Capitalization
	output->is_caps = nk_false;

	//LineEndings
	output->line_ending = 0;

	//Colors and Styles
	output->is_center = nk_false;
	output->is_dash = nk_false;
	output->no_typesetting = nk_false;
	output->no_color_info = nk_false;
	output->is_default_color = nk_false;

	//Roll-up Captions
	output->is_onetime_or_realtime = 0;
	output->roll_limit = 0;
}


/*Tab specific functions*/
void draw_input_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Advanced Input", NK_TEXT_LEFT);
}

void draw_advanced_input_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Advanced input -2", NK_TEXT_LEFT);
}

void draw_output_tab(struct nk_context *ctx, int *tab_screen_height, struct output_tab *output)
{
	static int len;
	static char buffer[260];
	const float gen_enc_ratio[] = { 0.7f, 0.3f };
	const float type_ratio[] = { 0.3f,0.7f };
	const float out_file_ratio[] = { 0.3f,0.5f,0.2f };
	*tab_screen_height = 472;
	nk_layout_row(ctx, NK_DYNAMIC, 200, 2, gen_enc_ratio);
	if (nk_group_begin(ctx, "General", NK_WINDOW_TITLE |NK_WINDOW_NO_SCROLLBAR))
	{
		nk_layout_row(ctx, NK_DYNAMIC, 20, 2, type_ratio);
		nk_label(ctx, "Output Type:", NK_TEXT_LEFT);
		output->type_select = nk_combo(ctx, output->type,  13, output->type_select, 25, nk_vec2(200, 400));

		nk_layout_row(ctx, NK_DYNAMIC, 20, 3, out_file_ratio);
		nk_checkbox_label(ctx, "Output Filename:", &output->is_filename);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 255, nk_filter_ascii);
		if (nk_button_label(ctx, "Browse"))
		{
			//File browser code;
		}
		strncpy(output->filename,buffer,len);
		output->filename_len = len;
		//printf("%s\n", output->filename);
		nk_group_end(ctx);
	}
	if (nk_group_begin(ctx, "Encoding", NK_WINDOW_TITLE |NK_WINDOW_NO_SCROLLBAR))
	{
		enum {LATIN, UNIC, UTF};
		static int option = UTF;
		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_option_label(ctx, "Latin", option == LATIN)) {
			option = LATIN;
			output->encoding = LATIN;
		}
		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_option_label(ctx, "Unicode", option == UNIC)) {
			option = UNIC;
			output->encoding = UNIC;
		}
		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_option_label(ctx, "UTF-8", option == UTF)) {
			option = UTF;
			output->encoding = UTF;
		}

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Byte Order Mark", &output->is_bom);
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "BOM for UNIX programs", NK_TEXT_LEFT);
		nk_group_end(ctx);
	}
}

draw_decoders_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Decoders", NK_TEXT_LEFT);
}

draw_credits_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Credits", NK_TEXT_LEFT);
}

draw_debug_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Debug", NK_TEXT_LEFT);
}

draw_hd_homerun_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "HD Home RUn", NK_TEXT_LEFT);
}

draw_burned_subs_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Burned Subtitles", NK_TEXT_LEFT);
}