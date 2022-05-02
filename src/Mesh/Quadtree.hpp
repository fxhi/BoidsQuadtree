#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <vector>
#include <algorithm> //std::max
#include <cassert>
#include <memory>

#include <glm/glm.hpp>

#include "../Tools/Geometry/Line.hpp"
#include "../Tools/Geometry/Rectangle.hpp"

/* 
What happened if m_point are in the exact same location,
does the quadtree divided infinitely when the number of 
maximum m_point is reached in a node ?
*/


template<typename T=float, typename T_rect=float>
class Quadtree {
public:
    Quadtree() = default;
    Quadtree(Rectangle<T_rect> boundary, int max_points=4, int depth=0) :
        m_boundary(boundary), m_max_points(max_points), m_depth(depth){
            // std::cout << "Depth: " << depth << std::endl;

            assert(m_max_points > 0 && "ERROR: max_points need to be > 0.");
            assert(m_depth >= 0 && "ERROR: depth need to be >= 0.");
    }
    ~Quadtree() {
        if (m_divided){
            for(int i=0; i<4; i++) {
                // delete children[i]; //freed memory and call detructor of each children
                // children[i] = NULL; //pointed dangling ptr to NULL
                children[i].reset(); // freed memory for unique_ptr, children[i] will point to nullptr
            }
        }
    }

    // Divide this node by spawning four children nodes.""""
    void divide() {
        auto center = m_boundary.getCenter();
        auto width = m_boundary.getWidth()/2;
        auto height = m_boundary.getHeight()/2;
        
        children[0] = std::make_unique<Quadtree<T>>(Rectangle<T_rect>(center.x+width/2, center.y+height/2, width, height), 
                                    m_max_points, m_depth+1);
        children[1] = std::make_unique<Quadtree<T>>(Rectangle<T_rect>(center.x-width/2, center.y+height/2, width, height),
                                    m_max_points, m_depth+1);
        children[2] = std::make_unique<Quadtree<T>>(Rectangle<T_rect>(center.x-width/2, center.y-height/2, width, height),
                                    m_max_points, m_depth+1);
        children[3] = std::make_unique<Quadtree<T>>(Rectangle<T_rect>(center.x+width/2, center.y-height/2, width, height),
                                    m_max_points, m_depth+1);
        
        m_divided = true;
    }

    void insert(T in_point, T_rect in_x, T_rect in_y) {
        Data data{in_point, in_x, in_y};
        auto x = data.x;
        auto y = data.y;

        // Check if the m_point lies inside this boundary
        if (!m_boundary.contains(x,y)) {
            return;
        }

        // Otherwise the m_point lies inside this boundary
        // add +1 to the total number of points
        m_total_points += 1;

        if (!m_divided) {
            if(m_point.size() < m_max_points) {
                m_point.push_back(data);
                return;
            }
        }

        if (!m_divided) {
            divide();
            m_point.push_back(data);
            // for (T p : m_point) {
            for (auto & p : m_point) {
            // for (T const & d : m_point) {
                children[1]->insert(p.data, p.x, p.y);
                children[2]->insert(p.data, p.x, p.y);
                children[3]->insert(p.data, p.x, p.y);
                children[0]->insert(p.data, p.x, p.y);
            }
            m_point.clear();
        } else { // is already divided
                children[0]->insert(data.data, data.x, data.y);
                children[1]->insert(data.data, data.x, data.y);
                children[2]->insert(data.data, data.x, data.y);
                children[3]->insert(data.data, data.x, data.y);
        }
    }

    std::vector<Line<T_rect>> getLineToDrawShape() {
        // Return a vector of Vector2, where each vector2 reprent a line to draw
        // to get the shape of the present quadtree.
        
        std::vector<Line<T_rect>> lines;
        if (m_depth == 0) {
            //It is the first node, draw the rectangle around the domain
            lines.push_back(Line(m_boundary.getLeft(), m_boundary.getTop(), m_boundary.getRight(), m_boundary.getTop()));
            lines.push_back(Line(m_boundary.getLeft(), m_boundary.getTop(), m_boundary.getLeft(), m_boundary.getBottom()));
            lines.push_back(Line(m_boundary.getLeft(), m_boundary.getBottom(), m_boundary.getRight(), m_boundary.getBottom()));
            lines.push_back(Line(m_boundary.getRight(), m_boundary.getBottom(), m_boundary.getRight(), m_boundary.getTop()));
        }

        // second option : https://stackoverflow.com/questions/201718/concatenating-two-stdvectors
        if (m_divided) {
            // If divided, add the lines that divide the node
            lines.push_back(Line(m_boundary.getLeft(), (m_boundary.getTop()+m_boundary.getBottom())/2, m_boundary.getRight(), (m_boundary.getTop()+m_boundary.getBottom())/2));
            lines.push_back(Line((m_boundary.getLeft()+m_boundary.getRight())/2, m_boundary.getTop(), (m_boundary.getLeft()+m_boundary.getRight())/2, m_boundary.getBottom()));
           
            std::vector<Line<T_rect>> buffer;
            buffer = children[0]->getLineToDrawShape();
            lines.insert(lines.end(), buffer.begin(), buffer.end());
            buffer.clear();

            buffer = children[1]->getLineToDrawShape();
            lines.insert(lines.end(), buffer.begin(), buffer.end());
            buffer.clear();

            buffer = children[2]->getLineToDrawShape();
            lines.insert(lines.end(), buffer.begin(), buffer.end());
            buffer.clear();

            buffer = children[3]->getLineToDrawShape();
            lines.insert(lines.end(), buffer.begin(), buffer.end());
            buffer.clear();
        }

        return lines;
    }

    std::vector<glm::vec2> getPointPositionToDrawPoints() {
        // Todo : m_point.x1 not always same type than T_rect
        std::vector<glm::vec2> points;
        for (auto const & p : m_point) {
            points.push_back(Vector2(p.x, p.y));
        }

        // std::cout << "depth: " << m_depth << " / number of particles: " << m_point.size() << std::endl;

        if (m_divided) {
            std::vector<glm::vec2> buffer;
            buffer = children[0]->getPointPositionToDrawPoints();
            points.insert(points.end(), buffer.begin(), buffer.end());
            buffer.clear();

            buffer = children[1]->getPointPositionToDrawPoints();
            points.insert(points.end(), buffer.begin(), buffer.end());
            buffer.clear();

            buffer = children[2]->getPointPositionToDrawPoints();
            points.insert(points.end(), buffer.begin(), buffer.end());
            buffer.clear();

            buffer = children[3]->getPointPositionToDrawPoints();
            points.insert(points.end(), buffer.begin(), buffer.end());
            buffer.clear();
        }

        return points;
    }

    int getMaxDepth() {
        int maxDepth = 0;

        maxDepth = m_depth;

        if (m_divided) {
            maxDepth = children[0]->getMaxDepth();
            maxDepth = std::max(maxDepth, children[1]->getMaxDepth());
            maxDepth = std::max(maxDepth, children[2]->getMaxDepth());
            maxDepth = std::max(maxDepth, children[3]->getMaxDepth());
        }

        return maxDepth;
    }

    std::vector<T> query(Rectangle<T_rect> in_boundary) {
        // Find the points in the quadtree that lie within boundary.

        std::vector<T> out_points;

        if (!m_boundary.intersects(in_boundary)){
            // The query boundary does not intersect the node boundary.
            return out_points;
        }

        if (m_divided) {
            // If already divided, search only inside the children.
            std::vector<T> buffer;

            buffer = children[0]->query(in_boundary);
            out_points.insert(out_points.end(), buffer.begin(), buffer.end());
            
            buffer = children[1]->query(in_boundary);
            out_points.insert(out_points.end(), buffer.begin(), buffer.end());
            
            buffer = children[2]->query(in_boundary);
            out_points.insert(out_points.end(), buffer.begin(), buffer.end());
            
            buffer = children[3]->query(in_boundary);
            out_points.insert(out_points.end(), buffer.begin(), buffer.end());
        } else {
            // Otherwide return all the points that are inside this node and inside the query boundary.
            for (auto const & p : m_point) {
                if (in_boundary.contains(p.x, p.y)) {
                    out_points.push_back(p);
                }
            }
        }

        return out_points;
    }

    std::vector<T> queryRadius(float cx, float cy, float radius) {
        // First get all the points that lie inside the rectangle wich bounds the associated circle.
        // Then get all the points inside the requested circle, boundary included.
        std::vector<T> out_points;
        std::vector<T> buffer;

        buffer = query(Rectangle<T_rect>(cx, cy, radius*2+1, radius*2+1));
        //width and height of boundary are +1 to ensure getting all the points, since for the Rectangle class
        //the "contains" function is lower equal to for left/bottom and strictly lower for rith/top.

        for (auto const & p : buffer) {
            if (cx-radius <= p.x && p.x <= cx+radius &&
                cy-radius <= p.y && p.y <= cy+radius) {
                
                out_points.push_back(p);
            }
        }

        return buffer;
    }

    // Need to update this function with the new format where a Data structure is used.
    // void update(std::vector<T> points) {
    //     // Deletes all the nodes except the first node and inserts the points passed as argument.

    //     if (m_divided){
    //         for(int i=0; i<4; i++) {
    //             // delete children[i]; //freed memory and call detructor of each children
    //             // children[i] = NULL; //pointed dangling ptr to NULL
    //             children[i].reset();
    //         }
    //     }

    //     m_divided = false;
    //     m_total_points = 0;

    //     for (auto const & p : points) {
    //         insert(p);
    //     }
        
    // }

    void clear() {
        // Deletes all the nodes except the first node and inserts the points passed as argument.

        if (m_divided){
            for(int i=0; i<4; i++) {
                // delete children[i]; //freed memory and call detructor of each children
                // children[i] = NULL; //pointed dangling ptr to NULL
                children[i].reset();
            }
        }

        m_divided = false;
        m_total_points = 0;
    }


public:
    struct Data {
        T data;
        T_rect x;
        T_rect y;
    };
    std::vector<Data> m_point;
    Rectangle<T_rect> m_boundary;
    int m_max_points = 4; // Maximal number of points that a node can have.
    int m_depth = 0; // Depth of the quadtree, first node is 0.
    bool m_divided = false; // Is the node is divided, is the node had childrens ?
    //right-top, top-left, left-bottom, bottom-right
    // Quadtree<T>* children[4] = {nullptr, nullptr, nullptr, nullptr}; 
    std::unique_ptr<Quadtree<T>> children[4];

    int m_total_points = 0; // Total number of m_point inside a node, childrens included.

};


#endif