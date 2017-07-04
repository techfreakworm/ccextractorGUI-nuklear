#ifndef POPUPS_H
#define POPUPS_H

#include "tabs.h"

struct network_popup
{
	char udp_ipv4[30];
	int udp_ipv4_len;
	char tcp_pass[30];
	int tcp_pass_len;
	char tcp_desc[30];
	int tcp_desc_len;
	char send_port[30];
	int send_port_len;
	char send_host[30];
	int send_host_len;
};

void draw_network_popup(struct nk_context *ctx, int *show_preferences_network, struct network_popup *network_settings);
void draw_getting_started_popup(struct nk_context *ctx, int *show_getting_started);
void draw_about_ccx_popup(struct nk_context *ctx, int *show_about_ccx, struct nk_user_font *droid_big, struct nk_user_font *droid_head);
void draw_progress_details_popup(struct nk_context *ctx, int *show_progress_details);
void draw_color_popup(struct nk_context *ctx, struct output_tab *output);

void setup_network_settings(struct network_popup *network_settings);

#endif //!POPUPS_H
