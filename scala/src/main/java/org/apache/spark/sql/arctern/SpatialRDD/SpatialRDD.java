package org.apache.spark.sql.arctern.SpatialRDD;

import org.apache.spark.rdd.RDD;
import org.apache.spark.sql.arctern.GeometryUDT;
import org.apache.spark.sql.arctern.index.IndexBuilder;
import org.apache.spark.sql.arctern.index.IndexType;
import org.locationtech.jts.geom.*;
import org.locationtech.jts.index.SpatialIndex;
import org.apache.spark.api.java.JavaRDD;


import java.io.Serializable;
import java.util.*;

public class SpatialRDD<T extends GeometryUDT>
        implements Serializable
{

    public JavaRDD<SpatialIndex> indexedRDD;

    public JavaRDD<T> rawSpatialRDD;

    public List<Envelope> grids;

    public List<String> fieldNames;

    public void buildIndex(final IndexType indexType)
            throws Exception
    {
        if (this.rawSpatialRDD == null) {
            throw new Exception("build index error.");
        }
        this.indexedRDD = this.rawSpatialRDD.mapPartitions(new IndexBuilder(indexType));
    }

    public JavaRDD<T> getRawSpatialRDD()
    {
        return rawSpatialRDD;
    }

    public void setRawSpatialRDD(JavaRDD<T> rawSpatialRDD)
    {
        this.rawSpatialRDD = rawSpatialRDD;
    }

    public void setRawSpatialRDD1(RDD<T> rawSpatialRDD)
    {
        this.rawSpatialRDD = rawSpatialRDD.toJavaRDD();
    }
}
