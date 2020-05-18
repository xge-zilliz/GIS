package org.apache.spark.sql.arctern.SpatialRDD;

import org.apache.spark.sql.arctern.enums.FileDataSplitter;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.sql.arctern.formatMapper.PointFormatMapper;
import org.apache.spark.storage.StorageLevel;
import org.apache.spark.sql.arctern.GeometryUDT;

public class PointRDD
        extends SpatialRDD<GeometryUDT>
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
