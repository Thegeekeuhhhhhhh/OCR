#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Reco_Network/neuralnetwork.h"

// Structure of the graphical user interface.
typedef struct UserInterface
{
    GtkWindow* window;                         // Main window
    GtkImage* image_display;                   // Image area
    GtkFileChooserButton* file_chooser_button;
    GtkButton* preprocessing_button;
    GtkButton* rotation_button;
    GtkButton* segmentation_button;
    GtkButton* ai_button;
    GtkButton* solver_button;
    GtkButton* ocr_button;
    GtkEntry* angle_entry;
} UserInterface;

typedef struct App
{
    UserInterface ui;
    char* filename;
} Game;

void on_set(GtkFileChooserButton *file_chooser_button, gpointer user_data)
{
    struct App *app = user_data;
    app->filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER
                                                   (file_chooser_button));
    gtk_image_set_from_file(app->ui.image_display, app->filename);

    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.rotation_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.segmentation_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.ai_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.solver_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.preprocessing_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.ocr_button), TRUE);
}

void on_preprocess(GtkButton *preprocessing_button, gpointer user_data)
{
     struct App *app = user_data;
     long i = 0, y = 1, c = 0;
     char *param1, param2, param3;
     while (app->ui.param_entry[i] != NULL)
	     if (app->ui.param_entry[i] == ' '){
	         if (y == 1)
		 {y ++; *(param1+c) = 0;c=0;}
                 if (y == 2)
	         {y ++; *(param2+c) = 0;c=0;}
                 if (y == 3)
	         {y ++; *(param3+c) = 0;c=0;}
	     }
	     if (y == 1)
	     {
             *(param1+c) = app->ui.param_entry[i];c++;
         }
         if (y == 2)
	     {
             *(param2+c) = app->ui.param_entry[i];c++;
         }
	     if (y == 3)
	     {
             *(param3+c) = app->ui.param_entry[i];c++;
         }

     //add first preprocess function
     exec("./",param1,param2);

     //add second preprocess function
     exec("./",param3);

     //add third preprocess function
     exec("./");



     app->filename = "process.png";

     gtk_image_set_from_file(app->ui.image_display, app->filename);

     gtk_widget_set_sensitive(GTK_WIDGET(app->ui.segmentation_button), TRUE);
     gtk_widget_set_sensitive(GTK_WIDGET(app->ui.rotation_button), TRUE);
     gtk_widget_set_sensitive(GTK_WIDGET(app->ui.ai_button), FALSE);
     gtk_widget_set_sensitive(GTK_WIDGET(app->ui.solver_button), FALSE);
}

void on_rotate(GtkButton *rotation_button, gpointer user_data)
{
    struct App *app = user_data;
    int angle = atoi(gtk_entry_get_text(app->ui.param_entry));

    //add rotation function
     exec("./rota %i",angle);

    gtk_image_set_from_file(appa->ui.image_display, app->filename);

    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.ai_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.solver_button), FALSE);
}

void on_segmentation(GtkButton *segmentation_button, gpointer user_data)
{
    struct App *app = user_data;

    //add segmentation function

    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.ai_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.solver_button), FALSE);
}

void on_ai(GtkButton *ai_button, gpointer user_data)
{
    struct App *app = user_data;
    // fonction pour les parametre: gtk_entry_get_text(app->ui.param_entry)
    //add character recognition function
    exec();
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.solver_button), TRUE);
}

void on_solve(GtkButton *ai_button, gpointer user_data)
{
    struct App *app = user_data;

    //add solver function
    exec("./sudoku_solver %s",gtk_entry_get_text(app->ui.param_entry));
    //add la nouvelle image avec les chiffres rouge
    exec("./                 %s %s.result",gtk_entry_get_text(app->ui.param_entry),gtk_entry_get_text(app->ui.param_entry));
    //app->filename = filename created by display function
    

    gtk_image_set_from_file(app->ui.image_display, app->filename);

    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.preprocessing_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.rotation_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.segmentation_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.ai_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.solver_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.ocr_button), FALSE);
}

void on_ocr(GtkButton *ocr_button, gpointer user_data)
{
    struct App *app = user_data;

    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.preprocessing_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.rotation_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.segmentation_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.ai_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.solver_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.ocr_button), FALSE);

    //be sad
}

int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    //Constructs a GtkBuilder instance.
    GtkBuilder* builder = gtk_builder_new ();

    //Loads the UI description.
    //(Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "app.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    //Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "ocr"));
    GtkImage* image_display = GTK_IMAGE(gtk_builder_get_object
                                        (builder, "image_display"));
    GtkFileChooserButton* file_chooser_button = GTK_FILE_CHOOSER_BUTTON
        (gtk_builder_get_object(builder, "file_chooser_button"));
    GtkButton* preprocessing_button = GTK_BUTTON(gtk_builder_get_object
                                                 (builder, "preprocessing_button"));
    GtkButton* rotation_button = GTK_BUTTON(gtk_builder_get_object
                                       (builder, "rotation_button"));;
    GtkButton* segmentation_button = GTK_BUTTON(gtk_builder_get_object
                                                (builder, "segmentation_button"));;
    GtkButton* ai_button = GTK_BUTTON(gtk_builder_get_object
                                      (builder, "ai_button"));;
    GtkButton* solver_button = GTK_BUTTON(gtk_builder_get_object
                                          (builder, "solver_button"));;
    GtkButton* ocr_button = GTK_BUTTON(gtk_builder_get_object
                                       (builder, "ocr_button"));;
    GtkEntry* angle_entry = GTK_ENTRY(gtk_builder_get_object
                                      (builder, "angle_entry"));;

    char filename[256];
    //Creates the "App" structure.
    struct App app =
        {
            .ui =
            {
                .window = window,
                .image_display = image_display,
                .file_chooser_button = file_chooser_button,
                .preprocessing_button = preprocessing_button,
                .rotation_button = rotation_button,
                .segmentation_button = segmentation_button,
                .ai_button = ai_button,
                .solver_button = solver_button,
                .ocr_button = ocr_button,
                    .angle_entry = angle_entry,
            },
            .filename = filename,
        };

    //Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(file_chooser_button, "file-set", G_CALLBACK(on_set), &app);
    g_signal_connect(preprocessing_button, "clicked", G_CALLBACK(on_preprocess),
                     &app);
    g_signal_connect(rotation_button, "clicked", G_CALLBACK(on_rotate), &app);
    g_signal_connect(segmentation_button, "clicked",
                     G_CALLBACK(on_segmentation), &app);
    g_signal_connect(ai_button, "clicked", G_CALLBACK(on_ai), &app);
    g_signal_connect(solver_button, "clicked", G_CALLBACK(on_solve), &app);
    g_signal_connect(ocr_button, "clicked", G_CALLBACK(on_ocr),
                     &app);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
