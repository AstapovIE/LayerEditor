#ifndef OSHMETKOV_H
#define OSHMETKOV_H

#include <vector>
#include "GeometryOperations.h"

enum class OperationType { UNION, INTERSECTION, SUBTRACTION};

namespace PolygonOperations {

	std::vector<Trapezoid> splitPolygonIntoTrapezoids (const Polygon& polygon);

	std::vector<Polygon> reconnection (std::vector<Trapezoid>& trapezoids);

	std::vector<Polygon> processPolygons (const std::vector<Polygon>& polygons1, 
										 const std::vector<Polygon>& polygons2, 
										 OperationType operation);
};

#endif