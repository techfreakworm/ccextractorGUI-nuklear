#ifndef NK_IMPLEMENTATION
#include "nuklear_lib/nuklear.h"
#include <stdio.h>
#endif // !NK_IMPLEMENTATION
#include "tabs.h"
#include "popups.h"

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
	output->is_export_xds = nk_false;
	//Encoding
	output->encoding = UTF;
	output->is_bom = nk_false;
	
	//Capitalization
	output->is_cap_standard = nk_false;
	output->is_cap_file = nk_false;

	//LineEndings
	output->line_ending = 0;

	//Colors and Styles
	output->is_center = nk_false;
	output->is_dash = nk_false;
	output->no_typesetting = nk_false;
	output->no_color_info = nk_false;
	output->is_default_color = nk_false;
	output->color_popup = nk_false;
	output->color_rgb = nk_rgb(255, 255, 255);

	//Roll-up Captions
	output->onetime_or_realtime = ONETIME;
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
	static int cap_len;
	static int color_len;
	static char outFileBuffer[260];
	static char colorBuffer[7];
	static char capFileBuffer[260];
	const float gen_enc_ratio[] = { 0.695f, 0.29f,0.005f };
	const float type_ratio[] = { 0.3f,0.7f };
	const float out_file_ratio[] = { 0.3f,0.53f,0.17f };
	const float cap_file_ratio[] = { 0.2f,0.63f,0.17f };
	const float delay_ratio[] = { 0.5f, 0.2f, 0.3f };
	const float color_roll_ratio[] = { 0.6f,0.4f };
	const float color_check_ratio[] = { 0.6f,0.23f,0.07f,0.1f };
	*tab_screen_height = 472;
	nk_layout_row(ctx, NK_DYNAMIC, 160, 2, gen_enc_ratio);

	//General Group
	if (nk_group_begin(ctx, "General", NK_WINDOW_TITLE |NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
	{
		//Output Type
		nk_layout_row(ctx, NK_DYNAMIC, 20, 2, type_ratio);
		nk_label(ctx, "Output Type:", NK_TEXT_LEFT);
		output->type_select = nk_combo(ctx, output->type,  13, output->type_select, 25, nk_vec2(225, 200));

		//Output File
		nk_layout_row(ctx, NK_DYNAMIC, 25, 3, out_file_ratio);
		nk_checkbox_label(ctx, "Output File:", &output->is_filename);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, outFileBuffer, &len, 255, nk_filter_ascii);
		if (nk_button_label(ctx, "Browse"))
		{
			//File browser code;
		}
			//Make every character null in output->filename before copying buffer
		for (int i = 0; i < sizeof(output->filename); i++)
			output->filename[i] = '\0';
		strncpy(output->filename,outFileBuffer,len);
		output->filename_len = len;

		//Subtitle Delay
		nk_layout_row(ctx, NK_DYNAMIC, 25, 3, delay_ratio);
		nk_checkbox_label(ctx, "Add delay in subtitles for", &output->is_delay);
		nk_property_int(ctx, "", 0, &output->delay_sec, 1000, 1, 1);
		nk_label(ctx, "seconds", NK_TEXT_LEFT);
		
		//Export XDS info
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_checkbox_label(ctx, "Export XDS information (transcripts)", &output->is_export_xds);

		nk_group_end(ctx);
	}
	//Encoding Group
	if (nk_group_begin(ctx, "Encoding", NK_WINDOW_TITLE |NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
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
		nk_checkbox_label(ctx, "Byte Order Mark*", &output->is_bom);
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "*( For UNIX programs )", NK_TEXT_LEFT);
		nk_group_end(ctx);
	}

	nk_layout_row(ctx, NK_DYNAMIC, 100, 2, gen_enc_ratio);
	//Capitalization Group
	if(nk_group_begin(ctx, "Capitalization", NK_WINDOW_TITLE|NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
	{
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Standard capitalization rules.", &output->is_cap_standard);

		nk_layout_row(ctx, NK_DYNAMIC, 25, 3, cap_file_ratio);
		nk_checkbox_label(ctx, "Cap file:", &output->is_cap_file);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, capFileBuffer, &cap_len, 255, nk_filter_ascii);
		if (nk_button_label(ctx, "Browse"))
		{
			//File browser code;
		}
		//Make every character null in output->filename before copying buffer
		for (int i = 0; i < sizeof(output->cap_dictionary); i++)
			output->cap_dictionary[i] = '\0';
		strncpy(output->cap_dictionary, capFileBuffer, cap_len);
		output->cap_dictionary_len = cap_len;

/*-----------------------------------This makes realtime nk_edit_string as wanted for filebrowser----------------------
		for (int i = 0; i < sizeof(output->cap_dictionary); i++)
			output->cap_dictionary[i] = '\0';
		strncpy(output->cap_dictionary, outFileBuffer, len);
		output->cap_dictionary_len = len;
-----------------------------------------------------------------------------------------------------------------------*/

		nk_group_end(ctx);
	}

	//Line Endings
	if (nk_group_begin(ctx, "Line Endings:", NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR |NK_WINDOW_BORDER))
	{
		enum {CRLF, LF};
		static int option = CRLF;
		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_option_label(ctx, "CRLF (Windows)", option == CRLF)) {
			option = CRLF;
			output->line_ending = CRLF;
		}
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_option_label(ctx, "LF (UNIX-like)", option == LF)) {
			option = LF;
			output->line_ending = LF;
		}

		nk_group_end(ctx);
	}

	nk_layout_row(ctx, NK_DYNAMIC, 170, 2, color_roll_ratio);
	//Colors and Styles Group
	if (nk_group_begin(ctx, "Colors and Styles", NK_WINDOW_TITLE | NK_WINDOW_BORDER))
	{
		
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Center Text (remove left and right spaces)", &output->is_center);

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Add dash (-) when speaker changes (for .srt)", &output->is_dash);
		
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Don't add typesetting tags(bold, italic, etc.)", &output->no_typesetting);

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Don't add color information", &output->no_color_info);

		nk_layout_row(ctx, NK_DYNAMIC, 20, 4, color_check_ratio);
		nk_checkbox_label(ctx, "Default color (RRGGBB)#", &output->is_default_color);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, colorBuffer, &color_len, 7, nk_filter_hex);
		nk_label(ctx, "or", NK_TEXT_CENTERED);

		if(sizeof(colorBuffer) == 6)
			sscanf(colorBuffer, "%X%X%X", output->color_rgb.r, output->color_rgb.g, output->color_rgb.b);


		if (nk_button_color(ctx, output->color_rgb))
			output->color_popup = nk_true;

			
		nk_group_end(ctx);
	}

	//Roll-up Captions Group
	if (nk_group_begin(ctx, "Roll-up Captions", NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
	{
		enum { ONETIME, REALTIME };
		static int option = ONETIME;
		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_option_label(ctx, "Letters appear line by line", option == ONETIME)) {
			option = ONETIME;
			output->onetime_or_realtime = ONETIME;
		}
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "(No duplication of content)", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_option_label(ctx, "Letters appear in realtime", option == REALTIME)) {
			option = REALTIME;
			output->onetime_or_realtime = REALTIME;
		}
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "(Allows duplication of content)", NK_TEXT_LEFT);

		nk_group_end(ctx);
	}
	

}

void draw_decoders_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Decoders", NK_TEXT_LEFT);
}

void draw_credits_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Credits", NK_TEXT_LEFT);
}

void draw_debug_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Debug", NK_TEXT_LEFT);
}

void draw_hd_homerun_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "HD Home RUn", NK_TEXT_LEFT);
}

void draw_burned_subs_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Burned Subtitles", NK_TEXT_LEFT);
}