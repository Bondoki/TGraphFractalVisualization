#pragma once

#include <string>
#include <sstream>
#include <algorithm>    // std::find_if std::max
#include <cstddef>      // NULL
#include <cstdlib>      // std::strtod
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <functional>
#include <queue>
#include <vector>
#include <math.h>

typedef uint32_t NodeIdx;
typedef std::map<NodeIdx, int> Nodelist;
typedef std::map<NodeIdx, Nodelist> Graph;
//typedef std::pair<NodeIdx, Nodelist> Node;
typedef std::pair<int, NodeIdx> Edge; //! (tentative distance, idxNode)
typedef std::vector<NodeIdx> NodeVector;


class TreeGraph
{
public:
  
    
    void dijkstra(Graph &graph, NodeIdx source, Nodelist &distance);

    void findCentersOfTree(Graph &graph,  Nodelist degreeNodes, NodeVector &centerNodes);
    
    Graph graph;
    std::map<NodeIdx, int> eccentricity;
    NodeVector centerGraph;
    NodeVector peripheralGraph;
    
  public:
    
    TreeGraph()
    {
      
    }
    
    void clear()
    {
      graph.clear();
      eccentricity.clear();
      centerGraph.clear();
      peripheralGraph.clear();
    }

  public:
    void addEdge(NodeIdx node1, NodeIdx node2, int weight)
    {
      graph[node1][node2] = weight;
      graph[node2][node1] = weight;
    }
    
    void printTreeGraph()
    {
      for (const auto& [node1, nodelist] : graph) {
        std::cout << node1 << " => ";
        
        for (const auto& [node2, weight] : nodelist) {
          std::cout << "(" << node2 << ":" << weight << "); ";
        }
        std::cout << std::endl;
      }
    }
    
    // calculate eccentricity of graph for all nodes
    void calculateEccentricity()
    {
        std::cout << "Calculate eccentricity can take time..." << std::endl;
			
         eccentricity.clear();
         centerGraph.clear();
         peripheralGraph.clear();
			
        /*for (const auto& [node1, nodelist] : graph) {
          std::cout << node1 << " => ";
          
          for (const auto& [node2, weight] : nodelist) {
          std::cout << "(" << node2 << ":" << weight << "); ";
        }
        std::cout << std::endl;
        }
        */
        
        for (const auto& [node1, nodelist] : graph) {
          
          NodeIdx startNode = node1;
          
          Nodelist topo_distance;
          dijkstra(graph, startNode, topo_distance);
          
          int maxTopologicalDistance = 0;
          
          for(Nodelist::iterator it=topo_distance.begin(); it!=topo_distance.end(); ++it)
          {
            //std::cout<< startNode << " -> " << it->first << "  => "<<it->second<<std::endl;
            //distance[startNode][it->first] = it->second;
            
            if(maxTopologicalDistance < it->second)
              maxTopologicalDistance=it->second;
          }
          
          eccentricity[startNode]=maxTopologicalDistance;
          std::cout<< startNode << " -> eccentricity: " << eccentricity[startNode] <<std::endl;
        }
        
        // calculate the radius of the graph = min eccentricity
        int radiusGraph = std::numeric_limits<int>::max();
        
        for (std::map<NodeIdx, int>::iterator it_ecc=eccentricity.begin(); it_ecc!=eccentricity.end(); ++it_ecc)
        {
          if(it_ecc->second < radiusGraph)
            radiusGraph = it_ecc->second;
        }
        std::cout << "Radius Graph = " << radiusGraph <<std::endl;
        
        // calculate center nodes: eccentricity[center] == radiusGraph
        
        for (std::map<NodeIdx, int>::iterator it_ecc=eccentricity.begin(); it_ecc!=eccentricity.end(); ++it_ecc)
        {
          if(it_ecc->second == radiusGraph)
            centerGraph.push_back(it_ecc->first);
        }
        
        std::cout << "Center Graph = ( ";
        for(NodeVector::iterator it_center=centerGraph.begin(); it_center!=centerGraph.end(); ++it_center){
          std::cout << *it_center << " : ";
        }
        std::cout << " )" << std::endl;
        
        // calculate the diameter of the graph = max eccentricity
        int diameterGraph = std::numeric_limits<int>::min();
        
        for (std::map<NodeIdx, int>::iterator it_ecc=eccentricity.begin(); it_ecc!=eccentricity.end(); ++it_ecc)
        {
          if(diameterGraph < it_ecc->second)
            diameterGraph = it_ecc->second;
        }
        std::cout << "Diameter Graph = " << diameterGraph <<std::endl;
        
        // calculate peripheral nodes: eccentricity[peripheral] == diameterGraph
        
        
        for (std::map<NodeIdx, int>::iterator it_ecc=eccentricity.begin(); it_ecc!=eccentricity.end(); ++it_ecc)
        {
          if(it_ecc->second == diameterGraph)
            peripheralGraph.push_back(it_ecc->first);
        }
        
        std::cout << "Periphery Graph = ( ";
        for(NodeVector::iterator it_periphery=peripheralGraph.begin(); it_periphery!=peripheralGraph.end(); ++it_periphery){
          std::cout << *it_periphery << " : ";
        }
        std::cout << " ) : num: " << peripheralGraph.size() << std::endl;
        
          
    }
};

void TreeGraph::findCentersOfTree(Graph &graph, Nodelist degreeNodes, NodeVector &centerNodes){

	std::queue <NodeIdx> queueLeaves; // Queue for algorithm holding (reduced) leaves
	std::queue <NodeIdx> queueNextLeaves; // Queue for algorithm holding next (reduced) leaves
	std::queue <NodeIdx> tmpOldLeaves; // Queue for algorithm holding (reduced) leaves

	// Fill and start from leaves

	Nodelist::iterator it;
	for(it=degreeNodes.begin(); it!=degreeNodes.end(); ++it)
	{
	        int degreeOfNode = it->second;
	        NodeIdx labelNode = it->first;

	        if (degreeOfNode == 1)
	        {
	        	queueLeaves.push(labelNode);
	        }
	}

	NodeIdx Seperator = -1;

	queueLeaves.push(Seperator);

	//  while ( (queueLeaves.front()!=Seperator)||(queueLeaves.size() > 2) ) {
	while ( (queueLeaves.size() > 0) ) {
		  NodeIdx node = queueLeaves.front();
		  queueLeaves.pop();

		  if( node == Seperator)
		  {
			  if(queueNextLeaves.empty())
				  break;

			 // std::cout << "next iteration: " << node << std::endl;
			  tmpOldLeaves = queueNextLeaves;
			  queueLeaves = queueNextLeaves;

			  if(queueNextLeaves.size() >0)
				  queueLeaves.push(Seperator);

			  queueNextLeaves=std::queue <NodeIdx>();

			  continue;
		  }


	 	      int dN_old = degreeNodes[node];

	 	      degreeNodes[node]--;

	 	     // new subgraph of all neighbors
	 	     Nodelist tempgraph=graph[node];

	 	     // connected nodes

	 	    // std::cout << "connected nodes from: " << node << std::endl;

	 	     Nodelist::iterator it;
	 	     for(it=tempgraph.begin(); it!=tempgraph.end(); ++it){

	 	    //	std::cout << it->first << " with degree: " << degreeNodes[it->first];

	 	    	 if(degreeNodes[it->first] > 0)
	 	    	 {
	 	    		degreeNodes[it->first]--;

	 	    	//	std::cout << " reduced to degree: " << degreeNodes[it->first];
	 	    	 }

	 	    	 if(degreeNodes[it->first] == 1)
	 	    	 {
	 	    		//queueLeaves.push(it->first);
	 	    		queueNextLeaves.push(it->first);

	 	    	//	std::cout << " added to queue the idx: " << (it->first);
	 	    	 }

	 	    	//std::cout << std::endl;
	 	     }


	 	  }

	std::cout << "center queue contains: ";
		  while (!tmpOldLeaves.empty())
		  {
			  centerNodes.push_back(tmpOldLeaves.front());
			  std::cout << ' ' <<  tmpOldLeaves.front();
			  tmpOldLeaves.pop();
		  }
		  std::cout << '\n';
}


void TreeGraph::dijkstra(Graph &graph, NodeIdx source, Nodelist &distance){

	distance.clear(); //! clear all tentative distance information

	//! list of all nodes to be visit and addressed already with least distance on top
  std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge> > queueNode;

  // starting node with tentative distance=0, starting node = source
  queueNode.push( Edge(0, source) );

  while(!queueNode.empty()){

	  //get the element with least tentative distance
    Edge tmped=queueNode.top();

    // access the node index
    NodeIdx tmpnl=tmped.second;

    // removes the top element
    queueNode.pop();

    // if the node never visited before
    if(distance.count(tmpnl)==0){

    	// tentative distance to the recent node
      int dist=tmped.first;

      // set the tentative distance to the node
      distance[tmpnl]=dist;

      // new subgraph of all neighbors
      Nodelist tempgraph=graph[tmpnl];

      Nodelist::iterator it;
      for(it=tempgraph.begin(); it!=tempgraph.end(); ++it){
        int distint=it->second;
        NodeIdx distlabel=it->first;
        queueNode.push(Edge(dist+distint, distlabel));
      }

    }
  }

}



/*
int main()
{
	TreeGraph simpleTreeGraph; 
        
        //Nodelist  degreeNode; // Degree of node
        
        //Graph treeGraph;
        
       simpleTreeGraph.addEdge(0,1,1);
        simpleTreeGraph.addEdge(1,2,1);
        simpleTreeGraph.addEdge(1,3,1);
        
//         simpleTreeGraph.addEdge(3,4,1);
//         simpleTreeGraph.addEdge(4,5,1);
//         simpleTreeGraph.addEdge(4,6,1);
//         
//         simpleTreeGraph.addEdge(3,9,1);
//         simpleTreeGraph.addEdge(9,7,1);
//         simpleTreeGraph.addEdge(9,8,1);
//         
        
        simpleTreeGraph.printTreeGraph();
        simpleTreeGraph.calculateEccentricity();
        
        std::cout << simpleTreeGraph.graph.size() << std::endl ;
        
        //TreeGraph simpleTreeGraphNextGeneration;
        
        //nextGenerationGraph(simpleTreeGraph, simpleTreeGraphNextGeneration);
        
        for(int i = 2; i <= 6; i++)
        {
          std::cout << "generation: " << i << std::endl;
          nextGenerationGraph(simpleTreeGraph, simpleTreeGraph);
          std::cout << "size: " << simpleTreeGraph.graph.size() << std::endl ;
          simpleTreeGraph.calculateEccentricity();
          simpleTreeGraph.printTreeGraph();
        }
      
        
      
	
	return 0;
}
*/
