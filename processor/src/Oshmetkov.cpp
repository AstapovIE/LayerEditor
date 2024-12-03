#include "Entity.h"
#include "GeometryOperations.h"
#include "Oshmetkov.h"

#include <algorithm>
#include <unordered_set>
#include <set>


template<typename T>
void additionToVector (std::vector<T>& mainVector, const std::vector<T>& addition) {
    mainVector.insert(mainVector.end(), addition.begin(), addition.end());
}

double crossProduct (const Point& A, const Point& B, const Point& P) {
    return (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
}


void erasecCollinearPoints (AbstractPolygon* polygon) {
	bool doesEraseSmth = false;
	for (int i = 0; i < (*polygon).get_points().size(); i++) {
		int size = (*polygon).get_points().size();
		if (!crossProduct((*polygon)[(i-1+size) % size], (*polygon)[i], (*polygon)[(i+1) % size])) {
			(*polygon).remove(i);
			i--;
			doesEraseSmth = true;
		}
	}
	if (doesEraseSmth) {
		erasecCollinearPoints(polygon);
	}
}

double polygonArea(const AbstractPolygon* polygon) {
    double area = 0.0;
    int size = (*polygon).get_points().size();
    for (int i = 0; i < size; ++i) {
        Point p1 = (*polygon)[i];
        Point p2 = (*polygon)[(i + 1) % size];
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    return area / 2.0;
}


namespace FunctionalForSplit {

double straightLine (const Point& A, const Point& B, double y) {
	return (B.x - A.x)/(B.y - A.y) * y + (B.y*A.x - A.y*B.x)/(B.y - A.y);
}

void setTwoEdges (int& leftBottom, int& leftTop, int& rightBottom, int& rightTop, 
				  int lBottom, int lTop, int rBottom, int rTop) {
	leftBottom = lBottom;
	leftTop = lTop;
	rightBottom = rBottom;
	rightTop = rTop;
}

void findTwoEdges (double yMin, int indexMin, int& leftBottom, int& leftTop, int& rightBottom, 
				   int& rightTop, const AbstractPolygon* polygon) {
	int size = (*polygon).get_points().size(); 
	indexMin += size;

	if ((*polygon)[(indexMin-1) % size].y == yMin) {
		indexMin--;
	}
	int bottom = indexMin % size;
	int next = (indexMin+1) % size;
	int doubleNext = (indexMin+2) % size;
	int prev = (indexMin-1) % size;

	if ((*polygon)[next].y == yMin) {
		if ( (*polygon)[doubleNext].x > (*polygon)[prev].x ||
		     (  (*polygon)[doubleNext].x == (*polygon)[prev].x &&
		        (*polygon)[next].x > (*polygon)[bottom].x  ) ) {
			setTwoEdges(leftBottom, leftTop, rightBottom, rightTop, bottom, prev, next, doubleNext);
			return;
		}
		setTwoEdges(leftBottom, leftTop, rightBottom, rightTop, next, doubleNext, bottom, prev);
		return;
	}
	if (crossProduct((*polygon)[prev], (*polygon)[bottom], (*polygon)[next]) > 0) {
		setTwoEdges(leftBottom, leftTop, rightBottom, rightTop, 
					bottom, prev, bottom, next);
		return;
	}
	setTwoEdges(leftBottom, leftTop, rightBottom, rightTop, 
				bottom, next, bottom, prev);
}

void findBottomAndTop (double& yMin, double& yMax, int& indexMin, const AbstractPolygon* polygon) {
	yMin = (*polygon)[0].y;
	yMax = (*polygon)[0].y;
	indexMin = 0;

	for (int i = 1; i < (*polygon).get_points().size(); i++) {
		yMax = std::max(yMax, (*polygon)[i].y);
		if (yMin > (*polygon)[i].y) {
			yMin = (*polygon)[i].y;
			indexMin = i;
		}
	}
}

void updateTradepoid (Trapezoid& trapezoid0, double y0, double yNew, const int& leftBottom, 
					  const int& leftTop, const int& rightBottom, const int& rightTop, 
					  const AbstractPolygon* polygon) {
	trapezoid0.x1_bottom = trapezoid0.x1_top;
	trapezoid0.x2_bottom = trapezoid0.x2_top;
	trapezoid0.y_bottom = y0;
	trapezoid0.y_top = yNew;
	trapezoid0.x1_top = straightLine((*polygon)[leftBottom], (*polygon)[leftTop], yNew);
	trapezoid0.x2_top = straightLine((*polygon)[rightBottom], (*polygon)[rightTop], yNew);
}

void updatePairOfVertex (double x, double y, int& vertexBottom, int& vertexTop,
						 const AbstractPolygon* polygon) {
	if (x == (*polygon)[vertexTop].x && y == (*polygon)[vertexTop].y) {
		vertexBottom = vertexTop;
		int size = (*polygon).get_points().size();
		int next = (vertexTop+1) % size;
		int prev = (vertexTop-1+size) % size;

		vertexTop = ( (*polygon)[prev].y > (*polygon)[vertexTop].y ) ? prev : next;
	}
}

std::vector<Trapezoid> splitConvexPolygon (const AbstractPolygon* polygon) {
	std::vector<Trapezoid> trapezoids;

	double y0; // start y0 == yMin
	double yNew, yMax;
	int indexMin;
	int leftBottom, leftTop, rightBottom, rightTop;

	findBottomAndTop(y0, yMax, indexMin, polygon);
	findTwoEdges(y0, indexMin, leftBottom, leftTop, rightBottom, rightTop, polygon);

	Trapezoid trapezoid0{(*polygon)[leftBottom].x, (*polygon)[rightBottom].x, -1, -1, y0, -1};
	while (y0 < yMax) {

		yNew = ( (*polygon)[leftTop].y < (*polygon)[rightTop].y ) ? 
				 (*polygon)[leftTop].y : (*polygon)[rightTop].y;

		updateTradepoid(trapezoid0, y0, yNew, leftBottom, leftTop, rightBottom, rightTop, polygon);

		trapezoids.push_back( trapezoid0 );

		y0 = yNew;

		updatePairOfVertex(trapezoid0.x1_top, y0, leftBottom, leftTop, polygon);
		updatePairOfVertex(trapezoid0.x2_top, y0, rightBottom, rightTop, polygon);
	}

	return trapezoids;
}

bool isPointInTriangle (const Point& A, const Point& B, const Point& C, const Point& P) {
    double cross1 = crossProduct(A, B, P);
    double cross2 = crossProduct(B, C, P);
    double cross3 = crossProduct(C, A, P);

    return (cross1 > 0 && cross2 > 0 && cross3 > 0) || 
           (cross1 < 0 && cross2 < 0 && cross3 < 0);
}

bool isConcaveVertex (const AbstractPolygon* polygon, int index, bool isClockwise) {
    int size = (*polygon).get_points().size();

    Point prevPoint = (*polygon)[(index + size - 1) % size];
    Point currentPoint = (*polygon)[index];
    Point nextPoint = (*polygon)[(index + 1) % size];

    double cross = crossProduct(prevPoint, currentPoint, nextPoint);

    return ( isClockwise ) ? cross > 0 : cross < 0;
}

std::vector<int> findConcaveVertexIndexes (const AbstractPolygon* polygon) {
	std::vector<int> ConcaveVertex;
	bool isClockwise = polygonArea(polygon) < 0;
	for (int index = 0; index < (*polygon).get_points().size(); index++) {
		if (isConcaveVertex(polygon, index, isClockwise)) {
			ConcaveVertex.push_back(index);
		}
	}
	return ConcaveVertex;
}

bool isFullTriangle (int i, const std::vector<Point>& smoothPolygonVec) {
	int size = smoothPolygonVec.size();
	for (int j = 0; j < smoothPolygonVec.size(); j++) {
		if (j == i-1 || j == i || j == i+1) {
			continue;
		}
		if ( isPointInTriangle(smoothPolygonVec[(i-1+size) % size], smoothPolygonVec[i],
						  smoothPolygonVec[(i+1) % size], smoothPolygonVec[j]) ) {
			return true;
		}

	}
	return false;
}

void addPosibleConcaveVertex (int posibleConcaveIndex, std::vector<int>& ConcaveVertex, 
							  const std::vector<Point>& smoothPolygonVec, bool isClockwise) {
	Polygon smoothPolygon{smoothPolygonVec};
	if (find(ConcaveVertex.begin(), ConcaveVertex.end(), posibleConcaveIndex) == ConcaveVertex.end() && 
		isConcaveVertex(&smoothPolygon, posibleConcaveIndex, isClockwise)) {
		ConcaveVertex.push_back(posibleConcaveIndex);
	}
}

void updateConcaveVertex (int concaveIndex, std::vector<int>& ConcaveVertex) {
	for (int k = 0; k < ConcaveVertex.size(); k++) {
		if (ConcaveVertex[k] > concaveIndex) {
			ConcaveVertex[k]--;
		}
		if (ConcaveVertex[k] == concaveIndex) {
			ConcaveVertex.erase( ConcaveVertex.begin() + k );
			k--;
		}
	}
}

std::vector<Trapezoid> splitConcavePolygon (const AbstractPolygon* polygon) {
	std::vector<Trapezoid> triangles;
	std::vector<Point> smoothPolygonVec = (*polygon).get_points();
	std::vector<int> ConcaveVertex = findConcaveVertexIndexes(polygon);
	bool isClockwise = polygonArea(polygon) < 0;
	int size = smoothPolygonVec.size();

	while (true) {
		if (!ConcaveVertex.size()) {
			break;
		}
		int concaveIndex = -1;
		for (int i : ConcaveVertex) {
			if ( !isFullTriangle(i, smoothPolygonVec) ) {
				Polygon triangle{ { smoothPolygonVec[(i-1+size) % size], smoothPolygonVec[i], 
									smoothPolygonVec[(i+1) % size] } };
				smoothPolygonVec.erase( smoothPolygonVec.begin() + i );
				size--;
				additionToVector( triangles, splitConvexPolygon(&triangle) );
				concaveIndex = i;
				break;
			}
		}
		updateConcaveVertex(concaveIndex, ConcaveVertex);
		addPosibleConcaveVertex((concaveIndex-1+size) % size, ConcaveVertex, 
								 smoothPolygonVec, isClockwise);
		addPosibleConcaveVertex(concaveIndex % size, ConcaveVertex, smoothPolygonVec, isClockwise);
	}
	Polygon smoothPolygon{ smoothPolygonVec };
	erasecCollinearPoints(&smoothPolygon);
	std::vector<Trapezoid> smoothPolygonTrapezoids = splitConvexPolygon(&smoothPolygon);

	return TrapezoidOperations::subtract(smoothPolygonTrapezoids, triangles);
}

bool isConcavePolygon (const AbstractPolygon* polygon) {
	int size = (*polygon).get_points().size();
	bool isClockwise = polygonArea(polygon) < 0;
	for (int k = 0; k < size; k++) {
		if (isConcaveVertex(polygon, k, isClockwise)) {
			return true;
		}
	}
	return false;
}

std::vector<Trapezoid> splitPolygonIntoTrapezoids (const Polygon& polygon){
	std::vector<Trapezoid> trapezoids = (isConcavePolygon(&polygon)) ? splitConcavePolygon(&polygon)
														   			: splitConvexPolygon(&polygon);
	std::vector<Trapezoid> holes;

	for (const Hole& hole : polygon.get_holes()) {
		std::vector<Trapezoid> holeTrapezoids = (isConcavePolygon(&hole)) ? splitConcavePolygon(&hole)
														   				 : splitConvexPolygon(&hole);
		additionToVector( holes, holeTrapezoids );
	}

	return TrapezoidOperations::subtract(trapezoids, holes);
}

};


std::vector<Trapezoid> PolygonOperations::splitPolygonIntoTrapezoids (const Polygon& polygon) {
	return FunctionalForSplit::splitPolygonIntoTrapezoids(polygon);
}


////////////////////////////////////////////////////////////////////////////////////////


namespace FunctionalForReconnection {

bool areCollinear (const Point& A, const Point& B, const Point& P) {
    return crossProduct(A, B, P) == 0;
}

bool onInterval (const Point& p, const Point& q, const Point& r) {
    return std::min(p.x, r.x) <= q.x && q.x <= std::max(p.x, r.x) &&
           std::min(p.y, r.y) <= q.y && q.y <= std::max(p.y, r.y) && !(p==q) && !(r==q);
}

bool areSimilar (const Point& A, const Point& B, const Point& C, const Point& D) {
	return (A==C && B==D) || (A==D && B==C);
}

bool IntervalsOverlap (const Point& A, const Point& B, const Point& C, const Point& D) {
	if (A == B || C == D) return false;
	if (areSimilar(A, B, C, D)) return true;
    if (areCollinear(A, B, C) && areCollinear(A, B, D)) {
        return onInterval(A, C, B) || onInterval(A, D, B) || onInterval(C, A, D) || onInterval(C, B, D);
    }
    return false;
}

bool areAdjacent(const Trapezoid& t1, const Trapezoid& t2) {

	if (IntervalsOverlap(Point{t1.x1_bottom, t1.y_bottom}, Point{t1.x2_bottom, t1.y_bottom},
					 	 Point{t2.x1_top, t2.y_top}, Point{t2.x2_top, t2.y_top}) ||

		IntervalsOverlap(Point{t1.x1_top, t1.y_top}, Point{t1.x2_top, t1.y_top},
					 	 Point{t2.x1_bottom, t2.y_bottom}, Point{t2.x2_bottom, t2.y_bottom}) ||

		IntervalsOverlap(Point{t1.x2_bottom, t1.y_bottom}, Point{t1.x2_top, t1.y_top},
					 Point{t2.x1_bottom, t2.y_bottom}, Point{t2.x1_top, t2.y_top}) ||

		IntervalsOverlap(Point{t1.x1_bottom, t1.y_bottom}, Point{t1.x1_top, t1.y_top},
					 Point{t2.x2_bottom, t2.y_bottom}, Point{t2.x2_top, t2.y_top})
	   ) {
		return true;
	}
    return false;
}

std::unordered_map<int, std::vector<int>> createEdgeList (const std::vector<Trapezoid>& trapezoids) {
	std::unordered_map<int, std::vector<int>> edgeList;
	std::vector<int> neighbors;
	for (int i = 0; i < trapezoids.size(); i++){
		edgeList[i] = neighbors;
	}
    for (int i = 0; i < trapezoids.size(); i++) {
        for (int j = i + 1; j < trapezoids.size(); j++) {
            if (areAdjacent(trapezoids[i], trapezoids[j])) {
                edgeList[i].push_back(j);
                edgeList[j].push_back(i);
            }
        }
    }
    return edgeList;
}

void findComponent (const std::vector<Trapezoid>& trapezoids, const std::unordered_map<int, std::vector<int>>& edgeList,
		  int vertex, std::vector<bool>& marked, std::vector<Trapezoid>& component) {

	if (marked[vertex]) return;
	marked[vertex] = true;
	component.push_back(trapezoids[vertex]);

	for (int neighbor : edgeList.at(vertex)) {
		findComponent (trapezoids, edgeList, neighbor, marked, component);
	}
}

std::vector<std::vector<Trapezoid>> findConnectedComponents (const std::vector<Trapezoid>& trapezoids) {
    std::vector<std::vector<Trapezoid>> components;
    std::unordered_map<int, std::vector<int>> edgeList = createEdgeList(trapezoids); 
    std::vector<bool> marked(trapezoids.size(), false);


    for (int vertex = 0; vertex < trapezoids.size(); vertex++) {
    	std::vector<Trapezoid> component;
    	findComponent(trapezoids, edgeList, vertex, marked, component);
    	if (component.size() > 0) {
    		components.push_back(component);
    	}
    }
    return components;
}

Polygon createPolygonFromTrapezoid(const Trapezoid& tr) {
	Polygon poly({Point{tr.x1_bottom, tr.y_bottom}, Point{tr.x1_top, tr.y_top},
				  Point{tr.x2_top, tr.y_top}, Point{tr.x2_bottom, tr.y_bottom}});
	erasecCollinearPoints(&poly);
	return poly;
}

struct Node {
	Polygon polygon;
	std::vector<Node*> neighbors;
	Point averagePoint{0, 0};
	int marked = 0;
	static int currMarked;

	Node (const Polygon& poly) : polygon{poly.get_points(), poly.get_holes()} {
		udpateAveragePoint();
	}

	void addNeighbor (Node* neighbor) {
		for (Node* node : neighbors) {
			if (node == neighbor) {
				return;
			}
		}
		neighbors.push_back(neighbor);
	}
	void eraseNeighbor (Node* neighbor) {
		auto iterator = std::find(neighbors.begin(), neighbors.end(), neighbor);
		if (iterator != neighbors.end()) {
			neighbors.erase(iterator);
		}
	}
	void udpateAveragePoint () {
		averagePoint = Point{0.0, 0.0};
		for (const Point& point : polygon.get_points()) {
			averagePoint.x += point.x;
			averagePoint.y += point.y;

		}
		averagePoint.x /= polygon.get_points().size();
		averagePoint.y /= polygon.get_points().size();
	}
};

int Node::currMarked = 0;

Node* createGraph (const std::vector<Trapezoid>& component) {
	std::vector<Node*> polygons;

	for (const Trapezoid& tr : component) {
		Node* node = new Node(createPolygonFromTrapezoid(tr));
		polygons.push_back(node);
	}
	std::unordered_map<int, std::vector<int>> edgeList = createEdgeList(component);

	for (int k = 0; k < component.size(); k++) {
		for (int neighbor : edgeList[k]) {
			polygons[k]->addNeighbor(polygons[neighbor]);
		}		
	}
	return polygons[0];
}

std::vector<std::pair<int, int>> findInsertIndexes (Node* mainPolygonNode, Node* leafNode) {
	const Polygon& mainPolygon = mainPolygonNode->polygon;
	const Polygon& leaf = leafNode->polygon;

	std::vector<std::pair<int, int>> insertIndexesVec;
	int sizeMain = mainPolygon.get_points().size();
	int sizeLeaf = leaf.get_points().size();

	for (int i = 0; i < sizeMain; i++) {
		for (int j = 0; j < sizeLeaf; j++) {
			if (IntervalsOverlap( mainPolygon[i], mainPolygon[(i+1) % sizeMain],
								  leaf[j], leaf[(j+1) % sizeLeaf])) {
				std::pair<int, int> insertIndexes(-1, -1);
				insertIndexes.first = (i + 1) % sizeMain;
				insertIndexes.second = (j + 1) % sizeLeaf;

				insertIndexesVec.push_back(insertIndexes);
			}
		}
	}
	std::sort(insertIndexesVec.begin(), insertIndexesVec.end(), 
			  [](auto pair1, auto pair2) { return pair1.first < pair2.first; });

	std::vector<std::pair<int, int>> insertIndexesVecNew;
	int sizeVec = insertIndexesVec.size();

	for (int i = 0; i < sizeVec; i++) {
		if ( (insertIndexesVec[i].first + 1) % sizeMain != insertIndexesVec[(i+1) % sizeVec].first ) {
			insertIndexesVecNew.push_back(insertIndexesVec[i]);
		}
	}
	return insertIndexesVecNew;
}


void updateNodes (Node*& root, Node* mainNode, Node* leaf) {
	mainNode->eraseNeighbor(leaf);
	for (Node* neighbor : leaf->neighbors) {
		if (neighbor == mainNode ) {
			continue;
		}
		neighbor->eraseNeighbor(leaf);
		neighbor->addNeighbor(mainNode);
		mainNode->addNeighbor(neighbor);
	}
	if (root == leaf) {
		root = root->neighbors[0];
	}
	delete leaf;
}


void createHole (Polygon& mainPolygon, Polygon& addedPolygon, std::vector<std::pair<int, int>>& insertIndexes) {
	int sizeMain = mainPolygon.get_points().size();
	int sizeAddedNode = addedPolygon.get_points().size();

	std::vector<Point> holeVec;

	int currIndex = insertIndexes[0].first;
	int currRemoveIndex = insertIndexes[0].first;

	while (currIndex != insertIndexes[1].first) {
		holeVec.push_back(mainPolygon[currRemoveIndex]);
		mainPolygon.remove(currRemoveIndex);
		currIndex = (currIndex + 1) % sizeMain;

		if (currRemoveIndex == 0) {
			insertIndexes[0].first--;
		}
		if (insertIndexes[0].first == mainPolygon.get_points().size()) {
			currRemoveIndex = 0;
		}
	}
	currIndex = insertIndexes[1].second;

	while (currIndex != insertIndexes[0].second) {
		holeVec.push_back(addedPolygon[currIndex]);
		currIndex = (currIndex + 1) % sizeAddedNode;
	}

	Hole hole(holeVec);
	erasecCollinearPoints(&hole);
	mainPolygon.add_hole(hole);
}

bool doesCreateHole (Node* mainNode, Node* addedNode, std::vector<std::pair<int, int>> insertIndexes) {
	Polygon mainPolygon{mainNode->polygon.get_points(), mainNode->polygon.get_holes()};
	Polygon addedPolygon{addedNode->polygon.get_points(), addedNode->polygon.get_holes()};

	createHole(mainPolygon, addedPolygon, insertIndexes);

	return polygonArea(&mainPolygon) < 0;
}

void createHole (Node* mainNode, Node* addedNode, std::vector<std::pair<int, int>>& insertIndexes) {
	if (!doesCreateHole(mainNode, addedNode, insertIndexes)) {
		std::swap(insertIndexes[0], insertIndexes[1]);
	}
	createHole(mainNode->polygon, addedNode->polygon, insertIndexes);
}

void unionPolygons (Node*& root, Node* mainNode, Node* leaf) {
	int sizeMain = mainNode->polygon.get_points().size();
	int sizeLeaf = leaf->polygon.get_points().size();

	std::vector<std::pair<int, int>> insertIndexes = findInsertIndexes(mainNode, leaf);

	if ( insertIndexes.size() > 1 ) {
		createHole(mainNode, leaf, insertIndexes);
	}

	int currIndex = insertIndexes[0].second;
	int maxIndex = ( insertIndexes.size() > 1 ) ? insertIndexes[1].second : insertIndexes[0].second;

	std::vector<Point> leafPoints;
	std::vector<Point> newPoints = mainNode->polygon.get_points();

	for (int k = 0; currIndex != maxIndex || k == 0; k++) {
		leafPoints.push_back(leaf->polygon[currIndex]);
		currIndex = (currIndex + 1) % sizeLeaf;		
	}
	newPoints.insert(newPoints.begin() + insertIndexes[0].first, leafPoints.begin(), leafPoints.end());
	mainNode->polygon = Polygon{newPoints, mainNode->polygon.get_holes()};

	erasecCollinearPoints(&(mainNode->polygon));
	mainNode->udpateAveragePoint();

	for (const Hole& hole : leaf->polygon.get_holes()) {
		mainNode->polygon.add_hole(hole);
	}

	updateNodes(root, mainNode, leaf);
}


bool isPointOnSegment(const Point& point, const Point& a, const Point& b) {
    return areCollinear(point, a, b) &&
           point.x >= std::min(a.x, b.x) && point.x <= std::max(a.x, b.x) &&
           point.y >= std::min(a.y, b.y) && point.y <= std::max(a.y, b.y);
}

bool doesRayIntersect(const Point& point, const Point& a, const Point& b) {
    if (a.y > b.y) {
    	return doesRayIntersect(point, b, a);
    }
    if (isPointOnSegment(point, a, b)) return false;

    if (point.y <= a.y || point.y > b.y || point.x >= std::max(a.x, b.x))
        return false;

    double xIntersection = a.x + (point.y - a.y) * (b.x - a.x) / (b.y - a.y);
    return point.x < xIntersection;
}

bool isPointInsidePolygon (const Point& point, const std::vector<Point>& polygon) {
    int intersections = 0;
    int size = polygon.size();
    
    for (int i = 0; i < size; i++) {
        const Point& a = polygon[i];
        const Point& b = polygon[(i + 1) % size];
        
        if (isPointOnSegment(point, a, b)) {
            return false;
        }
        if (doesRayIntersect(point, a, b)) {
            intersections++;
        }
    }
    return intersections % 2 == 1;
}

bool isInnerCycleWithOtherCycle (std::vector<Node*>& currentCycle, std::vector<Node*>& otherCycle) {
	std::vector<Point> polygon;
	for (Node* node : currentCycle) {
		polygon.push_back(node->averagePoint);
	}
	for (Node* node : otherCycle) {
		if ( isPointInsidePolygon(node->averagePoint, polygon) ) {
			return false;
		}
	}
	return true;
}

bool isInnerCycle (int currentCycleIndex, std::vector<std::vector<Node*>>& cycles) {
	for (int j = 0; j < cycles.size(); j++) {
		if (j == currentCycleIndex) {
			continue;
		}
		if (!isInnerCycleWithOtherCycle(cycles[currentCycleIndex], cycles[j])) {
			return false;
		}
	}
	return true;
}

std::vector<std::vector<Node*>> findInnerCycles (std::vector<std::vector<Node*>>& cycles) {
	std::vector<std::vector<Node*>> innerCycles;
	for (int i = 0; i < cycles.size(); i++) {
		if (isInnerCycle(i, cycles)) {
			innerCycles.push_back(cycles[i]);
		}
	}
	return innerCycles;
}

std::vector<Node*> findInnerCycle (std::vector<std::vector<Node*>>& cycles) {
	std::vector<std::vector<Node*>> innerCycles = findInnerCycles(cycles);
	int minSize = innerCycles[0].size();
	int indexMin = 0;
	for (int j = 1; j < innerCycles.size(); j++) {
		if (innerCycles[j].size() < minSize) {
			minSize = innerCycles[j].size();
			indexMin = j;
		}
	}
	return innerCycles[indexMin];
}

std::vector<Node*> sortCycle (const std::vector<Node*>& cycle) {
    std::vector<Node*> normalized = cycle;
    std::sort(normalized.begin(), normalized.end());
    return normalized;
}

void dfsFindCyclesWithStartNode (Node* current, Node* start, std::unordered_set<Node*>& visited,
                    std::vector<Node*>& path, std::set<std::vector<Node*>>& uniqueCycles,
                    std::vector<std::vector<Node*>>& cycles) {
    path.push_back(current);
    visited.insert(current);

    for (Node* neighbor : current->neighbors) {
        if (neighbor == start && path.size() > 2) {
            // Сycle is found
            std::vector<Node*> sortedCycle = sortCycle(path);
            if (uniqueCycles.find(sortedCycle) == uniqueCycles.end()) {
            	cycles.push_back(path);
            	uniqueCycles.insert(sortedCycle);
            }
        } else if (visited.find(neighbor) == visited.end()) {
            dfsFindCyclesWithStartNode(neighbor, start, visited, path, uniqueCycles, cycles);
        }
    }
    path.pop_back();
    visited.erase(current);
}

void dfsFindCycles (Node* root, std::set<std::vector<Node*>>& uniqueCycles, std::vector<std::vector<Node*>>& cycles) {
	root->marked = Node::currMarked;

	std::unordered_set<Node*> visited;
	std::vector<Node*> path;
	dfsFindCyclesWithStartNode(root, root, visited, path, uniqueCycles, cycles);

	for (Node* neighbor : root->neighbors) {
		if (neighbor->marked < Node::currMarked) {
			dfsFindCycles(neighbor, uniqueCycles, cycles);
		}
	}
}

std::vector<std::vector<Node*>> findAllUniqueCycles (Node* root) {
    std::set<std::vector<Node*>> uniqueCycles;
    std::vector<std::vector<Node*>> cycles;

    Node::currMarked++;
    dfsFindCycles(root, uniqueCycles, cycles);

    return cycles;
}

void cycleProcess (Node*& root) {
	std::vector<std::vector<Node*>> cycles = findAllUniqueCycles(root);

	while (cycles.size() > 0) {
		std::vector<Node*> cycle = findInnerCycle(cycles);

		for (int j = 1; j < cycle.size(); j++) {
			unionPolygons(root, cycle[0], cycle[j]);
		}
		cycles = findAllUniqueCycles(root);
	}
}

void dfsFindLeaf(Node* root, Node*& leaf) {
	if (leaf != nullptr) return;

	root->marked = Node::currMarked;
	if (root->neighbors.size() == 1) {
		leaf = root;
		return;
	}
	for (Node* neighbor : root->neighbors) {
		if (neighbor->marked != Node::currMarked) {
			dfsFindLeaf(neighbor, leaf);
		}
	}
}

void leafProcess (Node*& root) {
	Node* leaf  = nullptr;
	Node::currMarked++;
	dfsFindLeaf(root, leaf);

	while (leaf != nullptr) {
		Node* mainNode = leaf->neighbors[0];
		unionPolygons(root, mainNode, leaf);

		leaf = nullptr;
		Node::currMarked++;
		dfsFindLeaf(root, leaf);
	}
}

Polygon mergeTrapezoidsToPolygon (const std::vector<Trapezoid>& component) {
	Node* root = createGraph(component);

	cycleProcess(root);
	leafProcess(root);

	Polygon polygon = root->polygon;
	delete root;

	return polygon;
}

std::vector<Polygon> reconnection (std::vector<Trapezoid>& trapezoids) {
    std::vector<Polygon> polygons;
    auto components = findConnectedComponents(trapezoids);

    for (const auto& component : components) {
    	polygons.push_back( mergeTrapezoidsToPolygon(component) );
    }
    return polygons;
}

};


std::vector<Polygon> PolygonOperations::reconnection (std::vector<Trapezoid>& trapezoids) {
	return FunctionalForReconnection::reconnection(trapezoids);
}




/////////////////////////////////////////////////////////////////////////


std::vector<Trapezoid> splitLayerIntoTrapezoids (const std::vector<Polygon>& polygons) {
	std::vector<Trapezoid> layer;
	for (const Polygon& polygon : polygons) {
		additionToVector( layer, PolygonOperations::splitPolygonIntoTrapezoids(polygon) );
	}
	return layer;
}

std::vector<Polygon> PolygonOperations::processPolygons (const std::vector<Polygon>& polygons1, 
										 				 const std::vector<Polygon>& polygons2, 
										 				 OperationType operation) {

	std::vector<Trapezoid> layer1 = splitLayerIntoTrapezoids(polygons1);
	std::vector<Trapezoid> layer2 = splitLayerIntoTrapezoids(polygons2);

	std::vector<Trapezoid> newLayer;

	switch (operation)
	{
	case OperationType::UNION:
		newLayer = TrapezoidOperations::unite(layer1, layer2);
		break;
	case OperationType::INTERSECTION:
		newLayer = TrapezoidOperations::intersect(layer1, layer2);
		break;
	case OperationType::SUBTRACTION:
		newLayer = TrapezoidOperations::subtract(layer1, layer2);
		break;
	}
	return PolygonOperations::reconnection(newLayer);
}