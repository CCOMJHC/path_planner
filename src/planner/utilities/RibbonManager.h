#ifndef SRC_RIBBONMANAGER_H
#define SRC_RIBBONMANAGER_H

#include <list>
#include <path_planner/State.h>
#include "Ribbon.h"
extern "C" {
#include <dubins.h>
}

class RibbonManager {
public:
    enum Heuristic {
        MaxDistance,
        TspPointRobotNoSplitAllRibbons,
        TspPointRobotNoSplitKRibbons,
        TspDubinsNoSplitAllRibbons,
        TspDubinsNoSplitKRibbons,
    };

    RibbonManager();
    explicit RibbonManager(Heuristic heuristic);
    RibbonManager(Heuristic heuristic, double turningRadius);
    RibbonManager(Heuristic heuristic, double turningRadius, int k);

    void add(double x1, double y1, double x2, double y2);

    void cover(double x, double y);

    bool done() const;

    double approximateDistanceUntilDone(double x, double y, double yaw);

    double minDistanceFrom(double x, double y);

    State getNearestEndpointAsState(const State& state) const;

//    RibbonManager& operator=(const RibbonManager& other);

    std::string dumpRibbons() const;
private:
    Heuristic m_Heuristic;
    double m_TurningRadius = -1;
    int m_K;

    std::list<Ribbon> m_Ribbons;

    static double distance(std::pair<double, double> p1, std::pair<double, double> p2) {
        return distance(p1.first, p1.second, p2.first, p2.second);
    }
    static double distance(std::pair<double, double> p, double x, double y){
        return distance(p.first, p.second, x, y);
    }
    static double distance(double x1, double y1, double x2, double y2) {
        return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    }
    double dubinsDistance(double x, double y, double h, const State& s) {
        if (m_TurningRadius == -1) throw std::logic_error("Cannot compute ribbon dubins distance with unset turning radius");
        DubinsPath dubinsPath;
        double q1[] = {x, y, h}, q2[] = {s.x, s.y, s.yaw()};
        dubins_shortest_path(&dubinsPath, q1, q2, m_TurningRadius);
        return dubins_path_length(&dubinsPath);
    }

    void add(const Ribbon& r, std::list<Ribbon>::iterator i);

    static double tspPointRobotNoSplitAllRibbons(std::list<Ribbon> ribbonsLeft, double distanceSoFar, std::pair<double, double> point);

    double tspDubinsNoSplitAllRibbons(std::list<Ribbon> ribbonsLeft, double distanceSoFar, double x,
                                      double y, double yaw);

    double tspPointRobotNoSplitKRibbons(std::list<Ribbon> ribbonsLeft, double distanceSoFar, std::pair<double, double> point);

    double tspDubinsNoSplitKRibbons(std::list<Ribbon> ribbonsLeft, double distanceSoFar, double x,
                                    double y, double yaw);
};


#endif //SRC_RIBBONMANAGER_H