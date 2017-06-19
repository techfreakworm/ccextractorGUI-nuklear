/*Global variable for output tab*/
int show_color_from_picker;

/*Data containers for other functions*/

struct output_tab
{
	//General
	char **type;
	int type_select;
	int is_filename;
	char filename[260];
	int filename_len;
	int is_delay;
	int delay_sec;
	int is_export_xds;
	//Encoding
	enum { LATIN, UNIC, UTF } encoding;
	int is_bom;
	//Capitalization
	int is_cap_standard;
	int is_cap_file;
	char cap_dictionary[260];
	int cap_dictionary_len;
	//Line Endings
	enum { CRLF, LF } line_ending;
	//Colors and Styles
	int is_center;
	int is_dash;
	int no_typesetting;
	int no_color_info;
	int is_default_color;
	int color_popup;
	char color_hex[7];
	struct nk_color color_rgb;
	
	char default_color[6];
	//Roll-up Captions
	enum {ONETIME, REALTIME} onetime_or_realtime;
	int roll_limit;
};

struct time {
	int hours, minutes, seconds;
};

struct input_tab {
	//General
	char **type;
	int type_select;
	int is_split;
	int is_live_stream;
	int wait_data_sec;
	//Timing
	int is_process_from;
	int is_process_until;
	struct time from_time;
	struct time until_time;
	//Elementary Stream
	enum stream_type {AUTO_DETECT,STREAM_TYPE,STREAM_PID} elementary_stream;
	int is_assume_mpeg;
	int stream_type;
	long stream_pid;
	int mpeg_type;
	//Teletext
	enum { AUTO_DECODE, FORCE, DISABLE } teletext_decoder;
	int is_process_teletext_page;
	int teletext_page_number;
	//Screenfuls limit
	enum { NO_LIMIT, LIMITED } is_limit;
	int screenful_limit;
	//Clock
	enum { AUTO, GOP, PTS } clock_input;
};

/*Tab Functions*/
void setup_output_tab(struct output_tab *output);
void setup_input_tab(struct input_tab *input);

void draw_input_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_advanced_input_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_output_tab(struct nk_context *ctx, int *tab_screen_height, struct output_tab *output);
void draw_decoders_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_credits_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_debug_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_hd_homerun_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_burned_subs_tab(struct nk_context *ctx, int *tab_screen_height);

