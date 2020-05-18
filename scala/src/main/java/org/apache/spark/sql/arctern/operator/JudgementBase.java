package org.apache.spark.sql.arctern.operator;

import org.apache.spark.sql.arctern.utils.HalfOpenRectangle;
import org.locationtech.jts.geom.*;

import java.io.Serializable;


abstract class JudgementBase
        implements Serializable
{
    transient private HalfOpenRectangle extent;

    protected JudgementBase() {}


    protected boolean match(Geometry left, Geometry right)
    {
        if (extent != null) {
            if (left instanceof Point || right instanceof Point) {
                return geoMatch(left, right);
            }

            Envelope intersection =
                    left.getEnvelopeInternal().intersection(right.getEnvelopeInternal());
            if (!intersection.isNull()) {
                final Point referencePoint =
                        makePoint(intersection.getMinX(), intersection.getMinY(), left.getFactory());
                if (!extent.contains(referencePoint)) {
                    return false;
                }
            }
        }

        return geoMatch(left, right);
    }

    private Point makePoint(double x, double y, GeometryFactory factory)
    {
        return factory.createPoint(new Coordinate(x, y));
    }

    private boolean geoMatch(Geometry left, Geometry right)
    {
        return left.covers(right);
    }
}
