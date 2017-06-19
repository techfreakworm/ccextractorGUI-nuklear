
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


/*Tab Functions*/
void setup_output_tab(struct output_tab *output);

void draw_input_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_advanced_input_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_output_tab(struct nk_context *ctx, int *tab_screen_height, struct output_tab *output);
void draw_decoders_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_credits_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_debug_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_hd_homerun_tab(struct nk_context *ctx, int *tab_screen_height);
void draw_burned_subs_tab(struct nk_context *ctx, int *tab_screen_height);

