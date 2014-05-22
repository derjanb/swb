#define _XOPEN_SOURCE
#include <webkit2/webkit2.h>
#include "swb.h"
#include "helpers.h"
#include "handlers.h"
#include "config.h"

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
					open_page(b);
					break;
				}
			case TABOPEN_KEY:
				{
					handled = TRUE;
					tabopen_page(b);
					break;
				}
			case CLOSE_TAB_KEY:
				{
					handled = TRUE;
					close_tab(b);
					break;
				}
			case FORWARD_KEY:
				{
					handled = TRUE;
					go_forward(b);
					break;
				}
			case BACKWARD_KEY:
				{
					handled = TRUE;
					go_back(b);
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

void update_title_signal_handler(GObject *gobj, GParamSpec *p, gpointer user_data)
{
	Browser *b = (Browser *)user_data;
	WebKitWebView *wv = (WebKitWebView *)gobj;
	if(webkit_web_view_get_title(wv)!=NULL)
	{
		set_tab_title(b, wv, webkit_web_view_get_title(wv));
	}
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
				set_tab_title(b, wv, "Loading");
				break;
			}
		case WEBKIT_LOAD_FINISHED:
			{
				const char *url = webkit_web_view_get_uri(wv);
				save_history(b, url);
				break;
			}
		default:
			{
				break;
			}
	}

	DEBUG("load_changed_signal_handler returned");
}
