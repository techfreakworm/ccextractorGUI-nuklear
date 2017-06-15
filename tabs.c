#ifndef NK_IMPLEMENTATION
#include "nuklear_lib/nuklear.h"
#endif // !NK_IMPLEMENTATION

draw_input_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Advanced Input", NK_TEXT_LEFT);
}

draw_advanced_input_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Advanced input -2", NK_TEXT_LEFT);
}

draw_output_tab(struct nk_context *ctx, int *tab_screen_height)
{
	*tab_screen_height = 472;
	nk_layout_row_dynamic(ctx, 30, 1);
	nk_label(ctx, "Output", NK_TEXT_LEFT);
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