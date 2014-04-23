typedef struct 
{
	GtkWidget *window;
	GtkWidget *notebook;
	WebKitWebContext *web_context;
} Browser;

WebKitWebView *new_web_view(Browser *b);
void setup_browser(Browser *b);
gboolean decide_policy_signal_handler(WebKitWebView *web_view,
	WebKitPolicyDecision *decision,
	WebKitPolicyDecisionType type,
	gpointer user_data);
GtkWidget* create_signal_handler(WebKitWebView *web_view,
	gpointer user_data);
