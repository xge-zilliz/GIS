package org.apache.spark.sql.arctern.operator;

import org.locationtech.jts.geom.Geometry;
import org.locationtech.jts.index.SpatialIndex;
import org.apache.commons.lang3.tuple.Pair;
import org.apache.spark.api.java.function.FlatMapFunction2;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class RightIndexLookupJudgement<T extends Geometry, U extends Geometry>
        extends JudgementBase
        implements FlatMapFunction2<Iterator<T>, Iterator<SpatialIndex>, Pair<T, U>>, Serializable
{

    public RightIndexLookupJudgement()
    {
        super();
    }

    @Override
    public Iterator<Pair<T, U>> call(Iterator<T> streamShapes, Iterator<SpatialIndex> indexIterator)
            throws Exception
    {
        List<Pair<T, U>> result = new ArrayList<>();

        if (!indexIterator.hasNext() || !streamShapes.hasNext()) {
            return result.iterator();
        }

        SpatialIndex treeIndex = indexIterator.next();
        while (streamShapes.hasNext()) {
            T streamShape = streamShapes.next();
            List<Geometry> candidates = treeIndex.query(streamShape.getEnvelopeInternal());
            for (Geometry candidate : candidates) {
                // Refine phase. Use the real polygon (instead of its MBR) to recheck the spatial relation.
                if (match(streamShape, candidate)) {
                    result.add(Pair.of(streamShape, (U) candidate));
                }
            }
        }
        return result.iterator();
    }
}
