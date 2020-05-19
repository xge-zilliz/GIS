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
    print(resultsize)
  }

//  test("test 2") {
//    val data1 = Seq(
//      Row(1, "POINT (15 15)"),
//      Row(2, "POINT (35 55)"),
//      Row(3, "POINT (15 30)"),
//      Row(4, "POINT (35 35)")
//    )
//
//    val data2 = Seq(
//      Row(1, "POLYGON ((0 0, 20 0, 20 20, 0 20, 0 0))"),
//      Row(2, "POLYGON ((20 0, 40 0, 40 20, 20 20, 20 0))"),
//      Row(3, "POLYGON ((0 20, 20 20, 20 40, 0 40, 0 20))"),
//      Row(4, "POLYGON ((20 20, 40 20, 40 40, 20 40, 20 20))")
//    )
//
//    val rdd_1 = spark.sparkContext.parallelize(data1)
//    val rdd_2 = spark.sparkContext.parallelize(data2)
//    val pointRDD = new SpatialRDD[GeometryUDT]()
//    pointRDD.setRawSpatialRDD1(rdd_1)
//    val polygonRDD = new SpatialRDD[GeometryUDT]()
//    polygonRDD.setRawSpatialRDD1(rdd_2)
//
//    val PolygonRDDIndexType = IndexType.RTREE
//
//    polygonRDD.buildIndex(PolygonRDDIndexType)
//  }
}
