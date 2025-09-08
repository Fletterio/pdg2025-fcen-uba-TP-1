//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-04 22:09:56 gtaubin>
//------------------------------------------------------------------------
//
// Faces.cpp
//
// Written by: <Your Name>
//
// Software developed for the course
// Digital Geometry Processing
// Copyright (c) 2025, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <math.h>
#include "Faces.hpp"

// Assuming there's at least one face in the vector and that it's valid (ends in -1)
Faces::Faces(const int _nV, const vector<int>& _coordIndex)
{
    coordIndex = _coordIndex;
    unsigned int index = 0u;
    unsigned int currentFaceStartingIndex = 0u;
    for (auto vertex : coordIndex)
    {
        if (vertex >= 0)
            nVertices++;
        else
        {
            faceStartingIndex.push_back(currentFaceStartingIndex);
            currentFaceStartingIndex = index + 1;
        }
        index++;
    }
}

int Faces::getNumberOfVertices() const
{
    return nVertices;
}

int Faces::getNumberOfFaces() const
{
    return faceStartingIndex.size();
}

int Faces::getNumberOfCorners() const
{
    return coordIndex.size();
}

// We have indices for where a face begins. The difference between the index where this face begins and the next is
// exactly the face size + 1 (counting the separating -1). The only exception is the last face, for which we know
// the ending -1 is at the last index of the coordIndex array.
int Faces::getFaceSize(const int iF) const
{
    if (iF >= int(faceStartingIndex.size()))
        return 0;
    unsigned int thisFaceStartingIndex = faceStartingIndex[iF];
    // In the case of the last face, the next face "would" start 1 position past the last index in coordIndex
    unsigned int nextFaceStartingIndex = (iF == int(faceStartingIndex.size()) - 1) ? coordIndex.size() : faceStartingIndex[iF + 1];
    return nextFaceStartingIndex - thisFaceStartingIndex - 1;
}

int Faces::getFaceFirstCorner(const int iF) const
{
    if (iF >= int(faceStartingIndex.size()))
        return -1;
    return faceStartingIndex[iF];
}

// Assuming j instead of iC means its indexed as a local corner index (0 through face size - 1)
// Also interpreting "corner index" means index into the array
int Faces::getFaceVertex(const int iF, const int j) const
{
    if (iF >= int(faceStartingIndex.size()) || j >= getFaceSize(iF))
        return -1;
    return coordIndex[faceStartingIndex[iF] + j];
}

int Faces::getCornerFace(const int iC) const
{
    if (iC >= int(coordIndex.size()) || -1 == coordIndex[iC])
        return -1;
    // Since we sorted faces starting indices in ascending order we can binary search the face
    // It's going to be the face iF such that faceStartingIndex[iF] <= iC < faceStartingIndex[iF + 1]
    unsigned int left = 0, right = faceStartingIndex.size() - 1;
    while (right - left > 1)
    {
        unsigned int mid = (right + left) / 2;
        if (iC < int(faceStartingIndex[mid]))
        {
            right = mid;
        }
        else if (iC > int(faceStartingIndex[mid]))
        {
            left = mid;
        }
        // early return
        else
        {
            return mid;
        }
    }
    if (iC == int(faceStartingIndex[left]))
        return left;
    return right;
}

int Faces::getNextCorner(const int iC) const
{
    if (iC >= int(coordIndex.size()) || -1 == coordIndex[iC])
        return -1;
    if (coordIndex[iC+1] != -1)
        return iC + 1;
    return faceStartingIndex[getCornerFace(iC)];
}

