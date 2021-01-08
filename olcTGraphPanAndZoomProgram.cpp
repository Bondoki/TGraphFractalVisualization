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
          
                // Initialise offset so 0,0 in world space is middle of the screen
		
                
                ScreenOffSetX = (ScreenWidth()-256) / 2;
                ScreenOffSetY = 10;
          
                fOffsetX = -256 / 2;
		fOffsetY = -256 / 2;
                                
                offsetAngle = 0;
                
                order=1;
                
                simpleTreeGraph.clear();
                
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
                /*
                positionNode[0]={-10.0f, 0.0f};
                positionNode[1]={0.0f,0.0f};
                positionNode[2]={5.0f, -8.660254037f};
                positionNode[3]={5.0f, 8.660254037f};
                */
                  
                simpleTreeGraph.printTreeGraph();
                simpleTreeGraph.calculateEccentricity();
        
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
          
        
        void nextGenerationGraph(const TreeGraph oldGraph, TreeGraph& nextGraph)
        {
          nextGraph.clear();
          
          NodeIdx centerNode = oldGraph.centerGraph.at(0);
          NodeIdx peripheryNode = oldGraph.peripheralGraph.at(0);
          
          //float angleOne = 120-offsetAngle;
          //float angleTwo = 240-2*offsetAngle;
                  float angleOne = 240-2*offsetAngle;
                  float angleTwo = 120-offsetAngle;
          
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
                nextGraph.addEdge(node1+sizeGraph+correctionIdxNode1,node2+sizeGraph+correctionIdxNode2,weight);
                nextGraph.addEdge(node1+(2*sizeGraph-1)+correctionIdxNode1,node2+(2*sizeGraph-1)+correctionIdxNode2,weight);
                
                olc::vf2d oldNode1 = positionNode[node1]-positionNode[peripheryNode];
                olc::vf2d newNode1A;
                VectorRotationByAngle(angleOne, oldNode1, newNode1A);
                positionNewNode[node1+sizeGraph+correctionIdxNode1]=positionNode[peripheryNode]+newNode1A;
              
                olc::vf2d oldNode2 = positionNode[node2]-positionNode[peripheryNode];
                olc::vf2d newNode2A;
                VectorRotationByAngle(angleOne, oldNode2, newNode2A);
                positionNewNode[node2+sizeGraph+correctionIdxNode2]=positionNode[peripheryNode]+newNode2A;
                
                olc::vf2d newNode1B;
                VectorRotationByAngle(angleTwo, oldNode1, newNode1B);
                positionNewNode[node1+(2*sizeGraph-1)+correctionIdxNode1]=positionNode[peripheryNode]+newNode1B;
              
                olc::vf2d newNode2B;
                VectorRotationByAngle(angleTwo, oldNode2, newNode2B);
                positionNewNode[node2+(2*sizeGraph-1)+correctionIdxNode2]=positionNode[peripheryNode]+newNode2B;
                
              }
              else
              {
                if(node1 == startNode)
                {
                  nextGraph.addEdge(node1,node2+sizeGraph+correctionIdxNode2,weight);
                  nextGraph.addEdge(node1,node2+(2*sizeGraph-1)+correctionIdxNode2,weight);
                  
                  olc::vf2d oldNode2 = positionNode[node2]-positionNode[peripheryNode];
                  olc::vf2d newNode2A;
                  VectorRotationByAngle(angleOne, oldNode2, newNode2A);
                  positionNewNode[node2+sizeGraph+correctionIdxNode2]=positionNode[peripheryNode]+newNode2A;
                
                  olc::vf2d newNode2B;
                  VectorRotationByAngle(angleTwo, oldNode2, newNode2B);
                  positionNewNode[node2+(2*sizeGraph-1)+correctionIdxNode2]=positionNode[peripheryNode]+newNode2B;
                
                }
                
                if(node2 == startNode)
                {
                  nextGraph.addEdge(node1+sizeGraph+correctionIdxNode1,node2,weight);
                  nextGraph.addEdge(node1+(2*sizeGraph-1)+correctionIdxNode1,node2,weight);
                  
                  olc::vf2d oldNode1 = positionNode[node1]-positionNode[peripheryNode];
                  olc::vf2d newNode1A;
                  VectorRotationByAngle(angleOne, oldNode1, newNode1A);
                  positionNewNode[node1+sizeGraph+correctionIdxNode1]=positionNode[peripheryNode]+newNode1A;
                  
                  
                  olc::vf2d newNode1B;
                  VectorRotationByAngle(angleTwo, oldNode1, newNode1B);
                  positionNewNode[node1+(2*sizeGraph-1)+correctionIdxNode1]=positionNode[peripheryNode]+newNode1B;
                  
                  
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
                  
                  nextGenerationGraph(simpleTreeGraph, simpleTreeGraph);
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
                  
                  simpleTreeGraph.clear();
                  positionNode.clear();
                  
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
                  /*
                   p *ositionNode[0]={-10.0f, 0.0f};
                   positionNode[1]={0.0f,0.0f};
                   positionNode[2]={5.0f, -8.660254037f};
                   positionNode[3]={5.0f, 8.660254037f};
                   */
                  
                  //simpleTreeGraph.printTreeGraph();
                  simpleTreeGraph.calculateEccentricity();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph);
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
                  
                  simpleTreeGraph.clear();
                  positionNode.clear();
                  
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
                  /*
                   p o*sitionNode[0]={-10.0f, 0.0f};
                   positionNode[1]={0.0f,0.0f};
                   positionNode[2]={5.0f, -8.660254037f};
                   positionNode[3]={5.0f, 8.660254037f};
                   */
                  
                  //simpleTreeGraph.printTreeGraph();
                  simpleTreeGraph.calculateEccentricity();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph);
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
                  
                  simpleTreeGraph.clear();
                  positionNode.clear();
                  
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
                  /*
                   positionNode[0]={-10.0f, 0.0f};
                   positionNode[1]={0.0f,0.0f};
                   positionNode[2]={5.0f, -8.660254037f};
                   positionNode[3]={5.0f, 8.660254037f};
                   */
                  
                  //simpleTreeGraph.printTreeGraph();
                  simpleTreeGraph.calculateEccentricity();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph);
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
                
                DrawString(10, ScreenHeight()-10, "USAGE: R ARROWS ESC MOUSE", olc::YELLOW);
                DrawString(10, ScreenHeight()-20, "scale: " + std::to_string(fScaleX), olc::YELLOW);
                DrawString(10, ScreenHeight()-30, "OffSetAngle: " + std::to_string(offsetAngle) + " -> 0; " + std::to_string(int (240-2*offsetAngle)) + ";" + std::to_string(int(120-offsetAngle)), olc::YELLOW);
                
                
                DrawRect(ScreenOffSetX, ScreenOffSetY, 256, 256, olc::GREEN);
                
                // Graceful exit if user is in full screen mode
		return !GetKey(olc::Key::ESCAPE).bPressed;
                
                

	}
	
	
public: int order;
        
        
        std::vector<LineSegment> TGraph;
        
        const int orderMAX = 7;
        //const int NMAX = int(std::pow(2,orderMAX));
        
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
        
};


int main()
{
	Example demo;
	if (demo.Construct(300, 320, 2, 2))
		demo.Start();

	return 0;
}
