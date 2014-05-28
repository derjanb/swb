#define _XOPEN_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <webkit2/webkit2.h>
#include "swb.h"
#include "config.h"
#include "helpers.h"
#include "handlers.h"
#include "navigation.h"

int main (int argc, char *argv[])
{
	Browser browser;

	gtk_init (&argc, &argv);

	setup_browser(&browser);

	gtk_main ();

	return 0;
}

void setup_browser(Browser *b)
{
	//create config dir if it doesn't exist already
	mkdir(CONFIG_PATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	//give default settings to web_context
	b->web_context = webkit_web_context_get_default();
	//if we're able to, we'd like to use separate processes for each webview
#ifdef WEBKIT_PROCESS_MODEL_MULTIPLE_SECONDARY_PROCESSES
	webkit_web_context_set_process_model(b->web_context, WEBKIT_PROCESS_MODEL_MULTIPLE_SECONDARY_PROCESSES);
#endif
	//accept cookies
	webkit_cookie_manager_set_accept_policy(
			webkit_web_context_get_cookie_manager(b->web_context),
			WEBKIT_COOKIE_POLICY_ACCEPT_NO_THIRD_PARTY);

	//tell webkit where to store cookies
	webkit_cookie_manager_set_persistent_storage(
			webkit_web_context_get_cookie_manager(b->web_context),
			COOKIE_FILE,
			WEBKIT_COOKIE_PERSISTENT_STORAGE_SQLITE);

	//create gtk notebook, it controls our tabs
	b->notebook = gtk_notebook_new();

	//create webview, put it in notebook and load home page
	WebKitWebView *wv = new_web_view(b);
	gtk_notebook_append_page(GTK_NOTEBOOK(b->notebook), GTK_WIDGET(wv), NULL);
	webkit_web_view_load_uri(wv, HOME_PAGE);

	//set custom stylesheet for webviewgroupi
	char *css;
	if(g_file_get_contents(STYLESHEET_FILE, &css, NULL, NULL))
	{
		webkit_web_view_group_add_user_style_sheet(
				webkit_web_view_get_group(wv),
				css,
				NULL,
				NULL,
				NULL,
				WEBKIT_INJECTED_CONTENT_FRAMES_ALL);
	}
	g_free(css);

	GtkCssProvider *css_provider = gtk_css_provider_new();
	gtk_style_context_add_provider_for_screen(
			gdk_display_get_default_screen(
				gdk_display_get_default()),
			GTK_STYLE_PROVIDER(css_provider),
			GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	gtk_css_provider_load_from_path(css_provider,
			BROWSER_STYLE,
			NULL);
	g_object_unref(css_provider);

	//create a gtk window
	b->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	//add signals to window
	g_signal_connect(b->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(b->window, "key-press-event", G_CALLBACK(key_press_event_handler), b);

	//show the window, the notebook and the current webview
	gtk_container_add(GTK_CONTAINER(b->window), GTK_WIDGET(b->notebook)); 
	gtk_widget_show(b->notebook);
	gtk_widget_show(GTK_WIDGET(wv));
	gtk_widget_show (b->window);
}

