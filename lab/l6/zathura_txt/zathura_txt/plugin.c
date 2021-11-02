#include "plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <cairo.h>

#define MAX_LINES 50
#define MAX_CHARS 50

typedef struct txt_page_s {
    char** lines;
}txt_page_t;

typedef struct txt_document_s {
    struct txt_page_s* pages;   
}txt_document_t;

ZATHURA_PLUGIN_REGISTER_WITH_FUNCTIONS(
    "txt",
    VERSION_MAJOR, VERSION_MINOR, VERSION_REV,
    ZATHURA_PLUGIN_FUNCTIONS({
    .document_open            = txt_document_open,
    .document_free            = txt_document_free,
    .page_init                = txt_page_init,
    .page_clear               = txt_page_clear,
    .page_render_cairo        = txt_page_render_cairo,
    }),
    ZATHURA_PLUGIN_MIMETYPES({
    "text/plain"
    })
)

zathura_error_t txt_document_open(zathura_document_t* document){
    zathura_error_t error = ZATHURA_ERROR_OK;
    txt_document_t *txt_document = g_malloc0(sizeof(txt_document_t));

    const char* path = zathura_document_get_path(document);
    FILE *f = fopen(path, "r");

    txt_document->pages = (txt_page_t *)malloc(sizeof(txt_page_t));
    txt_document->pages[0].lines = malloc(MAX_CHARS * sizeof(char*));

    char line[MAX_CHARS + 1];
    while(fgets(line, MAX_CHARS + 1, f)) txt_document->pages[0].lines[0] = strdup(line);
    fclose(f);

    zathura_document_set_number_of_pages(document, 1);
    zathura_document_set_data(document, txt_document);
    return error;
}

zathura_error_t txt_document_free(zathura_document_t* document, void* txt_document){
    txt_document_t *txt_document_cur = txt_document;
    if(txt_document_cur == NULL) return ZATHURA_ERROR_INVALID_ARGUMENTS;
    if(txt_document_cur->pages != NULL) free(txt_document_cur->pages);
    g_free(txt_document_cur);
    return ZATHURA_ERROR_OK;
}

zathura_error_t txt_page_render_cairo(zathura_page_t* page, void* txt_page, cairo_t* cairo, bool printing){
    txt_page_t* txt_page_cur = txt_page;
    zathura_document_t* document = zathura_page_get_document(page);
    cairo_set_font_size(cairo, 13);
    cairo_move_to(cairo, 10, 10);
    cairo_show_text(cairo, txt_page_cur->lines[0]);
    return ZATHURA_ERROR_OK;
}

zathura_error_t txt_page_init(zathura_page_t* page){
  zathura_document_t* document = zathura_page_get_document(page);
  txt_document_t* txt_document = zathura_document_get_data(document);
  // Size of A4 paper
  zathura_page_set_width(page, 650);
  zathura_page_set_height(page, 978);
  zathura_page_set_data(page, txt_document->pages);
  return ZATHURA_ERROR_OK;
}

zathura_error_t txt_page_clear(zathura_page_t* page, void* txt_page){
  txt_page_t* txt_page_cur = txt_page;
  if(txt_page_cur->lines != NULL) free(txt_page_cur->lines);
  return ZATHURA_ERROR_OK;
}
