package org.apache.spark.sql.arctern.formatMapper;

import org.apache.spark.sql.arctern.enums.FileDataSplitter;
import org.apache.spark.sql.arctern.enums.GeometryType;
import org.locationtech.jts.geom.*;
import org.locationtech.jts.io.ParseException;
import org.locationtech.jts.operation.valid.IsValidOp;
import org.apache.spark.api.java.function.FlatMapFunction;

import java.io.Serializable;
import java.util.*;

public class FormatMapper<T extends Geometry>
        implements Serializable, FlatMapFunction<Iterator<String>, T>
{

    protected final int startOffset;

    protected final int endOffset;

    protected final FileDataSplitter splitter;

    protected final boolean carryInputData;

    protected String otherAttributes = "";

    protected GeometryType geometryType = null;

    boolean allowTopologicallyInvalidGeometries;

    boolean skipSyntacticallyInvalidGeometries;

    transient protected GeometryFactory factory = new GeometryFactory();

    public FormatMapper(int startOffset, int endOffset, FileDataSplitter splitter, boolean carryInputData, GeometryType geometryType)
    {
        this.startOffset = startOffset;
        this.endOffset = endOffset;
        this.splitter = splitter;
        this.carryInputData = carryInputData;
        this.geometryType = geometryType;
        this.allowTopologicallyInvalidGeometries = true;
        this.skipSyntacticallyInvalidGeometries = false;
        if (geometryType == null)
        {
            assert splitter == FileDataSplitter.CSV;
        }
    }

    public FormatMapper(FileDataSplitter splitter, boolean carryInputData)
    {
        this(0,-1,splitter,carryInputData, null);
    }


    public FormatMapper(FileDataSplitter splitter, boolean carryInputData, GeometryType geometryType)
    {
        this(0, -1, splitter, carryInputData, geometryType);
    }

    public Coordinate[] readCoordinates(String line)
    {
        final String[] columns = line.split(splitter.getDelimiter());
        final int actualEndOffset = this.endOffset >= 0 ? this.endOffset : (this.geometryType == GeometryType.POINT? startOffset+1:columns.length - 1);
        final Coordinate[] coordinates = new Coordinate[(actualEndOffset - startOffset + 1) / 2];
        for (int i = this.startOffset; i <= actualEndOffset; i += 2) {
            coordinates[(i - startOffset) / 2 ] = new Coordinate(Double.parseDouble(columns[i]), Double.parseDouble(columns[i + 1]));
        }
        if (carryInputData)
        {
            boolean firstColumnFlag = true;
            otherAttributes = "";
            for (int i= 0;i<this.startOffset;i++)
            {
                if (firstColumnFlag)
                {
                    otherAttributes += columns[i];
                    firstColumnFlag = false;
                }
                else otherAttributes += "\t" + columns[i];
            }
            for (int i=actualEndOffset+1;i<columns.length;i++)
            {
                if (firstColumnFlag)
                {
                    otherAttributes += columns[i];
                    firstColumnFlag = false;
                }
                else otherAttributes += "\t" + columns[i];
            }
        }
        return coordinates;
    }

    public <T extends Geometry> void addMultiGeometry(GeometryCollection multiGeometry, List<T> result)
    {
        for (int i = 0; i < multiGeometry.getNumGeometries(); i++) {
            T geometry = (T) multiGeometry.getGeometryN(i);
            geometry.setUserData(multiGeometry.getUserData());
            result.add(geometry);
        }
    }

    public Geometry readGeometry(String line)
            throws ParseException
    {
        Geometry geometry = null;
        if (this.geometryType == null) {
            throw new IllegalArgumentException("[GeoSpark][FormatMapper] You must specify GeometryType when you use delimiter rather than WKB, WKT or GeoJSON");
        } else {
            geometry = createGeometry(readCoordinates(line), geometryType);
        }

        if (geometry == null){
            return null;
        }
        if (allowTopologicallyInvalidGeometries == false) {
            IsValidOp isvalidop = new IsValidOp(geometry);
            if (isvalidop.isValid() == false) {
                geometry = null;
            }
        }

        return geometry;
    }

    private Geometry createGeometry(Coordinate[] coordinates, GeometryType geometryType)
    {
        GeometryFactory geometryFactory = new GeometryFactory();
        Geometry geometry = null;
        switch (geometryType) {
            case POINT:
                geometry = geometryFactory.createPoint(coordinates[0]);
                break;
            case POLYGON:
                geometry = geometryFactory.createPolygon(coordinates);
                break;
            case LINESTRING:
                geometry = geometryFactory.createLineString(coordinates);
                break;
            case RECTANGLE:
                // The rectangle mapper reads two coordinates from the input line. The two coordinates are the two on the diagonal.
                assert  coordinates.length == 2;
                Coordinate[] polyCoordinates = new Coordinate[5];
                polyCoordinates[0] = coordinates[0];
                polyCoordinates[1] = new Coordinate(coordinates[0].x, coordinates[1].y);
                polyCoordinates[2] = coordinates[1];
                polyCoordinates[3] = new Coordinate(coordinates[1].x, coordinates[0].y);
                polyCoordinates[4] = polyCoordinates[0];
                geometry = factory.createPolygon(polyCoordinates);
                break;
            // Read string to point if no geometry type specified but GeoSpark should never reach here
            default:
                geometry = geometryFactory.createPoint(coordinates[0]);
        }
        if (carryInputData)
        {
            geometry.setUserData(otherAttributes);
        }
        return geometry;
    }

    @Override
    public Iterator<T> call(Iterator<String> stringIterator)
            throws Exception
    {
        List<T> result = new ArrayList<>();
        while (stringIterator.hasNext()) {
            String line = stringIterator.next();
            addGeometry(readGeometry(line), result);
        }
        return result.iterator();
    }

    private void addGeometry(Geometry geometry, List<T> result)
    {
        if (geometry == null) {
            return;
        }
        if (geometry instanceof MultiPoint) {
            addMultiGeometry((MultiPoint) geometry, result);
        }
        else if (geometry instanceof MultiLineString) {
            addMultiGeometry((MultiLineString) geometry, result);
        }
        else if (geometry instanceof MultiPolygon) {
            addMultiGeometry((MultiPolygon) geometry, result);
        }
        else {
            result.add((T) geometry);
        }
    }
}
