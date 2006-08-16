/* GStreamer non-core tag registration and tag utility functions
 * Copyright (C) 2005 Ross Burton <ross@burtonini.com>
 * Copyright (C) 2006 Tim-Philipp Müller <tim centricular net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst-i18n-plugin.h>
#include <gst/gst.h>
#include "tag.h"

#include <string.h>

/**
 * SECTION:gsttag
 * @short_description: additional tag definitions for plugins and applications
 * @see_also: #GstTagList
 * 
 * <refsect2>
 * <para>
 * Contains additional standardized GStreamer tag definitions for plugins
 * and applications, and functions to register them with the GStreamer
 * tag system.
 * </para>
 * </refsect2>
 */


static gpointer
gst_tag_register_musicbrainz_tags_internal (gpointer unused)
{
#ifdef ENABLE_NLS
  GST_DEBUG ("binding text domain %s to locale dir %s", GETTEXT_PACKAGE,
      LOCALEDIR);
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
#endif

  gst_tag_register (GST_TAG_MUSICBRAINZ_TRACKID, GST_TAG_FLAG_META,
      G_TYPE_STRING, _("track ID"), _("MusicBrainz track ID"), NULL);
  gst_tag_register (GST_TAG_MUSICBRAINZ_ARTISTID, GST_TAG_FLAG_META,
      G_TYPE_STRING, _("artist ID"), _("MusicBrainz artist ID"), NULL);
  gst_tag_register (GST_TAG_MUSICBRAINZ_ALBUMID, GST_TAG_FLAG_META,
      G_TYPE_STRING, _("album ID"), _("MusicBrainz album ID"), NULL);
  gst_tag_register (GST_TAG_MUSICBRAINZ_ALBUMARTISTID, GST_TAG_FLAG_META,
      G_TYPE_STRING,
      _("album artist ID"), _("MusicBrainz album artist ID"), NULL);
  gst_tag_register (GST_TAG_MUSICBRAINZ_TRMID, GST_TAG_FLAG_META,
      G_TYPE_STRING, _("track TRM ID"), _("MusicBrainz TRM ID"), NULL);
  gst_tag_register (GST_TAG_MUSICBRAINZ_SORTNAME, GST_TAG_FLAG_META,
      G_TYPE_STRING,
      _("artist sortname"), _("MusicBrainz artist sortname"), NULL);

  return NULL;
}

/**
 * gst_tag_register_musicbrainz_tags
 *
 * Registers additional musicbrainz-specific tags with the GStreamer tag
 * system. Plugins and applications that use these tags should call this
 * function before using them. Can be called multiple times.
 */
void
gst_tag_register_musicbrainz_tags (void)
{
  static GOnce mb_once = G_ONCE_INIT;

  g_once (&mb_once, gst_tag_register_musicbrainz_tags_internal, NULL);
}

static void
register_tag_image_type_enum (GType * id)
{
  static const GEnumValue image_types[] = {
    {GST_TAG_IMAGE_TYPE_UNDEFINED, "GST_TAG_IMAGE_TYPE_UNDEFINED", "undefined"},
    {GST_TAG_IMAGE_TYPE_FRONT_COVER, "GST_TAG_IMAGE_TYPE_FRONT_COVER",
        "front-cover"},
    {GST_TAG_IMAGE_TYPE_BACK_COVER, "GST_TAG_IMAGE_TYPE_BACK_COVER",
        "back-cover"},
    {GST_TAG_IMAGE_TYPE_LEAFLET_PAGE, "GST_TAG_IMAGE_TYPE_LEAFLET_PAGE",
        "leaflet-page"},
    {GST_TAG_IMAGE_TYPE_MEDIUM, "GST_TAG_IMAGE_TYPE_MEDIUM", "medium"},
    {GST_TAG_IMAGE_TYPE_LEAD_ARTIST, "GST_TAG_IMAGE_TYPE_LEAD_ARTIST",
        "lead-artist"},
    {GST_TAG_IMAGE_TYPE_ARTIST, "GST_TAG_IMAGE_TYPE_ARTIST", "artist"},
    {GST_TAG_IMAGE_TYPE_CONDUCTOR, "GST_TAG_IMAGE_TYPE_CONDUCTOR", "conductor"},
    {GST_TAG_IMAGE_TYPE_BAND_ORCHESTRA, "GST_TAG_IMAGE_TYPE_BAND_ORCHESTRA",
        "band-orchestra"},
    {GST_TAG_IMAGE_TYPE_COMPOSER, "GST_TAG_IMAGE_TYPE_COMPOSER", "composer"},
    {GST_TAG_IMAGE_TYPE_LYRICIST, "GST_TAG_IMAGE_TYPE_LYRICIST", "lyricist"},
    {GST_TAG_IMAGE_TYPE_RECORDING_LOCATION,
          "GST_TAG_IMAGE_TYPE_RECORDING_LOCATION",
        "recording-location"},
    {GST_TAG_IMAGE_TYPE_DURING_RECORDING, "GST_TAG_IMAGE_TYPE_DURING_RECORDING",
        "during-recording"},
    {GST_TAG_IMAGE_TYPE_DURING_PERFORMANCE,
          "GST_TAG_IMAGE_TYPE_DURING_PERFORMANCE",
        "during-performance"},
    {GST_TAG_IMAGE_TYPE_VIDEO_CAPTURE, "GST_TAG_IMAGE_TYPE_VIDEO_CAPTURE",
        "video-capture"},
    {GST_TAG_IMAGE_TYPE_FISH, "GST_TAG_IMAGE_TYPE_FISH", "fish"},
    {GST_TAG_IMAGE_TYPE_ILLUSTRATION, "GST_TAG_IMAGE_TYPE_ILLUSTRATION",
        "illustration"},
    {GST_TAG_IMAGE_TYPE_BAND_ARTIST_LOGO, "GST_TAG_IMAGE_TYPE_BAND_ARTIST_LOGO",
        "artist-logo"},
    {GST_TAG_IMAGE_TYPE_PUBLISHER_STUDIO_LOGO,
          "GST_TAG_IMAGE_TYPE_PUBLISHER_STUDIO_LOGO",
        "publisher-studio-logo"},
    {0, NULL, NULL}
  };

  *id = g_enum_register_static ("GstTagImageType", image_types);
}

GType
gst_tag_image_type_get_type (void)
{
  static GType id;
  static GOnce once = G_ONCE_INIT;

  g_once (&once, (GThreadFunc) register_tag_image_type_enum, &id);
  return id;
}

/**
 * gst_tag_parse_extended_comment:
 * @ext_comment: an extended comment string, see #GST_TAG_EXTENDED_COMMENT
 * @key: return location for the comment description key, or NULL
 * @lang: return location for the comment ISO-639 language code, or NULL
 * @value: return location for the actual comment string, or NULL
 * @fail_if_no_key: whether to fail if strings are not in key=value form
 *
 * Convenience function to parse a GST_TAG_EXTENDED_COMMENT string and
 * separate it into its components.
 *
 * If successful, @key, @lang and/or @value will be set to newly allocated
 * strings that you need to free with g_free() when done. @key and @lang
 * may also be set to NULL by this function if there is no key or no language
 * code in the extended comment string.
 *
 * Returns: TRUE if the string could be parsed, otherwise FALSE
 *
 * Since: 0.10.10
 */
gboolean
gst_tag_parse_extended_comment (const gchar * ext_comment, gchar ** key,
    gchar ** lang, gchar ** value, gboolean fail_if_no_key)
{
  const gchar *div, *bop, *bcl;

  g_return_val_if_fail (ext_comment != NULL, FALSE);
  g_return_val_if_fail (g_utf8_validate (ext_comment, -1, NULL), FALSE);

  if (key)
    *key = NULL;
  if (lang)
    *lang = NULL;

  div = strchr (ext_comment, '=');
  bop = strchr (ext_comment, '[');
  bcl = strchr (ext_comment, ']');

  if (div == NULL) {
    if (fail_if_no_key)
      return FALSE;
    if (value)
      *value = g_strdup (ext_comment);
    return TRUE;
  }

  if (bop != NULL && bop < div) {
    if (bcl < bop || bcl > div)
      return FALSE;
    if (key)
      *key = g_strndup (ext_comment, bop - ext_comment);
    if (lang)
      *lang = g_strndup (bop + 1, bcl - bop - 1);
  } else {
    if (key)
      *key = g_strndup (ext_comment, div - ext_comment);
  }

  if (value)
    *value = g_strdup (div + 1);

  return TRUE;
}
