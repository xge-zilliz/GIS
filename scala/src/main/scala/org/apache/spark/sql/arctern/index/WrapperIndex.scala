package org.apache.spark.sql.arctern.index

import java.util

import org.locationtech.jts.index.{ItemVisitor, SpatialIndex}
import org.locationtech.jts.geom.{Envelope, Geometry}
import org.apache.spark.sql.arctern.GeometryUDT

import scala.collection.JavaConverters._

class WrapperIndex[T, Index <: SpatialIndex](val indexBuider : () => Index) extends SpatialIndex with Serializable {
  protected var index = indexBuider()

  def insert(geom: GeometryUDT, key: String, value: T): Unit = insert(geom.deserialize().getEnvelopeInternal, (key, value))

  def remove(geom: GeometryUDT, key: String): T = {
    val envelope = geom.deserialize().getEnvelopeInternal
    index.query(envelope).asScala.asInstanceOf[Seq[(String, T)]].find(_._1 == key) match {
      case None => null.asInstanceOf[T]
      case Some(kv) => remove(envelope, kv); kv._2
    }
  }

  def get(geom: GeometryUDT, key: String): T = {
    val intersect = index.query(geom.deserialize().getEnvelopeInternal).asScala.asInstanceOf[Seq[(String, T)]]
    intersect.find(_._1 == key).map(_._2).getOrElse(null.asInstanceOf[T])
  }

  def query(xmin: Double, ymin: Double, xmax: Double, ymax: Double): Iterator[T] =
    index.query(new Envelope(xmin, xmax, ymin, ymax)).iterator.asScala.asInstanceOf[Iterator[(String, T)]].map(_._2)

  def query(): Iterator[T] = {
    query(-180,-90,180,90)
  }

  def size(): Int = query().size

  def clear(): Unit = index = indexBuider()

  override def insert(itemEnv: Envelope, item: Any): Unit = {
    index.insert(itemEnv, item)
  }

  override def query(searchEnv: Envelope, visitor: ItemVisitor): Unit = {
    index.query(searchEnv, visitor)
  }

  override def remove(itemEnv: Envelope, item: Any): Boolean = {
    index.remove(itemEnv, item)
  }

  override def query(searchEnv: Envelope): util.List[_] = {
    index.query(searchEnv)
  }
}
