package org.apache.spark.sql.arctern.expressions

import org.apache.spark.broadcast.Broadcast
import org.apache.spark.sql.arctern.GeometryUDT
import org.apache.spark.sql.arctern.index.IndexBuilder
import org.locationtech.jts.geom.Geometry

case class JoinCase(broadcast: Broadcast[IndexBuilder]) {
  def index = broadcast.value

  def join(input: Array[String]): Array[String] = {
    var result = new Array[String](input.size)
    input.indices.foreach { i =>
      val geo_search = GeometryUDT.FromWkt(input(i))
      val env = geo_search.getEnvelopeInternal
      val geo_list = index.query(env)
      geo_list.forEach{geo =>
        val loop_geo = geo.asInstanceOf[Geometry]
        if (geo_search.intersects(loop_geo))
          //TODO::mutil indexed result intersects with searched geometry
          result(i) = loop_geo.toString
      }
    }
    result
  }
}
