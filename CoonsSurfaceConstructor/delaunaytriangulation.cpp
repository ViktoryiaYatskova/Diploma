#include "delaunaytriangulation.h"

DelaunayTriangulation::
    DelaunayTriangulation(){}

DelaunayTriangulation::
    DelaunayTriangulation(Vector < Point2D >& trs): points(trs) {}

DelaunayTriangulation::
    DelaunayTriangulation(Vector < TriangularUnit >& trs): triangles(trs){}

DelaunayTriangulation::
    DelaunayTriangulation(Point2D* triangularPoints, int length) {

    points = Vector < Point2D >(triangularPoints, length);
}

/*Алгоритм Киркпатрика
Данный алгоритм основан на стандартной для многих алгоритмов методике сведения сложной задачи к более простым,
в которых решение очевидно. Сам алгоритм для N > 5 состоит из 3 шагов:

1. Разбиение исходного множества на более мелкие множества.
 Для этого мы проводим вертикальные или горизонтальные прямые в середине множества и
 уже относительно этих прямых разделяем точки на две части примерно по N/2.
 После для каждой группы точек рекурсивно запускаем процесс деления в зависимости от их количества:
    1. Если число точек N > 12, то делим множество с помощью прямых.
    2. Если число точек N ≤ 12, то делим множество на 3 и N − 3 точек.
    3. Если число точек N = 8, то делим множество на 2 группы по 4 точки.
Деление продолжается до тех пор, пока не останется 3 или 4 точки.

2. Построение триангуляции для множеств из 3 или 4 точек.
Для трёх точек триангуляция очевидна — просто соединяем попарно точки отрезками.
Для четырёх точек возможны два варианта:
    1. Если точки образуют невыпуклый четырёхугольник, то просто соединяем все 4 точки отрезками.
    2. Если точки образуют выпуклый четырёхугольник, то берём любые 3 точки и
 проверяем положение четвёртой точки относительно окружности,
 описанной вокруг первых трёх точек. Здесь возможны три варианта:
        1. Точка лежит за пределами окружности. Данная триангуляция оптимальна,
         строим треугольник из первых трёх точек и соединяем с четвёртой ближайшие к ней 2 точки.
        2. Точка лежит внутри окружности. В этом случае нарушается условие триангуляции Делоне,
        и отрезками соединяются четвёртая точка со всеми остальными точками, а также те точки,
        отрезки между которыми не создадут пересечений с уже проведёнными отрезками.
        3. Точка лежит на окружности. В этом случае любая триангуляция оптимальна.

3. Объединение оптимальных триангуляций. Сначала находятся две пары точек,
отрезки которых образуют в совокупности с построенными триангуляциями выпуклую фигуру.
Они соединяются отрезками, и один из полученных отрезков выбирается как начало для последующего обхода.
 Обход заключается в следующем: на этом отрезке мы как будто «надуваем пузырь» внутрь до первой точки,
 которую достигнет раздувающаяся окружность «пузыря». С найденной точкой соединяется та точка отрезка,
 которая не была с ней соединена. Полученный отрезок проверяется на пересечение с уже существующими отрезками триангуляции,
 и в случае пересечения они удаляются из триангуляции. После этого новый отрезок принимается за начало для нового «пузыря».
 Цикл повторяется до тех пор, пока начало не совпадёт со вторым отрезком выпуклой оболочки.
*/
DelaunayTriangulation& DelaunayTriangulation::
    build (DelaunayTriangulation& trianqular) {

    Vector < Point2D > points = trianqular.points;
    Vector < TriangularUnit > triangles = trianqular.triangles;
    DelaunayTriangulation *trianqular1, *trianqular2;
    TriangularUnit* triangle;

    int numberOfPoints = points.length();

    switch(numberOfPoints) {

    case 1:
    case 2:
        throw std::logic_error("Two little points number");

    case 3:
        triangle = new TriangularUnit(points.data());
        triangles.append(*triangle);
        break;

    case 4:
        if (isPolygonConvex(points.data())) {
            triangularsFromConvexPolygon(points);
            /*triangles.append(new TriangularUnit(points.mid(0, 2).data()));
            triangles.append(new TriangularUnit(points.mid(1, 3).data()));*/
        } else {
            triangle = new TriangularUnit(points.mid(0, 2).data());
            triangles.append(*triangle);
            triangle = new TriangularUnit(points.mid(1, 3).data());
            triangles.append(*triangle);
            Point2D thirdTriangular[] = {
                Point2D(points.at(0)),
                Point2D(points.at(2)),
                Point2D(points.at(3))
            };
            triangle = new TriangularUnit(thirdTriangular);
            triangles.append(*triangle);
        }
        break;

    case 8:
        trianqular1 = new DelaunayTriangulation(points.mid(0, 3).data(), 4);
        trianqular2 = new DelaunayTriangulation(points.mid(3, 7).data(), 4);
        build(*trianqular1);
        build(*trianqular2);
        return joinTriangulars(*trianqular1, *trianqular2);
        break;

    default:
        if (numberOfPoints >= 12) {
            int halfLength = ceil(numberOfPoints / 2.0);
            trianqular1 = new DelaunayTriangulation(points.mid(0, halfLength).data(), halfLength);
            trianqular2 = new DelaunayTriangulation(points.mid(halfLength + 1, numberOfPoints).data(), numberOfPoints - halfLength - 1);
            build(*trianqular1);
            build(*trianqular2);
            return joinTriangulars(*trianqular1, *trianqular2);
        } else {
            trianqular1 = new DelaunayTriangulation(points.mid(0, 2).data(), 3);
            trianqular2 = new DelaunayTriangulation(points.mid(3, numberOfPoints).data(), numberOfPoints - 3);
            build(*trianqular1);
            build(*trianqular2);
            return joinTriangulars(*trianqular1, *trianqular2);
        }
    }
    return *(new DelaunayTriangulation(triangles));
}


DelaunayTriangulation& DelaunayTriangulation::
    joinTriangulars(DelaunayTriangulation& triangulation1, DelaunayTriangulation& triangulation2) {

    return triangulation1;
}

bool DelaunayTriangulation::
    isPolygonConvex(Point2D* points) {

    Point2D p1, p2, p3;
    Point2D vect1, vect2;
    double cosAngle, angle;
    int isAngleBelowStraingtSum;

    for (int i = 0; i < 4; i++) {
        p1 = points[(i + 3) % 4];
        p2 = points[i % 4];
        p3 = points[(i + 1) % 4];

        vect1.setX( p2.x() - p1.x() );
        vect1.setY( p2.y() - p1.y() );

        vect2.setX( p3.x() - p2.x() );
        vect2.setY( p3.y() - p2.y() );

        cosAngle = (vect1.x()*vect2.x() + vect1.y()*vect2.y()) /
                   sqrt((vect1.x()*vect1.x() + vect1.y()*vect1.y())*(vect2.x()*vect2.x()+ vect2.y()*vect2.y()));
        angle = acos(cosAngle);
        
        isAngleBelowStraingtSum = angle < M_PI? 1: 0;
    }
    
    return isAngleBelowStraingtSum == 4 || isAngleBelowStraingtSum == 0;
}

/**
 * @brief triangularsFromConvexPolygon
 * @param polygon
 *
 *     2. Если точки образуют выпуклый четырёхугольник, то берём любые 3 точки и
 *проверяем положение четвёртой точки относительно окружности,
 *описанной вокруг первых трёх точек. Здесь возможны три варианта:
 *       1. Точка лежит за пределами окружности. Данная триангуляция оптимальна,
 *       строим треугольник из первых трёх точек и соединяем с четвёртой ближайшие к ней 2 точки.
 *       2. Точка лежит внутри окружности. В этом случае нарушается условие триангуляции Делоне,
 *    и отрезками соединяются четвёртая точка со всеми остальными точками, а также те точки,
 *    отрезки между которыми не создадут пересечений с уже проведёнными отрезками.
 *       3. Точка лежит на окружности. В этом случае любая триангуляция оптимальна.
 */
Vector < TriangularUnit >* DelaunayTriangulation::
    triangularsFromConvexPolygon(Vector < Point2D >& polygon) {

    Vector < TriangularUnit >* triangles = new Vector < TriangularUnit >();
    Point2D lastPoint = polygon.last();
    polygon.pop_back();
    int isPointInsideEscribedCircle = lastPoint.isPointInsideEscribedCircle(polygon);

    if (isPointInsideEscribedCircle >= 0) {

        /*
         * isPointInsideEscribedCircle > 0
         *
         * Точка лежит за пределами окружности. Данная триангуляция оптимальна,
         * строим треугольник из первых трёх точек и
         * соединяем с четвёртой ближайшие к ней 2 точки.
         */

        /* isPointInsideEscribedCircle = 0
         *
         * Точка лежит на окружности. В этом случае любая триангуляция оптимальна
         */
        TriangularUnit* newTriangle = new TriangularUnit(polygon.data());
        triangles->append(*newTriangle);

        Point2D* twoClosestPoints = lastPoint.getTwoClosestPointTo(polygon.data(), 3);
        newTriangle = new TriangularUnit(lastPoint, twoClosestPoints[0], twoClosestPoints[1]);
        triangles->append(*newTriangle);

        delete [] twoClosestPoints ;

    } else {

        /* Точка лежит внутри окружности.
         * В этом случае нарушается условие триангуляции Делоне,
         * и отрезками соединяются четвёртая точка со всеми остальными точками,
         * а также те точки, отрезки между которыми не создадут пересечений с уже проведёнными отрезками.
         */
        Vector < Edge > newEdges;
        for(int i = 0; i < 3; i++) {
            newEdges.append(Edge(lastPoint, polygon.at(i)));
        }
        newEdges.appendItems(Edge::createAllPossibleEdgesThatNotCrossOthers(polygon, newEdges));
    }
    return triangles;
}


