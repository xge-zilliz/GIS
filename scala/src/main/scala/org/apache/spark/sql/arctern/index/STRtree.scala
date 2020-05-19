package org.apache.spark.sql.arctern.index

import org.locationtech.jts.index.strtree.STRtree

class WrappedSTRtree[T](nodeCapacity:Int = 10) extends WrapperIndex[T,STRtree](
  indexBuider = () => new STRtree(nodeCapacity)
) with Serializable {

  override def size(): Int = index.size()

//  def insert(itemEnv: Envelope, item: Geometry): Unit = index.insert(itemEnv, item)

}
