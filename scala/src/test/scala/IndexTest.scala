/*
 * Copyright (C) 2019-2020 Zilliz. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import org.apache.spark.sql.Row
import org.apache.spark.sql.arctern.SpatialRDD.{PointRDD, PolygonRDD, SpatialRDD}
import org.apache.spark.sql.arctern.enums.FileDataSplitter
import org.apache.spark.sql.arctern.index.IndexType
import org.apache.spark.sql.arctern.operator.JoinQuery

class IndexTest extends AdapterTest {
  test("index_test") {

    val resourceFolder = System.getProperty("user.dir") + "/src/test/resources/"

    val PointRDDInputLocation = resourceFolder + "arealm-small.csv"
    val PointRDDSplitter = FileDataSplitter.CSV
    val PointRDDOffset = 1

    val PolygonRDDInputLocation = resourceFolder + "primaryroads-polygon.csv"
    val PolygonRDDSplitter = FileDataSplitter.CSV
    val PolygonRDDIndexType = IndexType.RTREE
    val PolygonRDDStartOffset = 0
    val PolygonRDDEndOffset = 9

    val polygonRDD = new PolygonRDD(spark.sparkContext, PolygonRDDInputLocation, PolygonRDDStartOffset, PolygonRDDEndOffset, PolygonRDDSplitter,false)
    val pointRDD = new PointRDD(spark.sparkContext, PointRDDInputLocation, PointRDDOffset, PointRDDSplitter, false)

    polygonRDD.buildIndex(PolygonRDDIndexType)
    val resultsize = JoinQuery.SpatialJoinQuery(pointRDD, polygonRDD, true).count()
//    print(resultsize)
  }
}
