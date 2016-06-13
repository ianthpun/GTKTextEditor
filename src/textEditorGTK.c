/*
 ============================================================================
 Name        : textEditorGTK.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in GTK+
 ============================================================================
 */

#include <gtk/gtk.h>
#include "string.h"

struct fileDialog{
	GtkWidget *window;
	GtkWidget *buffer;
};


static void copyToClipboard(GtkButton* button,GtkTextBuffer *buffer){
	GtkTextIter startIter;
	GtkTextIter endIter;
	gtk_text_buffer_get_start_iter(buffer, &startIter);
	gtk_text_buffer_get_end_iter(buffer, &endIter);
	char * text = gtk_text_buffer_get_text (buffer,&startIter, &endIter, FALSE);

    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text (clipboard, text, -1);
	g_free(text);
}

static void selectAll(GtkButton* button,GtkTextBuffer *buffer){
	int txtLen = gtk_text_buffer_get_char_count (buffer);
	GtkTextIter startIter;
	GtkTextIter endIter;
	gtk_text_buffer_get_start_iter(buffer, &startIter);
	gtk_text_buffer_get_end_iter(buffer, &endIter);
	gtk_text_buffer_select_range(buffer,&startIter, &endIter);
}


static void clearTextbox(GtkButton* button,GtkTextBuffer *buffer){
	GtkTextIter startIter;
	GtkTextIter endIter;
	gtk_text_buffer_get_start_iter(buffer, &startIter);
	gtk_text_buffer_get_end_iter(buffer, &endIter);
	gtk_text_buffer_delete (buffer,&startIter, &endIter);

}


static void changeFontSerif(GtkButton* button,GtkWidget *textbox){
	PangoFontDescription *font_desc;
	font_desc = pango_font_description_from_string ("Serif");
	gtk_widget_modify_font (textbox, font_desc);
	pango_font_description_free (font_desc);
}

static void changeFontSansBold(GtkButton* button,GtkWidget *textbox){
	PangoFontDescription *font_desc;
	font_desc = pango_font_description_from_string ("Sans Bold");
	gtk_widget_modify_font (textbox, font_desc);
	pango_font_description_free (font_desc);
}

static void changeFontMono(GtkButton* button,GtkWidget *textbox){
	PangoFontDescription *font_desc;
    GtkStyle *defstyle;
    defstyle = gtk_widget_get_default_style ();

	font_desc = pango_font_description_from_string ("mono");
	gtk_widget_modify_font (textbox, font_desc);
	pango_font_description_free (font_desc);
}

//static void increaseFontsize(GtkButton* button, GtkWidget *textbox){
//	PangoFontDescription *font_desc;
//
//	font_desc = pango_font_description_from_string (fontsize.toString());
//	gtk_widget_modify_font (textbox, font_desc);
//	pango_font_description_free (font_desc);
//
//
//}
//
//static void decreaseFontsize(GtkButton* button, GtkWidget *textbox){
//
//}


static void saveAs(GtkButton* button,struct fileDialog *data)
{
	GtkWidget *dialog;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Save File", data->window, action,"_Cancel", GTK_RESPONSE_CANCEL,"_Save",GTK_RESPONSE_ACCEPT, NULL);
	chooser = GTK_FILE_CHOOSER (dialog);

	gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

	gtk_file_chooser_set_current_name (chooser, "Untitled document");

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	  {
	    char *filename;
	    filename = gtk_file_chooser_get_filename (chooser);
		GtkTextIter startIter;
		GtkTextIter endIter;
		gtk_text_buffer_get_start_iter(data->buffer, &startIter);
		gtk_text_buffer_get_end_iter(data->buffer, &endIter);
		char * text = gtk_text_buffer_get_text (data->buffer,&startIter, &endIter, FALSE);
		g_file_set_contents(filename, text, -1, NULL);
		g_free(text);
	    g_free(filename);
	  }

	gtk_widget_destroy (dialog);
}

static void openTxt(GtkButton* button, struct fileDialog *data){
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Open File", data->window, action,"_Cancel", GTK_RESPONSE_CANCEL,"_Save",GTK_RESPONSE_ACCEPT, NULL);
	gtk_window_set_transient_for(dialog, FALSE);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	  {
	    char *filename;
	    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	    filename = gtk_file_chooser_get_filename (chooser);
	    //open_file (filename);
	    char *text;
	    g_file_get_contents (filename,&text, NULL,NULL);
	    clearTextbox(NULL,data->buffer);
	    gtk_text_buffer_set_text (data->buffer, text, -1);
	    g_free(text);
	    g_free (filename);
	  }

	gtk_widget_destroy (dialog);
}


static void helpDialog(GtkButton* button, GtkWidget *parentWindow){
	GtkWidget *dialog;


	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
	dialog = gtk_message_dialog_new (parentWindow,
	                                 flags,
	                                 GTK_MESSAGE_ERROR,
	                                 GTK_BUTTONS_CLOSE,
	                                 "Ian Pun \n ipun@redhat.com \n ...Don't actually contact me please.");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);


}

int fontsize = 12;

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *vbox;

  GtkWidget *menubar;
  GtkWidget *fileMenu;
  GtkWidget *fileMi;
  GtkWidget *saveMi;
  GtkWidget *openMi;
  GtkWidget *quitMi;
  GtkWidget *slctAllMi;
  GtkWidget *clearButton;

  GtkWidget *helpMenu;
  GtkWidget *helpMi;
  GtkWidget *contactMi;

  GtkWidget *fontMenu;
  GtkWidget *fontMi;
  GtkWidget *font1Mi;
  GtkWidget *font2Mi;
  GtkWidget *font3Mi;

  GtkWidget *textbox;
  GtkTextBuffer *buffer;
  GtkWidget *scrolledWindow;

  GtkWidget *clipMi;
  GtkWidget *buttons;

  struct fileDialog file;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  gtk_window_set_title(GTK_WINDOW(window), "Simple Text Editor GTK");

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  // Menu Bar
  menubar = gtk_menu_bar_new();
  fileMenu = gtk_menu_new();
  helpMenu = gtk_menu_new();
  fontMenu = gtk_menu_new();

  fileMi = gtk_menu_item_new_with_label("File");
  helpMi = gtk_menu_item_new_with_label("Help");
  saveMi = gtk_menu_item_new_with_label("Save as");
  openMi = gtk_menu_item_new_with_label("Open");
  quitMi = gtk_menu_item_new_with_label("Quit");
  clipMi = gtk_menu_item_new_with_label("Copy All");

  contactMi = gtk_menu_item_new_with_label("Contact Us");
  slctAllMi = gtk_menu_item_new_with_label("Select All");

  fontMi  = gtk_menu_item_new_with_label("Fonts");
  font1Mi = gtk_menu_item_new_with_label("Serif");
  font2Mi = gtk_menu_item_new_with_label("Sans Bold");
  font3Mi = gtk_menu_item_new_with_label("Mono");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), slctAllMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), clipMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fontMi), fontMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fontMenu), font1Mi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fontMenu), font2Mi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fontMenu), font3Mi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fontMi);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpMi), helpMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu), contactMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), helpMi);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpMi), helpMenu);

  // end

  // text box
  textbox = gtk_text_view_new();
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textbox));
  gtk_text_buffer_set_text (buffer, "Hello", -1);
  scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolledWindow), textbox);
// end

  // clear text button

  clearButton = gtk_button_new_with_label ("Clear Text");
  buttons = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_button_box_set_layout (buttons,GTK_BUTTONBOX_END);
  gtk_container_add(GTK_CONTAINER(buttons), clearButton);

// end

  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), scrolledWindow, TRUE, TRUE, TRUE);

  gtk_box_pack_start(GTK_BOX(vbox), buttons, FALSE, FALSE, 0);

  file.window = window;
  file.buffer = buffer;

  g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(quitMi), "activate",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(slctAllMi), "activate",
        G_CALLBACK(selectAll), buffer);

  g_signal_connect(G_OBJECT(clearButton), "clicked",
        G_CALLBACK(clearTextbox), buffer);

  g_signal_connect(G_OBJECT(clipMi), "activate",
        G_CALLBACK(copyToClipboard), buffer);

  g_signal_connect(G_OBJECT(font1Mi), "activate",
        G_CALLBACK(changeFontSerif), textbox);

  g_signal_connect(G_OBJECT(font2Mi), "activate",
        G_CALLBACK(changeFontSansBold), textbox);

  g_signal_connect(G_OBJECT(font3Mi), "activate",
        G_CALLBACK(changeFontMono), textbox);

  g_signal_connect(G_OBJECT(saveMi), "activate",
        G_CALLBACK(saveAs), &file);

  g_signal_connect(G_OBJECT(openMi), "activate",
        G_CALLBACK(openTxt), &file);

  g_signal_connect(G_OBJECT(contactMi), "activate",
        G_CALLBACK(helpDialog), window);


  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}



