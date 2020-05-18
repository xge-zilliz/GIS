package org.apache.spark.sql.arctern.enums;

import java.io.Serializable;

public enum FileDataSplitter
        implements Serializable
{

    CSV(",");

    public static FileDataSplitter getFileDataSplitter(String str)
    {
        for (FileDataSplitter me : FileDataSplitter.values()) {
            if (me.getDelimiter().equalsIgnoreCase(str) || me.name().equalsIgnoreCase(str)) { return me; }
        }
        throw new IllegalArgumentException("[" + FileDataSplitter.class + "] Unsupported FileDataSplitter:" + str);
    }

    private String splitter;

    private FileDataSplitter(String splitter)
    {
        this.splitter = splitter;
    }

    public String getDelimiter()
    {
        return this.splitter;
    }
}

