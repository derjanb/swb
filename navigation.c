#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include "swb.h"
#include "config.h"
#include "helpers.h"

void enable_follow_hints(Browser *b, char *NOT_USED)
{
	webkit_web_view_run_javascript(GET_CURRENT_WEB_VIEW(b), "hintMode();", NULL, NULL, NULL);
	char *type_str = g_strdup_printf("%d", FOLLOW_HINT);
	char *cmd = READ_ANY_CMD("Follow:", type_str);
	exec_sh(b, cmd);
	g_free(cmd);
	g_free(type_str);
}

void follow_hint(Browser *b, char *hint)
{
	char *js = g_strdup_printf("judgeHintNum(%s);", hint);
	webkit_web_view_run_javascript(GET_CURRENT_WEB_VIEW(b), js, NULL, NULL, NULL);
	g_free(js);
	webkit_web_view_run_javascript(GET_CURRENT_WEB_VIEW(b), "removeHints();", NULL, NULL, NULL);
}
WebKitWebView *new_tab(Browser *b)
{
	WebKitWebView *wv = new_web_view(b);
	gtk_widget_show(GTK_WIDGET(wv));
	gtk_notebook_set_current_page(GTK_NOTEBOOK(b->notebook),
			gtk_notebook_append_page(GTK_NOTEBOOK(b->notebook), GTK_WIDGET(wv), NULL));
	return wv;
}

void find(Browser *b, char *find_string)
{
	if(find_string!=NULL)
	{
		WebKitFindController *fc = webkit_web_view_get_find_controller(GET_CURRENT_WEB_VIEW(b));
		webkit_find_controller_search(fc, find_string,
			WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE, 1<<31);
	}
}

void find_next(Browser *b, char *NOT_USED)
{
	WebKitFindController *fc = webkit_web_view_get_find_controller(GET_CURRENT_WEB_VIEW(b));
	webkit_find_controller_search_next(fc);
}


void open_page(Browser *b, char *uri)
{
	if(uri != NULL)
	{
		load_uri(GET_CURRENT_WEB_VIEW(b), uri);
	}
}

void tabopen_page(Browser *b, char *uri)
{
	if(uri != NULL)
	{
		load_uri(new_tab(b), uri);
	}
}

void close_tab(Browser *b, char *NOT_USED)
{
	if(gtk_notebook_get_n_pages(GTK_NOTEBOOK(b->notebook)) > 1)
	{
		int cur_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(b->notebook));
		WebKitWebView *wv = GET_CURRENT_WEB_VIEW(b);
		gtk_notebook_prev_page(GTK_NOTEBOOK(b->notebook));
		gtk_notebook_remove_page(GTK_NOTEBOOK(b->notebook), cur_page);
		gtk_widget_destroy(GTK_WIDGET(wv));
	}
}

void go_forward(Browser *b, char *NOT_USED)
{
	if(webkit_web_view_can_go_forward(GET_CURRENT_WEB_VIEW(b)))
	{
		webkit_web_view_go_forward(GET_CURRENT_WEB_VIEW(b));
	}
}

void go_back(Browser *b, char *NOT_USED)
{
	if(webkit_web_view_can_go_back(GET_CURRENT_WEB_VIEW(b)))
	{
		webkit_web_view_go_back(GET_CURRENT_WEB_VIEW(b));
	}
}

