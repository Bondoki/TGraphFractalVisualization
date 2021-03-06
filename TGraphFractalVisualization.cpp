/*
 Created on 6. Jan 2021
 Last modified on 10. Jan 2021
 Author: bondoki
 
 Purpose: Rendering of the T-Graph fractal aka Tree fractal aka T-fractal with functionality f=3,
 for different generations G, see  S. Havlin and H. Weissman, "Mapping between hopping on hierarchical
 structures and diffusion on a family of fractals", J. Phys. A: Math. Gen. (1986), 19, L1021-L1026.
 This fractal has the fractal dimension of df=ln(f)/ln(2) -> df=1.5849625 with a total number of
 nodes N=pow(f,G)+1.
 The fractal construction obeys the following rule: insert between every branch a new leave or
 equivalently to copy and rotate the previous structure and attach it to any periphery node being the new center:
 G0     G1     G2
                 _|
 ___ -> ___ -> ___|___
         |      |   |
 
 Inspired by the Pan&Zoom example of Javidx9 ( https://youtu.be/ZQ8qtAizis4 ) gave the idea for this.
 It's just for fun and educational purpose. Feel free to modify and use it :)
 
 Note1: according to the selected periphery node for the next generation the next fractal is still valid, but
 the projection onto 2D gives a variety of appearance for higher generation but fails for the
 unique right angle projection (offsetAngle=30). To check this look at lines 214-221.
 Note2: You may change the functionality of the fractal/graph in line 81 and use line 201-213
 for finding a good (but sadly not unique) way to render the fractal. For f=4 the fractal dimension
 is df=ln(4)/ln(2)=2.0 giving the Peano basin fractal (2D Greek Cross fractal), see also
 https://commons.wikimedia.org/wiki/File:Greek_cross_2D_1_through_4.svg .
 Note3: Also higher functionalities f>4 are possible but the fractals dimension df=ln(f)/ln(2)
 surly exceed 2D. Let me know if you recognize them and their corresponding name.
 
 License: This piece of code is licensed to GNU GPLv3 according to
 https://github.com/OneLoneCoder/videos/blob/master/LICENSE
 also attributing Javidx9 for the unique and easy-to-use olc::PixelGameEngine and the underlying OLC-3 license see
 https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/Licencing
 For more information about Javidx9 and OneLoneCoder please see https://github.com/OneLoneCoder
*/ 


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "olcPGEX_AdditionalColours.h"

#include "Graph.h"

#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

struct LineSegment
{
	olc::vf2d startPos;
	olc::vf2d endPos;
			
};

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "T-Graph Fractal";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
          
                // Initialise offset so world space is middle of the screen
		
                ScreenOffSetX = (ScreenWidth()-256) / 2;
                ScreenOffSetY = 10;
          
                fOffsetX = -256 / 2;
		fOffsetY = -256 / 2;
                                
                offsetAngle = 0;
                
                order=1;
                functionality = 5; // you may try f=4 and above and uncomment line
                
                createBasicTGraphUnit();
                 
                simpleTreeGraph.printTreeGraph();
                
                std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
        
                TGraph.clear();
                calculateTGraphDrawing(1);
                
		return true;
	}
	
		// Convert coordinates from World Space --> Screen Space
	void WorldToScreen(float fWorldX, float fWorldY, int &nScreenX, int &nScreenY)
	{
		nScreenX = (int)((fWorldX - fOffsetX) * fScaleX + ScreenOffSetX);
		nScreenY = (int)((fWorldY - fOffsetY) * fScaleY + ScreenOffSetY);
	}

	// Convert coordinates from Screen Space --> World Space
	void ScreenToWorld(int nScreenX, int nScreenY, float &fWorldX, float &fWorldY)
	{
		fWorldX = ((float)(nScreenX-ScreenOffSetX) / fScaleX) + fOffsetX;
		fWorldY = ((float)(nScreenY-ScreenOffSetY) / fScaleY) + fOffsetY;
	}
	
	void VectorRotationByAngle(float angle, olc::vf2d oldVec, olc::vf2d& newVec)
        {
          newVec.x = oldVec.x*std::cos(angle*3.14159265f/180.0f)-oldVec.y*std::sin(angle*3.14159265f/180.0f);
          newVec.y = oldVec.x*std::sin(angle*3.14159265f/180.0f)+oldVec.y*std::cos(angle*3.14159265f/180.0f);
        }
        
        void createBasicTGraphUnit()
        {
          simpleTreeGraph.clear();
          positionNode.clear();
          
          olc::vf2d oldNode1 = {-10.0f, 0.0f};
          positionNode[0]={0.0f,0.0f};
          
          for(int i = 1; i <= functionality; i++)
          {
            simpleTreeGraph.addEdge(0,i,1);
            
            //1 0 - 0
            //2 240 2
            //3 120 1
            olc::vf2d newNode1;
            float angle = (360.0/functionality)*(functionality+1-i)-((functionality+1-i)%functionality)*offsetAngle;
            
            VectorRotationByAngle(angle, oldNode1, newNode1);
            positionNode[i]=newNode1;
          }
          
          /*
          simpleTreeGraph.addEdge(0,1,1);
          simpleTreeGraph.addEdge(1,2,1);
          simpleTreeGraph.addEdge(1,3,1);
          
          //float angleOne = 120-offsetAngle;
          //float angleTwo = 240-2*offsetAngle;
          float angleOne = 240-2*offsetAngle;
          float angleTwo = 120-offsetAngle;
          
          olc::vf2d oldNode1 = {-10.0f, 0.0f};
          olc::vf2d newNode1;
          VectorRotationByAngle(0, oldNode1, newNode1);
          positionNode[0]=newNode1;
          //VectorRotationByAngle(0, oldNode1, newNode1);
          positionNode[1]={0.0f,0.0f};
          VectorRotationByAngle(angleOne, oldNode1, newNode1);
          positionNode[2]=newNode1;
          VectorRotationByAngle(angleTwo, oldNode1, newNode1);
          positionNode[3]=newNode1;
          */
          /*
           *         positionNode[0]={-10.0f, 0.0f};
           *         positionNode[1]={0.0f,0.0f};
           *         positionNode[2]={5.0f, -8.660254037f};
           *         positionNode[3]={5.0f, 8.660254037f};
           */
          simpleTreeGraph.calculateEccentricity();
          
        }
	
	
        void calculateTGraphDrawing(int o)
        {
          NodeIdx centerNode = simpleTreeGraph.centerGraph.at(0);
          NodeIdx peripheryNode = simpleTreeGraph.peripheralGraph.at(0);
          
          TGraph.clear();
          
          for (const auto& [node1, nodelist] : simpleTreeGraph.graph) {
            //std::cout << node1 << " => ";
            for (const auto& [node2, weight] : nodelist) {
              //std::cout << "(" << node2 << ":" << weight << "); ";
              
              if(node2 < node1)
              {
                LineSegment line({ {positionNode[node2]}, {positionNode[node1]}});
                
                TGraph.push_back(line);
              }
            }
          }
          
          return;
        }
          
        
        void nextGenerationGraph(const TreeGraph oldGraph, TreeGraph& nextGraph, int o)
        {
          nextGraph.clear();
          
          NodeIdx centerNode = oldGraph.centerGraph.at(0);          
          NodeIdx peripheryNode = oldGraph.peripheralGraph.at(0);
          
          // Note1: this approach is maybe useful to find the longest euclidean distance to the center
          // esp for offsetAngle=0 and f>=4
          /*
          float maxDistance2 = positionNode[peripheryNode].mag2();
          for (const auto& node : oldGraph.peripheralGraph)
          {
            if(positionNode[node].mag2() > maxDistance2)
            {
              maxDistance2 = positionNode[node].mag2();
              peripheryNode = node;
            }
          }
          */
          // Note2: if you use an other equivalent periphery node you get an valid fractal (for f=3)
          // but disordered for the right angle projection (offset 30)
          //if(o == 4)
          //  peripheryNode = 8;//6;
          //if(o == 5) // if previous peripheryNode = oldGraph.peripheralGraph.at(0);
          //  peripheryNode = 17;//15;
          //if(o == 3)
          // peripheryNode = 3;//8;//6;//5;
          
          std::cout << "Selected Periphery Node =  " << peripheryNode << std::endl;
          
          std::map<NodeIdx, olc::vf2d> positionNewNode;
          
          std::cout << "Periphery Old Graph = ( ";
          for (const auto& node : oldGraph.peripheralGraph)
          {
            std::cout << node << " ; ";
          }
          std::cout << " )" << std::endl;
          
          NodeIdx startNode = peripheryNode;//oldGraph.peripheralGraph.at(0);
          
          uint32_t sizeGraph = oldGraph.graph.size();
          std::cout << "Size Old Graph =  " << sizeGraph << std::endl;
          
          //copy graph
          for (const auto& [node1, nodelist] : oldGraph.graph) {
            //std::cout << node1 << " => ";
            
            for (const auto& [node2, weight] : nodelist) {
              //std::cout << "(" << node2 << ":" << weight << "); ";
              uint32_t correctionIdxNode1 = 0;
              if(startNode < node1)
              {
                correctionIdxNode1 = -1;
              }
              
              uint32_t correctionIdxNode2 = 0;
              if(startNode < node2)
              {
                correctionIdxNode2 = -1;
              }
              
              
              nextGraph.addEdge(node1,node2,weight);
              
              olc::vf2d oldNode1 = positionNode[node1]-positionNode[peripheryNode];
              olc::vf2d newNode1;
              VectorRotationByAngle(0, oldNode1, newNode1);
              positionNewNode[node1]=positionNode[peripheryNode]+newNode1;
              
              olc::vf2d oldNode2 = positionNode[node2]-positionNode[peripheryNode];
              olc::vf2d newNode2;
              VectorRotationByAngle(0, oldNode2, newNode2);
              positionNewNode[node2]=positionNode[peripheryNode]+newNode2;
              
              
              if(node1 != startNode && node2 != startNode)
              {
                for(int i = 1; i < functionality; i++)
                {
                  //nextGraph.addEdge(node1+sizeGraph+correctionIdxNode1,node2+sizeGraph+correctionIdxNode2,weight);
                  //nextGraph.addEdge(node1+(2*sizeGraph-1)+correctionIdxNode1,node2+(2*sizeGraph-1)+correctionIdxNode2,weight);
                
                  nextGraph.addEdge(node1+(i*sizeGraph-(i-1))+correctionIdxNode1,node2+(i*sizeGraph-(i-1))+correctionIdxNode2,weight);
                
                }
                
                for(int i = 1; i < functionality; i++)
                {
                  float angle = (360.0/functionality)*(functionality-i)-((functionality-i)%functionality)*offsetAngle;
                  
                  olc::vf2d oldNode1 = positionNode[node1]-positionNode[peripheryNode];
                  olc::vf2d newNode1A;
                  VectorRotationByAngle(angle, oldNode1, newNode1A);
                  positionNewNode[node1+(i*sizeGraph-(i-1))+correctionIdxNode1]=positionNode[peripheryNode]+newNode1A;
              
                  olc::vf2d oldNode2 = positionNode[node2]-positionNode[peripheryNode];
                  olc::vf2d newNode2A;
                  VectorRotationByAngle(angle, oldNode2, newNode2A);
                  positionNewNode[node2+(i*sizeGraph-(i-1))+correctionIdxNode2]=positionNode[peripheryNode]+newNode2A;
               
                  
                }
                   
              }
              else
              {
                if(node1 == startNode)
                {
                  for(int i = 1; i < functionality; i++)
                  {
                    
                    nextGraph.addEdge(node1,node2+(i*sizeGraph-(i-1))+correctionIdxNode2,weight);
                    
                  }
                  
                  for(int i = 1; i < functionality; i++)
                  {
                    float angle = (360.0/functionality)*(functionality-i)-((functionality-i)%functionality)*offsetAngle;
                    
                    olc::vf2d oldNode2 = positionNode[node2]-positionNode[peripheryNode];
                    olc::vf2d newNode2A;
                    VectorRotationByAngle(angle, oldNode2, newNode2A);
                    positionNewNode[node2+(i*sizeGraph-(i-1))+correctionIdxNode2]=positionNode[peripheryNode]+newNode2A;
                    
                    
                  }
               
                }
                
                if(node2 == startNode)
                {
                  for(int i = 1; i < functionality; i++)
                  {
                    //nextGraph.addEdge(node1+sizeGraph+correctionIdxNode1,node2+sizeGraph+correctionIdxNode2,weight);
                    //nextGraph.addEdge(node1+(2*sizeGraph-1)+correctionIdxNode1,node2+(2*sizeGraph-1)+correctionIdxNode2,weight);
                    
                    nextGraph.addEdge(node1+(i*sizeGraph-(i-1))+correctionIdxNode1,node2,weight);
                    
                  }
                  
                  for(int i = 1; i < functionality; i++)
                  {
                    float angle = (360.0/functionality)*(functionality-i)-((functionality-i)%functionality)*offsetAngle;
                    
                    olc::vf2d oldNode1 = positionNode[node1]-positionNode[peripheryNode];
                    olc::vf2d newNode1A;
                    VectorRotationByAngle(angle, oldNode1, newNode1A);
                    positionNewNode[node1+(i*sizeGraph-(i-1))+correctionIdxNode1]=positionNode[peripheryNode]+newNode1A;
                    
                    
                  }
                  
                }
              }
              
              
            }
            // std::cout << std::endl;
          }
          
          //std::cout << "Size Old Pos Graph =  " << positionNode.size() << std::endl;
          //std::cout << "Size New Pos Graph =  " << positionNewNode.size() << std::endl;
          positionNode = positionNewNode;
          
          //translate everything to the center of the new structure
          olc::vf2d offsetpos = positionNode[peripheryNode];
          for (auto& [nodeidx, posnode] : positionNode) {
            
            posnode -= offsetpos;
          }
        }
        
        

	bool OnUserUpdate(float fElapsedTime) override
	{
          
                // Just grab a copy of mouse coordinates for convenience
		float fMouseX = (float)GetMouseX();
		float fMouseY = (float)GetMouseY();
                
                // For panning, we need to capture the screen location when the user starts
		// to pan...
		if (GetMouse(0).bPressed)
		{
			fStartPanX = fMouseX;
			fStartPanY = fMouseY;
		}

		// ...as the mouse moves, the screen location changes. Convert this screen
		// coordinate change into world coordinates to implement the pan. Simples.
		if (GetMouse(0).bHeld)
		{
			fOffsetX -= (fMouseX - fStartPanX) / fScaleX;
			fOffsetY -= (fMouseY - fStartPanY) / fScaleY;

			// Start "new" pan for next epoch
			fStartPanX = fMouseX;
			fStartPanY = fMouseY;
		}
		
		// For zoom, we need to extract the location of the cursor before and after the
		// scale is changed. Here we get the cursor and translate into world space...
		float fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom;
		ScreenToWorld(fMouseX, fMouseY, fMouseWorldX_BeforeZoom, fMouseWorldY_BeforeZoom);

		// ...change the scale as required...
		//if (GetKey(olc::Key::Q).bHeld)
                if (GetMouseWheel() > 0)
		{
			fScaleX *= 1.01f;
			fScaleY *= 1.01f;
		}

		//if (GetKey(olc::Key::A).bHeld)
		if (GetMouseWheel() < 0)
		{
			fScaleX *= 0.99f;
			fScaleY *= 0.99f;
		}
		
		// ...now get the location of the cursor in world space again - It will have changed
		// because the scale has changed, but we can offset our world now to fix the zoom
		// location in screen space, because we know how much it changed laterally between
		// the two spatial scales. Neat huh? ;-)
		float fMouseWorldX_AfterZoom, fMouseWorldY_AfterZoom;
		ScreenToWorld(fMouseX, fMouseY, fMouseWorldX_AfterZoom, fMouseWorldY_AfterZoom);		
		fOffsetX += (fMouseWorldX_BeforeZoom - fMouseWorldX_AfterZoom);
		fOffsetY += (fMouseWorldY_BeforeZoom - fMouseWorldY_AfterZoom);



		
                // check for user input to change order
                // Handle User Input
                if (GetKey(olc::Key::UP).bPressed)
                {
                  order++;
                  order = order >= orderMAX ? orderMAX : order;
                  
                  //DrawStringDecal({30, 40}, "Calculating...", olc::YELLOW);
                
                  
                  nextGenerationGraph(simpleTreeGraph, simpleTreeGraph, order);
                  std::cout << "size: " << simpleTreeGraph.graph.size() << std::endl ;
                  simpleTreeGraph.calculateEccentricity();
                  //  simpleTreeGraph.printTreeGraph();
                   
                  TGraph.clear();
                  calculateTGraphDrawing(order);
                                                      
                }
                
                if (GetKey(olc::Key::DOWN).bPressed)
                {
                  order--;
                  order = order < 1 ? 1 : order;
                  
                  createBasicTGraphUnit();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph, i);
                    std::cout << "size: " << simpleTreeGraph.graph.size() << std::endl ;
                    simpleTreeGraph.calculateEccentricity();
                    //simpleTreeGraph.printTreeGraph();
                  }
                  
                  TGraph.clear();
                  calculateTGraphDrawing(order);
                  
                  
                }
                
                if (GetKey(olc::Key::RIGHT).bPressed)
                {
                  offsetAngle -= 5;
                  
                  createBasicTGraphUnit();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph, i);
                    std::cout << "size: " << simpleTreeGraph.graph.size() << std::endl ;
                    simpleTreeGraph.calculateEccentricity();
                    //simpleTreeGraph.printTreeGraph();
                  }
                  
                  TGraph.clear();
                  calculateTGraphDrawing(order);
                  
                  
                }
                
                if (GetKey(olc::Key::LEFT).bPressed)
                {
                  offsetAngle += 5;
                  
                  createBasicTGraphUnit();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph, i);
                    std::cout << "size: " << simpleTreeGraph.graph.size() << std::endl ;
                    simpleTreeGraph.calculateEccentricity();
                    //simpleTreeGraph.printTreeGraph();
                  }
                  
                  TGraph.clear();
                  calculateTGraphDrawing(order);
                  
                  
                }
                
                if (GetKey(olc::Key::ENTER).bPressed)
                {
                  
                }
                
                if (GetKey(olc::Key::W).bPressed)
                {
                  
                }
                
                if (GetKey(olc::Key::E).bPressed)
                {
                  
                }
                
                if (GetKey(olc::Key::R).bPressed)
                {
                  fOffsetX = -256 / 2;
                  fOffsetY = -256 / 2;
                  
                  offsetAngle = 0;
                  
                  fScaleX = 1.0f;
                  fScaleY = 1.0f;

                  fStartPanX = 0.0f;
                  fStartPanY = 0.0f;
                  
                  createBasicTGraphUnit();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph, i);
                    std::cout << "size: " << simpleTreeGraph.graph.size() << std::endl ;
                    simpleTreeGraph.calculateEccentricity();
                    //simpleTreeGraph.printTreeGraph();
                  }
                  
                  TGraph.clear();
                  calculateTGraphDrawing(order);
                  
                }
                
                if (GetKey(olc::Key::T).bPressed)
                {
                  
                }
                
                
		// called once per frame
                
                // Clip
		float fWorldLeft, fWorldTop, fWorldRight, fWorldBottom;
		ScreenToWorld(ScreenOffSetX, ScreenOffSetY, fWorldLeft, fWorldTop);
		ScreenToWorld(ScreenOffSetX+256, ScreenOffSetY+256, fWorldRight, fWorldBottom);
                //ScreenToWorld(ScreenWidth(), ScreenHeight(), fWorldRight, fWorldBottom);
                
		// Erase previous frame
                Clear(olc::DARK_BLUE);
                
                
                for(int i = 0; (i < TGraph.size()); i++)
                {
                  LineSegment tmpLine = TGraph[i];
                  
                  // Create a color using the hsv color space, this one makes a nice orange color
                  float idxMap = (i < TGraph.size()) ? i*360.0f/TGraph.size() : 360.0f;
                  auto hsvColor = olc::Colours::FromHsv(idxMap, 255, 255);
                  
                  int pixel_sx, pixel_sy, pixel_ex, pixel_ey;
                
                float w_sx = tmpLine.startPos.x;
                float w_sy = tmpLine.startPos.y;
                float w_ex = tmpLine.endPos.x;
                float w_ey = tmpLine.endPos.y;
                  
                  //clip to region
                w_sx = (w_sx < fWorldLeft) ? fWorldLeft : w_sx;
                w_sx = (w_sx > fWorldRight) ? fWorldRight : w_sx;
                w_sy = (w_sy < fWorldTop) ? fWorldTop : w_sy;
                w_sy = (w_sy > fWorldBottom) ? fWorldBottom : w_sy;
                w_ex = (w_ex > fWorldRight) ? fWorldRight : w_ex;
                w_ex = (w_ex < fWorldLeft) ? fWorldLeft : w_ex;
                w_ey = (w_ey > fWorldBottom) ? fWorldBottom : w_ey;
                w_ey = (w_ey < fWorldTop) ? fWorldTop : w_ey;

		WorldToScreen(w_sx, w_sy, pixel_sx, pixel_sy);
                WorldToScreen(w_ex, w_ey, pixel_ex, pixel_ey);
                 
                //if(drawAll )
                DrawLine(pixel_sx, pixel_sy, pixel_ex, pixel_ey, hsvColor);
                                
                }
                
                DrawString(10, ScreenHeight()-40, "Order: " + std::to_string(order), olc::YELLOW);
                DrawString(100, ScreenHeight()-40, "Points: " + std::to_string(positionNode.size()), olc::YELLOW);
                
                DrawString(10, ScreenHeight()-10, "USAGE: R ARROWS MOUSE ESC", olc::YELLOW);
                DrawString(10, ScreenHeight()-20, "scale: " + std::to_string(fScaleX), olc::YELLOW);
                //DrawString(10, ScreenHeight()-30, "OffSetAngle: " + std::to_string(offsetAngle) + " -> 0; " + std::to_string(int (240-2*offsetAngle)) + ";" + std::to_string(int(120-offsetAngle)), olc::YELLOW);
                DrawString(10, ScreenHeight()-30, "OffSetAngle: " + std::to_string(int(offsetAngle)), olc::YELLOW);
                
                std::string angleOffsetString = "";
                for(int i = 1; i < functionality; i++)
                  {
                    float angle = (360.0/functionality)*(functionality-i)-((functionality-i)%functionality)*offsetAngle;
                    angleOffsetString += std::to_string(int(angle)) +";";
                    
                  }
                DrawString(130, ScreenHeight()-30, " -> 0;"+angleOffsetString, olc::YELLOW);
                
                
                DrawRect(ScreenOffSetX, ScreenOffSetY, 256, 256, olc::GREEN);
                
                // Graceful exit if user is in full screen mode
		return !GetKey(olc::Key::ESCAPE).bPressed;
                
                

	}
	
	
public: int order;
        
        
        std::vector<LineSegment> TGraph;
        
        const int orderMAX = 7;
        
        float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;
	float fScaleX = 1.0f;
	float fScaleY = 1.0f;

	float fStartPanX = 0.0f;
	float fStartPanY = 0.0f;
        
        int ScreenOffSetX;
        int ScreenOffSetY;
        
        float offsetAngle;
        
        TreeGraph simpleTreeGraph; 
        std::map<NodeIdx, olc::vf2d> positionNode;
        
        int functionality;
        
};


int main()
{
	Example demo;
	if (demo.Construct(300, 320, 2, 2))
		demo.Start();

	return 0;
}
