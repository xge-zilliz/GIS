package org.apache.spark.sql.udt

import org.locationtech.jts.geom.Geometry
import org.apache.spark.sql.types.UDTRegistration

object UdtRegistratorWrapper {
  def registerUDT(): Unit = {
    UDTRegistration.register(classOf[Geometry].getName, classOf[GeometryUDT].getName)
  }
}
