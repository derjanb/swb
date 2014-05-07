#define _XOPEN_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <webkit2/webkit2.h>
#include "swb.h"
#include "config.h"

#define GET_CURRENT_WEB_VIEW(browser) \
	WEBKIT_WEB_VIEW(\
			gtk_notebook_get_nth_page(GTK_NOTEBOOK(browser->notebook),\
			gtk_notebook_get_current_page(GTK_NOTEBOOK(browser->notebook))))

#define DEBUGARG(format, ...) fprintf(stderr, format "\n", ##__VA_ARGS__)
#define DEBUG(format) fprintf(stderr, format "\n")

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
	webkit_settings_set_user_agent(webkit_web_view_get_settings(wv),
			USER_AGENT);
	g_signal_connect(wv, "create", G_CALLBACK(create_signal_handler), b);
	g_signal_connect(wv, "decide_policy", G_CALLBACK(decide_policy_signal_handler), b);
	g_signal_connect(wv, "load_changed", G_CALLBACK(load_changed_signal_handler), b);
	return wv;
}

WebKitWebView *new_tab(Browser *b)
{
	WebKitWebView *wv = new_web_view(b);
	gtk_widget_show(GTK_WIDGET(wv));
	gtk_notebook_set_current_page(GTK_NOTEBOOK(b->notebook),
			gtk_notebook_append_page(GTK_NOTEBOOK(b->notebook), GTK_WIDGET(wv), NULL));
	return wv;
}

char *read_url(char *buf)
{
	FILE *url_command_stream = popen(READ_URL_CMD, "r");

	DEBUGARG("%s", READ_URL_CMD);

	char *url = NULL;

	if(url_command_stream != NULL)
	{
		strcpy(buf,"http://");
		if(fgets(buf+strlen("http://"), BUF_LEN-1-strlen("http://"), url_command_stream) !=NULL)
		{
			if(strncmp("http", buf+strlen("http://"), strlen("http")) == 0)
			{
				url = buf+strlen("http://");
			}
			else
			{
				url = buf;
			}
		}
	}
	pclose(url_command_stream);
	return url;
}

void load_changed_signal_handler(WebKitWebView *wv,
		WebKitLoadEvent load_event,
		gpointer user_data)
{
	DEBUG("load_changed_signal_handler entered");

	Browser *b = (Browser *)user_data;

	switch (load_event)
	{
		case WEBKIT_LOAD_STARTED:
			{
				gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(b->notebook),
						GTK_WIDGET(GET_CURRENT_WEB_VIEW(b)),
						"Loading");
				break;
			}
		case WEBKIT_LOAD_FINISHED:
			{
				gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(b->notebook),
						GTK_WIDGET(GET_CURRENT_WEB_VIEW(b)),
						webkit_web_view_get_title(wv));

				//store url in history file
				sqlite3 *db;
				if(sqlite3_open(HISTORY_FILE, &db) == 0)
				{
					const char *url = webkit_web_view_get_uri(wv);
					char sql[BUF_LEN];
					sprintf(sql, "CREATE TABLE IF NOT EXISTS history(url varchar primary key, hits int);"\
							"INSERT OR IGNORE INTO history VALUES(\"%s\", 0);"\
							"UPDATE history SET hits = hits + 1 WHERE \"%s\"=url", url, url);
					sqlite3_exec(db, sql, NULL, NULL, NULL);
				}
				sqlite3_close(db);
				break;
			}
		default:
			{
				break;
			}
	}

	DEBUG("load_changed_signal_handler returned");
}
gboolean key_press_event_handler(GtkWidget *window,
		GdkEvent *event,
		gpointer user_data)
{
	DEBUG("key_press_event_signal_handler entered");

	Browser *b = (Browser *)user_data;
	gboolean handled = FALSE;

	guint keyval;

	gdk_event_get_keyval(event, &keyval);

	if(b->mode == COMMAND || keyval == COMMAND_MODE_KEY)
	{
		switch(keyval)
		{
			case OPEN_KEY:
				{
					handled = TRUE;
					char buf[BUF_LEN];
					char *url = read_url(buf);
					if(url != NULL)
					{
						webkit_web_view_load_uri(GET_CURRENT_WEB_VIEW(b),
								url);
					}
					break;
				}
			case TABOPEN_KEY:
				{
					handled = TRUE;
					char buf[BUF_LEN];
					char *url = read_url(buf);
					if(url != NULL)
					{
						webkit_web_view_load_uri(new_tab(b),
								url);
					}
					break;
				}
			case CLOSE_TAB_KEY:
				{
					handled = TRUE;
					if(gtk_notebook_get_n_pages(GTK_NOTEBOOK(b->notebook)) > 1)
					{
						int cur_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(b->notebook));
						WebKitWebView *wv = GET_CURRENT_WEB_VIEW(b);
						gtk_notebook_prev_page(GTK_NOTEBOOK(b->notebook));
						gtk_notebook_remove_page(GTK_NOTEBOOK(b->notebook), cur_page);
						gtk_widget_destroy(GTK_WIDGET(wv));
					}
					break;
				}
			case FORWARD_KEY:
				{
					handled = TRUE;
					if(webkit_web_view_can_go_forward(GET_CURRENT_WEB_VIEW(b)))
					{
						webkit_web_view_go_forward(GET_CURRENT_WEB_VIEW(b));
					}
					break;
				}
			case BACKWARD_KEY:
				{
					handled = TRUE;
					if(webkit_web_view_can_go_back(GET_CURRENT_WEB_VIEW(b)))
					{
						webkit_web_view_go_back(GET_CURRENT_WEB_VIEW(b));
					}
				break;
			}
			case INPUT_MODE_KEY:
				{
					handled = TRUE;
					b->mode = INPUT;
					break;
				}
			case COMMAND_MODE_KEY:
				{
					handled = TRUE;
					b->mode = COMMAND;
					break;
				}
		}
	}
	DEBUGARG("key_press_event_signal_handler returns %d", handled);
	return handled;
}

GtkWidget* create_signal_handler(WebKitWebView *web_view,
		gpointer user_data)
{
	DEBUG("create_signal_handler entered");

	Browser *b=(Browser *)user_data;

	WebKitWebView *wv = new_tab(b);

	DEBUG("create_signal_handler returns");
	return GTK_WIDGET(wv);
}

gboolean decide_policy_signal_handler(WebKitWebView *web_view,
		WebKitPolicyDecision *decision,
		WebKitPolicyDecisionType type,
		gpointer user_data)
{
	DEBUG("decide_policy_signal_handler entered");

	gboolean handled = FALSE;

	switch(type)
	{
		case WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION:
			webkit_policy_decision_use(decision);
			handled = TRUE;
			break;
		default:
			break;
	}
	DEBUGARG("decide_policy_signal_handler returns: %d", handled);
	return handled;
}

void setup_browser(Browser *b)
{
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
	gtk_notebook_append_page(GTK_NOTEBOOK(b->notebook), GTK_WIDGET(new_web_view(b)), NULL);
	webkit_web_view_load_uri(GET_CURRENT_WEB_VIEW(b), HOME_PAGE);

	//set custom stylesheet for webviewgroup
	FILE *style_css_fd = fopen(STYLESHEET_FILE, "r");
	if(style_css_fd!=NULL)
	{
		fseek(style_css_fd, 0L, SEEK_END);
		int css_size = ftell(style_css_fd);
		char *css = malloc(css_size+1);
		fseek(style_css_fd, 0L, SEEK_SET);
		fread(css, sizeof(char), css_size, style_css_fd);
		css[css_size] = '\0';
		fclose(style_css_fd);
		webkit_web_view_group_add_user_style_sheet(
		webkit_web_view_get_group(WEBKIT_WEB_VIEW(GET_CURRENT_WEB_VIEW(b))),
		css,
		NULL,
		NULL,
		NULL,
		WEBKIT_INJECTED_CONTENT_FRAMES_ALL);
		free(css);
	}

	//create a gtk window
	b->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	//add signals to window
	g_signal_connect(b->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(b->window, "key-press-event", G_CALLBACK(key_press_event_handler), b);

	//show the window, the notebook and the current webview
	gtk_container_add(GTK_CONTAINER(b->window), GTK_WIDGET(b->notebook)); 
	gtk_widget_show(b->notebook);
	gtk_widget_show(GTK_WIDGET(GET_CURRENT_WEB_VIEW(b)));
	gtk_widget_show (b->window);
}

