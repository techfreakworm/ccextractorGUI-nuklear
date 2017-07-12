#ifndef NK_IMPLEMENTATION
#include "nuklear_lib/nuklear.h"
#endif // !NK_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tabs.h"
#include "popups.h"
#include "file_browser.h"

/*Initialise data of corresponding tabs*/
void setup_output_tab(struct output_tab *output)
{
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
	output->type_select = 0;
	output->is_filename = nk_false;
	output->is_output_browser_active = nk_false;
	output->is_export_xds = nk_false;
	//Encoding
	output->encoding = UTF;
	output->is_bom = nk_false;
	
	//Capitalization
	output->is_cap_standard = nk_false;
	output->is_cap_file = nk_false;
	output->is_cap_browser_active = nk_false;
	//LineEndings
	output->line_ending = 0;

	//Colors and Styles
	output->is_center = nk_false;
	output->is_dash = nk_false;
	output->no_typesetting = nk_false;
	output->color_popup = nk_false;
	output->color_rgb = nk_rgb(255, 255, 255);
	strncpy(output->color_hex, "FFFFFF", 6);

	//Roll-up Captions
	output->onetime_or_realtime = ONETIME;
	output->roll_limit_select = 0;
	output->roll_limit = (char**)malloc(4 * sizeof(char *));
	output->roll_limit[0] = "No Limit";
	output->roll_limit[1] = "1 Line";
	output->roll_limit[2] = "2 Lines";
	output->roll_limit[3] = "3 Lines";
}

void setup_input_tab(struct input_tab *input)
{
	//General
	input->type = (char **)malloc(10 * sizeof(char *));
	input->type[0] = "Auto";
	input->type[1] = "ts";
	input->type[2] = "ps";
	input->type[3] = "es";
	input->type[4] = "asf";
	input->type[5] = "wtv";
	input->type[6] = "bin";
	input->type[7] = "raw";
	input->type[8] = "mp4";
	input->type[9] = "mkv";
	input->is_split = nk_false;
	input->is_live_stream = nk_false;
	input->type_select = 0;
	strncpy(input->wait_data_sec, "0", 1);
	input->wait_data_sec_len = 1;
	//Timing
	input->is_process_from = nk_false;
	input->is_process_until = nk_false;
	//Elementary Stream
	input->elementary_stream = AUTO_DETECT;
	input->is_assume_mpeg = nk_false;
	input->stream_type_len = 0;
	input->stream_pid_len = 0;
	input->mpeg_type_len = 0;
	//Teletext
	input->teletext_decoder = AUTO_DECODE;
	input->is_process_teletext_page = nk_false;
	//Screenfuls limit
	input->is_limit = NO_LIMIT;
	//Clock
	input->clock_input = AUTO;
}

void setup_advanced_input_tab(struct advanced_input_tab *advanced_input)
{
	//Multiple Programs
	advanced_input->is_multiple_program = nk_true;
	advanced_input->multiple_program = FIRST_PROG;

	//Myth TV
	advanced_input->set_myth = AUTO_MYTH;

	//Miscellaneous
	advanced_input->is_mpeg_90090 = nk_false;
	advanced_input->is_padding_0000 = nk_false;
	advanced_input->is_order_ccinfo = nk_false;
	advanced_input->is_win_bug = nk_false;
	advanced_input->is_hauppage_file = nk_false;
	advanced_input->is_process_mp4 = nk_false;
	advanced_input->is_ignore_broadcast = nk_false;
}

void setup_debug_tab(struct debug_tab *debug)
{
	debug->is_elementary_stream = nk_false;
	debug->elementary_stream_len = 0;
	debug->is_dump_packets = nk_false;
	debug->is_debug_608 = nk_false;
	debug->is_debug_708 = nk_false;
	debug->is_stamp_output = nk_false;
	debug->is_debug_analysed_vid = nk_false;
	debug->is_raw_608_708 = nk_false;
	debug->is_debug_parsed = nk_false;
	debug->is_disable_sync = nk_false;
	debug->is_no_padding = nk_false;
	debug->is_debug_xds = nk_false;
	debug->is_output_pat = nk_false;
	debug->is_output_pmt = nk_false;
	debug->is_scan_ccdata = nk_false;
	debug->is_output_levenshtein = nk_false;
	debug->is_debug_browser_active = nk_false;
}

void setup_burned_subs_tab(struct burned_subs_tab *burned_subs)
{
	burned_subs->is_burned_subs = nk_false;
	burned_subs->subs_color = (char**)malloc(7 * sizeof(char *));
	burned_subs->subs_color[0] = "white";
	burned_subs->subs_color[1] = "yellow";
	burned_subs->subs_color[2] = "green";
	burned_subs->subs_color[3] = "cyan";
	burned_subs->subs_color[4] = "blue";
	burned_subs->subs_color[5] = "magenta";
	burned_subs->subs_color[6] = "red";
	burned_subs->custom_hue_len = 0;
	burned_subs->ocr_mode = FRAME;
	strcpy(burned_subs->min_duration, "0.5");
	burned_subs->min_duration_len = strlen(burned_subs->min_duration);
	burned_subs->luminance_threshold = 95;
	burned_subs->confidence_threshold = 0;
	burned_subs->is_italic = nk_false;
}


/*Tab specific functions*/
void draw_input_tab(struct nk_context *ctx, int *tab_screen_height, struct input_tab *input)
{
	const float screenful_limit_ratio[] = { 0.47f, 0.3f };
	static struct time from_time, until_time;
	const float stream_type_pid_ratio[] = { 0.7f,0.3f };
	const float mpeg_type_ratio[] = { 0.7f,0.3f };
	const float teletext_page_ratio[] = { 0.75f,0.25f };
	const float stream_teletext_ratio[] = { 0.5f, 0.51f };
	const float wait_data_ratio[] = { 0.6f, 0.25f, 0.15f };
	const float gen_type_ratio[] = { 0.3f, 0.7f };
	const float gen_time_ratio[] = { 0.6f ,0.4f};
	const char *split_type[] = { "Individual Files", "Parts of same video. Cut by generic tool", "Parts of same video. Cut by video tool" };
	static int split_num;
	*tab_screen_height = 472;

	nk_layout_row(ctx, NK_DYNAMIC, 150, 2, gen_time_ratio);
	//General Group
	if (nk_group_begin(ctx, "General", NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
	{
		//Input Type
		nk_layout_row(ctx, NK_DYNAMIC, 25, 2, gen_type_ratio);
		nk_label(ctx, "Input Type:", NK_TEXT_LEFT);
		input->type_select = nk_combo(ctx, input->type, 9, input->type_select, 25, nk_vec2(225, 200));

		//Split Type
		nk_layout_row(ctx, NK_DYNAMIC, 25, 2, gen_type_ratio);
		nk_label(ctx, "Split Type:", NK_TEXT_LEFT);
		split_num = nk_combo(ctx, split_type, 3, split_num, 25, nk_vec2(240, 200));
		if (split_num == 2)
			input->is_split = nk_true;
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Live Stream.*", &input->is_live_stream);
		nk_layout_row(ctx, NK_DYNAMIC, 21, 3, wait_data_ratio);
		nk_label(ctx, "*Wait when no data arrives for", NK_TEXT_LEFT);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, input->wait_data_sec, &input->wait_data_sec_len, 999, nk_filter_decimal);
		nk_label(ctx, "sec", NK_TEXT_LEFT);

		nk_group_end(ctx);
	}

	//Timing Group
	if (nk_group_begin(ctx, "Timing", NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
	{
		//Process From
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Process From: (HH:MM:SS)", &input->is_process_from);
		nk_layout_row_dynamic(ctx, 25, 1);
		sprintf(input->from_time_buffer, "%02d:%02d:%02d", from_time.hours, from_time.minutes, from_time.seconds);
		if (nk_combo_begin_label(ctx, input->from_time_buffer, nk_vec2(180, 250))) {
			//time_selected = nk_true;
			nk_layout_row_dynamic(ctx, 25, 1);
			from_time.seconds = nk_propertyi(ctx, "#Seconds:", 0, from_time.seconds, 60, 1, 1);
			from_time.minutes = nk_propertyi(ctx, "#Minutes:", 0, from_time.minutes, 60, 1, 1);
			from_time.hours = nk_propertyi(ctx, "#Hours:", 0, from_time.hours, 99, 1, 1);
			nk_combo_end(ctx);
		}


		//Process Until
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Process Until: (HH:MM:SS)", &input->is_process_until);
		nk_layout_row_dynamic(ctx, 25, 1);
		sprintf(input->until_time_buffer, "%02d:%02d:%02d", until_time.hours, until_time.minutes, until_time.seconds);
		if (nk_combo_begin_label(ctx, input->until_time_buffer, nk_vec2(180, 250))) {
			//time_selected = nk_true;
			nk_layout_row_dynamic(ctx, 25, 1);
			until_time.seconds = nk_propertyi(ctx, "#Seconds:", 0, until_time.seconds, 60, 1, 1);
			until_time.minutes = nk_propertyi(ctx, "#Minutes:", 0, until_time.minutes, 60, 1, 1);
			until_time.hours = nk_propertyi(ctx, "#Hours:", 0, until_time.hours, 99, 1, 1);
			nk_combo_end(ctx);
		}

		nk_group_end(ctx);
	}

	nk_layout_row(ctx, NK_DYNAMIC, 150, 2, stream_teletext_ratio);
	//Elementary Stream Group
	if (nk_group_begin(ctx, "Elementary Stream", NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_BORDER))
	{
		static int option = AUTO_DETECT;
		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_option_label(ctx, "Auto", option == AUTO_DETECT)) {
			option = AUTO_DETECT;
			input->elementary_stream = AUTO_DETECT;
		}
		nk_layout_row(ctx, NK_DYNAMIC, 25, 2, stream_type_pid_ratio);
		if (nk_option_label(ctx, "Process stream of type:", option == STREAM_TYPE)) {
			option = STREAM_TYPE;
			input->elementary_stream = STREAM_TYPE;
		}
		nk_edit_string(ctx, NK_EDIT_SIMPLE, input->stream_type, &input->stream_type_len, 9, nk_filter_decimal);

		nk_layout_row(ctx, NK_DYNAMIC, 25, 2, stream_type_pid_ratio);
		if (nk_option_label(ctx, "Process stream with PID:", option == STREAM_PID)) {
			option = STREAM_PID;
			input->elementary_stream = STREAM_PID;
		}
		nk_edit_string(ctx, NK_EDIT_SIMPLE, input->stream_pid, &input->stream_pid_len, 9, nk_filter_decimal);

		nk_layout_row(ctx, NK_DYNAMIC, 25, 2, mpeg_type_ratio);
		nk_checkbox_label(ctx, "Assume MPEG type is:", &input->is_assume_mpeg);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, input->mpeg_type, &input->mpeg_type_len, 9, nk_filter_decimal);

		nk_group_end(ctx);
	}
	//Teletext Group
	if (nk_group_begin(ctx, "Teletext", NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_TITLE | NK_WINDOW_BORDER))
	{
		static int option = AUTO_DECODE;
		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_option_label(ctx, "Auto", option == AUTO_DECODE)) {
			option = AUTO_DECODE;
			input->teletext_decoder = AUTO_DECODE;
		}
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_option_label(ctx, "Force Teletext decoder", option == FORCE)) {
			option = FORCE;
			input->teletext_decoder = FORCE;
		}
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_option_label(ctx, "Disable Teletext decoder", option == DISABLE)) {
			option = DISABLE;
			input->teletext_decoder = DISABLE;
		}

		nk_layout_row(ctx, NK_DYNAMIC, 20, 2, teletext_page_ratio);
		nk_checkbox_label(ctx, "Process Teletext Page #", &input->is_process_teletext_page);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, input->teletext_page_number, &input->teletext_page_numer_len, 99999, nk_filter_decimal);

		nk_group_end(ctx);
	}


	//Screenfuls limit group
	nk_layout_row(ctx, NK_DYNAMIC, 95, 2, stream_teletext_ratio);
	if (nk_group_begin(ctx, "'Screenfuls' limit", NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR))
	{
		static int screenful_limits;
		static int option = NO_LIMIT;
		
		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_option_label(ctx, "No limit", option == NO_LIMIT))
		{
			option = NO_LIMIT;
			input->is_limit = NO_LIMIT;
		}
		nk_layout_row(ctx, NK_DYNAMIC, 25, 2, screenful_limit_ratio);
		if (nk_option_label(ctx, "Screenful Limit:", option == LIMITED)) {
			option = LIMITED;
			input->is_limit = LIMITED;
		}
		screenful_limits = nk_propertyi(ctx, "", 0, screenful_limits, 999, 1, 1);
		sprintf(input->screenful_limit_buffer, "%d", screenful_limits);
		


		nk_group_end(ctx);
	}

	//Clock group
	if (nk_group_begin(ctx, "Clock", NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR))
	{
		static int option = AUTO;
		nk_layout_row_dynamic(ctx, 20, 3);
		if (nk_option_label(ctx, "Auto", option == AUTO))
		{
			option = AUTO;
			input->clock_input = AUTO;
		}
		if (nk_option_label(ctx, "GOP", option == GOP)) {
			option = GOP;
			input->clock_input = GOP;
		}
		if (nk_option_label(ctx, "PTS", option == PTS)) {
			option = PTS;
			input->clock_input = PTS;
		}
		nk_group_end(ctx);
	}
}

void draw_advanced_input_tab(struct nk_context *ctx, int *tab_screen_height, struct advanced_input_tab *advanced_input)
{
	*tab_screen_height = 472;
	const float prog_myth_ratio[] = { 0.5f, 0.5f };
	const float prog_num_ratio[] = { 0.67f, 0.03f, 0.3f };

	nk_layout_row(ctx, NK_DYNAMIC, 125, 2, prog_myth_ratio);

	//Multiple Programs Group
	if(nk_group_begin(ctx, "Multiple Programs", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_TITLE))
	{
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_checkbox_label(ctx, "File contains multiple programs", &advanced_input->is_multiple_program);

		static int option = FIRST_PROG;
		nk_layout_row_dynamic(ctx, 20, 1);
		if(nk_option_label(ctx, "Process first suitable program", option == FIRST_PROG))
		{
			option = FIRST_PROG;
			advanced_input->multiple_program = FIRST_PROG;
		}
		nk_layout_row(ctx, NK_DYNAMIC, 25, 3, prog_num_ratio);
		if(nk_option_label(ctx, "Process program with #", option == PROG_NUM))
		{
			option = PROG_NUM;
			advanced_input->multiple_program = PROG_NUM;
		}
		nk_spacing(ctx, 1);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, advanced_input->prog_number, &advanced_input->prog_number_len, 6, nk_filter_decimal);

		nk_group_end(ctx);
	}

	//Myth TV group
	if(nk_group_begin(ctx, "Myth TV", NK_WINDOW_TITLE|NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
	{
		static int option = AUTO_MYTH;
		nk_layout_row_dynamic(ctx, 20, 1);
		if(nk_option_label(ctx, "Auto", option == AUTO_MYTH))
		{
			option = AUTO_MYTH;
			advanced_input->set_myth = AUTO_MYTH;
		}

		nk_layout_row_dynamic(ctx, 20, 1);
		if(nk_option_label(ctx, "Force usage of Myth TV decoder", option == FORCE_MYTH))
		{
			option = FORCE_MYTH;
			advanced_input->set_myth = FORCE_MYTH;
		}

		nk_layout_row_dynamic(ctx, 20, 1);
		if(nk_option_label(ctx, "Disable Myth TV decoder", option == DISABLE_MYTH))
		{
			option = DISABLE_MYTH;
			advanced_input->set_myth = DISABLE_MYTH;
		}

		nk_group_end(ctx);
	}

	//Miscellaneous group
	nk_layout_row_dynamic(ctx, 210, 1);
	if(nk_group_begin(ctx, "Miscellaneous", NK_WINDOW_TITLE|NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR))
	{
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Use 90090 as MPEG clock frequency instead of 90000 (needed for some DVDs", &advanced_input->is_mpeg_90090);
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Use 0000 as CC padding instead of 8080", &advanced_input->is_padding_0000);
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Use the pic_order_cnt_lsb in AVC/H.264 data streams to order the CC information", &advanced_input->is_order_ccinfo);
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Work around  a bug in Win 7s software when converted from *.wtv to *.dvr-ms", &advanced_input->is_win_bug);
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "File was captured from Hauppage card", &advanced_input->is_hauppage_file);
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Force processing video track in MP4(instead of dedicated CC track", &advanced_input->is_process_mp4);
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Ignore broadcast date info (only affects Teletext in timed transcript with -datets)", &advanced_input->is_ignore_broadcast);


		nk_group_end(ctx);
	}
}

void draw_output_tab(struct nk_context *ctx, int *tab_screen_height, struct output_tab *output, struct main_tab *main_settings)
{
	const float roll_limit_ratio[] = { 0.55f, 0.45f };
	nk_flags active;
	static int len;
	static int cap_len;
	static int color_len = 6;
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
		static int delay_secs;
		//Output Type
		nk_layout_row(ctx, NK_DYNAMIC, 20, 2, type_ratio);
		nk_label(ctx, "Output Type:", NK_TEXT_LEFT);
		output->type_select = nk_combo(ctx, output->type,  13, output->type_select, 25, nk_vec2(225, 200));

		//Output File
		nk_layout_row(ctx, NK_DYNAMIC, 25, 3, out_file_ratio);
		nk_checkbox_label(ctx, "Output File:", &output->is_filename);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, output->filename, &output->filename_len, 255, nk_filter_ascii);
		if (nk_button_label(ctx, "Browse"))
		{
			main_settings->scaleWindowForFileBrowser = nk_true;
			output->is_output_browser_active = nk_true;
		}

		//Subtitle Delay
		nk_layout_row(ctx, NK_DYNAMIC, 25, 3, delay_ratio);
		nk_checkbox_label(ctx, "Add delay in subtitles for", &output->is_delay);
		delay_secs = nk_propertyi(ctx, "", 0, delay_secs, 1000, 1, 1);
		sprintf(output->delay_sec_buffer, "%d", delay_secs);
		nk_label(ctx, "seconds", NK_TEXT_LEFT);
		
		//Export XDS info
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_checkbox_label(ctx, "Export XDS information (transcripts)", &output->is_export_xds);

		nk_group_end(ctx);
	}
	//Encoding Group
	if (nk_group_begin(ctx, "Encoding", NK_WINDOW_TITLE |NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
	{
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
		nk_edit_string(ctx, NK_EDIT_SIMPLE, output->cap_dictionary, &output->cap_dictionary_len, 255, nk_filter_ascii);
		if (nk_button_label(ctx, "Browse"))
		{
			main_settings->scaleWindowForFileBrowser = nk_true;
			output->is_cap_browser_active = nk_true;
		}

		nk_group_end(ctx);
	}

	//Line Endings
	if (nk_group_begin(ctx, "Line Endings:", NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR |NK_WINDOW_BORDER))
	{
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
		static int option = NO_COLOR;

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Center Text (remove left and right spaces)", &output->is_center);

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Add dash (-) when speaker changes (for .srt)", &output->is_dash);
		
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, "Don't add typesetting tags(bold, italic, etc.)", &output->no_typesetting);

		nk_layout_row_dynamic(ctx, 20, 1);
		if(nk_option_label(ctx, "Don't add color information", option == NO_COLOR)) {
			option = NO_COLOR;
			output->font_color = NO_COLOR;
		}

		nk_layout_row(ctx, NK_DYNAMIC, 20, 4, color_check_ratio);
		if (nk_option_label(ctx, "Default color (RRGGBB)#", option == DEFAULT_COLOR)) {
			option = DEFAULT_COLOR;
			output->font_color = DEFAULT_COLOR;
		}
		active = nk_edit_string(ctx, NK_EDIT_FIELD|NK_EDIT_SIG_ENTER, output->color_hex, &color_len, 7, nk_filter_hex);
		nk_label(ctx, "or", NK_TEXT_CENTERED);
		if (nk_button_color(ctx, output->color_rgb))
			output->color_popup = nk_true;
		if (show_color_from_picker)
		{
			sprintf(output->color_hex, "%02X%02X%02X", output->color_rgb.r, output->color_rgb.g, output->color_rgb.b);
			show_color_from_picker = nk_false;
		}
		/*if (active & NK_EDIT_COMMITED) {
			output->color_rgb.r = (int)strtol(strncat(output->color_hex[0], output->color_hex[1], 2), NULL, 16);
			output->color_rgb.g = (int)strtol(strncat(output->color_hex[2], output->color_hex[3], 2), NULL, 16);
			output->color_rgb.b = (int)strtol(strncat(output->color_hex[4], output->color_hex[5], 2), NULL, 16);
			printf("%d%d%d", output->color_rgb.r, output->color_rgb.g, output->color_rgb.b);
		}*/

			
		nk_group_end(ctx);
	}

	

	//Roll-up Captions Group
	if (nk_group_begin(ctx, "Roll-up Captions", NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
	{
		static int option = ONETIME;
		
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_option_label(ctx, "Letters appear in realtime", option == REALTIME)) {
			option = REALTIME;
			output->onetime_or_realtime = REALTIME;
		}
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "(Allows duplication of content)", NK_TEXT_LEFT);
		nk_layout_row(ctx, NK_DYNAMIC, 20, 2, roll_limit_ratio);
		nk_label(ctx, "Limit visible lines", NK_TEXT_LEFT);
		output->roll_limit_select = nk_combo(ctx, output->roll_limit, 4, output->roll_limit_select, 25, nk_vec2(80, 80));

		nk_layout_row_dynamic(ctx, 20, 1);
		if (nk_option_label(ctx, "Letters appear line by line", option == ONETIME)) {
			option = ONETIME;
			output->onetime_or_realtime = ONETIME;
		}
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "(No duplication of content)", NK_TEXT_LEFT);

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

void draw_debug_tab(struct nk_context *ctx, int *tab_screen_height,
		struct main_tab *main_settings,
		struct output_tab *output,
		struct debug_tab *debug)
{
	*tab_screen_height = 472;
	const float stream_ratio[] = { 0.45f, 0.45f, 0.1f };
	nk_layout_row_dynamic(ctx, 4, 1);
	nk_spacing(ctx, 1);
	nk_layout_row(ctx, NK_DYNAMIC, 25, 3, stream_ratio);
	nk_checkbox_label(ctx, "Write elementary stream to this file", &debug->is_elementary_stream);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, debug->elementary_stream, &debug->elementary_stream_len, 260, nk_filter_ascii);
	if(nk_button_label(ctx, "Browse"))
	{
		debug->is_debug_browser_active = nk_true;
		main_settings->scaleWindowForFileBrowser = nk_true;
	}

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Dump Intereseting packets, usually those that don't seem to follow specs.", &debug->is_dump_packets);

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Print debug traces from EIA-608 decoder.", &debug->is_debug_608);

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Print debug traces from EIA-708 decoder.", &debug->is_debug_708);

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Enable lots of time stamp output.", &debug->is_stamp_output);

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Print debug info about the analysed elementary video stream.", &debug->is_debug_analysed_vid);

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Print debug trace with raw 608/708 data with time stamps.", &debug->is_raw_608_708);

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Print debug info for parse container files (only for ASF files at the moment).", &debug->is_debug_parsed);

	if(!(strcmp(output->type[output->type_select], "bin")))
	{
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_checkbox_label(ctx, "Disable sync code when there's a timeline gap.", &debug->is_disable_sync);

		nk_layout_row_dynamic(ctx, 25, 1);
		nk_checkbox_label(ctx, "Don't remove trailing padding blocks.", &debug->is_no_padding);

	}

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Enable XDS debug traces.", &debug->is_debug_xds);

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Output Program Association Table (PAT) contents.", &debug->is_output_pat);

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Output Program Map Table (PMT) contents.", &debug->is_output_pmt);

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "If no suitable packet found in PMT, analyse packet contents and scan for CC data.", &debug->is_scan_ccdata);

	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, "Output Levenshtein debug info(calculated distance, allowed, etc)", &debug->is_output_levenshtein);
}

void draw_hd_homerun_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "HD Home RUn", NK_TEXT_LEFT);
}

void draw_burned_subs_tab(struct nk_context *ctx, int *tab_screen_height, struct burned_subs_tab *burned_subs)
{
	*tab_screen_height = 472;
	const float color_mode_ratio[] = { 0.65f, 0.35f, 0.05f };
	const float preset_ratio[] = { 0.4f, 0.5f };
	const float custom_ratio[] = { 0.4f, 0.5f};
	const float delay_ratio[] = { 0.4f, 0.2f, 0.2f };
	const float threshold_ratio[] = { 0.9f, 0.1f };
	static char buffer[5];

	nk_layout_row_dynamic(ctx, 30, 1);
#if ENABLE_OCR
	nk_checkbox_label(ctx, "Enable Burned-in Subtitle Extraction", &burned_subs->is_burned_subs);
#else
	nk_label_colored(ctx, "Required Library not found. Cannot perform Burned subtitle extraction.", NK_TEXT_LEFT, nk_rgb(255, 56, 38));
#endif

	nk_layout_row(ctx, NK_DYNAMIC, 140, 2, color_mode_ratio);
	if(nk_group_begin(ctx, "Subtitle Color", NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
	{
		static int option = PRESET;
		nk_layout_row(ctx, NK_DYNAMIC, 25, 2, preset_ratio);
		if(nk_option_label(ctx, "Preset color:", option == PRESET)){
			option = PRESET;
			burned_subs->color_type = PRESET;
		}
		burned_subs->subs_color_select = nk_combo(ctx, burned_subs->subs_color, 7, burned_subs->subs_color_select, 25, nk_vec2(100, 100));

		nk_layout_row(ctx, NK_DYNAMIC, 25, 2, custom_ratio);
		if(nk_option_label(ctx, "Custom Hue:", option == CUSTOM)){
			option = CUSTOM;
			burned_subs->color_type = CUSTOM;
		}
		nk_edit_string(ctx, NK_EDIT_SIMPLE, burned_subs->custom_hue, &burned_subs->custom_hue_len, 4, nk_filter_decimal);
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label_wrap(ctx, "Custom Hue can be between 1 and 360");
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label_wrap(ctx, "Refer to HSV color chart.");

		nk_group_end(ctx);
	}

	if(nk_group_begin(ctx, "OCR mode", NK_WINDOW_TITLE|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER))
	{
		static int option = FRAME;
		nk_layout_row_dynamic(ctx, 25, 1);
		if(nk_option_label(ctx, "Frame - wise", option == FRAME)){
			option = FRAME;
			burned_subs->ocr_mode = FRAME;
		}
		nk_layout_row_dynamic(ctx, 25, 1);
		if(nk_option_label(ctx, "Word - wise", option == WORD)){
			option = WORD;
			burned_subs->ocr_mode = WORD;
		}
		nk_layout_row_dynamic(ctx, 25, 1);
		if(nk_option_label(ctx, "Letter - wise", option == LETTER)){
			option = LETTER;
			burned_subs->ocr_mode = LETTER;
		}

		if(burned_subs->is_italic)
		{
			option = WORD;
			burned_subs->ocr_mode = WORD;
		}
		nk_group_end(ctx);
	}
	nk_layout_row_dynamic(ctx, 120, 1);
	if(nk_group_begin(ctx, "Minimum Subtitle Duration", NK_WINDOW_TITLE|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER))
	{
		nk_layout_row(ctx, NK_DYNAMIC, 25, 3, delay_ratio);
		nk_label(ctx, "Set the minimum subtitle duration to:", NK_TEXT_LEFT);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, burned_subs->min_duration, &burned_subs->min_duration_len, 4, nk_filter_float);
		nk_label(ctx, "seconds", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "Lower values give better results but take more time.", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_label(ctx, "0.5 is the recommended value.", NK_TEXT_LEFT);
		nk_group_end(ctx);
	}

	if(!burned_subs->subs_color_select && burned_subs->color_type == PRESET)
	{
		nk_layout_row_dynamic(ctx, 60, 1);
		if(nk_group_begin(ctx, "Luminance Threshold", NK_WINDOW_TITLE|NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
		{
			nk_layout_row(ctx, NK_DYNAMIC, 20, 2, threshold_ratio);
			nk_slider_int(ctx, 0, &burned_subs->luminance_threshold, 100, 1);
			sprintf(buffer, "%d", burned_subs->luminance_threshold);
			nk_label(ctx, buffer, NK_TEXT_LEFT);

			nk_group_end(ctx);
		}
	}

	nk_layout_row_dynamic(ctx, 60, 1);
	if(nk_group_begin(ctx, "Confidence Threshold", NK_WINDOW_TITLE|NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER))
	{
		nk_layout_row(ctx, NK_DYNAMIC, 20, 2, threshold_ratio);
		nk_slider_int(ctx, 0, &burned_subs->confidence_threshold, 100, 1);
		sprintf(buffer, "%d", burned_subs->confidence_threshold);
		nk_label(ctx, buffer, NK_TEXT_LEFT);

		nk_group_end(ctx);
	}

	nk_layout_row_dynamic(ctx, 30, 1);
	nk_checkbox_label(ctx, "Enable italics detection.", &burned_subs->is_italic);

}
