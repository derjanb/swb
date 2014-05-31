#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include "swb.h"
#include "config.h"
#include "helpers.h"

WebKitWebView *new_tab(Browser *b)
{
	WebKitWebView *wv = new_web_view(b);
	gtk_widget_show(GTK_WIDGET(wv));
	gtk_notebook_set_current_page(GTK_NOTEBOOK(b->notebook),
			gtk_notebook_append_page(GTK_NOTEBOOK(b->notebook), GTK_WIDGET(wv), NULL));
	return wv;
}

void find(Browser *b, char *NOT_USED)
{
	char *find_string = read_user_input(READ_ANY_CMD("Find :"));
	if(find_string!=NULL)
	{
		WebKitFindController *fc = webkit_web_view_get_find_controller(GET_CURRENT_WEB_VIEW(b));
		webkit_find_controller_search(fc, find_string,
			WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE||WEBKIT_FIND_OPTIONS_WRAP_AROUND, 1<<31);
	}
	g_free(find_string);
}

void find_next(Browser *b, char *NOT_USED)
{
	WebKitFindController *fc = webkit_web_view_get_find_controller(GET_CURRENT_WEB_VIEW(b));
	webkit_find_controller_search_next(fc);
}

void open_page(Browser *b, char *NOT_USED)
{
	const char *cur_uri = webkit_web_view_get_uri(GET_CURRENT_WEB_VIEW(b));
	if(cur_uri!=NULL)
	{
		char *uri = read_user_input(READ_URL_CMD(cur_uri, "Open:"));
		if(uri != NULL)
		{
			load_uri(GET_CURRENT_WEB_VIEW(b), uri);
			g_free(uri);
		}
	}
}

void tabopen_page(Browser *b, char *NOT_USED)
{
	const char *cur_uri = webkit_web_view_get_uri(GET_CURRENT_WEB_VIEW(b));
	if(cur_uri!=NULL)
	{
		char *uri = read_user_input(READ_URL_CMD(cur_uri, "Open:"));
		if(uri != NULL)
		{
			load_uri(new_tab(b), uri);
			g_free(uri);
		}
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

