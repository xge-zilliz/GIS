package org.apache.spark.sql.arctern.operator;

import org.apache.spark.sql.arctern.SpatialRDD.SpatialRDD;
import org.apache.commons.lang3.tuple.Pair;
import org.apache.spark.SparkContext;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.function.Function2;
import org.apache.spark.api.java.function.PairFunction;
import org.locationtech.jts.geom.Geometry;
import scala.Tuple2;

import java.util.HashSet;

public class JoinQuery {

    private static <U extends Geometry, T extends Geometry> JavaPairRDD<U, HashSet<T>> collectGeometriesByKey(JavaPairRDD<U, T> input)
    {
        return input.aggregateByKey(
                new HashSet<T>(),
                new Function2<HashSet<T>, T, HashSet<T>>()
                {
                    @Override
                    public HashSet<T> call(HashSet<T> ts, T t)
                            throws Exception
                    {
                        ts.add(t);
                        return ts;
                    }
                },
                new Function2<HashSet<T>, HashSet<T>, HashSet<T>>()
                {
                    @Override
                    public HashSet<T> call(HashSet<T> ts, HashSet<T> ts2)
                            throws Exception
                    {
                        ts.addAll(ts2);
                        return ts;
                    }
                });
    }

    public static <U extends Geometry, T extends Geometry> JavaPairRDD<U, T> spatialJoin(
            SpatialRDD<U> leftRDD,
            SpatialRDD<T> rightRDD)
            throws Exception
    {
        SparkContext sparkContext = leftRDD.rawSpatialRDD.context();

        final JavaRDD<Pair<U, T>> resultRDD;
        final RightIndexLookupJudgement judgement = new RightIndexLookupJudgement();
        resultRDD = leftRDD.rawSpatialRDD.zipPartitions(rightRDD.indexedRDD, judgement);

        final JavaRDD<Pair<U, T>> result = resultRDD.distinct();

        return result.mapToPair(new PairFunction<Pair<U, T>, U, T>()
        {
            @Override
            public Tuple2<U, T> call(Pair<U, T> pair)
                    throws Exception
            {
                return new Tuple2<>(pair.getKey(), pair.getValue());
            }
        });
    }

    public static <U extends Geometry, T extends Geometry> JavaPairRDD<U, HashSet<T>> SpatialJoinQuery(SpatialRDD<U> spatialRDD, SpatialRDD<T> queryRDD, boolean useIndex)
            throws Exception
    {
        final JavaPairRDD<U, T> joinResults = spatialJoin(spatialRDD, queryRDD);
        return collectGeometriesByKey(joinResults);
    }

    public static <U extends Geometry, T extends Geometry> JavaPairRDD<U, HashSet<T>> SpatialJoinQuery(JavaRDD<T> spatialRDD, JavaRDD<U> queryRDD, boolean useIndex)
            throws Exception
    {
        SpatialRDD<T> left = new SpatialRDD<T>();
        left.setRawSpatialRDD(spatialRDD);
        SpatialRDD<U> right = new SpatialRDD<U>();
        right.setRawSpatialRDD(queryRDD);
        final JavaPairRDD<U, T> joinResults = spatialJoin(right, left);
        return collectGeometriesByKey(joinResults);
    }
}
