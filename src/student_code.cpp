/*
 * Student solution for CMU 15-462 Project 2 (MeshEdit)
 *
 * Implemented by ____ on ____.
 *
 */

#include "student_code.h"
#include "mutablePriorityQueue.h"

namespace CMU462
{
   VertexIter HalfedgeMesh::splitEdge( EdgeIter e0 )
   {  
      //ignore requests to split boundary edges
      if (e0->isBoundary()) return e0->halfedge()->vertex();

      //COLLECT ELEMENTS
      //Halfedges
      HalfedgeIter h0 = e0->halfedge();
      HalfedgeIter h1 = h0->next();
      HalfedgeIter h2 = h1->next();
      HalfedgeIter h3 = h0->twin();
      HalfedgeIter h4 = h3->next();
      HalfedgeIter h5 = h4->next();
      HalfedgeIter h6 = h1->twin();
      HalfedgeIter h7 = h2->twin();
      HalfedgeIter h8 = h4->twin();
      HalfedgeIter h9 = h5->twin();

      //Vertices
      VertexIter v0 = h0->vertex();
      VertexIter v1 = h1->vertex();
      VertexIter v2 = h2->vertex();
      VertexIter v3 = h8->vertex();
      
      //Edges
      EdgeIter e1 = h1->edge();
      EdgeIter e2 = h2->edge();
      EdgeIter e3 = h4->edge();
      EdgeIter e4 = h5->edge();
      
      //Faces
      FaceIter f0 = h0->face();
      FaceIter f1 = h3->face();
      
      //ALLOCATE NEW ELEMENT
      //Halfedges
      HalfedgeIter h10 = newHalfedge();
      HalfedgeIter h11 = newHalfedge();
      HalfedgeIter h12 = newHalfedge();
      HalfedgeIter h13 = newHalfedge();
      HalfedgeIter h14 = newHalfedge();
      HalfedgeIter h15 = newHalfedge();
      
      //Vertices
      VertexIter v4 = newVertex(); //or newVertex()?

      //Edges
      EdgeIter e5 = newEdge();
      EdgeIter e6 = newEdge(); 
      EdgeIter e7 = newEdge();       

      //Faces
      FaceIter f2 = newFace();
      FaceIter f3 = newFace();

      //Denote the position of v4
      v4->position = (v0->position + v1->position)/2;

      //REASSIGN ELEMENTS
      //Halfedges
       //setNeighbors(next,twin,vertex,edge,face)
       h0->setNeighbors(h5,h3,v4,e0,f0);
       h1->setNeighbors(h11,h7,v2,e2,f3);
       h2->setNeighbors(h13,h8,v0,e3,f2);
       h3->setNeighbors(h14,h0,v1,e0,f1);
       h4->setNeighbors(h3,h9,v3,e4,f1);
       h5->setNeighbors(h15,h6,v1,e1,f0);
       h6->setNeighbors(h6->next(),h5,v2,e1,h6->face());
       h7->setNeighbors(h7->next(),h1,v0,e2,h7->face());
       h8->setNeighbors(h8->next(),h2,v3,e3,h8->face());
       h9->setNeighbors(h9->next(),h4,v1,e4,h9->face());
       h10->setNeighbors(h1,h15,v4,e5,f3);
       h11->setNeighbors(h10,h12,v0,e6,f3);
       h12->setNeighbors(h2,h11,v4,e6,f2);
       h13->setNeighbors(h12,h14,v3,e7,f2);
       h14->setNeighbors(h4,h13,v4,e7,f1);
       h15->setNeighbors(h0,h10,v2,e5,f0);

      //Vertices
      v0->halfedge() = h2;
      v1->halfedge() = h5;
      v2->halfedge() = h1;
      v3->halfedge() = h4;
      v4->halfedge() = h0;

      //Edges
      e0->halfedge() = h0;
      e1->halfedge() = h5;
      e2->halfedge() = h1;
      e3->halfedge() = h2;
      e4->halfedge() = h4;
      e5->halfedge() = h10;
      e6->halfedge() = h11;
      e7->halfedge() = h13;
      
      //Faces
      f0->halfedge() = h0;
      f1->halfedge() = h3;
      f2->halfedge() = h2;
      f3->halfedge() = h1;

      // TODO This method should split the given edge and return an iterator to the newly inserted vertex.
      // TODO The halfedge of this vertex should point along the edge that was split, rather than the new edges.

      return v4;
   }

   bool HalfedgeMesh::isConnected( VertexCIter v1, VertexCIter v2 )
   {
      HalfedgeCIter h = v1->halfedge(); // get one of the outgoing halfedges of the vertex
      do
      {
        HalfedgeCIter h_twin = h->twin(); // get the vertex of the current halfedge
        VertexCIter v = h_twin->vertex(); // vertex is 'source' of the half edge.
                                        // so h->vertex() is v,
                                        // whereas h_twin->vertex() is the neighbor vertex.
        if(v == v2) return true;
        h = h_twin->next(); // move to the next outgoing halfedge of the vertex.
      }
      while( h != v1->halfedge() ); // keep going until we're back at the beginning

      return false;
   }

   VertexIter HalfedgeMesh::collapseEdge( EdgeIter e )
   {
      //Ignore the requests to flip boundary edges
      if (e->isBoundary()) return e->halfedge()->vertex();
      //Ignore requests to perform any edge collapse that would make the surface non-manifold or otherwise invalidate the mesh
      //Exclude all tetrahedron topology
      VertexIter v1 = e->halfedge()->vertex();
      VertexIter v2 = e->halfedge()->twin()->vertex();

      //Double iterate all vertices connected with v1
      //if find a connected pair && they are both connected with v2
      //then do not collapse

      HalfedgeCIter h = v1->halfedge(); // get one of the outgoing halfedges of the vertex
      do
      {
        HalfedgeCIter h_twin = h->twin(); // get the vertex of the current halfedge
        VertexCIter v = h_twin->vertex(); // vertex is 'source' of the half edge.
                                        // so h->vertex() is v,
                                        // whereas h_twin->vertex() is the neighbor vertex.
        HalfedgeCIter hh = v1->halfedge(); // get one of the outgoing halfedges of the vertex
        do
        {
          HalfedgeCIter hh_twin = hh->twin(); 
          VertexCIter vv = hh_twin->vertex(); 
                                          
          if ( isConnected(v,vv) && isConnected(v,v2) && isConnected(vv,v2) )
             return e->halfedge()->vertex();  //end collapsing function

          hh = hh_twin->next(); 
        }
        while( hh != v1->halfedge() ); // keep going until we're back at the beginning

        h = h_twin->next(); // move to the next outgoing halfedge of the vertex.
      }
      while( h != v1->halfedge() ); // keep going until we're back at the beginning
 
      //COLLECT ELEMENTS INSIDE QUAD acbd
      //Halfedges
      HalfedgeIter h1 = e->halfedge();
      HalfedgeIter h2 = h1->next();
      HalfedgeIter h3 = h2->next();
      HalfedgeIter h4 = h1->twin();
      HalfedgeIter h5 = h4->next();
      HalfedgeIter h6 = h5->next();
      HalfedgeIter h7 = h2->twin();
      HalfedgeIter h8 = h3->twin();
      HalfedgeIter h9 = h6->twin();
      HalfedgeIter h10 = h5->twin();

      //Vertices
      VertexIter va = h3->vertex();
      VertexIter vb = h6->vertex();
      VertexIter vc = h2->vertex();
      VertexIter vd = h1->vertex();
      
      //Edges
      EdgeIter e1 = h1->edge();
      EdgeIter e2 = h2->edge();
      EdgeIter e3 = h3->edge();
      EdgeIter e4 = h6->edge();
      EdgeIter e5 = h5->edge();
      
      //Faces
      FaceIter f1 = h1->face();
      FaceIter f2 = h4->face();
      
      //Denote new vertex m
      VertexIter vm = newVertex();
      vm->position = (vc->position + vd->position)/2;
            
      //DEALING WITH ELEMNTS IN FACES CONTAINING c
      
      //Initialization
      HalfedgeIter hx = h2->twin(); 
      HalfedgeIter hy;
      
      do      
      
      {
        //COLLECT ELEMENTS INSIDE THE FACE
        hy = hx->next();
        HalfedgeIter hz = hy->next();
      
        EdgeIter ex = hx->edge();
        EdgeIter ey = hy->edge();
        EdgeIter ez = hz->edge();
      
        VertexIter vx = hz->vertex();
        VertexIter vy = hx->vertex();
      
        FaceIter f = hz->face();
      
        //REASSIGN ELEMENTS
        //Halfedges
        //setNeighbors(next,twin,vertex,edge,face)
        hx->setNeighbors(hy,hx->twin(),vy,ex,f);
        hy->setNeighbors(hz,hy->twin(),vm,ey,f);
        hz->setNeighbors(hx,hz->twin(),vx,ez,f);
      
        //Vertices
        vx->halfedge() = hz;
        vy->halfedge() = hx;
        vm->halfedge() = hy;
      
      
        //Edges
        ex->halfedge() = hx;
        ey->halfedge() = hy;
        ez->halfedge() = hz;

        //Faces
        f->halfedge() = hz;

        //Iterate
         hx = hy->twin();

      }
      while ( hy->twin() != h6 );

      //DEALING WITH ELEMNTS IN FACES CONTAINING d

      //Initialization
      hx = h3->twin(); 
      HalfedgeIter hz;
      
      do
      
      {
        //COLLECT ELEMENTS INSIDE THE FACE
        HalfedgeIter hy = hx->next();
        hz = hy->next();

        EdgeIter ex = hx->edge();
        EdgeIter ey = hy->edge();
        EdgeIter ez = hz->edge();
      
        VertexIter vx = hz->vertex();
        VertexIter vz = hy->vertex();

        FaceIter f = hx->face();

        //REASSIGN ELEMENTS
        //Halfedges
        //setNeighbors(next,twin,vertex,edge,face)
        hx->setNeighbors(hy,hx->twin(),vm,ex,f);
        hy->setNeighbors(hz,hy->twin(),vz,ey,f);
        hz->setNeighbors(hx,hz->twin(),vx,ez,f);

        //Vertices
        vx->halfedge() = hz;
        vz->halfedge() = hy;
        vm->halfedge() = hx;

        //Edges
        ex->halfedge() = hx;
        ey->halfedge() = hy;
        ez->halfedge() = hz;
      
        //Faces
        f->halfedge() = hy;
      
        //Iterate
         hx = hz->twin();
      }
      while ( hz->twin() != h5 );

      //Dealing with merging edges
      h7->setNeighbors(h7->next(),h8,va,e2,h7->face());
      h8->setNeighbors(h8->next(),h7,vm,e2,h8->face());
      h9->setNeighbors(h9->next(),h10,vm,e4,h9->face());
      h10->setNeighbors(h10->next(),h9,vb,e4,h10->face());

      //DELETE UNUSED ELEMENTS
      //Halfedges
      deleteHalfedge(h1);
      deleteHalfedge(h2);
      deleteHalfedge(h3);
      deleteHalfedge(h4);
      deleteHalfedge(h5);
      deleteHalfedge(h6);

      //Vertices
      deleteVertex(vc);
      deleteVertex(vd);
      
      //Edges
      deleteEdge(e1);
      deleteEdge(e3);
      deleteEdge(e5);
 
      //Faces();
      deleteFace(f1);
      deleteFace(f2);

      // TODO This method should collapse the given edge and return an iterator to the new vertex created by the collapse.

   return vm;
   }

   EdgeIter HalfedgeMesh::flipEdge( EdgeIter e0 )
   {
      //Ignore the requests to flip boundary edges
      if (e0->isBoundary()) return e0;
      //Ignore requests to perform any edge flip that would make the surface non-manifold or otherwise invalidate the mesh
      //If v2 and v3 are already connected, then flip e0 would destruct maniflodness, just ignore this kind of flip request
      VertexCIter v_opposite1 = e0->halfedge()->next()->next()->vertex(); 
      VertexCIter v_opposite2 = e0->halfedge()->twin()->next()->next()->vertex();   

      if ( isConnected( v_opposite1 , v_opposite2 )) return e0;

      //COLLECT ELEMENTS
      //Halfedges
      HalfedgeIter h0 = e0->halfedge();
      HalfedgeIter h1 = h0->next();
      HalfedgeIter h2 = h1->next();
      HalfedgeIter h3 = h0->twin();
      HalfedgeIter h4 = h3->next();
      HalfedgeIter h5 = h4->next();
      HalfedgeIter h6 = h1->twin();
      HalfedgeIter h7 = h2->twin();
      HalfedgeIter h8 = h4->twin();
      HalfedgeIter h9 = h5->twin();

      //Vertices
      VertexIter v0 = h0->vertex();
      VertexIter v1 = h1->vertex();
      VertexIter v2 = h2->vertex();
      VertexIter v3 = h8->vertex();
      
      //Edges
      EdgeIter e1 = h1->edge();
      EdgeIter e2 = h2->edge();
      EdgeIter e3 = h4->edge();
      EdgeIter e4 = h5->edge();
      
      //Faces
      FaceIter f0 = h0->face();
      FaceIter f1 = h3->face();

      //REASSIGN ELEMENTS
      //Halfedges
      h0->twin() = h3;
      h0->next() = h1;
      h0->vertex() = v3;
      h0->edge() = e0;
      h0->face() = f0;

      h1->twin() = h7;
      h1->next() = h2;
      h1->vertex() = v2;
      h1->edge() = e2;
      h1->face() = f0;

      h2->twin() = h8;
      h2->next() = h0;
      h2->vertex() = v0;
      h2->edge() = e3;
      h2->face() = f0;

      h3->twin() = h0;
      h3->next() = h4;
      h3->vertex() = v2;
      h3->edge() = e0;
      h3->face() = f1;

      h4->twin() = h9;
      h4->next() = h5;
      h4->vertex() = v3;
      h4->edge() = e4;
      h4->face() = f1;

      h5->twin() = h6;
      h5->next() = h3;
      h5->vertex() = v1;
      h5->edge() = e1;
      h5->face() = f1;

      h6->twin() = h5;
      h6->next() = h6->next();
      h6->vertex() = v2;
      h6->edge() = e1;
      h6->face() = h6->face();

      h7->twin() = h1;
      h7->next() = h7->next();
      h7->vertex() = v0;
      h7->edge() = e2;
      h7->face() = h7->face();

      h8->twin() = h2;
      h8->next() = h8->next();
      h8->vertex() = v3;
      h8->edge() = e3;
      h8->face() = h8->face();

      h9->twin() = h4;
      h9->next() = h9->next();
      h9->vertex() = v1;
      h9->edge() = e4;
      h9->face() = h9->face();

      //Vertices
      v0->halfedge() = h2;
      v1->halfedge() = h5;
      v2->halfedge() = h3;
      v3->halfedge() = h0;

      //Edges
      e0->halfedge() = h0;
      e1->halfedge() = h5;
      e2->halfedge() = h1;
      e3->halfedge() = h2;
      e4->halfedge() = h4;
      
      //Faces
      f0->halfedge() = h0;
      f1->halfedge() = h3;

      // TODO This method should flip the given edge and return an iterator to the flipped edge.

   return e0;
   }

   void MeshResampler::upsample( HalfedgeMesh& mesh )
   // This routine should increase the number of triangles in the mesh using Loop subdivision.
   {
      // Each vertex and edge of the original surface can be associated with a vertex in the new (subdivided) surface.
      // Therefore, our strategy for computing the subdivided vertex locations is to *first* compute the new positions
      // using the connectity of the original (coarse) mesh; navigating this mesh will be much easier than navigating
      // the new subdivided (fine) mesh, which has more elements to traverse.  We will then assign vertex positions in
      // the new mesh based on the values we computed for the original mesh.


      //STEP1 Mark all vertices as belonging to the original mesh
      for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++)
      {
         v->isNew = false; 
      }

      for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++)
      {
         e->isNew = false;
         e->isDevided = false;
      }

      //STEP 2
      // TODO Compute new positions for all the vertices in the input mesh, using the Loop subdivision rule,
      // TODO and store them in Vertex::newPosition. At this point, we also want to mark each vertex as being
      // TODO a vertex of the original mesh.

      for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++)
      {
         int degreeOfVertex = v->degree(); //Since we only deal with non-boundary case
                                             //degree of vertices is equal to degree of faces 
         double neighborWeight;
         if (degreeOfVertex == 3) (neighborWeight = 3.0/16);
         else                     (neighborWeight = 3.0/(8 * degreeOfVertex));
         double selfWeight = 1 - (degreeOfVertex * neighborWeight);

         Vector3D tempNewPosition;
         tempNewPosition += (v->position * selfWeight);

         //Iterate all neighbor vertices of v         
         HalfedgeCIter h = v->halfedge(); // get one of the outgoing halfedges of the vertex
         do
         {
            HalfedgeCIter h_twin = h->twin(); // get the vertex of the current halfedge
            VertexCIter vn = h_twin->vertex(); // vertex is 'source' of the half edge.
                                              // so h->vertex() is v,
                                              // whereas h_twin->vertex() is the neighbor vertex.
 
            tempNewPosition += (vn->position * neighborWeight); // print the vertex position
  
            h = h_twin->next(); // move to the next outgoing halfedge of the vertex.
         }
         while( h != v->halfedge() ); // keep going until we're back at the beginning
         
         v->newPosition = tempNewPosition;
      }


      //STEP 3
      // TODO Next, compute the updated vertex positions associated with edges, and store it in Edge::newPosition.

      for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++)
      {
         HalfedgeIter h = e->halfedge();
         
         VertexIter v1 = h->vertex();
         VertexIter v2 = h->next()->next()->vertex(); 
         VertexIter v3 = h->next()->vertex();         
         VertexIter v4 = h->twin()->next()->next()->vertex();

         Vector3D tempNewPosition;
         tempNewPosition = (v1->position + v3->position) * 3/8 + (v2->position + v4->position) * 1/8;
         
         e->newPosition = tempNewPosition;
      }


      //STEP 4
      // TODO Next, we're going to split every edge in the mesh, in any order.  For future
      // TODO reference, we're also going to store some information about which subdivided
      // TODO edges come from splitting an edge in the original mesh, and which edges are new,
      // TODO by setting the flat Edge::isNew.  Note that in this loop, we only want to iterate
      // TODO over edges of the original mesh---otherwise, we'll end up splitting edges that we
      // TODO just split (and the loop will never end!)

//      EdgeIter nextEdge; //IMPORTANT!
      for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++)
      {
         if (e->isDevided == false)
         {
            // get the next edge NOW!
//            nextEdge = e;
//            nextEdge++;

            //Retrieve information of elements returned by SplitEdge function
            VertexIter v4 = mesh.splitEdge(e);

            HalfedgeIter h0 = v4->halfedge();

            EdgeIter e0 = h0->edge();
            EdgeIter e7 = h0->twin()->next()->edge();
            EdgeIter e6 = h0->twin()->next()->twin()->next()->edge();
            EdgeIter e5 = h0->twin()->next()->twin()->next()->twin()->next()->edge();

            //update the position of the new vertex
            v4->position = h0->edge()->newPosition;
            
            //Set the isNew flags
            v4->isNew = true;

            e0->isNew = false; 
            e5->isNew = true;
            e6->isNew = false;
            e7->isNew = true;

            e0->isDevided = true; 
            e5->isDevided = true;
            e6->isDevided = true;
            e7->isDevided = true;
         }
      }

      //STEP 5
      // TODO Now flip any new edge that connects an old and new vertex.

      for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++)
      {
         if (e->isNew == true)
         {
//            nextEdge = e;
//            nextEdge++;

            VertexIter v1 = e->halfedge()->vertex();
            VertexIter v2 = e->halfedge()->twin()->vertex();

            if ( (v1->isNew == true && v2->isNew == false) 
               ||(v1->isNew == false && v2->isNew == true) )
            mesh.flipEdge(e);

            e->isNew = false;
         }
      }

      //STEP 6
      // TODO Finally, copy the new vertex positions into final Vertex::position.
      for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++)
      {
         if (v->isNew == false)
            v->position = v->newPosition; 
      }
        
   }

   // Given an edge, the constructor for EdgeRecord finds the
   // optimal point associated with the edge's current quadric,
   // and assigns this edge a cost based on how much quadric
   // error is observed at this optimal point.
   EdgeRecord::EdgeRecord( EdgeIter& _edge )
   : edge( _edge )
   {
      //STEP 1
      // TODO Compute the combined quadric from the edge endpoints.


      //STEP2
      // TODO Build the 3x3 linear system whose solution minimizes
      // the quadric error associated with these two endpoints.


      //STEP 3
      // TODO Use this system to solve for the optimal position, and
      // TODO store it in EdgeRecord::optimalPoint.


      //STEP 4
      // TODO Also store the cost associated with collapsing this edge
      // TODO in EdgeRecord::Cost.

      //STEP 5
      // TODO Store the edge in EdgeRecord::edge

   }

   void MeshResampler::downsample( HalfedgeMesh& mesh )
   {
      // TODO Compute initial quadrics for each face by simply writing the plane
      // equation for the face in homogeneous coordinates.  These quadrics should
      // be stored in Face::quadric


      // TODO Compute an initial quadric for each vertex as the sum of the quadrics
      // associated with the incident faces, storing it in Vertex::quadric


      // TODO Build a priority queue of edges according to their quadric error cost,
      // TODO i.e., by building an EdgeRecord for each edge and sticking it in the queue.


      // TODO Until we reach the target edge budget, collapse the best edge.  Remember
      // TODO to remove from the queue any edge that touches the collapsing edge BEFORE
      // TODO it gets collapsed, and add back into the queue any edge touching the collapsed
      // TODO vertex AFTER it's been collapsed.  Also remember to assign a quadric to the
      // TODO collapsed vertex, and to pop the collapsed edge off the top of the queue.
   }

   void Vertex::computeCentroid( void )
   {
      // TODO Compute the average position of all neighbors of this vertex, and
      // TODO store it in Vertex::centroid.  This value will be used for resampling.

      Vector3D sumOfPosition ( 0., 0., 0. );
      int degree = this->degree();

      HalfedgeCIter h = halfedge(); 
      do
      {
         HalfedgeCIter h_twin = h->twin(); // get the vertex of the current halfedge
         VertexCIter v = h_twin->vertex(); // vertex is 'source' of the half edge.
                                           // so h->vertex() is v,
                                           // whereas h_twin->vertex() is the neighbor vertex.
 
         sumOfPosition += v->position;
 
         h = h_twin->next(); // move to the next outgoing halfedge of the vertex.
      }
      while( h != this->halfedge() ); // keep going until we're back at the beginning
      
      this->centroid = sumOfPosition / degree;
    }

   Vector3D Vertex::normal( void ) const
   // TODO Returns an approximate unit normal at this vertex, computed by
   // TODO taking the area-weighted average of the normals of neighboring
   // TODO triangles, then normalizing.
   {
     // TODO Compute and return the area-weighted unit normal.
  
     //Iterate over all faces contains this vertex using halfedges     
     //compute the area of each face, add them into area sum
     //Add the crossproduct of two neighbor edges into vector sum

     Vector3D vectorSum ( 0., 0. , 0. );

     //Collect Elements
     HalfedgeIter h = _halfedge;

     do
     {  
        VertexIter v1 = h->next()->vertex();
        VertexIter v2 = h->next()->next()->vertex();

        Vector3D vector1 = ( v1->position - this->position);
        Vector3D vector2 = ( v2->position - this->position);
        Vector3D areaNormal =  cross ( vector1 , vector2 );
         
        vectorSum += areaNormal; 

        h = h->next()->next()->twin();
     }
     while ( h != halfedge() );

     Vector3D vertexNormal = vectorSum.unit();

     return vertexNormal;
   }

   void MeshResampler::resample( HalfedgeMesh& mesh )
   {

      //STEP 1
      // TODO Compute the mean edge length.
      int numberOfEdges = 0;
      double sumEdgeLength = 0;
      for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++)
      {
         numberOfEdges ++;
         sumEdgeLength += e->length(); 
      }
      double averageEdgeLength = sumEdgeLength / numberOfEdges;
      double L = averageEdgeLength;

      //Itertation of STEP 2 - 6
      // TODO Repeat the four main steps for 5 or 6 iterations

      const int iterationTimes = 10;
      for (int i = 0; i < iterationTimes; i++)
    {

      //STEP 2
      // TODO Split edges much longer than the target length (being careful about how the loop is written!)

      EdgeIter e = mesh.edgesBegin();
      do
      {
         EdgeIter nextEdge = e;
         nextEdge++;
         
         if ( e->length() > (L*4.0/3))
            {
            mesh.splitEdge(e);
            }

         e = nextEdge; 
      }
      while (e != mesh.edgesEnd());

     //STEP 3
      // TODO Collapse edges much shorter than the target length.  Here we need to be EXTRA careful about
      // TODO advancing the loop, because many edges may have been destroyed by a collapse (which ones?)

      bool hasEnded = false; 
      e = mesh.edgesBegin();
      do
      {
         EdgeIter nextEdge = e;
         nextEdge++;
         if ( nextEdge == mesh.edgesEnd() ) hasEnded = true;

         if ( e->length() < (L*4.0/5))
         {

           //Collect the edges which will be deleted by collapse edge
            HalfedgeIter h1 = e->halfedge();
            EdgeIter e3 = h1->next()->next()->edge();
            EdgeIter e5 = h1->twin()->next()->edge();
            
            //Check if next edge happens to be the edges that will be deleted by collapseEdge
            while ((nextEdge == e3) || (nextEdge == e5) || (nextEdge == e))
               {
                nextEdge++;
                if ( nextEdge == mesh.edgesEnd() ) hasEnded = true;
               }

           mesh.collapseEdge(e);

         }

         e = nextEdge; 
      }
      while (e != mesh.edgesEnd() && (! hasEnded ));

      //STEP 4
      // TODO Now flip each edge if it improves vertex degree
      e = mesh.edgesBegin();
      do
      {
         EdgeIter nextEdge = e;
         nextEdge++;
         
         //Collect Elements;
         HalfedgeIter h0 = e->halfedge();
         
         VertexIter v0 = h0->vertex();
         VertexIter v1 = h0->twin()->vertex();
         VertexIter v2 = h0->next()->next()->vertex();
         VertexIter v3 = h0->twin()->next()->next()->vertex();
      
         //Count the degree of 4 vertices
         int d0 = v0->degree(),
             d1 = v1->degree(),
             d2 = v2->degree(),
             d3 = v3->degree();
 
         int oldTotalDegreeDeviation = abs(d0 - 6) + abs(d1 - 6) + abs(d2 - 6) + abs(d3 - 6);

         d0 = d0 - 1;
         d1 = d1 - 1;
         d2 = d2 + 1;
         d3 = d3 + 1;

         int newTotalDegreeDeviation = abs(d0 - 6) + abs(d1 - 6) + abs(d2 - 6) + abs(d3 - 6); 
        
         if ( newTotalDegreeDeviation < oldTotalDegreeDeviation)
             mesh.flipEdge(e);

         e = nextEdge; 
      }
      while (e != mesh.edgesEnd());

     for (int j = 0 ; j < 10 ;j++)
    {
      //STEP 5
      //Compute the centeroid for all vertices
      for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++)
      {
           v->computeCentroid(); 
//           cerr << "centroid =" << v->centroid << endl;
      }


      //STEP 6
      // TODO Finally, apply some tangential smoothing to the vertex positions
      //This is just a temporary impelementation which moves a vertex directly to the averaged place
      for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++)
      {
          Vector3D N = v->normal();
          Vector3D movingVector = v->centroid - v->position;
          Vector3D tangentialMovingVector = movingVector - N * ( dot(movingVector, N) );

//          cerr << "Length of tangential moving vector =" << tangentialMovingVector.norm() << endl;
         
          double weight = 0.2;

          v->position += tangentialMovingVector * weight; 
      }
    }
   }
  }
}
