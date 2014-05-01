#define BUF_LEN 1000

typedef struct 
{
	GtkWidget *window;
	GtkWidget *notebook;
	WebKitWebContext *web_context;
} Browser;

WebKitWebView *new_web_view(Browser *b);
WebKitWebView *new_tab(Browser *b);
char *read_url(char *buf);

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
