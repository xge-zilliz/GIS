package org.apache.spark.sql.arctern.SpatialRDD;

import org.apache.spark.sql.arctern.enums.FileDataSplitter;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.sql.arctern.formatMapper.PointFormatMapper;
import org.locationtech.jts.geom.Point;

public class PointRDD
        extends SpatialRDD<Point>
{
    public PointRDD() {}

    public PointRDD(JavaSparkContext sparkContext, String InputLocation, Integer Offset, FileDataSplitter splitter,
                    boolean carryInputData)
    {
        JavaRDD rawTextRDD = sparkContext.textFile(InputLocation);
        if (Offset != null) {this.setRawSpatialRDD(rawTextRDD.mapPartitions(new PointFormatMapper(Offset, splitter, carryInputData)));}
        else {this.setRawSpatialRDD(rawTextRDD.mapPartitions(new PointFormatMapper(splitter, carryInputData)));}
    }
}
