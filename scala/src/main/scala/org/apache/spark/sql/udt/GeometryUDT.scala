package org.apache.spark.sql.udt

import org.apache.spark.sql.catalyst.util._
import org.apache.spark.sql.types._
import org.locationtech.jts.geom.Geometry
import org.locationtech.jts.io.{WKBReader, WKBWriter}

class GeometryUDT extends UserDefinedType[Geometry] {
  override def sqlType: DataType = ArrayType(ByteType, containsNull = false)

  override def serialize(obj: Geometry): GenericArrayData = {
    val wkb: Array[Byte] = new WKBWriter().write(obj)
    new GenericArrayData(wkb)
  }

  override def deserialize(datum: Any): Geometry = {
    datum match {
      case values: ArrayData => new WKBReader().read(values.toByteArray())
    }
  }

  override def userClass: Class[Geometry] = classOf[Geometry]
}