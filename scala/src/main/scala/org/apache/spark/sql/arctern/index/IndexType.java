package org.apache.spark.sql.arctern.index;

import java.io.Serializable;

public enum IndexType
        implements Serializable
{
    QUADTREE,
    RTREE;

    public static IndexType getIndexType(String str)
    {
        for (IndexType type : IndexType.values()) {
            if (type.name().equalsIgnoreCase(str))
                return type;
        }
        return null;
    }
}