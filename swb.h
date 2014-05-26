#pragma once
typedef struct 
{
	GtkWidget *window;
	GtkWidget *notebook;
	WebKitWebContext *web_context;
	enum {INPUT, COMMAND} mode;
} Browser;

WebKitWebView *new_web_view(Browser *b);
WebKitWebView *new_tab(Browser *b);
char *read_url(char *buf, Browser *b);

void setup_browser(Browser *b);
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

void set_tab_title(Browser *b, WebKitWebView *wv, const gchar *title);
bool go_back(Browser *b);
bool go_forward(Browser *b);
bool close_tab(Browser *b);
bool tabopen_page(Browser *b);
bool open_page(Browser *b);
