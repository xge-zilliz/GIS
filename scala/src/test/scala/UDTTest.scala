import org.scalatest.FunSuite
import org.apache.spark.sql.{Row, SparkSession}
import org.apache.log4j.Logger
import org.apache.log4j.Level
import org.apache.spark.sql.types._
import org.locationtech.jts.geom.{Coordinate, GeometryFactory}
import org.locationtech.jts.io.WKTReader

class UDTTest extends FunSuite {
  test("GeometryUDT") {
    import org.apache.spark.sql.udt.geometry._

    Logger.getLogger("org").setLevel(Level.WARN)
    val spark = SparkSession.builder()
      .master("local[*]")
      .appName("geometry_udt_test")
      .getOrCreate()

    val data = Seq(
      Row(1, new ArcternGeometry(new WKTReader().read("POINT (10 20)"))),
      Row(2, new ArcternGeometry(new WKTReader().read("LINESTRING (0 0, 10 10, 20 20)"))),
      Row(3, new ArcternGeometry(new WKTReader().read("POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))"))),
      Row(4, new ArcternGeometry(new WKTReader().read("MULTIPOINT ((10 40), (40 30), (20 20), (30 10))"))),
      Row(5, new ArcternGeometry(new WKTReader().read("MULTIPOLYGON (((30 20, 45 40, 10 40, 30 20)), ((15 5, 40 10, 10 20, 5 10, 15 5)))")))
    )

    val rdd_d = spark.sparkContext.parallelize(data)
    val schema = StructType(Array(StructField("idx", IntegerType, nullable = false), StructField("geometry", new GeometryUDT, nullable = false)))
    val df = spark.createDataFrame(rdd_d, schema)
    df.createOrReplaceTempView("data")
    val rst = spark.sql("select * from data")
    rst.show()

    spark.stop()
  }

  test("PointUDT") {
    import org.apache.spark.sql.udt.point._

    Logger.getLogger("org").setLevel(Level.WARN)
    val spark = SparkSession.builder()
      .master("local[*]")
      .appName("point_udt_test")
      .getOrCreate()

    val data = Seq(
      Row(1, new ArcternPoint(new GeometryFactory().createPoint(new Coordinate(1, 1)))),
      Row(2, new ArcternPoint(new GeometryFactory().createPoint(new Coordinate(2, 2)))),
      Row(3, new ArcternPoint(new GeometryFactory().createPoint(new Coordinate(3, 3)))),
      Row(4, new ArcternPoint(new GeometryFactory().createPoint(new Coordinate(4, 4)))),
      Row(5, new ArcternPoint(new GeometryFactory().createPoint(new Coordinate(5, 5))))
    )

    val rdd_d = spark.sparkContext.parallelize(data)
    val schema = StructType(Array(StructField("idx", IntegerType, nullable = false), StructField("point", new PointUDT, nullable = false)))
    val df = spark.createDataFrame(rdd_d, schema)
    df.createOrReplaceTempView("data")

    val rst = spark.sql("select * from data")
    rst.queryExecution.debug.codegen()
    rst.show()

    spark.stop()
  }

}
