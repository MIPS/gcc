/* AffineTransformOp.java --  This class performs affine 
 * transformation between two images or rasters in 2 
 * dimensions. Copyright (C) 2004 Free Software Foundation

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */

package java.awt.image;

import java.awt.*;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.*;


/**
 * This class performs affine transformation between two images or 
 * rasters in 2 dimensions. 
 *
 * @author Olga Rodimina <rodimina@redhat.com> 
 */
 
public class AffineTransformOp implements BufferedImageOp, RasterOp
{

    public static final int TYPE_BILINEAR = 0;
    public static final int TYPE_NEAREST_NEIGHBOR = 1;

    private AffineTransform transform;
    private int interpolationType;
    private RenderingHints hints;

    
    /**
     * Construct AffineTransformOp with the given xform and interpolationType.
     * Interpolation type can be either TYPE_BILINEAR or TYPE_NEAREST_NEIGHBOR.
     *
     * @param xform AffineTransform that will applied to the source image 
     * @param interpolationType type of interpolation used
     */

    AffineTransformOp(AffineTransform xform, int interpolationType)
    {
      this.transform = xform;
      this.interpolationType = interpolationType;
    }

    /**
     * Construct AffineTransformOp with the given xform and rendering hints.
     * 
     * @param xform AffineTransform that will applied to the source image
     * @param hints rendering hints that will be used during transformation
     */

    AffineTransformOp(AffineTransform xform, RenderingHints hints)
    {
      this.transform = xform;
      this.hints = hints;
    }

    /**
     * Creates empty BufferedImage with the same size and number of bands as the
     * source image. The newly created image is created with the specified 
     * ColorModel. If the ColorModel is equal to null, then image is created 
     * with the ColorModel of the source image.
     *
     * @param src source image
     * @param destCM color model for the destination image
     * @return new compatible destination image
     */

    public BufferedImage createCompatibleDestImage(BufferedImage src,
                                                   ColorModel destCM)
    {
      throw new UnsupportedOperationException("not implemented yet");	
    }

    /**
     * Creates empty WritableRaster with the same size and number of bands 
     * as the source image. 
     *
     * @param src source raster
     * @return new compatible raster
     */

    public WritableRaster createCompatibleDestRaster(Raster src)
    {
      throw new UnsupportedOperationException("not implemented yet");	
    }

    /**
     * Transforms source image using transform specified at the constructor.
     * The resulting transformed image is stored in the destination image. 
     *
     * @param src source image
     * @param dst destination image
     * @return transformed source image
     */

    public BufferedImage filter(BufferedImage src, BufferedImage dst)
    {
      throw new UnsupportedOperationException("not implemented yet");	
    }

    /**
     * Transforms source raster using transform specified at the constructor.
     * The resulting raster is stored in the destination raster.
     *
     * @param src source raster
     * @param dst destination raster
     * @return transformed raster
     */

    public WritableRaster filter(Raster src, WritableRaster dst)
    {
      throw new UnsupportedOperationException("not implemented yet");	
    }

    /**
     * Transforms source image using transform specified at the constructor and 
     * returns bounds of the transformed image.
     *
     * @param src image to be transformed
     * @return bounds of the transformed image.
     */

    public Rectangle2D getBounds2D(BufferedImage src)
    {
      throw new UnsupportedOperationException("not implemented yet");	
    }
   
    /**
     * Transforms source raster using transform specified at the constructor
     * and returns bounds of the transformed raster.
     *
     * @param src raster to be transformed
     * @return bounds of the transformed raster.
     */

    public Rectangle2D getBounds2D(Raster src)
    {
      throw new UnsupportedOperationException("not implemented yet");	
    }

    /**
     * Returns interpolation type used during transformations
     *
     * @return interpolation type
     */

    public int getInterpolationType()
    {
      return interpolationType;
    }

    /** 
     * Returns location of the transformed source point. The resulting point 
     * is stored in the dstPt if one is specified.
     *  
     * @param srcPt point to be transformed
     * @param dstPt destination point
     * @return the location of the transformed source point.
     */
     
    public Point2D getPoint2D(Point2D srcPt, Point2D dstPt)
    {
      throw new UnsupportedOperationException("not implemented yet");	
    }

    /** Returns rendering hints that are used during transformation.
     *
     * @return rendering hints
     */

    public RenderingHints getRenderingHints()
    {
      return hints;
    }

    /** Returns transform used in transformation between source and destination
     * image.
     *
     * @return transform
     */
     
    public AffineTransform getTransform()
    {
      return transform;
    }
}
