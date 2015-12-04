// 
// @author Andy Wang, UC Berkeley. Copyright 2015 reserve.
// Catmull-Clark subdivision project in order to create sculptures like Eva-Hilds.
// Advised by Prof. Sequin H. Carlos.
// 
//

#ifndef __ZIPPER_H__
#define __ZIPPER_H__

#include <vector>
#include <math.h>
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

// Forward declarations
class Vertex;
class Edge;
class Face;
class Polyline;

//////////////////////////////////////////////////////////////////////
// Polyline Class -- A Polyline is a list of vertices
class Zipper
{
public:
    // Constructor
    Zipper();
    // Function zipper, zippering two polylines.
    Mesh zip(Polyline * b1, Polyline * b2, float trianglePenalty = 1.3);
};

Zipper::Zipper()
{

}
Mesh Zipper::zip(Polyline * b1, Polyline * b2, float trianglePenalty)
{
    Mesh result;
    if(!(b1 -> isLoop) && !(b2 -> isLoop))
    {
        vector<Vertex*> boarder1 = b1 -> vertices;
        vector<Vertex*> boarder2 = b2 -> vertices;
        int m = boarder1.size();
        int n = boarder2.size();
        if(m < 2 || n < 2)
        {
            cout<<"Zipper Function ERROR: Please provide both "<<
            "boarders with more than or equal to 2 points."<<endl;
            return result;
        }
        vector<Vertex*>::iterator b1It;
        vector<Vertex*>::iterator b2It;
        for(b1It = boarder1.begin(); b1It < boarder1.end(); b1It ++) {
            result.addVertex(*b1It);
        }
        for(b2It = boarder2.begin(); b2It < boarder2.end(); b2It ++) {
            result.addVertex(*b2It);
        }
        vector<Vertex*>::iterator bIt1start = boarder1.begin();
        vector<Vertex*>::iterator bIt1end = boarder1.end() - 1;
        vector<Vertex*>::iterator bIt2start = boarder2.begin();
        vector<Vertex*>::iterator bIt2end = boarder2.end() - 1;
        // Find the matching of the end vertices first
        float optionA = distance((*bIt1start) -> position, (*bIt2start) -> position)
         + distance((*bIt1end) -> position, (*bIt2end) -> position);
        float optionB = distance((*bIt1start) -> position, (*bIt2end) -> position)
         + distance((*bIt1end) -> position, (*bIt2start) -> position);
        if(optionA <= optionB) //Start match start, end match end
        {
            Vertex * b1left = (*bIt1start);
            Vertex * b2left = (*bIt2start);
            Vertex * b1right = (*bIt1end);
            Vertex * b2right = (*bIt2end);
            int counter = 0;
            do
            {
                if(counter % 2 == 0) // We need to do this symetrically
                {
                    Vertex * b1leftnext = (*(bIt1start + 1));
                    Vertex * b2leftnext = (*(bIt2start + 1));
                    float optionA1 = distance(b1leftnext -> position, b2leftnext -> position);
                    float optionA2 = distance(b1left -> position, b2leftnext -> position) * trianglePenalty;
                    float optionA3 = distance(b2left -> position, b1leftnext -> position) * trianglePenalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1left, b2left, b2leftnext, b1leftnext);
                        bIt1start += 1;
                        bIt2start += 1;
                        b1left = (*bIt1start);
                        b2left = (*bIt2start);
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b1left, b2left, b2leftnext);
                        bIt2start += 1;
                        b2left = (*bIt2start);
                    }
                    else
                    {
                        result.addTriFace(b1left, b2left, b1leftnext);
                        bIt1start += 1;
                        b1left = (*bIt1start);
                    }
                }
                else
                {
                    Vertex * b1rightnext = (*(bIt1end - 1));
                    Vertex * b2rightnext = (*(bIt2end - 1));
                    float optionA1 = distance(b1rightnext -> position, b2rightnext -> position);
                    float optionA2 = distance(b1right -> position, b2rightnext -> position) * trianglePenalty;
                    float optionA3 = distance(b2right -> position, b1rightnext -> position) * trianglePenalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1rightnext, b2rightnext, b2right, b1right);
                        bIt1end -= 1;
                        bIt2end -= 1;
                        b1right = (*bIt1end);
                        b2right = (*bIt2end);
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b2rightnext, b2right, b1right);
                        bIt2end -= 1;
                        b2right = (*bIt2end);
                    }
                    else
                    {
                        result.addTriFace(b1rightnext, b2right, b1right);
                        bIt1end -= 1;
                        b1right = (*bIt1end);
                    }
                }
            counter += 1;
            }
            while (b1left != b1right && b2left != b2right);
            if(b1left == b1right)
            {
                while(b2left != b2right)
                {
                    Vertex * b2leftnext = (*(bIt2start + 1));
                    result.addTriFace(b1left, b2left, b2leftnext);
                    bIt2start += 1;
                    b2left = (*bIt2start);
                }
            }
            else
            {
                while(b1left != b1right)
                {
                    Vertex * b1leftnext = (*(bIt1start + 1));
                    result.addTriFace(b1left, b2left, b1leftnext);
                    bIt1start += 1;
                    b1left = (*bIt1start);
                }
            }
        }
        else // Start match end, end match start
        {
            Vertex * b1left = (*bIt1start);
            Vertex * b2left = (*bIt2end);
            Vertex * b1right = (*bIt1end);
            Vertex * b2right = (*bIt2start);
            int counter = 0;
            do
            {
                if(counter % 2 == 0) // We need to do this symetrically
                {
                    Vertex * b1leftnext = (*(bIt1start + 1));
                    Vertex * b2leftnext = (*(bIt2end - 1));
                    float optionA1 = distance(b1leftnext -> position, b2leftnext -> position);
                    float optionA2 = distance(b1left -> position, b2leftnext -> position) * trianglePenalty;
                    float optionA3 = distance(b2left -> position, b1leftnext -> position) * trianglePenalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1left, b2left, b2leftnext, b1leftnext);
                        bIt1start += 1;
                        bIt2end -= 1;
                        b1left = (*bIt1start);
                        b2left = (*bIt2end);
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b1left, b2left, b2leftnext);
                        bIt2end -= 1;
                        b2left = (*bIt2end);
                    }
                    else
                    {
                        result.addTriFace(b1left, b2left, b1leftnext);
                        bIt1start += 1;
                        b1left = (*bIt1start);
                    }
                }
                else
                {
                    Vertex * b1rightnext = (*(bIt1end - 1));
                    Vertex * b2rightnext = (*(bIt2start + 1));
                    float optionA1 = distance(b1rightnext -> position, b2rightnext -> position);
                    float optionA2 = distance(b1right -> position, b2rightnext -> position) * trianglePenalty;
                    float optionA3 = distance(b2right -> position, b1rightnext -> position) * trianglePenalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1rightnext, b2rightnext, b2right, b1right);
                        bIt1end -= 1;
                        bIt2start += 1;
                        b1right = (*bIt1end);
                        b2right = (*bIt2start);
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b2rightnext, b2right, b1right);
                        bIt2start += 1;
                        b2right = (*bIt2start);
                    }
                    else
                    {
                        result.addTriFace(b1rightnext, b2right, b1right);
                        bIt1end -= 1;
                        b1right = (*bIt1end);
                    }
                }
            counter += 1;
            }
            while (b1left != b1right && b2left != b2right);
            if(b1left == b1right)
            {
                while(b2left != b2right)
                {
                    Vertex * b2leftnext = (*(bIt2end - 1));
                    result.addTriFace(b1left, b2left, b2leftnext);
                    bIt2end -= 1;
                    b2left = (*bIt2end);
                }
            }
            else
            {
                while(b1left != b1right)
                {
                    Vertex * b1leftnext = (*(bIt1start + 1));
                    result.addTriFace(b1left, b2left, b1leftnext);
                    bIt1start += 1;
                    b1left = (*bIt1start);
                }
            }
  
        }
    }
    result.buildBoundary();
    return result;
}

#endif //__ZIPPER_H__