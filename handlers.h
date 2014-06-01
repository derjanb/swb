void load_uri(WebKitWebView *, char *);
gboolean decide_policy_signal_handler(WebKitWebView *web_view,
		WebKitPolicyDecision *decision,
		WebKitPolicyDecisionType type,
		gpointer user_data);
GtkWidget* create_signal_handler(WebKitWebView *web_view,
		gpointer user_data);
gboolean key_press_event_handler(GtkWidget *window,
		GdkEvent *event,
		gpointer user_data);
void load_changed_signal_handler(WebKitWebView *wv,
		WebKitLoadEvent load_event,
		gpointer user_data);
void update_title_signal_handler(GObject *gobj, 
		GParamSpec *p, 
		gpointer user_data);
