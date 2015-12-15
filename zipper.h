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
    Mesh zip_skewness(Polyline * b1, Polyline * b2, float penalty = 1.3);
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
    } else if(b1 -> isLoop && b2 -> isLoop) {
        cout<<"The two boarders are all loops."<<endl;
        vector<Vertex*> boarder1 = b1 -> vertices;
        vector<Vertex*> boarder2 = b2 -> vertices;
        int m = boarder1.size();
        int n = boarder2.size();
        if(m < 3 || n < 3)
        {
            cout<<"Zipper Function ERROR: Please provide both "<<
            "boarders with more than or equal to 3 points when they are loops."<<endl;
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
        vector<Vertex*>::iterator bIt1end = boarder1.begin();
        vector<Vertex*>::iterator bIt2start = boarder2.begin();
        vector<Vertex*>::iterator bIt2end = boarder2.begin();
        float optionA = distance((*(boarder1.begin() + 1)) -> position, (*(boarder2.begin() + 1)) -> position)
         + distance((*(boarder1.end() - 1)) -> position, (*(boarder2.end() - 1)) -> position);
        float optionB = distance((*(boarder1.begin() + 1)) -> position, (*(boarder2.end() - 1)) -> position)
         + distance((*(boarder1.end() - 1)) -> position, (*(boarder2.begin() + 1)) -> position);
         if (optionA <= optionB) { // When the two loops have the same orientation
            Vertex * b1left = (*bIt1start);
            Vertex * b2left = (*bIt2start);
            Vertex * b1right = (*bIt1end);
            Vertex * b2right = (*bIt2end);
            int counter = 0;
            bool testToCont = true;
            bool bIt1startMoved = false;
            bool bIt2startMoved = false;
            bool bIt1endMoved = false;
            bool bIt2endMoved = false;
            do
            {
                if(counter % 2 == 0) // We need to do this symetrically
                {
                    Vertex * b1leftnext;
                    Vertex * b2leftnext;
                    if(bIt1start != boarder1.end() - 1)
                    {
                        b1leftnext = *(bIt1start + 1);
                    } else 
                    {
                        b1leftnext = *boarder1.begin();
                    }
                    if(bIt2start != boarder2.end() - 1)
                    {
                        b2leftnext = *(bIt2start + 1);
                    } else
                    {
                        b2leftnext = *boarder2.begin();
                    }
                    float optionA1 = distance(b1leftnext -> position, b2leftnext -> position);
                    float optionA2 = distance(b1left -> position, b2leftnext -> position) * trianglePenalty;
                    float optionA3 = distance(b2left -> position, b1leftnext -> position) * trianglePenalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1left, b2left, b2leftnext, b1leftnext);
                        //cout<<b1left -> ID<<" "<<b2left -> ID<<" "<<b1leftnext -> ID<<" "<<b2leftnext -> ID<<" "<<endl;
                        bIt1startMoved = true;
                        bIt2startMoved = true;
                        if(bIt1start != boarder1.end() - 1) {
                            bIt1start += 1;
                        } else {
                            bIt1start = boarder1.begin();
                        }
                        if(bIt2start != boarder2.end() - 1) {
                            bIt2start += 1;
                        } else {
                            bIt2start = boarder2.begin();
                        }
                        b1left = (*bIt1start);
                        b2left = (*bIt2start);
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b1left, b2left, b2leftnext);
                        bIt2startMoved = true;
                        if(bIt2start != boarder2.end() - 1) {
                            bIt2start += 1;
                        } else {
                            bIt2start = boarder2.begin();
                        }
                        b2left = (*bIt2start);
                    }
                    else
                    {
                        result.addTriFace(b1left, b2left, b1leftnext);
                        bIt1startMoved = true;
                        if(bIt1start != boarder1.end() - 1) {
                            bIt1start += 1;
                        } else {
                            bIt1start = boarder1.begin();
                        }
                        b1left = (*bIt1start);
                    }
                }
                else
                {
                    Vertex * b1rightnext;
                    Vertex * b2rightnext;
                    if(bIt1end != boarder1.begin())
                    {
                        b1rightnext = *(bIt1end - 1);
                    } else 
                    {
                        b1rightnext = *(boarder1.end() - 1);
                    }
                    if(bIt2end != boarder2.begin()) 
                    {
                        b2rightnext = *(bIt2end - 1);
                    } else
                    {
                        b2rightnext = *(boarder2.end() - 1);
                    }
                    float optionA1 = distance(b1rightnext -> position, b2rightnext -> position);
                    float optionA2 = distance(b1right -> position, b2rightnext -> position) * trianglePenalty;
                    float optionA3 = distance(b2right -> position, b1rightnext -> position) * trianglePenalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1rightnext, b2rightnext, b2right, b1right);
                        //cout<<b1right -> ID<<" "<<b2right -> ID<<" "<<b1rightnext -> ID<<" "<<b2rightnext -> ID<<" "<<endl;
                        bIt1endMoved = true;
                        bIt2endMoved = true;
                        if(bIt1end != boarder1.begin()) {
                            bIt1end -= 1;
                        } else {
                            bIt1end = boarder1.end() - 1;
                        }
                        if(bIt2end != boarder2.begin()) {
                            bIt2end -= 1;
                        } else {
                            bIt2end = boarder2.end() - 1;
                        }
                        b1right = (*bIt1end);
                        b2right = (*bIt2end);
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b2rightnext, b2right, b1right);
                        bIt2endMoved = true;
                        if(bIt2end != boarder2.begin()) {
                            bIt2end -= 1;
                        } else {
                            bIt2end = boarder2.end() - 1;
                        }
                        b2right = (*bIt2end);
                    }
                    else
                    {
                        result.addTriFace(b1rightnext, b2right, b1right);
                        bIt1endMoved = true;
                        if(bIt1end != boarder1.begin()) {
                            bIt1end -= 1;
                        } else {
                            bIt1end = boarder1.end() - 1;
                        }
                        b1right = (*bIt1end);
                    }
                }
            counter += 1;
            testToCont = (b1left != b1right && b2left != b2right)
             || (b1left != b1right && !bIt2startMoved && !bIt2endMoved)
             || (b2left != b2right && !bIt1startMoved && !bIt1endMoved);
            //cout<<counter<<endl;
            }
            while (testToCont);
            if(b1left == b1right && (bIt1startMoved || bIt1endMoved))
            {
                while(b2left != b2right || (!bIt2startMoved && !bIt2endMoved))
                {
                    Vertex * b2leftnext;
                    if(bIt2start != boarder2.end() - 1) {
                        b2leftnext = *(bIt2start + 1);
                    } else {
                        b2leftnext = *(boarder2.begin());
                    }
                    result.addTriFace(b1left, b2left, b2leftnext);
                    bIt2startMoved = true;
                    if(bIt2start != boarder2.end() - 1) {
                        bIt2start += 1;
                    } else {
                        bIt2start = boarder2.begin();
                    }
                    b2left = (*bIt2start);
                }
            }
            else
            {
                while(b1left != b1right || (!bIt1startMoved && !bIt1endMoved))
                {
                    Vertex * b1leftnext;
                    if(bIt1start != boarder1.end() - 1) {
                        b1leftnext = *(bIt1start + 1);
                    } else {
                        b1leftnext = *(boarder1.begin());
                    }
                    result.addTriFace(b1left, b2left, b1leftnext);
                    bIt1startMoved = true;
                    if(bIt1start != boarder1.end() - 1) {
                        bIt1start += 1;
                    } else {
                        bIt1start = boarder1.begin();
                    }
                    b1left = (*bIt1start);
                }
            }
         } else {
            cout<<"The other case."<<endl;
            Vertex * b1left = (*bIt1start);
            Vertex * b2left = (*bIt2end);
            Vertex * b1right = (*bIt1end);
            Vertex * b2right = (*bIt2start);
            int counter = 0;
            bool testToCont = true;
            bool bIt1startMoved = false;
            bool bIt2startMoved = false;
            bool bIt1endMoved = false;
            bool bIt2endMoved = false;
            do
            {
                if(counter % 2 == 0) // We need to do this symetrically
                {
                    Vertex * b1leftnext;
                    Vertex * b2leftnext;
                    if(bIt1start != boarder1.end() - 1)
                    {
                        b1leftnext = *(bIt1start + 1);
                    } else 
                    {
                        b1leftnext = *boarder1.begin();
                    }
                    if(bIt2end != boarder2.begin())
                    {
                        b2leftnext = *(bIt2end - 1);
                    } else
                    {
                        b2leftnext = *(boarder2.end() - 1);
                    }
                    float optionA1 = distance(b1leftnext -> position, b2leftnext -> position);
                    float optionA2 = distance(b1left -> position, b2leftnext -> position) * trianglePenalty;
                    float optionA3 = distance(b2left -> position, b1leftnext -> position) * trianglePenalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1left, b2left, b2leftnext, b1leftnext);
                        //cout<<b1left -> ID<<" "<<b2left -> ID<<" "<<b1leftnext -> ID<<" "<<b2leftnext -> ID<<" "<<endl;
                        bIt1startMoved = true;
                        bIt2endMoved = true;
                        if(bIt1start != boarder1.end() - 1) {
                            bIt1start += 1;
                        } else {
                            bIt1start = boarder1.begin();
                        }
                        if(bIt2end != boarder2.begin()) {
                            bIt2end -= 1;
                        } else {
                            bIt2end = boarder2.end() - 1;
                        }
                        b1left = (*bIt1start);
                        b2left = (*bIt2end);
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b1left, b2left, b2leftnext);
                        bIt2endMoved = true;
                        if(bIt2end != boarder2.begin()) {
                            bIt2end -= 1;
                        } else {
                            bIt2end = boarder2.end() - 1;
                        }
                        b2left = (*bIt2end);
                    }
                    else
                    {
                        result.addTriFace(b1left, b2left, b1leftnext);
                        bIt1startMoved = true;
                        if(bIt1start != boarder1.end() - 1) {
                            bIt1start += 1;
                        } else {
                            bIt1start = boarder1.begin();
                        }
                        b1left = (*bIt1start);
                    }
                }
                else
                {
                    Vertex * b1rightnext;
                    Vertex * b2rightnext;
                    if(bIt1end != boarder1.begin())
                    {
                        b1rightnext = *(bIt1end - 1);
                    } else 
                    {
                        b1rightnext = *(boarder1.end() - 1);
                    }
                    if(bIt2start != boarder2.end() - 1) 
                    {
                        b2rightnext = *(bIt2start + 1);
                    } else
                    {
                        b2rightnext = *(boarder2.begin());
                    }
                    float optionA1 = distance(b1rightnext -> position, b2rightnext -> position);
                    float optionA2 = distance(b1right -> position, b2rightnext -> position) * trianglePenalty;
                    float optionA3 = distance(b2right -> position, b1rightnext -> position) * trianglePenalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1rightnext, b2rightnext, b2right, b1right);
                        //cout<<b1right -> ID<<" "<<b2right -> ID<<" "<<b1rightnext -> ID<<" "<<b2rightnext -> ID<<" "<<endl;
                        bIt1endMoved = true;
                        bIt2startMoved = true;
                        if(bIt1end != boarder1.begin()) {
                            bIt1end -= 1;
                        } else {
                            bIt1end = boarder1.end() - 1;
                        }
                        if(bIt2start != boarder2.end() - 1) {
                            bIt2start += 1;
                        } else {
                            bIt2start = boarder2.begin();
                        }
                        b1right = (*bIt1end);
                        b2right = (*bIt2start);
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b2rightnext, b2right, b1right);
                        bIt2startMoved = true;
                        if(bIt2start != boarder2.end() - 1) {
                            bIt2start += 1;
                        } else {
                            bIt2start = boarder2.begin();
                        }
                        b2right = (*bIt2start);
                    }
                    else
                    {
                        result.addTriFace(b1rightnext, b2right, b1right);
                        bIt1endMoved = true;
                        if(bIt1end != boarder1.begin()) {
                            bIt1end -= 1;
                        } else {
                            bIt1end = boarder1.end() - 1;
                        }
                        b1right = (*bIt1end);
                    }
                }
            counter += 1;
            testToCont = (b1left != b1right && b2left != b2right)
             || (b1left != b1right && !bIt2startMoved && !bIt2endMoved)
             || (b2left != b2right && !bIt1startMoved && !bIt1endMoved);
            //cout<<counter<<endl;
            }
            while (testToCont);
            if(b1left == b1right && (bIt1startMoved || bIt1endMoved))
            {
                while(b2left != b2right || (!bIt2startMoved && !bIt2endMoved))
                {
                    Vertex * b2leftnext;
                    if(bIt2end != boarder2.begin()) {
                        b2leftnext = (*(bIt2end - 1));
                    } else {
                        b2leftnext = *(boarder2.end() - 1);
                    }
                    result.addTriFace(b1left, b2left, b2leftnext);
                    bIt2endMoved = true;
                    if(bIt2end != boarder2.begin()) {
                        bIt2end -= 1;
                    } else {
                        bIt2end = boarder2.end() - 1;
                    }
                    b2left = (*bIt2end);
                }
            }
            else
            {
                while(b1left != b1right || (!bIt1startMoved && !bIt1endMoved))
                {
                    Vertex * b1leftnext;
                    if(bIt1start != boarder1.end() - 1) {
                        b1leftnext = *(bIt1start + 1);
                    } else {
                        b1leftnext = *(boarder1.begin());
                    }
                    result.addTriFace(b1left, b2left, b1leftnext);
                    bIt1startMoved = true;
                    if(bIt1start != boarder1.end() - 1) {
                        bIt1start += 1;
                    } else {
                        bIt1start = boarder1.begin();
                    }
                    b1left = (*bIt1start);
                }
            }
        }
    }
    result.buildBoundary();
    return result;
}

Mesh Zipper::zip_skewness(Polyline * b1, Polyline * b2, float penalty)
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
        vector<float> b1sk = b1 -> skewness();
        vector<float> b2sk = b2 -> skewness();
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
        vector<float>::iterator bskIt1start = b1sk.begin();
        vector<float>::iterator bskIt2start = b2sk.begin();
        vector<float>::iterator bskIt1end = b1sk.end() - 1;
        vector<float>::iterator bskIt2end = b2sk.end() - 1;
        // Find the matching of the end vertices first
        float optionA = distance((*bIt1start) -> position, (*bIt2start) -> position)
         + distance((*bIt1end) -> position, (*bIt2end) -> position);
        float optionB = distance((*bIt1start) -> position, (*bIt2end) -> position)
         + distance((*bIt1end) -> position, (*bIt2start) -> position);
        if(optionA <= optionB) //Start match start, end match end
        {
            int counter = 0;
            do
            {
                if(counter % 2 == 0) // We need to do this symetrically
                {
                    float optionA1 = std::abs((*(bskIt1start + 1)) - (*(bskIt2start + 1)));
                    float optionA2 = std::abs((*(bskIt1start)) - (*(bskIt2start + 1))) * penalty;
                    float optionA3 = std::abs((*(bskIt1start + 1)) - (*(bskIt2start))) * penalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(*bIt1start, *bIt2start, *(bIt2start + 1), *(bIt1start + 1));
                        bIt1start += 1;
                        bIt2start += 1;
                        bskIt1start += 1;
                        bskIt2start += 1;
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(*bIt1start, *bIt2start, *(bIt2start + 1));
                        bIt2start += 1;
                        bskIt2start += 1;
                    }
                    else
                    {
                        result.addTriFace(*bIt1start, *bIt2start, *(bIt1start + 1));
                        bIt1start += 1;
                        bskIt1start += 1;
                    }
                }
                else
                {
                    float optionA1 = std::abs(*(bskIt1end - 1) - *(bskIt2end - 1));
                    float optionA2 = std::abs(*bskIt1end - *(bskIt2end - 1)) * penalty;
                    float optionA3 = std::abs(*bskIt2end - *(bskIt1end - 1)) * penalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(*(bIt1end - 1), *(bIt2end - 1), *bIt2end, *bIt1end);
                        bIt1end -= 1;
                        bIt2end -= 1;
                        bskIt1end -= 1;
                        bskIt2end -= 1;
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(*(bIt2end - 1), *bIt2end, *bIt1end);
                        bIt2end -= 1;
                        bskIt2end -= 1;
                    }
                    else
                    {
                        result.addTriFace(*(bIt1end - 1), *bIt2end, *bIt1end);
                        bIt1end -= 1;
                        bskIt1end -= 1;
                    }
                }
            counter += 1;
            }
            while (bIt1start != bIt1end && bIt2start != bIt2end);
            if(bIt1start == bIt1end)
            {
                while(bIt2start != bIt2end)
                {
                    result.addTriFace(*bIt1start, *bIt2start, *(bIt2start + 1));
                    bIt2start += 1;
                }
            }
            else
            {
                while(bIt1start != bIt1end)
                {
                    result.addTriFace(*bIt1start, *bIt2start, *(bIt1start + 1));
                    bIt1start += 1;
                }
            }   
        }
        else // Start match end, end match start
        {
            int counter = 0;
            do
            {
                if(counter % 2 == 0) // We need to do this symetrically
                {
                    float optionA1 = std::abs((*(bskIt1start + 1)) + (*(bskIt2end - 1)) - 1);
                    float optionA2 = std::abs((*(bskIt1start)) + (*(bskIt2end - 1)) - 1) * penalty;
                    float optionA3 = std::abs((*(bskIt1start + 1)) + (*(bskIt2end)) - 1) * penalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(*bIt1start, *bIt2end, *(bIt2end - 1), *(bIt1start + 1));
                        bIt1start += 1;
                        bIt2end -= 1;
                        bskIt1start += 1;
                        bskIt2end -= 1;
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(*bIt1start, *bIt2end, *(bIt2end - 1));
                        bIt2end -= 1;
                        bskIt2end -= 1;
                    }
                    else
                    {
                        result.addTriFace(*bIt1start, *bIt2end, *(bIt1start + 1));
                        bIt1start += 1;
                        bskIt1start += 1;
                    }
                }
                else
                {
                    float optionA1 = std::abs(*(bskIt1end - 1) + *(bskIt2start + 1) - 1);
                    float optionA2 = std::abs(*bskIt1end + *(bskIt2start + 1) - 1) * penalty;
                    float optionA3 = std::abs(*bskIt2start + *(bskIt1end - 1) - 1) * penalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(*(bIt1end - 1), *(bIt2start + 1), *bIt2start, *bIt1end);
                        bIt1end -= 1;
                        bIt2start += 1;
                        bskIt1end -= 1;
                        bskIt2start += 1;
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(*(bIt2start + 1), *bIt2start, *bIt1end);
                        bIt2start += 1;
                        bskIt2start += 1;
                    }
                    else
                    {
                        result.addTriFace(*(bIt1end - 1), *bIt2start, *bIt1end);
                        bIt1end -= 1;
                        bskIt1end -= 1;
                    }
                }
            counter += 1;
            }
            while (bIt1start != bIt1end && bIt2start != bIt2end);
            if(bIt1start == bIt1end)
            {
                while(bIt2start != bIt2end)
                {
                    result.addTriFace(*bIt1start, *bIt2end, *(bIt2end - 1));
                    bIt2end -= 1;
                }
            }
            else
            {
                while(bIt1start != bIt1end)
                {
                    result.addTriFace(*bIt1start, *bIt2end, *(bIt1start + 1));
                    bIt1start += 1;
                }
            }
        }
    } else if(b1 -> isLoop && b2 -> isLoop) {
        //cout<<"The two boarders are all loops."<<endl;
        vector<Vertex*> boarder1 = b1 -> vertices;
        vector<Vertex*> boarder2 = b2 -> vertices;
        int m = boarder1.size();
        int n = boarder2.size();
        if(m < 3 || n < 3)
        {
            cout<<"Zipper Function ERROR: Please provide both "<<
            "boarders with more than or equal to 3 points when they are loops."<<endl;
            return result;
        }
        vector<float> b1sk = b1 -> skewness();
        vector<float> b2sk = b2 -> skewness();
        vector<Vertex*>::iterator b1It;
        vector<Vertex*>::iterator b2It;
        for(b1It = boarder1.begin(); b1It < boarder1.end(); b1It ++) {
            result.addVertex(*b1It);
        }
        for(b2It = boarder2.begin(); b2It < boarder2.end(); b2It ++) {
            result.addVertex(*b2It);
        }
        vector<Vertex*>::iterator bIt1start = boarder1.begin();
        vector<Vertex*>::iterator bIt1end = boarder1.begin();
        vector<Vertex*>::iterator bIt2start = boarder2.begin();
        vector<Vertex*>::iterator bIt2end = boarder2.begin();
        vector<float>::iterator bskIt1start = b1sk.begin();
        vector<float>::iterator bskIt2start = b2sk.begin();
        vector<float>::iterator bskIt1end = b1sk.begin();
        vector<float>::iterator bskIt2end = b2sk.begin();
        float optionA = distance((*(boarder1.begin() + 1)) -> position, (*(boarder2.begin() + 1)) -> position)
         + distance((*(boarder1.end() - 1)) -> position, (*(boarder2.end() - 1)) -> position);
        float optionB = distance((*(boarder1.begin() + 1)) -> position, (*(boarder2.end() - 1)) -> position)
         + distance((*(boarder1.end() - 1)) -> position, (*(boarder2.begin() + 1)) -> position);
         if (optionA <= optionB) { // When the two loops have the same orientation
            Vertex * b1left = *bIt1start;
            Vertex * b1right = *bIt1end;
            Vertex * b2left = *bIt2start;
            Vertex * b2right = *bIt2end;
            float b1skleft = (*bskIt1start);
            float b2skleft = (*bskIt2start);
            float b1skright = (*bskIt1end);
            float b2skright = (*bskIt2end);
            int counter = 0;
            bool testToCont = true;
            bool bIt1startMoved = false;
            bool bIt2startMoved = false;
            bool bIt1endMoved = false;
            bool bIt2endMoved = false;
            do
            {
                if(counter % 2 == 0) // We need to do this symetrically
                {
                    float b1skleftnext;
                    float b2skleftnext;
                    Vertex * b1leftnext;
                    Vertex * b2leftnext;
                    if(bIt1start != boarder1.end() - 1)
                    {
                        b1skleftnext = *(bskIt1start + 1);
                        b1leftnext = *(bIt1start + 1);
                    } else 
                    {
                        b1skleftnext = *(b1sk.begin());
                        b1leftnext = *(boarder1.begin());
                    }
                    if(bIt2start != boarder2.end() - 1)
                    {
                        b2skleftnext = *(bskIt2start + 1);
                        b2leftnext = *(bIt2start + 1);
                    } else
                    {
                        b2skleftnext = *(b2sk.begin());
                        b2leftnext = *(boarder2.begin());
                    }
                    float optionA1 = std::abs(b1skleftnext - b2skleftnext);
                    float optionA2 = std::abs(b1skleft - b2skleftnext) * penalty;
                    float optionA3 = std::abs(b2skleft - b1skleftnext) * penalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1left, b2left, b2leftnext, b1leftnext);
                        //cout<<b1left -> ID<<" "<<b2left -> ID<<" "<<b1leftnext -> ID<<" "<<b2leftnext -> ID<<" "<<endl;
                        bIt1startMoved = true;
                        bIt2startMoved = true;
                        if(bIt1start != boarder1.end() - 1) {
                            bIt1start += 1;
                            bskIt1start += 1;
                        } else {
                            bIt1start = boarder1.begin();
                            bskIt1start = b1sk.begin();
                        }
                        if(bIt2start != boarder2.end() - 1) {
                            bIt2start += 1;
                            bskIt2start += 1;
                        } else {
                            bIt2start = boarder2.begin();
                            bskIt2start = b2sk.begin();
                        }
                        b1left = (*bIt1start);
                        b2left = (*bIt2start);
                        b1skleft = (*bskIt1start);
                        b2skleft = (*bskIt2start);
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b1left, b2left, b2leftnext);
                        bIt2startMoved = true;
                        if(bIt2start != boarder2.end() - 1) {
                            bIt2start += 1;
                            bskIt2start += 1;
                        } else {
                            bIt2start = boarder2.begin();
                            bskIt2start = b2sk.begin();
                        }
                        b2left = *bIt2start;
                        b2skleft = *bskIt2start;
                    }
                    else
                    {
                        result.addTriFace(b1left, b2left, b1leftnext);
                        bIt1startMoved = true;
                        if(bIt1start != boarder1.end() - 1) {
                            bIt1start += 1;
                            bskIt1start += 1;
                        } else {
                            bIt1start = boarder1.begin();
                            bskIt1start = b1sk.begin();
                        }
                        b1left = *bIt1start;
                        b1skleft = *bskIt1start;
                    }
                }
                else
                {
                    float b1skrightnext;
                    float b2skrightnext;
                    Vertex * b1rightnext;
                    Vertex * b2rightnext;
                    if(bIt1end != boarder1.begin())
                    {
                        b1rightnext = *(bIt1end - 1);
                        b1skrightnext = *(bskIt1end - 1);
                    } else 
                    {
                        b1rightnext = *(boarder1.end() - 1);
                        b1skrightnext = *(b1sk.end() - 1);
                    }
                    if(bIt2end != boarder2.begin()) 
                    {
                        b2rightnext = *(bIt2end - 1);
                        b2skrightnext = *(bskIt2end - 1);
                    } else
                    {
                        b2rightnext = *(boarder2.end() - 1);
                        b2skrightnext = *(b2sk.end() - 1);
                    }
                    float optionA1 = std::abs(b1skrightnext - b2skrightnext);
                    float optionA2 = std::abs(b1skright - b2skrightnext) * penalty;
                    float optionA3 = std::abs(b2skright - b1skrightnext) * penalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1rightnext, b2rightnext, b2right, b1right);
                        //cout<<b1right -> ID<<" "<<b2right -> ID<<" "<<b1rightnext -> ID<<" "<<b2rightnext -> ID<<" "<<endl;
                        bIt1endMoved = true;
                        bIt2endMoved = true;
                        if(bIt1end != boarder1.begin()) {
                            bIt1end -= 1;
                            bskIt1end -= 1;
                        } else {
                            bIt1end = boarder1.end() - 1;
                            bskIt1end = b1sk.end() - 1;
                        }
                        if(bIt2end != boarder2.begin()) {
                            bIt2end -= 1;
                            bskIt2end -= 1;
                        } else {
                            bIt2end = boarder2.end() - 1;
                            bskIt2end = b2sk.end() - 1;
                        }
                        b1right = (*bIt1end);
                        b2right = (*bIt2end);
                        b1skright = *bskIt1end;
                        b2skright = *bskIt2end;
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b2rightnext, b2right, b1right);
                        bIt2endMoved = true;
                        if(bIt2end != boarder2.begin()) {
                            bIt2end -= 1;
                            bskIt2end -= 1;
                        } else {
                            bIt2end = boarder2.end() - 1;
                            bskIt2end = b2sk.end() - 1;
                        }
                        b2right = (*bIt2end);
                        b2skright = *bskIt2end;
                    }
                    else
                    {
                        result.addTriFace(b1rightnext, b2right, b1right);
                        bIt1endMoved = true;
                        if(bIt1end != boarder1.begin()) {
                            bIt1end -= 1;
                            bskIt1end -= 1;
                        } else {
                            bIt1end = boarder1.end() - 1;
                            bskIt1end = b1sk.end() - 1;
                        }
                        b1skright = *bskIt1end;
                        b1right = (*bIt1end);
                    }
                }
            counter += 1;
            testToCont = (b1left != b1right && b2left != b2right)
             || (b1left != b1right && !bIt2startMoved && !bIt2endMoved)
             || (b2left != b2right && !bIt1startMoved && !bIt1endMoved);
            //cout<<counter<<endl;
            }
            while (testToCont);
            if(b1left == b1right && (bIt1startMoved || bIt1endMoved))
            {
                while(b2left != b2right || (!bIt2startMoved && !bIt2endMoved))
                {
                    Vertex * b2leftnext;
                    if(bIt2start != boarder2.end() - 1) {
                        b2leftnext = *(bIt2start + 1);
                    } else {
                        b2leftnext = *(boarder2.begin());
                    }
                    result.addTriFace(b1left, b2left, b2leftnext);
                    bIt2startMoved = true;
                    if(bIt2start != boarder2.end() - 1) {
                        bIt2start += 1;
                    } else {
                        bIt2start = boarder2.begin();
                    }
                    b2left = (*bIt2start);
                }
            }
            else
            {
                while(b1left != b1right || (!bIt1startMoved && !bIt1endMoved))
                {
                    Vertex * b1leftnext;
                    if(bIt1start != boarder1.end() - 1) {
                        b1leftnext = *(bIt1start + 1);
                    } else {
                        b1leftnext = *(boarder1.begin());
                    }
                    result.addTriFace(b1left, b2left, b1leftnext);
                    bIt1startMoved = true;
                    if(bIt1start != boarder1.end() - 1) {
                        bIt1start += 1;
                    } else {
                        bIt1start = boarder1.begin();
                    }
                    b1left = (*bIt1start);
                }
            }
         } else {
            cout<<"The other case."<<endl;
            Vertex * b1left = *bIt1start;
            Vertex * b1right = *bIt1end;
            Vertex * b2left = *bIt2end;
            Vertex * b2right = *bIt2start;
            float b1skleft = *bskIt1start;
            float b2skleft = *bskIt2end;
            float b1skright = *bskIt1end;
            float b2skright = *bskIt2start;
            int counter = 0;
            bool testToCont = true;
            bool bIt1startMoved = false;
            bool bIt2startMoved = false;
            bool bIt1endMoved = false;
            bool bIt2endMoved = false;
            do
            {
                if(counter % 2 == 0) // We need to do this symetrically
                {
                    float b1skleftnext;
                    float b2skleftnext;
                    Vertex * b1leftnext;
                    Vertex * b2leftnext;
                    if(bIt1start != boarder1.end() - 1)
                    {
                        b1skleftnext = *(bskIt1start + 1);
                        b1leftnext = *(bIt1start + 1);
                    } else 
                    {
                        b1skleftnext = *(b1sk.begin());
                        b1leftnext = *(boarder1.begin());
                    }
                    if(bIt2end != boarder2.begin())
                    {
                        b2skleftnext = *(bskIt2end - 1);
                        b2leftnext = *(bIt2end - 1);
                    } else
                    {
                        b2skleftnext = *(b2sk.end() - 1);
                        b2leftnext = *(boarder2.end() - 1);
                    }
                    float optionA1 = std::abs(b1skleftnext + b2skleftnext - 1);
                    float optionA2 = std::abs(b1skleft + b2skleftnext - 1) * penalty;
                    float optionA3 = std::abs(b2skleft + b1skleftnext - 1) * penalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1left, b2left, b2leftnext, b1leftnext);
                        //cout<<b1left -> ID<<" "<<b2left -> ID<<" "<<b1leftnext -> ID<<" "<<b2leftnext -> ID<<" "<<endl;
                        bIt1startMoved = true;
                        bIt2endMoved = true;
                        if(bIt1start != boarder1.end() - 1) {
                            bIt1start += 1;
                            bskIt1start += 1;
                        } else {
                            bIt1start = boarder1.begin();
                            bskIt1start = b1sk.begin();
                        }
                        if(bIt2end != boarder2.begin()) {
                            bIt2end -= 1;
                            bskIt2end -= 1;
                        } else {
                            bIt2end = boarder2.end() - 1;
                            bskIt2end = b2sk.end() - 1;
                        }
                        b1left = (*bIt1start);
                        b2left = (*bIt2end);
                        b1skleft = (*bskIt1start);
                        b2skleft = (*bskIt2end);
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b1left, b2left, b2leftnext);
                        bIt2endMoved = true;
                        if(bIt2end != boarder2.begin()) {
                            bIt2end -= 1;
                            bskIt2end -= 1;
                        } else {
                            bIt2end = boarder2.end() - 1;
                            bskIt2end = b2sk.end() - 1;
                        }
                        b2left = *bIt2end;
                        b2skleft = *bskIt2end;
                    }
                    else
                    {
                        result.addTriFace(b1left, b2left, b1leftnext);
                        bIt1startMoved = true;
                        if(bIt1start != boarder1.end() - 1) {
                            bIt1start += 1;
                            bskIt1start += 1;
                        } else {
                            bIt1start = boarder1.begin();
                            bskIt1start = b1sk.begin();
                        }
                        b1left = *bIt1start;
                        b1skleft = *bskIt1start;
                    }
                }
                else
                {
                    float b1skrightnext;
                    float b2skrightnext;
                    Vertex * b1rightnext;
                    Vertex * b2rightnext;
                    if(bIt1end != boarder1.begin())
                    {
                        b1rightnext = *(bIt1end - 1);
                        b1skrightnext = *(bskIt1end - 1);
                    } else 
                    {
                        b1rightnext = *(boarder1.end() - 1);
                        b1skrightnext = *(b1sk.end() - 1);
                    }
                    if(bIt2start != boarder2.end() - 1) 
                    {
                        b2rightnext = *(bIt2start + 1);
                        b2skrightnext = *(bskIt2start + 1);
                    } else
                    {
                        b2rightnext = *(boarder2.begin());
                        b2skrightnext = *(b2sk.begin());
                    }
                    float optionA1 = std::abs(b1skrightnext + b2skrightnext - 1);
                    float optionA2 = std::abs(b1skright + b2skrightnext - 1) * penalty;
                    float optionA3 = std::abs(b2skright + b1skrightnext - 1) * penalty;
                    float bestOption = std::min(optionA1, std::min(optionA2, optionA3));
                    if(bestOption == optionA1)
                    {
                        result.addQuadFace(b1rightnext, b2rightnext, b2right, b1right);
                        //cout<<b1right -> ID<<" "<<b2right -> ID<<" "<<b1rightnext -> ID<<" "<<b2rightnext -> ID<<" "<<endl;
                        bIt1endMoved = true;
                        bIt2startMoved = true;
                        if(bIt1end != boarder1.begin()) {
                            bIt1end -= 1;
                            bskIt1end -= 1;
                        } else {
                            bIt1end = boarder1.end() - 1;
                            bskIt1end = b1sk.end() - 1;
                        }
                        if(bIt2start != boarder2.end() - 1) {
                            bIt2start += 1;
                            bskIt2start += 1;
                        } else {
                            bIt2start = boarder2.begin();
                            bskIt2start = b2sk.begin();
                        }
                        b1right = *bIt1end;
                        b2right = *bIt2start;
                        b1skright = *bskIt1end;
                        b2skright = *bskIt2start;
                    }
                    else if(bestOption == optionA2)
                    {
                        result.addTriFace(b2rightnext, b2right, b1right);
                        bIt2startMoved = true;
                        if(bIt2start != boarder2.end() - 1) {
                            bIt2start += 1;
                            bskIt2start += 1;
                        } else {
                            bIt2start = boarder2.begin();
                            bskIt2start = b2sk.begin();
                        }
                        b2right = *bIt2start;
                        b2skright = *bskIt2start;
                    }
                    else
                    {
                        result.addTriFace(b1rightnext, b2right, b1right);
                        bIt1endMoved = true;
                        if(bIt1end != boarder1.begin()) {
                            bIt1end -= 1;
                            bskIt1end -= 1;
                        } else {
                            bIt1end = boarder1.end() - 1;
                            bskIt1end = b1sk.end() - 1;
                        }
                        b1skright = *bskIt1end;
                        b1right = (*bIt1end);
                    }
                }
            counter += 1;
            testToCont = (b1left != b1right && b2left != b2right)
             || (b1left != b1right && !bIt2startMoved && !bIt2endMoved)
             || (b2left != b2right && !bIt1startMoved && !bIt1endMoved);
            //cout<<counter<<endl;
            }
            while (testToCont);
            if(b1left == b1right && (bIt1startMoved || bIt1endMoved))
            {
                while(b2left != b2right || (!bIt2startMoved && !bIt2endMoved))
                {
                    Vertex * b2leftnext;
                    if(bIt2end != boarder2.begin()) {
                        b2leftnext = *(bIt2end - 1);
                    } else {
                        b2leftnext = *(boarder2.end() - 1);
                    }
                    result.addTriFace(b1left, b2left, b2leftnext);
                    bIt2endMoved = true;
                    if(bIt2end != boarder2.begin()) {
                        bIt2end -= 1;
                    } else {
                        bIt2end = boarder2.end() - 1;
                    }
                    b2left = *bIt2end;
                }
            }
            else
            {
                while(b1left != b1right || (!bIt1startMoved && !bIt1endMoved))
                {
                    Vertex * b1leftnext;
                    if(bIt1start != boarder1.end() - 1) {
                        b1leftnext = *(bIt1start + 1);
                    } else {
                        b1leftnext = *(boarder1.begin());
                    }
                    result.addTriFace(b1left, b2left, b1leftnext);
                    bIt1startMoved = true;
                    if(bIt1start != boarder1.end() - 1) {
                        bIt1start += 1;
                    } else {
                        bIt1start = boarder1.begin();
                    }
                    b1left = (*bIt1start);
                }
            }
        }
    }
    result.buildBoundary();
    return result;
}

#endif //__ZIPPER_H__