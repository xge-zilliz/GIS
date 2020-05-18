package org.apache.spark.sql.arctern.formatMapper;

import org.apache.spark.sql.arctern.enums.FileDataSplitter;
import org.apache.spark.sql.arctern.enums.GeometryType;

public class PointFormatMapper
        extends FormatMapper
{
    public PointFormatMapper(FileDataSplitter Splitter, boolean carryInputData)
    {
        super(0, 1, Splitter, carryInputData, GeometryType.POINT);
    }

    public PointFormatMapper(Integer startOffset, FileDataSplitter Splitter,
            boolean carryInputData)
    {
        super(startOffset, startOffset+1, Splitter, carryInputData, GeometryType.POINT);
    }

}
