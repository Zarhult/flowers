#include <gtk/gtk.h>

#define FLOWER_COUNT 10
#define FLOWER_WIDGET_COUNT 9

/* used for specifying which side of the display to interact with */
typedef enum side { LEFT, RIGHT } Side;

/* array indices for flower struct's info string array  */
typedef enum flowerInfo {
  FILENAME,
  SEPALS,
  PETALS,
  STAMENS,
  FLOWER_INFO_ENUM_COUNT /* equal to the number of other enums */
} FlowerInfo;

typedef enum flowerFamily {
  FIVE_PARTED,
  FOUR_PARTED,
  THREE_PARTED
} FlowerFamily;

/* matches the order of the flowers defined in flower array in activate function
 */
typedef enum flowerSubfamily {
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
} FlowerSubfamily;

typedef enum ovaryPosition { INFERIOR, SUPERIOR } OvaryPosition;

/* order of widgets in left/right flower widget arrays */
typedef enum sideWidget {
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

typedef struct flower {
  gchar *info[FLOWER_INFO_ENUM_COUNT];
  FlowerFamily family;
  OvaryPosition ovaryPos;
} Flower;

/* to be passed to callback functions, effectively like a parameter */
typedef struct directionUserData {
  Side direction;
} DirectionUserData;

/* evil global variables */
GtkWidget *window;
GtkWidget *box; /* top-level horizontal box that holds two vertical boxes */

GtkWidget *left_widgets[FLOWER_WIDGET_COUNT]; /* widgets for flower info */
GtkWidget *left_dummy_label; /* fills up space to push button to bottom */
GtkWidget *left_toggle_button;
gboolean left_shown = FALSE; /* whether or not left flower is shown */
DirectionUserData left_data = {LEFT};

GtkWidget *right_widgets[FLOWER_WIDGET_COUNT]; /* widgets for flower info */
GtkWidget *right_dummy_label; /* fills up space to push button to bottom */
GtkWidget *right_toggle_button;
gboolean right_shown = FALSE; /* whether or not right flower is shown */
DirectionUserData right_data = {RIGHT};

Flower flowers[FLOWER_COUNT];

static gchar *family_to_string(FlowerFamily fam) {
  switch (fam) {
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

static gchar *subfamily_to_string(FlowerSubfamily subFam) {
  switch (subFam) {
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

static gchar *ovary_pos_to_string(OvaryPosition pos) {
  switch (pos) {
  case INFERIOR:
    return "Inferior";
    break;
  case SUPERIOR:
    return "Superior";
    break;
  }
}

static gchar *subfamily_to_nickname_string(FlowerSubfamily subFam) {
  switch (subFam) {
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

static void display_flower(Side grid, FlowerSubfamily flwr) {
  gchar filepath_buf[50] = "images/";
  gchar petals_label_buf[100] = "Petals: ";
  gchar sepals_label_buf[100] = "Sepals: ";
  gchar stamens_label_buf[100] = "Stamens: ";
  gchar ovary_label_buf[100] = "Ovary: ";
  gchar family_label_buf[100] = "Family: ";
  gchar nickname_label_buf[100] = "Nickname: ";

  /* prepare info strings */
  g_strlcat(filepath_buf, flowers[flwr].info[FILENAME], sizeof(filepath_buf));
  g_strlcat(petals_label_buf, flowers[flwr].info[PETALS],
            sizeof(petals_label_buf));
  g_strlcat(sepals_label_buf, flowers[flwr].info[SEPALS],
            sizeof(sepals_label_buf));
  g_strlcat(stamens_label_buf, flowers[flwr].info[STAMENS],
            sizeof(stamens_label_buf));
  g_strlcat(ovary_label_buf, ovary_pos_to_string(flowers[flwr].ovaryPos),
            sizeof(family_label_buf));
  g_strlcat(family_label_buf, family_to_string(flowers[flwr].family),
            sizeof(family_label_buf));
  g_strlcat(nickname_label_buf, subfamily_to_nickname_string(flwr),
            sizeof(stamens_label_buf));

  /* set new flower info */
  if (grid == LEFT) {
    gtk_image_set_from_file(GTK_IMAGE(left_widgets[IMAGE]), filepath_buf);
    gtk_widget_set_size_request(left_widgets[BOX], 640, -1);
    gtk_label_set_text(GTK_LABEL(left_widgets[PETALS_LABEL]), petals_label_buf);
    gtk_label_set_text(GTK_LABEL(left_widgets[SEPALS_LABEL]), sepals_label_buf);
    gtk_label_set_text(GTK_LABEL(left_widgets[STAMENS_LABEL]),
                       stamens_label_buf);
    gtk_label_set_text(GTK_LABEL(left_widgets[OVARY_LABEL]), ovary_label_buf);
    gtk_label_set_text(GTK_LABEL(left_widgets[FAMILY_LABEL]), family_label_buf);
    gtk_label_set_text(GTK_LABEL(left_widgets[NICKNAME_LABEL]),
                       nickname_label_buf);
  } else if (grid == RIGHT) {
    gtk_image_set_from_file(GTK_IMAGE(right_widgets[IMAGE]), filepath_buf);
    gtk_widget_set_size_request(right_widgets[BOX], 640, -1);
    gtk_label_set_text(GTK_LABEL(right_widgets[PETALS_LABEL]),
                       petals_label_buf);
    gtk_label_set_text(GTK_LABEL(right_widgets[SEPALS_LABEL]),
                       sepals_label_buf);
    gtk_label_set_text(GTK_LABEL(right_widgets[STAMENS_LABEL]),
                       stamens_label_buf);
    gtk_label_set_text(GTK_LABEL(right_widgets[OVARY_LABEL]), ovary_label_buf);
    gtk_label_set_text(GTK_LABEL(right_widgets[FAMILY_LABEL]),
                       family_label_buf);
    gtk_label_set_text(GTK_LABEL(right_widgets[NICKNAME_LABEL]),
                       nickname_label_buf);
  }
}

static void on_button_click(GtkButton *button, gpointer user_data) {
  DirectionUserData *data = user_data;
  gint i;

  if (data->direction == LEFT) {

    if (left_shown) {
      for (i = IMAGE; i < G_N_ELEMENTS(left_widgets); ++i) {
        gtk_widget_hide(left_widgets[i]);
      }

      left_shown = FALSE;
    } else {
      for (i = IMAGE; i < G_N_ELEMENTS(left_widgets); ++i) {
        gtk_widget_show(left_widgets[i]);
      }

      left_shown = TRUE;
    }

  } else if (data->direction == RIGHT) {

    if (right_shown) {
      for (i = IMAGE; i < G_N_ELEMENTS(left_widgets); ++i) {
        gtk_widget_hide(right_widgets[i]);
      }

      right_shown = FALSE;
    } else {
      for (i = IMAGE; i < G_N_ELEMENTS(left_widgets); ++i) {
        gtk_widget_show(right_widgets[i]);
      }

      right_shown = TRUE;
    }
  }
}

static void on_combo_box_change(GtkComboBox *combo_box, gpointer user_data) {
  FlowerSubfamily chosen_flower = gtk_combo_box_get_active(
      combo_box); /* returns int that equals the appropriate enum */
  DirectionUserData *data = user_data;

  if (data->direction == LEFT) {
    display_flower(LEFT, chosen_flower);
  } else if (data->direction == RIGHT) {
    display_flower(RIGHT, chosen_flower);
  }
}

static void activate(GtkApplication *app, gpointer user_data) {
  gint i;
  /* prepare flower info */
  flowers[SOLANACEAE] = (Flower)
  {{"solanaceae.png", 
    "5, partly fused",
    "5, mostly fused", 
    "5, joined to petals"},
    FIVE_PARTED,
    SUPERIOR};

  flowers[HYDROPHYLLACEAE] = (Flower)
  {{"hydrophyllaceae.png", 
    "5, partly fused",
    "5, joined to form a tube", 
    "5, attached to the corolla tube"},
    FIVE_PARTED,
    SUPERIOR};

  flowers[BORAGINACEAE] = (Flower)
  {{"boraginaceae.png", 
    "5, partly fused",
    "5, joined to form a tube",
    "5, attached to the corolla tube"},
    FIVE_PARTED,
    SUPERIOR};

  flowers[LAMIACEAE] = (Flower)
  {{"lamiaceae.png",
     "often 5, fused",
     "5, irregular, divided into two lips and joined midway to form a tube",
     "2 or 4"},
     FIVE_PARTED,
     SUPERIOR};

  flowers[SCROPHULARIACEAE] = (Flower)
  {{"scrophulariaceae.png", 
    "5, partly fused",
    "5, arranged in two-lipped petal lobes joined to form a tube",
    "usually 4, sometimes 2 or 5"},
    FIVE_PARTED,
    SUPERIOR};

  flowers[BRASSICACEAE] = (Flower)
  {{"brassicaceae.png", 
    "4",
    "4, cross-like",
    "6, of 2 lengths"},
    FOUR_PARTED,
    SUPERIOR};

  flowers[ONAGRACEAE] = (Flower)
  {{"onagraceae.png",
    "4",
    "4",
    "4 or 8"},
    FOUR_PARTED,
    INFERIOR};

  flowers[PAPAVERACEAE] = (Flower)
  {{"papaveraceae.png",
    "2 or 3, often fall away as the flowers open",
    "4 or 6",
    "6 to numerous"},
    FOUR_PARTED,
    SUPERIOR};

  flowers[POLYGONACEAE] = (Flower)
  {{"polygonaceae.png",
    "3 tepals",
    "3 tepals",
    "3 to 9"},
    THREE_PARTED,
    SUPERIOR};

  flowers[LILIACEAE] = (Flower)
  {{"liliaceae.png",
    "6 colored tepals",
    "6 colored tepals",
    "6 (sometimes 3)"},
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
  left_widgets[BOX] = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  right_widgets[BOX] = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(box), left_widgets[BOX]);
  gtk_container_add(GTK_CONTAINER(box), right_widgets[BOX]);

  /* prepare dropdowns */
  left_widgets[COMBO_BOX] = gtk_combo_box_text_new();
  for (i = 0; i < FLOWER_COUNT; i++) {
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(left_widgets[COMBO_BOX]),
                                   subfamily_to_string(i));
  }
  gtk_container_add(GTK_CONTAINER(left_widgets[BOX]), left_widgets[COMBO_BOX]);
  gtk_combo_box_set_active(GTK_COMBO_BOX(left_widgets[COMBO_BOX]), 0);

  right_widgets[COMBO_BOX] = gtk_combo_box_text_new();
  for (i = 0; i < FLOWER_COUNT; i++) {
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(right_widgets[COMBO_BOX]),
                                   subfamily_to_string(i));
  }
  gtk_container_add(GTK_CONTAINER(right_widgets[BOX]),
                    right_widgets[COMBO_BOX]);
  gtk_combo_box_set_active(GTK_COMBO_BOX(right_widgets[COMBO_BOX]), 1);

  /* initialize empty widgets to be updated by display_flower */
  left_widgets[IMAGE] = gtk_image_new();
  for (i = PETALS_LABEL; i < G_N_ELEMENTS(left_widgets); ++i) {
    left_widgets[i] = gtk_label_new("");
  }
  right_widgets[IMAGE] = gtk_image_new();
  for (i = PETALS_LABEL; i < G_N_ELEMENTS(right_widgets); ++i) {
    right_widgets[i] = gtk_label_new("");
  }

  /* add them to left and right boxes */
  for (i = IMAGE; i < G_N_ELEMENTS(left_widgets); ++i) {
    gtk_container_add(GTK_CONTAINER(left_widgets[BOX]), left_widgets[i]);
  }
  for (i = IMAGE; i < G_N_ELEMENTS(right_widgets); ++i) {
    gtk_container_add(GTK_CONTAINER(right_widgets[BOX]), right_widgets[i]);
  }

  /* add visiblity toggle buttons, at the bottom of the window with dummy label
   * filling the remaining space */
  left_dummy_label = gtk_label_new("");
  right_dummy_label = gtk_label_new("");
  gtk_container_add(GTK_CONTAINER(left_widgets[BOX]), left_dummy_label);
  gtk_container_add(GTK_CONTAINER(right_widgets[BOX]), right_dummy_label);

  gtk_widget_set_vexpand(left_dummy_label, TRUE);
  gtk_widget_set_vexpand(right_dummy_label, TRUE);

  left_toggle_button = gtk_button_new_with_label("Toggle display");
  right_toggle_button = gtk_button_new_with_label("Toggle display");

  gtk_box_pack_end(GTK_BOX(left_widgets[BOX]), left_toggle_button, FALSE, FALSE,
                   0);
  gtk_container_add(GTK_CONTAINER(right_widgets[BOX]), right_toggle_button);

  /* connect button click signals to callback */
  g_signal_connect(GTK_BUTTON(left_toggle_button), "clicked",
                   G_CALLBACK(on_button_click), &left_data);
  g_signal_connect(GTK_BUTTON(right_toggle_button), "clicked",
                   G_CALLBACK(on_button_click), &right_data);

  /* connect dropdown selection signals to callback */
  g_signal_connect(GTK_COMBO_BOX(left_widgets[COMBO_BOX]), "changed",
                   G_CALLBACK(on_combo_box_change), &left_data);
  g_signal_connect(GTK_COMBO_BOX(right_widgets[COMBO_BOX]), "changed",
                   G_CALLBACK(on_combo_box_change), &right_data);

  /* fix window size */
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  /* start with comparison of solanaceae to hydrophyllaceae */
  display_flower(LEFT, SOLANACEAE);
  display_flower(RIGHT, HYDROPHYLLACEAE);

  gtk_widget_show_all(window);
  left_shown = TRUE;
  right_shown = TRUE;
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("com.github.zarhult.flowers",
                            G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
