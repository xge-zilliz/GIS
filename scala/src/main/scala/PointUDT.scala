package org.apache.spark.sql.udt.point {

  import org.apache.spark.sql.catalyst.util._
  import org.apache.spark.sql.types._
  import org.locationtech.jts.geom.{Coordinate, GeometryFactory, Point}
  import org.locationtech.jts.io.WKTWriter

  @SQLUserDefinedType(udt = classOf[PointUDT])
  class ArcternPoint(val point: Point) extends Serializable {
    override def equals(other: Any): Boolean = other match {
      case that: ArcternPoint => this.point.equals(that.point)
      case _ => false
    }

    override def toString: String = new WKTWriter().write(point)
  }

  class PointUDT extends UserDefinedType[ArcternPoint] {
    override def sqlType: DataType = ArrayType(DoubleType, containsNull = false)

    override def serialize(obj: ArcternPoint): GenericArrayData = {
      val output = new Array[Double](2)
      output(0) = obj.point.getX
      output(1) = obj.point.getY
      new GenericArrayData(output)
    }

    override def deserialize(datum: Any): ArcternPoint = {
      datum match {
        case values: ArrayData => new ArcternPoint(new GeometryFactory().createPoint(new Coordinate(values.getDouble(0), values.getDouble(1))))
      }
    }

    override def userClass: Class[ArcternPoint] = classOf[ArcternPoint]
  }
}