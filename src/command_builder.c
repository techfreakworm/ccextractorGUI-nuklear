#ifndef NK_IMPLEMENTATION
#include "nuklear_lib/nuklear.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#endif //!NK_IMPLEMENTATION

#include "ccextractorGUI.h"
#include "tabs.h"
#include "command_builder.h"

void command_builder(struct built_string *command, struct main_tab *main_settings, struct network_popup *network_settings,struct input_tab *input, struct output_tab *output, struct burned_subs_tab *burned_subs)
{
	static char buffer[1000];	
	strcpy(buffer, "./ccextractor --gui_mode_reports -autoprogram");

	/*INPUT COMMANDS*/
	if (main_settings->port_or_files == FILES)
	{
		if (input->type_select != 0)
		{
			strcat(buffer, " -in=");
			strcat(buffer, input->type[input->type_select]);
		}

		if (input->is_split)
			strcat(buffer, " --videoedited");

		if (input->is_process_from) {
			strcat(buffer, " -startat ");
			strcat(buffer, input->from_time_buffer);
		}
		
		if (input->is_process_until) {
			strcat(buffer, " -endat ");
			strcat(buffer, input->until_time_buffer);
		}

		switch (input->elementary_stream) {
		case AUTO_DETECT:
			break;
		case STREAM_TYPE:
			strcat(buffer, " -datastreamtype ");
			strncat(buffer, input->stream_type, input->stream_type_len);
			break;
		case STREAM_PID:
			strcat(buffer, " -datapid ");
			strncat(buffer, input->stream_pid, input->stream_pid_len);
		}

		if (input->is_assume_mpeg) {
			strcat(buffer, " -streamtype ");
			strncat(buffer, input->mpeg_type, input->mpeg_type_len);
		}

		switch (input->teletext_decoder) {
		case AUTO_DECODE:
			break;
		case FORCE:
			strcat(buffer, " -teletext");
			break;
		case DISABLE:
			strcat(buffer, " -noteletext");
		}

		if (input->is_process_teletext_page) {
			strcat(buffer, " -tpage ");
			strncat(buffer, input->teletext_page_number, input->teletext_page_numer_len);
		}

		switch (input->is_limit) {
		case NO_LIMIT:
			break;
		case LIMITED:
			strcat(buffer, " --screenfuls ");
			strcat(buffer, input->screenful_limit_buffer);
		}

		switch (input->clock_input) {
		case AUTO:
			break;
		case GOP:
			strcat(buffer, " --goptime");
			break;
		case PTS:
			strcat(buffer, " --nogoptime");
			break;
		}

	}

	if (main_settings->port_or_files == PORT)
	{
		switch (main_settings->port_select)
		{
		case 0:
			strcat(buffer, " -udp ");
			if (!strstr(network_settings->udp_ipv4, "None")) {
				strncat(buffer, network_settings->udp_ipv4, network_settings->udp_ipv4_len);
				strcat(buffer, ":");
			}
			strncat(buffer, main_settings->port_num, main_settings->port_num_len);
			break;
		case 1:
			strcat(buffer, " -tcp ");
			strncat(buffer, main_settings->port_num, main_settings->port_num_len);
			if (!strstr(network_settings->tcp_pass, "None")) {
				strcat(buffer, " -tcppassword ");
				strncat(buffer, network_settings->tcp_pass, network_settings->tcp_pass_len);
			}
			if (!strstr(network_settings->tcp_desc, "None")) {
				strcat(buffer, " -tcpdesc ");
				strncat(buffer, network_settings->tcp_desc, network_settings->tcp_desc_len);
			}
			break;
		default:
			break;
		}

		if (input->is_live_stream) {
			strcat(buffer, " -s ");
			strncat(buffer, input->wait_data_sec, input->wait_data_sec_len);
		}

		if (input->is_process_from) {
			strcat(buffer, " -startat ");
			strcat(buffer, input->from_time_buffer);
		}

		if (input->is_process_until) {
			strcat(buffer, " -endat ");
			strcat(buffer, input->until_time_buffer);
		}

		switch (input->elementary_stream) {
		case AUTO_DETECT:
			break;
		case STREAM_TYPE:
			strcat(buffer, " -datastreamtype ");
			strncat(buffer, input->stream_type, input->stream_type_len);
			break;
		case STREAM_PID:
			strcat(buffer, " -datapid ");
			strncat(buffer, input->stream_pid, input->stream_pid_len);
		}

		if (input->is_assume_mpeg) {
			strcat(buffer, " -streamtype ");
			strncat(buffer, input->mpeg_type, input->mpeg_type_len);
		}

		switch (input->teletext_decoder) {
		case AUTO_DECODE:
			break;
		case FORCE:
			strcat(buffer, " -teletext");
			break;
		case DISABLE:
			strcat(buffer, " -noteletext");
		}

		if (input->is_process_teletext_page) {
			strcat(buffer, " -tpage ");
			strncat(buffer, input->teletext_page_number, input->teletext_page_numer_len);
		}

		switch (input->is_limit) {
		case NO_LIMIT:
			break;
		case LIMITED:
			strcat(buffer, " --screenfuls ");
			strcat(buffer, input->screenful_limit_buffer);
		}

		switch (input->clock_input) {
		case AUTO:
			break;
		case GOP:
			strcat(buffer, " --goptime");
			break;
		case PTS:
			strcat(buffer, " --nogoptime");
			break;
		}


	}

	if(burned_subs->is_burned_subs)
	{
		strcat(buffer, " -hardsubx -ocr_mode");
		switch(burned_subs->ocr_mode)
		{
		case FRAME:
			strcat(buffer, " frame");
			break;
		case WORD:
			strcat(buffer, " word");
			break;
		case LETTER:
			strcat(buffer, " letter");
			break;
		}

		strcat(buffer, " -min_sub_duration ");
		strcat(buffer, burned_subs->min_duration);

		if(!burned_subs->subs_color_select && burned_subs->color_type == PRESET)
			sprintf(buffer, "%s -whiteness_thresh %d", buffer, burned_subs->luminance_threshold);

		sprintf(buffer, "%s -conf_thresh %d", buffer, burned_subs->confidence_threshold);

		if(burned_subs->is_italic)
			strcat(buffer, " -detect_italics");
	}

	//Output
	{
			strcat(buffer, " -out=");
			strcat(buffer, output->type[output->type_select]);
			if (output->is_filename) {
				strcat(buffer, " -o \"");
				strncat(buffer, output->filename, output->filename_len);
				strcat(buffer, "\"");
			}

			if (output->is_delay) {
				strcat(buffer, " -delay ");
				strcat(buffer, output->delay_sec_buffer);
			}

			if (output->is_export_xds)
				strcat(buffer, " -xds");

			switch (output->encoding) {
			case LATIN:
				strcat(buffer, " -latin1");
				break;
			case UNIC:
				strcat(buffer, " -unicode");
				break;
			case UTF:
				strcat(buffer, " -utf8");
				break;
			}

			if (output->is_bom)
				strcat(buffer, " -bom");
			else
				strcat(buffer, " -nobom");

			if (output->is_cap_standard)
				strcat(buffer, " --sentencecap");

			if (output->is_cap_file) {
				strcat(buffer, " --capfile \"");
				strncat(buffer, output->cap_dictionary, output->cap_dictionary_len);
				strcat(buffer, "\"");
			}

			switch (output->line_ending)
			{
			case CRLF:
				break;
			case LF:
				strcat(buffer, " -lf");
				break;
			}

			if (output->is_center)
				strcat(buffer, " -trim");

			if (output->is_dash)
				strcat(buffer, " -autodash");

			if (output->no_typesetting)
				strcat(buffer, " --notypesetting");

			switch (output->font_color) {
			case NO_COLOR:
				strcat(buffer, " --nofontcolor");
				break;
			case DEFAULT_COLOR:
				strcat(buffer, " --defaultcolor #");
				strcat(buffer, output->color_hex);
				break;
			}

			switch (output->onetime_or_realtime)
			{
			case ONETIME:
				strcat(buffer, " --norollup");
				break;
			case REALTIME:
				strcat(buffer, " -dru");
				switch (output->roll_limit_select)
				{
				case 1:
					strcat(buffer, " -ru1");
					break;
				case 2:
					strcat(buffer, " -ru2");
					break;
				case 3:
					strcat(buffer, " -ru3");
					break;
				default:
					break;
				}

			}
	}

	memset(command->term_string, 0, sizeof(command->term_string));
	strncpy(command->term_string, buffer, strlen(buffer));
}

