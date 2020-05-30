#include <gtk/gtk.h>

/* make function to return string given subfamily and use those enums
 * instead of hardcoded strings */

/* make things no-global-variables again if have time */

/* replace left/right widgets with an array for each side's widgets that you can
 * loop thru */

#define FLOWER_COUNT 10

/* used for specifying which side of the display to interact with */
typedef enum side { LEFT, RIGHT } Side;

/* array indices for flower struct's info string array  */
typedef enum flowerInfo {
  FILENAME,
  SEPALS,
  PETALS,
  STAMENS,
  NICKNAME,
  FLOWER_INFO_ENUM_COUNT /* equal to the number of other enums */
} FlowerInfo;

typedef enum flowerFamily {
  FIVE_PARTED,
  FOUR_PARTED,
  THREE_PARTED
} FlowerFamily;

/* matches the order of the flowers declared in array in activate function  */
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

typedef struct flower {
  gchar *info[FLOWER_INFO_ENUM_COUNT];
  FlowerFamily family;
} Flower;

/* to be passed to callback function, effectively like parameters */
typedef struct comboBoxUserdata {
  Side direction;
} ComboBoxUserData;

/* evil global variables */
GtkWidget *window;
GtkWidget *box;

GtkWidget *left_box;
GtkWidget *left_combo_box;
GtkWidget *left_image;
GtkWidget *left_petals_label;
GtkWidget *left_sepals_label;
GtkWidget *left_stamens_label;
ComboBoxUserData left_data;

GtkWidget *right_box;
GtkWidget *right_combo_box;
GtkWidget *right_image;
GtkWidget *right_petals_label;
GtkWidget *right_sepals_label;
GtkWidget *right_stamens_label;
ComboBoxUserData right_data;

Flower flowers[FLOWER_COUNT];

static void display_flower(Side grid, FlowerSubfamily flwr) {
  gchar petals_label_buf[100] = "Petals: ";
  gchar sepals_label_buf[100] = "Sepals: ";
  gchar stamens_label_buf[100] = "Stamens: ";
  gchar filepath_buf[50] = "images/";

  /* prepare info strings */
  g_strlcat(filepath_buf, flowers[flwr].info[FILENAME], sizeof(filepath_buf));
  g_strlcat(petals_label_buf, flowers[flwr].info[PETALS],
            sizeof(petals_label_buf));
  g_strlcat(sepals_label_buf, flowers[flwr].info[SEPALS],
            sizeof(sepals_label_buf));
  g_strlcat(stamens_label_buf, flowers[flwr].info[STAMENS],
            sizeof(stamens_label_buf));

  g_print("%s\n", petals_label_buf);

  /* clear and repopulate appropriate side */
  if (grid == LEFT) {
    gtk_image_set_from_file(GTK_IMAGE(left_image), filepath_buf);
    gtk_label_set_text(GTK_LABEL(left_petals_label), petals_label_buf);
    gtk_label_set_text(GTK_LABEL(left_sepals_label), sepals_label_buf);
    gtk_label_set_text(GTK_LABEL(left_stamens_label), stamens_label_buf);
  } else if (grid == RIGHT) {
    gtk_image_set_from_file(GTK_IMAGE(right_image), filepath_buf);
    gtk_label_set_text(GTK_LABEL(right_petals_label), petals_label_buf);
    gtk_label_set_text(GTK_LABEL(right_sepals_label), sepals_label_buf);
    gtk_label_set_text(GTK_LABEL(right_stamens_label), stamens_label_buf);
  }
}

static void on_combo_box_change(GtkComboBox *combo_box, gpointer user_data) {
  FlowerSubfamily chosen_flower = gtk_combo_box_get_active(
      combo_box); /* returns int that equals the appropriate enum */
  ComboBoxUserData *data = user_data;

  if (data->direction == LEFT) {
    display_flower(LEFT, chosen_flower);
  } else if (data->direction == RIGHT) {
    display_flower(RIGHT, chosen_flower);
  }
}

static void activate(GtkApplication *app, gpointer user_data) {
  gint i;
  /* initialize flower info */
  flowers[SOLANACEAE] =
      (Flower){{"solanaceae.png", "5, partly fused", "5, mostly fused",
                "5, joined to petals", "Nightshade"},
               FIVE_PARTED};
  flowers[HYDROPHYLLACEAE] = (Flower){
      {"hydrophyllaceae.png", "5, partly fused", "5, joined to form a tube",
       "5, attached to the corolla tube", "Waterleaf"},
      FIVE_PARTED};
  flowers[BORAGINACEAE] = (Flower){
      {"boraginaceae.png", "5, partly fused", "5, joined to form a tube",
       "5, attached to the corolla tube", "Borage"},
      FIVE_PARTED};
  flowers[LAMIACEAE] = (Flower){
      {"lamiaceae.png", "often 5, fused",
       "5, irregular, divided into two lips and joined midway to form a tube",
       "2 or 4", "Mint"},
      FIVE_PARTED};
  flowers[SCROPHULARIACEAE] =
      (Flower){{"scrophulariaceae.png", "5, partly fused",
                "5, arranged in two-lipped petal lobes joined to form a tube",
                "usually 4, sometimes 2 or 5", "Snapdragon"},
               FIVE_PARTED};
  flowers[BRASSICACEAE] = (Flower){
      {"brassicaceae.png", "4", "4, cross-like", "6, of 2 lengths", "Mustard"},
      FOUR_PARTED};
  flowers[ONAGRACEAE] = (Flower){
      {"onagraceae.png", "4", "4", "4 or 8", "Evening Pimrose"}, FOUR_PARTED};
  flowers[PAPAVERACEAE] = (Flower){
      {"papaveraceae.png", "2 or 3, often fall away as the flowers open",
       "4 or 6", "6 to numerous", "Poppy"},
      FOUR_PARTED};
  flowers[POLYGONACEAE] = (Flower){
      {"polygonaceae.png", "3 tepals", "3 tepals", "3 to 9", "Buckwheat"},
      THREE_PARTED};
  flowers[LILIACEAE] = (Flower){{"liliaceae.png", "6 colored tepals",
                                 "6 colored tepals", "6 (sometimes 3)", "Lily"},
                                THREE_PARTED};

  /* prepare window */
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Flowers");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

  /* place a box in the window */
  box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_container_add(GTK_CONTAINER(window), box);

  /* split box in to two vertical boxes for flower info */
  left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(box), left_box);
  gtk_container_add(GTK_CONTAINER(box), right_box);

  /* prepare dropdowns */
  left_combo_box = gtk_combo_box_text_new();
  for (i = 0; i < FLOWER_COUNT; i++) {
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(left_combo_box),
                                   flowers[i].info[NICKNAME]);
  }
  gtk_container_add(GTK_CONTAINER(left_box), left_combo_box);
  gtk_combo_box_set_active(GTK_COMBO_BOX(left_combo_box), 0);

  right_combo_box = gtk_combo_box_text_new();
  for (i = 0; i < FLOWER_COUNT; i++) {
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(right_combo_box),
                                   flowers[i].info[NICKNAME]);
  }
  gtk_container_add(GTK_CONTAINER(right_box), right_combo_box);
  gtk_combo_box_set_active(GTK_COMBO_BOX(right_combo_box), 1);

  /* initialize empty widgets to be updated by display_flower */
  left_image = gtk_image_new_from_file("");
  left_petals_label = gtk_label_new("");
  left_sepals_label = gtk_label_new("");
  left_stamens_label = gtk_label_new("");
  right_image = gtk_image_new_from_file("");
  right_petals_label = gtk_label_new("");
  right_sepals_label = gtk_label_new("");
  right_stamens_label = gtk_label_new("");

  gtk_container_add(GTK_CONTAINER(left_box), left_image);
  gtk_container_add(GTK_CONTAINER(left_box), left_petals_label);
  gtk_container_add(GTK_CONTAINER(left_box), left_sepals_label);
  gtk_container_add(GTK_CONTAINER(left_box), left_stamens_label);
  gtk_container_add(GTK_CONTAINER(right_box), right_image);
  gtk_container_add(GTK_CONTAINER(right_box), right_petals_label);
  gtk_container_add(GTK_CONTAINER(right_box), right_sepals_label);
  gtk_container_add(GTK_CONTAINER(right_box), right_stamens_label);

  /* initialize comparison of solanaceae to hydrophyllaceae */
  display_flower(LEFT, SOLANACEAE);
  display_flower(RIGHT, HYDROPHYLLACEAE);

  /* prepare data parameter to dropdown callback */
  left_data.direction = LEFT;
  right_data.direction = RIGHT;

  /* connect dropdown selection signal to callback */
  g_signal_connect(GTK_COMBO_BOX(left_combo_box), "changed",
                   G_CALLBACK(on_combo_box_change), &left_data);
  g_signal_connect(GTK_COMBO_BOX(right_combo_box), "changed",
                   G_CALLBACK(on_combo_box_change), &right_data);

  gtk_widget_show_all(window);
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
