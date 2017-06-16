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
	//Encoding
	int encoding;
	int is_bom;
	//Capitalization
	int is_caps;
	char caps_dictionary[255];
	//Line Endings
	int line_ending;
	//Colors and Styles
	int is_center;
	int is_dash;
	int no_typesetting;
	int no_color_info;
	int is_default_color;
	char default_color[6];
	//Roll-up Captions
	int is_onetime_or_realtime;
	int roll_limit;
};

void setup_output_tab(struct output_tab *output);
