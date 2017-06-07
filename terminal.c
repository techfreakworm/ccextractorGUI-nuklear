

static int
terminal(struct nk_context *ctx, int x, int y, int width, int height)
{
	if (nk_begin(ctx, "Terminal", nk_rect(x, y, width, height),0))
	{
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "Here terminal info will appear", 0);
	}
	nk_end(ctx);
	return !nk_window_is_closed(ctx, "Terminal");
}