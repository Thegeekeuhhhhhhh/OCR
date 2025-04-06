#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "./Reco_Network/neuralnetwork.h"
#include "./line/line.h"
#include "./pretreatment/pretreatment.h"
#include "./pretreatment/reconstruction.c"
#include <string.h>

// Structure of the graphical user interface.
typedef struct UserInterface
{
    GtkWindow* window; // Main window
    GtkImage* image_display; // Image area
    GtkFileChooserButton* file_chooser_button;
    GtkButton* preprocessing_button;
    GtkButton* rotation_button;
    GtkButton* segmentation_button;
    GtkButton* ai_button;
    GtkButton* solver_button;
    GtkButton* ocr_button;
    GtkEntry* param_entry;
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

SDL_Surface* GtkImageToSDLSurface(GtkImage* gtkImage)
{
    // Get the GdkPixbuf from GtkImage
    GdkPixbuf* pixbuf = gtk_image_get_pixbuf(gtkImage);
    if (!pixbuf)
    {
        return NULL;
    }

    // Get information about the GdkPixbuf
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);

    // Create an SDL_Surface
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
            pixels, width, height, n_channels * 8, rowstride, 0xff0000, 0x00ff00, 0x0000ff, 0);

    // Return the created SDL_Surface
    return surface;
}

void on_preprocess(GtkButton *preprocessing_button, gpointer user_data)
{
    struct App *app = user_data;
    long i = 0, y = 1, c = 0;
    char param1[100];
    char param2[100];
    char param3[100];
    for (int i = 0; i < 100; i++)
    {
        param1[i] = 0;
        param2[i] = 0;
        param3[i] = 0;
    }
    param1[0] = '0';
    param2[0] = '0';
    param3[0] = '0';
    const gchar *test = gtk_entry_get_text(app->ui.param_entry);
    while (test[i] != NULL)
    {
        if (test[i] == ' ')
        {
            y++;
            c = 0;
        }
        else 
        {
            if (y == 1)
            {
                param1[c] = test[i];
            }
            if (y == 2)
            {
                param2[c] = test[i];
            }
            if (y == 3)
            {
                param3[c] = test[i];
            }
            c++;
        }
        i++;
    }
    printf("\n");
    printf("%s, %s, %s\n", param1, param2, param3);

    SDL_Surface* one = GtkImageToSDLSurface(app->ui.image_display);
    SDL_Surface* two = SDL_CreateRGBSurfaceWithFormat(0, one->w, one->h, 32, SDL_PIXELFORMAT_RGBA32);

    convertToGrayscale(one,two);
    dilateImage(two, one, atof(param1));
    applySauvolaFilter(one,two, atoi(param2), atof(param3));

    applySobelFilter(two,one);
    SDL_SaveBMP(one, "sob.bmp");
    SDL_SaveBMP(two, "sauv.bmp");

    SDL_FreeSurface(one);
    SDL_FreeSurface(two);

    app->filename = "sauv.bmp";
    //free(app->ui.image_display);

    //gtk_image_set_from_file(app->ui.image_display, app->filename);
    //gtk_image_clear(app->ui.image_display);
    //app->ui.image_display = GTK_IMAGE(gtk_image_new_from_file("sauv.bmp"));
    //GtkImage *image = GTK_IMAGE(app->ui.image_display);
    //gtk_image_set_from_file(GTK_IMAGE(image), app->filename);

    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.segmentation_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.rotation_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.ai_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.solver_button), FALSE);

    printf("END OF PREPROCESS\n");
}

void on_rotate(GtkButton *rotation_button, gpointer user_data)
{
    struct App *app = user_data;

    char *args = {"rota", gtk_entry_get_text(app->ui.param_entry)};
    //add rotation function
    execvp("rota", args);

    gtk_image_set_from_file(app->ui.image_display, app->filename);

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

    DIR *d;
    struct dirent *dir;
    d = opendir("./line/output");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            int res = guess("./Reco_Network/1K_Training", dir->d_name);
        }
        closedir(d);
    }
    gtk_widget_set_sensitive(GTK_WIDGET(app->ui.solver_button), TRUE);
}

void on_solve(GtkButton *ai_button, gpointer user_data)
{
    struct App *app = user_data;

    char *args[] = {"sudoku_solver", gtk_entry_get_text(app->ui.param_entry)};
    //add solver function
    execvp("sudoku_solver", args);

    char buffer[256];
    asprintf(&buffer, "%s.result", gtk_entry_get_text(app->ui.param_entry));
    char *args2[] = {"[NOM DE LA FONCTION]", gtk_entry_get_text(app->ui.param_entry), buffer};
    //add la nouvelle image avec les chiffres rouge
    execvp("[NOM ENCORE]", args2);
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
    if (gtk_builder_add_from_file(builder, "ui.glade", &error) == 0)
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
            (builder, "rotation_button"));
    GtkButton* segmentation_button = GTK_BUTTON(gtk_builder_get_object
            (builder, "segmentation_button"));
    GtkButton* ai_button = GTK_BUTTON(gtk_builder_get_object
            (builder, "ai_button"));
    GtkButton* solver_button = GTK_BUTTON(gtk_builder_get_object
            (builder, "solver_button"));
    GtkButton* ocr_button = GTK_BUTTON(gtk_builder_get_object
            (builder, "ocr_button"));
    GtkEntry* param_entry = GTK_ENTRY(gtk_builder_get_object
            (builder, "param_entry"));

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
            .param_entry = param_entry,
        },
        .filename = filename,
    };

    //Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(file_chooser_button, "file-set", G_CALLBACK(on_set), &app);
    g_signal_connect(preprocessing_button, "clicked", G_CALLBACK(on_preprocess), &app);
    g_signal_connect(rotation_button, "clicked", G_CALLBACK(on_rotate), &app);
    g_signal_connect(segmentation_button, "clicked", G_CALLBACK(on_segmentation), &app);
    g_signal_connect(ai_button, "clicked", G_CALLBACK(on_ai), &app);
    g_signal_connect(solver_button, "clicked", G_CALLBACK(on_solve), &app);
    g_signal_connect(ocr_button, "clicked", G_CALLBACK(on_ocr), &app);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
