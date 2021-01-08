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
		sAppName = "Hilbert Curve Demo";
                
                
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
          
                // Initialise offset so 0,0 in world space is middle of the screen
		//fOffsetX = 0;//-(ScreenWidth()-256) / 2;
		//fOffsetY = 0;//-10 ;
                
                ScreenOffSetX = (ScreenWidth()-256) / 2;
                ScreenOffSetY = 10;
          
                fOffsetX = -256 / 2;
		fOffsetY = -256 / 2;
                
                
                offsetAngle = 0;

                bDrawIdx = false;
                
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
                
                
                //TGraph.push_back({ {-100.0f, 0.0f}, {0.0f, 0.0f} });
                //TGraph.push_back({ { 50.0f, -86.60254037f},{0.0f, 0.0f} });
                //TGraph.push_back({ { 50.0f, 86.60254037f},{0.0f, 0.0f} });
                
//                  TGraph.push_back({ {0.0f, 0.0f}, {-100.0f, 0.0f} });
//                 TGraph.push_back({ {0.0f, 0.0f}, {100.0f, 0.0f} });
//                 TGraph.push_back({ {0.0f, 0.0f}, {0.0f, -100.0f} });
                //TGraph.push_back({ {0.0f, 0.0f}, {100.0f, 0.0f} });
                //calculateTGraph(1);
                
                calculatePath();
                
                
                counter = 0;
                counterInc = 1;
                drawAll = false;
                
                pointOnCurveIdx=1./(totNumPoints-1); //0.5 length
                
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
	
	void calculatePath()
        {
                //order=7;
                N = int(std::pow(2,order));
                
                totNumPoints = N*N;
                
                path.clear();
                path.resize(totNumPoints);
                
                for(int i = 0; i < totNumPoints; i++)
                {
                  path[i]=hilbert(i);
                  float len = int(std::pow(2,orderMAX)/N);//ScreenWidth()/N;
                  path[i] *= len;
                  path[i] += olc::vf2d(int(len/2),int(len/2));
                }
                
                counter = 0;
		return;
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
          
          
          counter = 0;
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
          
          std::cout << "Size Old Pos Graph =  " << positionNode.size() << std::endl;
          std::cout << "Size New Pos Graph =  " << positionNewNode.size() << std::endl;
          positionNode = positionNewNode;
        }
        
        void calculateTGraph(int o)
        {
                std::vector<LineSegment> tmpNextOrder;
                
                //LineSegment lineOffset = TGraph[TGraph.size()-1];
                
                LineSegment lineOffset = TGraph[int (2*o-2)];
                
//                 olc::vf2d startOld = lineOffset.endPos;
//                 olc::vf2d startNew;
//                 
//                 
//                 VectorRotationByAngle(60, startOld, startNew);
//                 std::cout << startNew << std::endl;
//                 
                 for(int k = 0; k <3 ; k++)
                for(int i = TGraph.size()-1; i >= 0; i--)
                //  for(int i = 0; i < TGraph.size(); i++)
                    
                 
                {
                  LineSegment line = TGraph[i];
                  
                  olc::vf2d startOld = line.startPos;//{0.0f,0.0f};//line.startPos;//-lineOffset.startPos;
                  olc::vf2d endOld = line.endPos;//-lineOffset.startPos;
                
                  olc::vf2d startNew;
                  olc::vf2d endNew;
                  
                  float angle = 0;
                  
                  if(k==0)
                    angle = 0.0f;//o%2 == 0 ? 120.0f : -120.0f;
                  else if(k==1)
                    angle = 120.0f-offsetAngle;//sign*120+da;
                  else if(k==2)
                    angle = 240.0f-2*offsetAngle;//sign*240+da;//o%2 == 0 ? -120.0f : 120.0f;
                  //angle = -120; angle <= 120; angle += 120
                  
                  
                  VectorRotationByAngle(angle, startOld, startNew);
                  VectorRotationByAngle(angle, endOld, endNew);
                  
                  olc::vf2d lineOffsetEndNewRot;
                  VectorRotationByAngle(angle, lineOffset.endPos, lineOffsetEndNewRot);
                  
                  //if(o%2 == 0)
                  if(o > 1)
                  {
                    LineSegment tmpline1({ {startNew+(lineOffset.endPos-lineOffsetEndNewRot)}, {endNew+(lineOffset.endPos-lineOffsetEndNewRot)}});
                    tmpNextOrder.push_back(tmpline1);
                  }
                  else
                  {
                    LineSegment tmpline1({ {endNew+(lineOffset.endPos-lineOffsetEndNewRot)}, {startNew+(lineOffset.endPos-lineOffsetEndNewRot)}});
                    tmpNextOrder.push_back(tmpline1);
                  } 
                  
                /*for(int k = 0; k <3 ; k++)
                for(int i = TGraph.size()-1; i >= 0; i--)
                {
                  LineSegment line = TGraph[i];
                  
                  olc::vf2d startOld = line.startPos-lineOffset.startPos;
                  olc::vf2d endOld = line.endPos-lineOffset.startPos;
                  
                 
                  
                  olc::vf2d startNew;
                  olc::vf2d endNew;
                  
                  float angle = 0;
                  
                  olc::vf2d eApara = (lineOffset.endPos-lineOffset.startPos).norm();
                  float da = -std::acos(eApara.x)*180.f/3.14159265f;
                   std::cout << da << std::endl;
                  
                  float sign=1.0; 
//                   if(o%2==0)
//                     sign=-1.0;
//                   
                  da = 0.0f;
                   
                  if(k==0)
                    angle = sign*0+da;//o%2 == 0 ? 120.0f : -120.0f;
                  else if(k==1)
                    angle = sign*120+da;
                  else if(k==2)
                    angle = sign*240+da;//o%2 == 0 ? -120.0f : 120.0f;
                  //angle = -120; angle <= 120; angle += 120
                  
                  VectorRotationByAngle(angle, startOld, startNew);
                  VectorRotationByAngle(angle, endOld, endNew);
                  
                  
                  
               if(o%2 == 1)
                  {
                    LineSegment tmpline1({{startNew+lineOffset.endPos}, {endNew+lineOffset.endPos}});
                    tmpNextOrder.push_back(tmpline1);
                  }
                  else
                  {
                    LineSegment tmpline1({ {endNew+lineOffset.endPos}, {startNew+lineOffset.endPos}});
                    tmpNextOrder.push_back(tmpline1);
                  } 
                  */
                  /*
                  float length = (line.endPos-line.startPos).mag();
                  
                  olc::vf2d eApara = (line.endPos-line.startPos).norm();
                  
                  //olc::vf2d eAperp = ((line.endPos-line.startPos).perp()).norm();
                  //olc::vf2d eAperp = o%2 == 0 ?  ((line.startPos-line.endPos).perp()).norm() : ((line.endPos-line.startPos).perp()).norm();
                  //olc::vf2d eAperp = ((line.startPos-line.endPos).perp()).norm();
                  
                  //olc::vf2d eAperp1 = o%2 == 0 ?  -((line.startPos-line.endPos).perp()).norm() : ((line.endPos-line.startPos).perp()).norm();
                  //olc::vf2d eAperp2 = o%2 == 0 ?  -((line.startPos-line.endPos).perp()).norm() : ((line.endPos-line.startPos).perp()).norm();7
                  
                  olc::vf2d eAperp1 = o<1 ? -((line.startPos-line.endPos).perp()).norm() : ((line.endPos-line.startPos).perp()).norm();
                  olc::vf2d eAperp2 = o<1 ? -((line.startPos-line.endPos).perp()).norm() : ((line.endPos-line.startPos).perp()).norm();
                  
                  float normLength = 1.0f/2.0f;///std::pow(2,o+1);
                  
                  if(o <= 1)
                  {
                  //LineSegment tmpline1({{line.startPos+eApara*length*normLength}, {line.startPos}});
                  LineSegment tmpline1({{line.startPos}, {line.startPos+eApara*length*normLength}});
                  tmpNextOrder.push_back(tmpline1);
                  }
                  else
                  {
                    LineSegment tmpline1({{line.startPos+eApara*length*normLength}, {line.startPos}});
                    //LineSegment tmpline1({{line.startPos}, {line.startPos+eApara*length*normLength}});
                  tmpNextOrder.push_back(tmpline1);
                  }
                    
                  float angle =  120.0f;//0.0f;
                  float COS = std::cos(angle*3.14159265f/180.0f);
                  float SIN = std::sin(angle*3.14159265f/180.0f);
                  
                  if(o <= 1)
                  {
                  //LineSegment tmpline2({{line.startPos+eApara*length*normLength},{line.startPos+eApara*length*normLength+eApara*length*normLength*COS+eAperp1*length*normLength*SIN}});
                  LineSegment tmpline2({{line.startPos+eApara*length*normLength+eApara*length*normLength*COS+eAperp1*length*normLength*SIN}, {line.startPos+eApara*length*normLength}});
                  tmpNextOrder.push_back(tmpline2);
                  }
                  else
                    {
                  LineSegment tmpline2({{line.startPos+eApara*length*normLength},{line.startPos+eApara*length*normLength+eApara*length*normLength*COS+eAperp1*length*normLength*SIN}});
                  //LineSegment tmpline2({{line.startPos+eApara*length*normLength+eApara*length*normLength*COS+eAperp1*length*normLength*SIN}, {line.startPos+eApara*length*normLength}});
                  tmpNextOrder.push_back(tmpline2);
                  }
                  angle = 120.0f; //= o%2 == 0 ? 270.0f : 90.0f;
                  COS = std::cos((360.f-angle)*3.14159265f/180.0f);
                  SIN = std::sin((360.f-angle)*3.14159265f/180.0f);
                  
                  if(o <= 1)
                  {
                  //LineSegment tmpline3({{line.startPos+eApara*length*normLength},{line.startPos+eApara*length*normLength+eApara*length*normLength*COS+eAperp2*length*normLength*SIN}});
                  LineSegment tmpline3({{line.startPos+eApara*length*normLength+eApara*length*normLength*COS+eAperp2*length*normLength*SIN},{line.startPos+eApara*length*normLength}});
                  tmpNextOrder.push_back(tmpline3);
                  }
                  else
                    {
                  LineSegment tmpline3({{line.startPos+eApara*length*normLength},{line.startPos+eApara*length*normLength+eApara*length*normLength*COS+eAperp2*length*normLength*SIN}});
                  //LineSegment tmpline3({{line.startPos+eApara*length*normLength+eApara*length*normLength*COS+eAperp2*length*normLength*SIN},{line.startPos+eApara*length*normLength}});
                  tmpNextOrder.push_back(tmpline3);
                  }
                  */
                  
                  /*float length = (line.endPos-line.startPos).mag();
                  
                  olc::vf2d v2 = (line.endPos-line.startPos).norm();
                  
                  //olc::vf2d v3 = ((line.endPos-line.startPos).perp()).norm();
                  olc::vf2d v3 = ((line.startPos-line.endPos).perp()).norm();
                  
                  float normLength = 1.0f/2.0f;///std::pow(2,o+1);
                  
                  LineSegment tmpline1({{line.startPos+v2*length*normLength},{line.startPos}});
                  LineSegment tmpline2({{line.startPos+v2*length*normLength},{line.endPos}});
                  LineSegment tmpline3({{line.startPos+v2*length*normLength},{line.startPos+v2*length*normLength+v3*length*normLength}});
                  
                  tmpNextOrder.push_back(tmpline1);
                  tmpNextOrder.push_back(tmpline2);
                  tmpNextOrder.push_back(tmpline3);
                  */
                }
                
                TGraph = tmpNextOrder;
                
                counter = 0;
		return;
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
                    simpleTreeGraph.printTreeGraph();
                    /*
                  
                  simpleTreeGraph.clear();
                  positionNode.clear();
                  
                  simpleTreeGraph.addEdge(0,1,1);
                  simpleTreeGraph.addEdge(1,2,1);
                  simpleTreeGraph.addEdge(1,3,1);
                  
                  positionNode[0]={-10.0f, 0.0f};
                  positionNode[1]={0.0f,0.0f};
                  positionNode[2]={5.0f, -8.660254037f};
                  positionNode[3]={5.0f, 8.660254037f};
                  
                  simpleTreeGraph.printTreeGraph();
                  simpleTreeGraph.calculateEccentricity();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph);
                    std::cout << "size: " << simpleTreeGraph.graph.size() << std::endl ;
                    simpleTreeGraph.calculateEccentricity();
                    simpleTreeGraph.printTreeGraph();
                  }
                  */
                  
                  TGraph.clear();
                  calculateTGraphDrawing(order);
                  
                  //TGraph.clear();
                  //TGraph.push_back({ {-100.0f, 0.0f}, {0.0f, 0.0f} });
                  //TGraph.push_back({ { 50.0f, -86.60254037f},{0.0f, 0.0f} });
                  //TGraph.push_back({ { 50.0f, 86.60254037f},{0.0f, 0.0f} });
                  
//                   TGraph.push_back({ {0.0f, 0.0f}, {-100.0f, 0.0f} });
//                 TGraph.push_back({ {0.0f, 0.0f}, {100.0f, 0.0f} });
//                 TGraph.push_back({ {0.0f, 0.0f}, {0.0f, -100.0f} });
//                   
                  //for(int i = 1; i <= order; i++)
                  //  calculateTGraph(i);
                  
                  
                  float oldpointOnCurveIdx = pointOnCurveIdx;
                  calculatePath();
                  pointOnCurveIdx = int(oldpointOnCurveIdx*(totNumPoints-1))/float(totNumPoints-1);
                  counterInc = 1;
                  
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
                  
                  simpleTreeGraph.printTreeGraph();
                  simpleTreeGraph.calculateEccentricity();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph);
                    std::cout << "size: " << simpleTreeGraph.graph.size() << std::endl ;
                    simpleTreeGraph.calculateEccentricity();
                    simpleTreeGraph.printTreeGraph();
                  }
                  
                  TGraph.clear();
                  calculateTGraphDrawing(order);
                  
                  //TGraph.clear();
                  //TGraph.push_back({ {-100.0f, 0.0f}, {0.0f, 0.0f} });
                  //TGraph.push_back({ { 50.0f, -86.60254037f},{0.0f, 0.0f} });
                  //TGraph.push_back({ { 50.0f, 86.60254037f},{0.0f, 0.0f} });
//                    TGraph.push_back({ {0.0f, 0.0f}, {-100.0f, 0.0f} });
//                 TGraph.push_back({ {0.0f, 0.0f}, {100.0f, 0.0f} });
//                 TGraph.push_back({ {0.0f, 0.0f}, {0.0f, -100.0f} });
                  
                  //for(int i = 1; i <= order; i++)
                  //  calculateTGraph(i);
                  
                  
                  float oldpointOnCurveIdx = pointOnCurveIdx;
                  calculatePath();
                  pointOnCurveIdx = int(oldpointOnCurveIdx*(totNumPoints-1))/float(totNumPoints-1);
                  counterInc = 1;
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
                  
                  simpleTreeGraph.printTreeGraph();
                  simpleTreeGraph.calculateEccentricity();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph);
                    std::cout << "size: " << simpleTreeGraph.graph.size() << std::endl ;
                    simpleTreeGraph.calculateEccentricity();
                    simpleTreeGraph.printTreeGraph();
                  }
                  
                  TGraph.clear();
                  calculateTGraphDrawing(order);
                  
                  //
                  //TGraph.clear();
                  //TGraph.push_back({ {-100.0f, 0.0f}, {0.0f, 0.0f} });
                  //TGraph.push_back({ { 50.0f, -86.60254037f},{0.0f, 0.0f} });
                  //TGraph.push_back({ { 50.0f, 86.60254037f},{0.0f, 0.0f} });
                  
//                   TGraph.push_back({ {0.0f, 0.0f}, {-100.0f, 0.0f} });
//                 TGraph.push_back({ {0.0f, 0.0f}, {100.0f, 0.0f} });
//                 TGraph.push_back({ {0.0f, 0.0f}, {0.0f, -100.0f} });
//                   
                 // for(int i = 1; i <= order; i++)
                  //  calculateTGraph(i);
                  
                  counterInc++;
                  counterInc = counterInc > N ? N : counterInc;
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
                   p o*sitionNode[0]={-10.0f, 0.0f};
                   positionNode[1]={0.0f,0.0f};
                   positionNode[2]={5.0f, -8.660254037f};
                   positionNode[3]={5.0f, 8.660254037f};
                   */
                  
                  simpleTreeGraph.printTreeGraph();
                  simpleTreeGraph.calculateEccentricity();
                  
                  std::cout << "TGraph Size:" << simpleTreeGraph.graph.size() << std::endl ;
                  
                  
                  
                  for(int i = 2; i <= order; i++)
                  {
                    std::cout << "generation: " << i << std::endl;
                    nextGenerationGraph(simpleTreeGraph, simpleTreeGraph);
                    std::cout << "size: " << simpleTreeGraph.graph.size() << std::endl ;
                    simpleTreeGraph.calculateEccentricity();
                    simpleTreeGraph.printTreeGraph();
                  }
                  
                  TGraph.clear();
                  calculateTGraphDrawing(order);
                  
                  //TGraph.push_back({ {-100.0f, 0.0f}, {0.0f, 0.0f} });
                  //TGraph.push_back({ { 50.0f, -86.60254037f},{0.0f, 0.0f} });
                  //TGraph.push_back({ { 50.0f, 86.60254037f},{0.0f, 0.0f} });
                  
//                   TGraph.push_back({ {0.0f, 0.0f}, {-100.0f, 0.0f} });
//                 TGraph.push_back({ {0.0f, 0.0f}, {100.0f, 0.0f} });
//                 TGraph.push_back({ {0.0f, 0.0f}, {0.0f, -100.0f} });
//                   
                 // for(int i = 1; i <= order; i++)
                  //  calculateTGraph(i);
                  
                  counterInc--;
                  counterInc = counterInc < 0 ? 0 : counterInc;
                }
                
                if (GetKey(olc::Key::ENTER).bPressed)
                {
                  drawAll = !drawAll;
                }
                
                if (GetKey(olc::Key::W).bPressed)
                {
                  pointOnCurveIdx += 1.0/(totNumPoints-1);
                  pointOnCurveIdx = pointOnCurveIdx >= 1.0 ? 1.0 : pointOnCurveIdx;
                }
                
                if (GetKey(olc::Key::E).bPressed)
                {
                  pointOnCurveIdx -= 1.0/(totNumPoints-1);
                  pointOnCurveIdx = pointOnCurveIdx < 0.0 ? 0.0 : pointOnCurveIdx;
                }
                
                if (GetKey(olc::Key::R).bPressed)
                {
                  fOffsetX = 0.0f;
                  fOffsetY = 0.0f;
                  fScaleX = 1.0f;
                  fScaleY = 1.0f;

                  fStartPanX = 0.0f;
                  fStartPanY = 0.0f;
                }
                
                if (GetKey(olc::Key::T).bPressed)
                {
                  bDrawIdx = !bDrawIdx;
                }
                
                
		// called once per frame
                counter += counterInc;
		// for (int x = 0; x < ScreenWidth(); x++)
		//	for (int y = 0; y < ScreenHeight(); y++)
		//		Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
                
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
                
                
                
                //DrawRect(pixel_sx, pixel_sy, pixel_ex-pixel_sx, pixel_ey-pixel_sy, olc::GREEN);
                //if(!drawAll && (i < counter) )
                //DrawLine(pixel_sx, pixel_sy, pixel_ex, pixel_ey, hsvColor);
                
                //if(drawAll )
                DrawLine(pixel_sx, pixel_sy, pixel_ex, pixel_ey, hsvColor);
                
                
                }
                
                /*
                //if(!drawAll)
                {
                
                
                // Draw Boundary
                //DrawLine(10, 10, ScreenWidth() - 10, 10, olc::YELLOW);
                //DrawLine(10, 10, 10, ScreenHeight() - 10, olc::YELLOW);
                
                for(int i = 1; (i < path.size()); i++)
                {
                  
                  // Create a color using the hsv color space, this one makes a nice orange color
                  float idxMap = (i < path.size()) ? i*360.0f/path.size() : 360.0f;
                  auto hsvColor = olc::Colours::FromHsv(idxMap, 255, 255);
                  
                  int pixel_sx, pixel_sy, pixel_ex, pixel_ey;
                
                float w_sx = path[i-1].x;
                float w_sy = path[i-1].y;
                float w_ex = path[i].x;
                float w_ey = path[i].y;
                  
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
                
                
                
                //DrawRect(pixel_sx, pixel_sy, pixel_ex-pixel_sx, pixel_ey-pixel_sy, olc::GREEN);
                if(!drawAll && (i < counter) )
                DrawLine(pixel_sx, pixel_sy, pixel_ex, pixel_ey, hsvColor);
                
                if(drawAll )
                DrawLine(pixel_sx, pixel_sy, pixel_ex, pixel_ey, hsvColor);
                
                
                }
                }
                
                if(bDrawIdx)
                for(int i = 0; i < path.size(); i++)
                {
                  int pixel_sx, pixel_sy, pixel_ex, pixel_ey;
                  float w_sx = path[i].x;
                  float w_sy = path[i].y;
                  
                  WorldToScreen(w_sx, w_sy, pixel_sx, pixel_sy);
                  
                  olc::vi2d sizeMsg = GetTextSize(std::to_string(i));
                  
                  if (w_sy >= fWorldTop && w_sy <= fWorldBottom && 
                    
                    w_sx >= fWorldLeft && w_sx <= fWorldRight
                )
                    if (pixel_sx+sizeMsg.x <= ScreenOffSetX+NMAX && pixel_sy+sizeMsg.y <= ScreenOffSetY+NMAX
                )
                    
                    
                    
                DrawStringProp(pixel_sx, pixel_sy, std::to_string(i), olc::YELLOW);
                  
                }
                
                
                if(counter >= path.size())
                  counter = 0;
                
                
                DrawString(10, ScreenHeight()-40, "Order: " + std::to_string(order), olc::YELLOW);
                DrawString(100, ScreenHeight()-40, "Points: " + std::to_string(totNumPoints), olc::YELLOW);

                
                //for(int i = 0; i < totNumPoints; i++)
                
                  olc::vf2d pos=hilbert(pointOnCurveIdx*(totNumPoints-1));
                  float len = int(std::pow(2,orderMAX)/N);//ScreenWidth()/N;
                  pos *= len;
                  pos += olc::vf2d(int(len/2),int(len/2));
                  int pixel_sx, pixel_sy, pixel_ex, pixel_ey;
                  
                  float w_sx = pos.x;
                  float w_sy = pos.y;
                
                    //clip to region
                  w_sx = (w_sx < fWorldLeft) ? fWorldLeft : w_sx;
                w_sx = (w_sx > fWorldRight) ? fWorldRight : w_sx;
                w_sy = (w_sy < fWorldTop) ? fWorldTop : w_sy;
                w_sy = (w_sy > fWorldBottom) ? fWorldBottom : w_sy;
                
                WorldToScreen(w_sx, w_sy, pixel_sx, pixel_sy);
                  
                  DrawCircle(pixel_sx, pixel_sy,2,olc::YELLOW);
                
                
                DrawString(10, ScreenHeight()-30, "length: " + std::to_string(pointOnCurveIdx), olc::YELLOW);
                DrawString(10, ScreenHeight()-20, "pos: (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ")", olc::YELLOW);
                DrawString(10, ScreenHeight()-10, "USAGE: WERT ARROWS ENTER ESC MOUSE", olc::YELLOW);
                DrawString(10, ScreenHeight()-50, "scale: (" + std::to_string(fScaleX) + "," + std::to_string(fScaleY) + ")", olc::YELLOW);
                */
                
                DrawString(10, ScreenHeight()-40, "Order: " + std::to_string(order), olc::YELLOW);
                DrawString(100, ScreenHeight()-40, "Points: " + std::to_string(TGraph.size()), olc::YELLOW);
                
                DrawRect(ScreenOffSetX, ScreenOffSetY, NMAX, NMAX, olc::GREEN);
                
                // Graceful exit if user is in full screen mode
		return !GetKey(olc::Key::ESCAPE).bPressed;
                
                

	}
	
	olc::vf2d hilbert(int indexNumber)
        {
          olc::vf2d point;
          
          int index = indexNumber;
          
          switch (index&3)
          {
            case 0: point = olc::vf2d(0,0);
                    break;
                    
            case 1: point = olc::vf2d(0,1);
                    break;
                    
            case 2: point = olc::vf2d(1,1);
                    break;
                    
            case 3: point = olc::vf2d(1,0);
                    break;
            
            default: point = olc::vf2d(0,0);
                     break;
          }
          
          for(int j = 1; j < order; j++)
          {
          float tmp;
          index = (index>>2);
          
          float length = std::pow(2,j);
          switch (index&3)
          {
            case 0: point += olc::vf2d(0,0);
                    std::swap(point.x, point.y);
                    break;
                    
            case 1: point += olc::vf2d(0,length);
                    break;
                    
            case 2: point += olc::vf2d(length,length);
                    break;
                    
            case 3: tmp = length-1.0f-point.x;
                    point.x= length-1.0f-point.y;
                    point.y= tmp;
                    
                    point += olc::vf2d(length,0);
                    break;
            
            default: point += olc::vf2d(0,0);
                     break;
          }
          
          }
          
          return point;
          
        }
	
public: int order;
        int N;
        int totNumPoints;
        
        std::vector<olc::vf2d> path;
        
        std::vector<LineSegment> TGraph;
        
        int counter;
        int counterInc;
        
        const int orderMAX = 8;
        const int NMAX = int(std::pow(2,orderMAX));
        
        bool drawAll;
        
        float pointOnCurveIdx;
        
        float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;
	float fScaleX = 1.0f;
	float fScaleY = 1.0f;

	float fStartPanX = 0.0f;
	float fStartPanY = 0.0f;
        
        int ScreenOffSetX;
        int ScreenOffSetY;
        
        bool bDrawIdx;
        
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
