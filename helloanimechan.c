// How to compile:
// g++ -o anime helloanimechen.c `pkg-config --cflags --libs gtk4` -lcurl

#include <gtk/gtk.h>
#include <curl/curl.h>
#include <string>

static size_t TheCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

static void print_citation(GtkWidget *button, gpointer data) 
{
	CURL *curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://animechan.vercel.app/api/random");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TheCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		
		readBuffer += "\n";		
		g_print(readBuffer.c_str());
	}
	else {
		g_print("Sorry, but it seems we were not able to connect to Animechan...\n");
	}
}


static void activate(GtkApplication *app, gpointer user_data) {
	GtkWidget *window;
	GtkWidget *button;

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Animechan Visual");
	gtk_window_set_default_size(GTK_WINDOW(window), 240, 200);

	button = gtk_button_new_with_label("Random Citation");
	g_signal_connect(button, "clicked", G_CALLBACK(print_citation), NULL);
	gtk_window_set_child(GTK_WINDOW(window), button);

	gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
	GtkApplication *app;
	int status;

	app = gtk_application_new("org.epic", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
