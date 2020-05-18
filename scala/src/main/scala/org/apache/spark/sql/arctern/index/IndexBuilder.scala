package org.apache.spark.sql.arctern.index

import org.locationtech.jts.geom.Envelope
import org.apache.spark.api.java.function.FlatMapFunction
import org.apache.spark.sql.arctern.GeometryUDT
import java.util

final class IndexBuilder[T <: GeometryUDT](var indexType: IndexType) extends FlatMapFunction[util.Iterator[T], WrappedSTRtree[T]] {
  override def call(objectIterator: util.Iterator[T]) = {
    var spatialIndex:WrappedSTRtree[T] = null;
    if (indexType eq IndexType.RTREE) spatialIndex = new WrappedSTRtree()
    else spatialIndex = new WrappedSTRtree()
    while ( {
      objectIterator.hasNext
    }) {
      val spatialObject = objectIterator.next
      spatialIndex.insert(spatialObject.deserialize().getEnvelopeInternal, spatialObject)
    }
    val result = new util.HashSet[WrappedSTRtree[T]]
    spatialIndex.query(new Envelope(0.0, 0.0, 0.0, 0.0))
    result.add(spatialIndex)
    result.iterator
  }

}