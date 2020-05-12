package org.apache.spark.udt.geometry

import org.apache.spark.sql.catalyst.util._
import org.apache.spark.sql.types._
import org.locationtech.jts.geom.Geometry
import org.locationtech.jts.io.{WKBReader, WKBWriter, WKTWriter}

@SQLUserDefinedType(udt = classOf[GeometryUDT])
class ArcternGeometry(val geo: Geometry) extends Serializable {
  override def equals(other: Any): Boolean = other match {
    case that: ArcternGeometry => this.geo.equals(that.geo)
    case _ => false
  }

  override def toString: String = new WKTWriter().write(geo)
}

class GeometryUDT extends UserDefinedType[ArcternGeometry] {
  override def sqlType: DataType = ArrayType(ByteType, containsNull = false)

  override def serialize(obj: ArcternGeometry): GenericArrayData = {
    val wkb: Array[Byte] = new WKBWriter().write(obj.geo)
    new GenericArrayData(wkb)
  }

  override def deserialize(datum: Any): ArcternGeometry = {
    datum match {
      case values: ArrayData => new ArcternGeometry(new WKBReader().read(values.toByteArray()))
    }
  }

  override def userClass: Class[ArcternGeometry] = classOf[ArcternGeometry]
}