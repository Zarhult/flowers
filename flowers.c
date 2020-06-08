#include <gtk/gtk.h>

#define FLWR_NUM 10
#define FLWR_WIDGET_NUM 9

/* used for specifying which side of the display to interact with */
typedef enum { LEFT, RIGHT } Side;

/* array indices for flower struct's info string array  */
typedef enum {
    FILENAME,
    SEPALS,
    PETALS,
    STAMENS,
    FLWR_INFO_ENUM_NUM /* equal to the number of other enums here */
} FlwrInfo;

typedef enum { FIVE_PARTED, FOUR_PARTED, THREE_PARTED } FlwrFam;

/* matches the order of the flowers defined in flower array in activate function */
typedef enum {
    SOLANACEAE,
    HYDROPHYLLACEAE,
    BORAGINACEAE,
    LAMIACEAE,
    SCROPHULARIACEAE,
    BRASSICACEAE,
    ONAGRACEAE,
    PAPAVERACEAE,
    POLYGONACEAE,
    LILIACEAE
} FlwrSubfam;

typedef enum { INFERIOR, SUPERIOR } OvaryPos;

/* order of widgets in left/right flower widget arrays */
typedef enum {
    BOX, /* contains all the other widgets of a side, including some outside of the array */
    COMBO_BOX,
    IMAGE, /* program assumes nothing but labels follow here */
    PETALS_LABEL,
    SEPALS_LABEL,
    STAMENS_LABEL,
    OVARY_LABEL,
    FAMILY_LABEL,
    NICKNAME_LABEL
} SideWidget;

typedef struct {
    gchar *info[FLWR_INFO_ENUM_NUM];
    FlwrFam family;
    OvaryPos ovaryPos;
} Flower;

/* to be passed to callback functions, effectively like a parameter */
typedef struct {
    Side direction;
} SideData;

/* evil global variables */
static GtkWidget *window;
static GtkWidget *box; /* top-level horizontal box that holds two vertical boxes */

static GtkWidget *l_widgets[FLWR_WIDGET_NUM]; /* widgets for flower info */
static GtkWidget *l_dummy_label; /* fills up space to push button to bottom */
static GtkWidget *l_toggle_button;
static gboolean l_shown = FALSE; /* whether or not left flower is shown */
static SideData l_data = {LEFT};

static GtkWidget *r_widgets[FLWR_WIDGET_NUM]; /* widgets for flower info */
static GtkWidget *r_dummy_label; /* fills up space to push button to bottom */
static GtkWidget *r_toggle_button;
static gboolean r_shown = FALSE; /* whether or not right flower is shown */
static SideData r_data = {RIGHT};

static Flower flowers[FLWR_NUM];

/* functions for converting enums to string info */
static gchar *fam_to_string(FlwrFam);
static gchar *subfam_to_string(FlwrSubfam);
static gchar *subfam_to_nickname_string(FlwrSubfam);
static gchar *ovary_pos_to_string(OvaryPos);

/* functions for gui */
static void display_flower(Side, FlwrSubfam);
static void on_button_click(GtkButton *, gpointer);
static void on_combo_box_change(GtkComboBox *, gpointer);

/* main gui function */
static void activate(GtkApplication *, gpointer);

static gchar *
fam_to_string(FlwrFam f) {
    switch (f) {
        case FIVE_PARTED:
            return "5-parted";
            break;
        case FOUR_PARTED:
            return "4-parted";
            break;
        case THREE_PARTED:
            return "3-parted";
            break;
    }
}

static gchar *
subfam_to_string(FlwrSubfam s) {
    switch (s) {
        case SOLANACEAE:
            return "Solanaceae";
            break;
        case HYDROPHYLLACEAE:
            return "Hydrophyllaceae";
            break;
        case BORAGINACEAE:
            return "Boraginaceae";
            break;
        case LAMIACEAE:
            return "Lamiaceae";
            break;
        case SCROPHULARIACEAE:
            return "Scrophulariaceae";
            break;
        case BRASSICACEAE:
            return "Brassicaceae";
            break;
        case ONAGRACEAE:
            return "Onagraceae";
            break;
        case PAPAVERACEAE:
            return "Papaveraceae";
            break;
        case POLYGONACEAE:
            return "Polygonaceae";
            break;
        case LILIACEAE:
            return "Liliaceae";
            break;
    }
}

static gchar *
subfam_to_nickname_string(FlwrSubfam s) {
    switch (s) {
        case SOLANACEAE:
            return "Nightshade";
            break;
        case HYDROPHYLLACEAE:
            return "Waterleaf";
            break;
        case BORAGINACEAE:
            return "Borage";
            break;
        case LAMIACEAE:
            return "Mint";
            break;
        case SCROPHULARIACEAE:
            return "Snapdragon";
            break;
        case BRASSICACEAE:
            return "Mustard";
            break;
        case ONAGRACEAE:
            return "Evening Pimrose";
            break;
        case PAPAVERACEAE:
            return "Poppy";
            break;
        case POLYGONACEAE:
            return "Buckwheat";
            break;
        case LILIACEAE:
            return "Lily";
            break;
    }
}

static gchar *
ovary_pos_to_string(OvaryPos p) {
    switch (p) {
        case INFERIOR:
            return "Inferior";
            break;
        case SUPERIOR:
            return "Superior";
            break;
    }
}

static void 
display_flower(Side grid, FlwrSubfam f) {
    gchar filepath_buf[50] = "images/";
    gchar petals_label_buf[100] = "Petals: ";
    gchar sepals_label_buf[100] = "Sepals: ";
    gchar stamens_label_buf[100] = "Stamens: ";
    gchar ovary_label_buf[100] = "Ovary: ";
    gchar family_label_buf[100] = "Family: ";
    gchar nickname_label_buf[100] = "Nickname: ";
    gchar *label_bufs[7] = {filepath_buf, petals_label_buf, sepals_label_buf,
                            stamens_label_buf, ovary_label_buf, family_label_buf, 
                            nickname_label_buf};
    gint i;

    /* prepare info strings */
    g_strlcat(filepath_buf, flowers[f].info[FILENAME], sizeof(filepath_buf));
    g_strlcat(petals_label_buf, flowers[f].info[PETALS], sizeof(petals_label_buf));
    g_strlcat(sepals_label_buf, flowers[f].info[SEPALS], sizeof(sepals_label_buf));
    g_strlcat(stamens_label_buf, flowers[f].info[STAMENS], sizeof(stamens_label_buf));
    g_strlcat(ovary_label_buf, ovary_pos_to_string(flowers[f].ovaryPos), sizeof(family_label_buf));
    g_strlcat(family_label_buf, fam_to_string(flowers[f].family), sizeof(family_label_buf));
    g_strlcat(nickname_label_buf, subfam_to_nickname_string(f), sizeof(stamens_label_buf));

    /* set new flower info */
    if (grid == LEFT) {
        gtk_image_set_from_file(GTK_IMAGE(l_widgets[IMAGE]), filepath_buf);
        gtk_widget_set_size_request(l_widgets[BOX], 640, -1);

        for (i = PETALS_LABEL; i <= NICKNAME_LABEL; ++i)
            gtk_label_set_text(GTK_LABEL(l_widgets[i]), label_bufs[i - 2]);
    } else if (grid == RIGHT) {
        gtk_image_set_from_file(GTK_IMAGE(r_widgets[IMAGE]), filepath_buf);
        gtk_widget_set_size_request(r_widgets[BOX], 640, -1);

        for (i = PETALS_LABEL; i <= NICKNAME_LABEL; ++i)
            gtk_label_set_text(GTK_LABEL(r_widgets[i]), label_bufs[i - 2]);
    }
}

static void 
on_button_click(GtkButton *b, gpointer user_data) {
    SideData *data = user_data;
    SideWidget i;

    if (data->direction == LEFT) {
        if (l_shown) {
            for (i = IMAGE; i < G_N_ELEMENTS(l_widgets); ++i)
                gtk_widget_hide(l_widgets[i]);

            l_shown = FALSE;
        } else {
            for (i = IMAGE; i < G_N_ELEMENTS(l_widgets); ++i)
                gtk_widget_show(l_widgets[i]);

            l_shown = TRUE;
        }
    } else if (data->direction == RIGHT) {
        if (r_shown) {
            for (i = IMAGE; i < G_N_ELEMENTS(l_widgets); ++i)
                gtk_widget_hide(r_widgets[i]);

            r_shown = FALSE;
        } else {
            for (i = IMAGE; i < G_N_ELEMENTS(l_widgets); ++i)
                gtk_widget_show(r_widgets[i]);

            r_shown = TRUE;
        }
    }
}

static void
on_combo_box_change(GtkComboBox *combo_box, gpointer user_data) {
    FlwrSubfam chosen_flower = (FlwrSubfam)gtk_combo_box_get_active(combo_box);
    SideData *data = user_data;
  
    if (data->direction == LEFT)
        display_flower(LEFT, chosen_flower);
    else if (data->direction == RIGHT)
        display_flower(RIGHT, chosen_flower);
}

static void 
activate(GtkApplication *app, gpointer user_data) {
    gint i;
    /* prepare flower info */
    flowers[SOLANACEAE] = (Flower)
    {{"solanaceae.png", "5, partly fused",
      "5, mostly fused", "5, joined to petals"},
      FIVE_PARTED,
      SUPERIOR};

    flowers[HYDROPHYLLACEAE] = (Flower)
    {{"hydrophyllaceae.png", "5, partly fused",
      "5, joined to form a tube", "5, attached to the corolla tube"},
      FIVE_PARTED,
      SUPERIOR};

    flowers[BORAGINACEAE] = (Flower)
    {{"boraginaceae.png", "5, partly fused",
      "5, joined to form a tube", "5, attached to the corolla tube"},
      FIVE_PARTED,
      SUPERIOR};

    flowers[LAMIACEAE] = (Flower)
    {{"lamiaceae.png", "often 5, fused",
      "5, irregular, divided into two lips and joined midway to form a tube",
      "2 or 4"},
      FIVE_PARTED,
      SUPERIOR};

    flowers[SCROPHULARIACEAE] = (Flower)
    {{"scrophulariaceae.png", "5, partly fused",
      "5, arranged in two-lipped petal lobes joined to form a tube",
      "usually 4, sometimes 2 or 5"},
      FIVE_PARTED,
      SUPERIOR};

    flowers[BRASSICACEAE] = (Flower)
    {{"brassicaceae.png", "4", "4, cross-like", "6, of 2 lengths"},
      FOUR_PARTED,
      SUPERIOR};

    flowers[ONAGRACEAE] = (Flower)
    {{"onagraceae.png", "4", "4", "4 or 8"}, 
      FOUR_PARTED, 
      INFERIOR};

    flowers[PAPAVERACEAE] = (Flower)
    {{"papaveraceae.png", "2 or 3, often fall away as the flowers open",
      "4 or 6", "6 to numerous"},
      FOUR_PARTED,
      SUPERIOR};

    flowers[POLYGONACEAE] = (Flower)
    {{"polygonaceae.png", "3 tepals (rather than sepals)",
      "3 tepals (rather than petals)", "3 to 9"},
      THREE_PARTED,
      SUPERIOR};

    flowers[LILIACEAE] = (Flower)
    {{"liliaceae.png", "6 colored tepals (rather than sepals)",
      "6 colored tepals (rather than petals)", "6 (sometimes 3)"},
      THREE_PARTED,
      SUPERIOR};

    /* prepare window */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Flowers");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

    /* place a box in the window */
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    /* split box in to two vertical boxes for flower info */
    l_widgets[BOX] = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    r_widgets[BOX] = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(box), l_widgets[BOX]);
    gtk_container_add(GTK_CONTAINER(box), r_widgets[BOX]);

    /* prepare dropdowns */
    l_widgets[COMBO_BOX] = gtk_combo_box_text_new();
    for (i = 0; i < FLWR_NUM; i++)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(l_widgets[COMBO_BOX]), subfam_to_string(i));

    gtk_container_add(GTK_CONTAINER(l_widgets[BOX]), l_widgets[COMBO_BOX]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(l_widgets[COMBO_BOX]), 0);

    r_widgets[COMBO_BOX] = gtk_combo_box_text_new();
    for (i = 0; i < FLWR_NUM; i++)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(r_widgets[COMBO_BOX]), subfam_to_string(i));

    gtk_container_add(GTK_CONTAINER(r_widgets[BOX]), r_widgets[COMBO_BOX]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(r_widgets[COMBO_BOX]), 1);

    /* initialize empty widgets to be updated by display_flower */
    l_widgets[IMAGE] = gtk_image_new();
    for (i = PETALS_LABEL; i < G_N_ELEMENTS(l_widgets); ++i)
        l_widgets[i] = gtk_label_new("");

    r_widgets[IMAGE] = gtk_image_new();
    for (i = PETALS_LABEL; i < G_N_ELEMENTS(r_widgets); ++i)
        r_widgets[i] = gtk_label_new("");

    /* add them to left and right boxes */
    for (i = IMAGE; i < G_N_ELEMENTS(l_widgets); ++i)
        gtk_container_add(GTK_CONTAINER(l_widgets[BOX]), l_widgets[i]);

    for (i = IMAGE; i < G_N_ELEMENTS(r_widgets); ++i)
        gtk_container_add(GTK_CONTAINER(r_widgets[BOX]), r_widgets[i]);

    /* add visiblity toggle buttons, at the bottom of the window with dummy label
     * filling the remaining space */
    l_dummy_label = gtk_label_new("");
    r_dummy_label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(l_widgets[BOX]), l_dummy_label);
    gtk_container_add(GTK_CONTAINER(r_widgets[BOX]), r_dummy_label);

    gtk_widget_set_vexpand(l_dummy_label, TRUE);
    gtk_widget_set_vexpand(r_dummy_label, TRUE);

    l_toggle_button = gtk_button_new_with_label("Toggle display");
    r_toggle_button = gtk_button_new_with_label("Toggle display");

    gtk_box_pack_end(GTK_BOX(l_widgets[BOX]), l_toggle_button, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(r_widgets[BOX]), r_toggle_button);

    /* connect button click signals to callback */
    g_signal_connect(GTK_BUTTON(l_toggle_button), "clicked", G_CALLBACK(on_button_click), &l_data);
    g_signal_connect(GTK_BUTTON(r_toggle_button), "clicked", G_CALLBACK(on_button_click), &r_data);

    /* connect dropdown selection signals to callback */
    g_signal_connect(GTK_COMBO_BOX(l_widgets[COMBO_BOX]), "changed",
                     G_CALLBACK(on_combo_box_change), &l_data);
    g_signal_connect(GTK_COMBO_BOX(r_widgets[COMBO_BOX]), "changed",
                     G_CALLBACK(on_combo_box_change), &r_data);

    /* fix window size */
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    /* start with comparison of solanaceae to hydrophyllaceae */
    display_flower(LEFT, SOLANACEAE);
    display_flower(RIGHT, HYDROPHYLLACEAE);

    gtk_widget_show_all(window);
    l_shown = TRUE;
    r_shown = TRUE;
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.github.zarhult.flowers", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
