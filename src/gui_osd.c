/*
 * See Licensing and Copyright notice in naev.h
 */

/**
 * @file gui.c
 *
 * @brief Contains the GUI stuff for the player.
 */


#include "gui_osd.h"

#include "naev.h"

#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "opengl.h"
#include "font.h"


/**
 * @brief On Screen Display element.
 */
typedef struct OSD_s {
   struct OSD_s *next; /**< Next OSD in the linked list. */

   unsigned int id; /**< OSD id. */
   char *title;

   char **items; /**< Items on the list. */
   int nitems; /**< Number of items on the list. */

   int active; /**< Active item. */
} OSD_t;


/*
 * OSD linked list.
 */
static unsigned int osd_idgen = 0; /**< ID generator for OSD. */
static OSD_t *osd_list        = NULL; /**< Linked list for OSD. */


/*
 * Prototypes.
 */
static OSD_t *osd_get( unsigned int osd );
static int osd_free( OSD_t *osd );


/**
 * @brief Creates an on-screen display.
 *
 *    @param title Title of the display.
 *    @param nitems Number of items in the display.
 *    @param items Items in the display.
 *    @return ID of newly created OSD.
 */
unsigned int osd_create( const char *title, int nitems, const char **items )
{
   int i;
   OSD_t *osd, *ll;

   /* Create. */
   osd         = calloc( 1, sizeof(OSD_t) );
   osd->next   = NULL;
   osd->id     = ++osd_idgen;
   osd->active = 0;

   /* Copy text. */
   osd->title  = strdup(title);
   osd->items  = malloc( sizeof(char *) );
   osd->nitems = nitems;
   for (i=0; i<osd->nitems; i++)
      osd->items[i] = strdup( items[i] );

   /* Append to linked list. */
   if (osd_list == NULL)
      osd_list = osd;
   else {
      for (ll = osd_list; ll->next != NULL; ll = ll->next);
      ll->next = osd;
   }

   return osd->id;
}


/**
 * @brief Gets an OSD by ID.
 *
 *    @param osd ID of the OSD to get.
 */
static OSD_t *osd_get( unsigned int osd )
{
   OSD_t *ll;

   for (ll = osd_list; ll != NULL; ll = ll->next) {
      if (ll->id == osd)
         break;
   }

   return ll;
}


/**
 * @brief Frees an OSD struct.
 */
static int osd_free( OSD_t *osd )
{
   int i;

   if (osd->title != NULL)
      free(osd->title);

   for(i=0; i<osd->nitems; i++)
      free( osd->items[i] );
   free(osd->items);

   free(osd);

   return 0;
}


/**
 * @brief Destroys an OSD.
 *
 *    @param osd ID of the OSD to destroy.
 */
int osd_destroy( unsigned int osd )
{
   OSD_t *ll, *lp;

   lp = NULL;
   for (ll = osd_list; ll != NULL; ll = ll->next) {

      /* Matches. */
      if (ll->id == osd) {

         /* Remove from list. */
         if (lp == NULL)
            osd_list = ll->next;
         else
            lp->next = ll->next;

         /* Free. */
         osd_free( ll );

         return 0;
      }

      /* Save last iteration. */
      lp = ll;
   }

   return -1;
}


/**
 * @brief Makes an OSD message active.
 *
 *    @param osd OSD to change active message.
 *    @param msg Message to make active in OSD.
 */
int osd_active( unsigned int osd, int msg )
{
   OSD_t *o;

   o = osd_get(osd);
   if (o == NULL)
      return -1;

   o->active = msg;
   return 0;
}


/**
 * @brief Destroys all the OSD.
 */
void osd_exit (void)
{
   while (osd_list != NULL)
      osd_free(osd_list);

   osd_list = NULL;
}


/**
 * @brief Renders all the OSD.
 */
void osd_render( double x, double y, double w, double h )
{
   OSD_t *ll;

   /* Nothing to render. */
   if (osd_list == NULL)
      return;

   for (ll = osd_list; ll != NULL; ll = ll->next) {
   }
}



