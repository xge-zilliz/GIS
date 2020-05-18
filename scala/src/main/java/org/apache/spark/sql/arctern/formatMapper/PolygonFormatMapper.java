package org.apache.spark.sql.arctern.formatMapper;

import org.apache.spark.sql.arctern.enums.FileDataSplitter;
import org.apache.spark.sql.arctern.enums.GeometryType;

public class PolygonFormatMapper
        extends FormatMapper
{
    public PolygonFormatMapper(FileDataSplitter Splitter, boolean carryInputData)
    {
        super(0, -1, Splitter, carryInputData, GeometryType.POLYGON);
    }

    public PolygonFormatMapper(Integer startOffset, Integer endOffset, FileDataSplitter Splitter,
            boolean carryInputData)
    {
        super(startOffset, endOffset, Splitter, carryInputData, GeometryType.POLYGON);
    }
}