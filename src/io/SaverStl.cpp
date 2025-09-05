//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2025-08-04 22:14:44 gtaubin>
//------------------------------------------------------------------------
//
// SaverStl.cpp
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

#include "SaverStl.hpp"

#include "wrl/Shape.hpp"
#include "wrl/Appearance.hpp"
#include "wrl/Material.hpp"
#include "wrl/IndexedFaceSet.hpp"

#include "core/Faces.hpp"

const char* SaverStl::_ext = "stl";

//////////////////////////////////////////////////////////////////////
bool SaverStl::save(const char* filename, SceneGraph& wrl) const {
    bool success = false;
    if(filename!=(char*)0) {

    // Check these conditions
    bool canSave;
    // 1) the SceneGraph should have a single child
    canSave = wrl.getNumberOfChildren() == 1;
    // 2) the child should be a Shape node
    canSave = canSave && wrl[0]->isShape();
    // 3) the geometry of the Shape node should be an IndexedFaceSet node
    canSave = canSave && reinterpret_cast<Shape*>(wrl[0])->hasGeometryIndexedFaceSet();
    if (!canSave)
        return false;
    IndexedFaceSet* faceSet = reinterpret_cast<IndexedFaceSet*>(reinterpret_cast<Shape*>(wrl[0])->getGeometry());

    // 4) the IndexedFaceSet should be a triangle mesh
    canSave = faceSet->isTriangleMesh();
    // 5) the IndexedFaceSet should have normals per face
    canSave &= faceSet->getNormalBinding() == IndexedFaceSet::PB_PER_FACE_INDEXED || faceSet->getNormalBinding() == IndexedFaceSet::PB_PER_FACE;
    if (canSave) {
        const IndexedFaceSet::Binding bindingType = faceSet->getNormalBinding();

        FILE* fp = fopen(filename,"w");
        if(	fp!=(FILE*)0) {

          // if set, use ifs->getName()
          // otherwise use filename,
          // but first remove directory and extension

          fprintf(fp,"solid %s\n",filename);

          // TODO ...
          // for each face {
          //   ...
          // }

          fclose(fp);
          success = true;
        }

    }

  }
  return success;
}
