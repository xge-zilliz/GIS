package org.apache.spark.sql.arctern.SpatialRDD;

import org.apache.spark.sql.arctern.GeometryUDT;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.sql.arctern.enums.FileDataSplitter;
import org.apache.spark.sql.arctern.formatMapper.PolygonFormatMapper;

public class PolygonRDD
        extends SpatialRDD<GeometryUDT>
{
    public PolygonRDD(JavaSparkContext sparkContext, String InputLocation, Integer startOffset, Integer endOffset,
                      FileDataSplitter splitter, boolean carryInputData)
    {
        JavaRDD rawTextRDD = sparkContext.textFile(InputLocation);
        if (startOffset != null && endOffset != null) {
            this.setRawSpatialRDD(rawTextRDD.mapPartitions(new PolygonFormatMapper(startOffset, endOffset, splitter, carryInputData)));
        }
        else {
            this.setRawSpatialRDD(rawTextRDD.mapPartitions(new PolygonFormatMapper(splitter, carryInputData)));
        }
    }
}

