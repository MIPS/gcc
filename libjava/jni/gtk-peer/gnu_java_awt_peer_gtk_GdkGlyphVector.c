   Copyright (C) 2003 Free Software Foundation, Inc.
JNIEXPORT void JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_initStaticState 
JNIEXPORT void JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_initState 
  (JNIEnv *env, jobject self, jobject font, jobject ctx)
  int begin = 0;
JNIEXPORT void JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_dispose
JNIEXPORT void JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_setChars 
  items = pango_itemize (vec->ctx, str, 0, len, attrs, NULL);
  g_assert (items != NULL);
  
  /*
    step 2: for each item:
    - shape the item into a glyphstring
    - store the (item, glyphstring) pair in the vec->glyphitems list
  */
  
      free_glyphitems (vec->glyphitems);
      vec->glyphitems = NULL;
  for (item = g_list_first (items); item != NULL; item = g_list_next (item))
    {
      g_assert (item->data != NULL);

      gi = NULL;
      gi = g_malloc0 (sizeof(PangoGlyphItem));
      g_assert (gi != NULL);

      gi->item = (PangoItem *)item->data;
      gi->glyphs = pango_glyph_string_new ();
      g_assert (gi->glyphs != NULL);

      pango_shape (str + gi->item->offset, 
		   gi->item->length, 
		   &(gi->item->analysis), 
		   gi->glyphs);

      vec->glyphitems = g_list_append (vec->glyphitems, gi);
    }

  /* 
     ownership of each item has been transferred to glyphitems, 
     but the list should be freed.
  */

  g_list_free (items);

JNIEXPORT void JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_setGlyphCodes 
  (JNIEnv *env, jobject self, jintArray codes)
JNIEXPORT jint JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_glyphCode 
JNIEXPORT jint JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_numGlyphs 
JNIEXPORT jint JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_glyphCharIndex 
  (JNIEnv *env, jobject self, jint idx)
JNIEXPORT jdoubleArray JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_allInkExtents 
  g_assert (vec->glyphitems != NULL);

  pointsize = pango_font_description_get_size (vec->desc);
  pointsize /= (double) PANGO_SCALE;

  for (i = g_list_first (vec->glyphitems); i != NULL; i = g_list_next (i))
      g_assert (i->data != NULL);
      gi = (PangoGlyphItem *)i->data;
      g_assert (gi->glyphs != NULL);

      face = pango_ft2_font_get_face (gi->item->analysis.font);
      assume_pointsize_and_identity_transform (pointsize, face);
      for (j = 0; j < gi->glyphs->num_glyphs; ++j)
	  FT_Load_Glyph (face, gi->glyphs->glyphs[j].glyph, FT_LOAD_DEFAULT);
	  /* FIXME: this needs to change for vertical layouts */
	  tmp.x = x + DOUBLE_FROM_26_6 (face->glyph->metrics.horiBearingX);
	  tmp.y = y + DOUBLE_FROM_26_6 (face->glyph->metrics.horiBearingY);
	  tmp.width = DOUBLE_FROM_26_6 (face->glyph->metrics.width);
	  tmp.height = DOUBLE_FROM_26_6 (face->glyph->metrics.height);
	  union_rects (&rect, &tmp);
	  x += DOUBLE_FROM_26_6 (face->glyph->advance.x);
	  y += DOUBLE_FROM_26_6 (face->glyph->advance.y);
	}
    }      
JNIEXPORT jdoubleArray JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_allLogicalExtents 
JNIEXPORT jdoubleArray JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_glyphLogicalExtents 
JNIEXPORT jdoubleArray JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_glyphInkExtents 
JNIEXPORT jboolean JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_glyphIsHorizontal 
  (JNIEnv *env, jobject self, jint idx)
JNIEXPORT jboolean JNICALL Java_gnu_java_awt_peer_gtk_GdkGlyphVector_isEqual 
