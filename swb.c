#include <stdio.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include "swb.h"
#include "config.h"

#define GET_CURRENT_WEB_VIEW(browser) \
	WEBKIT_WEB_VIEW(\
	gtk_notebook_get_nth_page(GTK_NOTEBOOK(browser->notebook),\
	gtk_notebook_get_current_page(GTK_NOTEBOOK(browser->notebook))))

#define DEBUG(format, ...) fprintf(stderr, format "\n", ##__VA_ARGS__)

int main (int argc, char *argv[])
{
	Browser browser;

	gtk_init (&argc, &argv);

	setup_browser(&browser);

	gtk_main ();

	return 0;
}

WebKitWebView *new_web_view(Browser *b)
{
	WebKitWebView *wv = WEBKIT_WEB_VIEW(webkit_web_view_new_with_context(b->web_context));
	g_object_ref(GTK_WIDGET(wv));
	g_signal_connect(wv, "create", G_CALLBACK(create_signal_handler), b);
	g_signal_connect(wv, "decide_policy", G_CALLBACK(decide_policy_signal_handler), b);
	return wv;
}



GtkWidget* create_signal_handler(WebKitWebView *web_view,
	gpointer user_data)
{
	DEBUG("create_signal_handler entered");

	Browser *b=(Browser *)user_data;

	WebKitWebView *wv = new_web_view(b);
	gtk_widget_show(GTK_WIDGET(wv));
	
	gtk_notebook_set_current_page(GTK_NOTEBOOK(b->notebook),
		gtk_notebook_append_page(GTK_NOTEBOOK(b->notebook), GTK_WIDGET(wv), NULL));

	DEBUG("create_signal_handler returns");
	return GTK_WIDGET(wv);
}

gboolean decide_policy_signal_handler(WebKitWebView *web_view,
	WebKitPolicyDecision *decision,
	WebKitPolicyDecisionType type,
	gpointer user_data)
{
	DEBUG("decide_policy_signal_handler entered");

	gboolean ret_val = TRUE;

	switch(type)
	{
		case WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION:
			webkit_policy_decision_use(decision);
			break;
		default:
			ret_val = FALSE;
	}
	DEBUG("decide_policy_signal_handler returns: %d", ret_val);
	return ret_val;
}

void setup_browser(Browser *b)
{
	//give default settings to web_context
	b->web_context = webkit_web_context_get_default();
	//if we're able to, we'd like to use separate processes for each webview
	#ifdef WEBKIT_PROCESS_MODEL_MULTIPLE_SECONDARY_PROCESSES
		webkit_web_context_set_process_model(b->web_context, WEBKIT_PROCESS_MODEL_MULTIPLE_SECONDARY_PROCESSES);
	#endif
	
	//create gtk notebook, it controls our tabs
	b->notebook = gtk_notebook_new();

	//create webview, put it in notebook and load home page
	gtk_notebook_append_page(GTK_NOTEBOOK(b->notebook), GTK_WIDGET(new_web_view(b)), NULL);
	webkit_web_view_load_uri(GET_CURRENT_WEB_VIEW(b), HOME_PAGE);
	
	//create a gtk window
	b->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);


	//show the window, the notebook and the current webview
	gtk_container_add(GTK_CONTAINER(b->window), GTK_WIDGET(b->notebook)); 
	g_signal_connect(b->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show(b->notebook);
	gtk_widget_show(GTK_WIDGET(GET_CURRENT_WEB_VIEW(b)));
	gtk_widget_show (b->window);
}

