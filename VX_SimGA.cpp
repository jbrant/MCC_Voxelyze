/*******************************************************************************
Copyright (c) 2010, Jonathan Hiller (Cornell University)
If used in publication cite "J. Hiller and H. Lipson "Dynamic Simulation of Soft Heterogeneous Objects" In press. (2011)"

This file is part of Voxelyze.
Voxelyze is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Voxelyze is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
See <http://www.opensource.org/licenses/lgpl-3.0.html> for license details.
*******************************************************************************/

#include "VX_SimGA.h"
#include <iostream>

CVX_SimGA::CVX_SimGA()
{
	Fitness = 0.0f;
	TrackVoxel = 0;
	FitnessFileName = "";
//	print_scrn = false;
	WriteFitnessFile = false;
	FitnessType = FT_NONE;	//no reporting is default
    pSimLog = new CXML_Rip();
}

void CVX_SimGA::SaveResultFile(std::string filename)
{
	CXML_Rip XML;
	WriteResultFile(&XML);
	XML.SaveFile(filename);
}


void CVX_SimGA::WriteResultFile(CXML_Rip* pXML)
{
// 	float totalPoints = 0;
// 	float goodSteps = 0;
// 	float badSteps = 0;
// 	for (std::map< float, float >::iterator it = floorIsLava.begin(); it != floorIsLava.end(); it++ )
// 	{
// 		totalPoints += it->second;
// 		if (it->second > 0 ) {goodSteps++;}
// 		if (it->second < 0 ) {badSteps++;}
// 	}
// 	// std::cout << "totalPoints: " << totalPoints << std::endl;

	float maxCageDistX = 0.0;
	for (int i=0; i<NumVox(); i++){
		if (VoxArray[i].GetMaterialIndex() == 5 and VoxArray[i].GetCurPos().x/LocalVXC.GetLatticeDim() > maxCageDistX)
		{
			maxCageDistX = VoxArray[i].GetCurPos().x/LocalVXC.GetLatticeDim();
		}
	}

	float numVoxelsOutside = 0.0;
	for (int i=0; i<NumVox(); i++){
		if (VoxArray[i].GetMaterialIndex() != 5)
		{
			if (VoxArray[i].GetCurPos().x/LocalVXC.GetLatticeDim() > maxCageDistX)
			{
				numVoxelsOutside += 1;
			}
		}
	}
 	if (numVoxelsOutside == 0)
 	{
		float farthestX = 0.0;
		for (int i=0; i<NumVox(); i++)
		{
			if (VoxArray[i].GetMaterialIndex() != 5 and VoxArray[i].GetCurPos().x/LocalVXC.GetLatticeDim() > farthestX)
			{
				farthestX = VoxArray[i].GetCurPos().x/LocalVXC.GetLatticeDim();
			}
		}
		numVoxelsOutside = farthestX/100.0;
	}

	pXML->DownLevel("Voxelyze_Sim_Result");
	pXML->SetElAttribute("Version", "1.0");
	pXML->DownLevel("Fitness");
	// pXML->Element("FinalCOM_Dist", SS.CurCM.Normalize()-IniCM.Normalize());
	// pXML->Element("FinalCOM_DistX", SS.CurCM.x-IniCM.x);
	// pXML->Element("FinalCOM_DistY", SS.CurCM.y-IniCM.y);
	// pXML->Element("FinalCOM_DistZ", SS.CurCM.z-IniCM.z);
	// pXML->Element("DistanceFitness", SS.CurCM.Normalize()-IniCM.Normalize());
	// pXML->Element("EnergyFitness", 0);
	// pXML->Element("StepsIn", stepsIn);
	// pXML->Element("StepsOut", stepsOut);
	// pXML->Element("GoodSteps", goodSteps);
	// pXML->Element("BadSteps", badSteps);
	// pXML->Element("DistanceFitness", totalPoints);
	// pXML->Element("CompositeFitness", (SS.CurCM-IniCM).Length()/LocalVXC.GetLatticeDim()*(COMZ/numSamples) );
	// pXML->Element("Height", COMZ/numSamples );
	//pXML->Element("CompositeFitness", dist/LocalVXC.GetLatticeDim()*COMZ );
	//pXML->Element("DistX", (SS.CurCM.x-IniCM.x)/LocalVXC.GetLatticeDim() );
	//pXML->Element("NumOutOfCageX", numVoxelsOutside);
	//pXML->Element("GoalDirected", pow(pow(fitness1,2)+pow(fitness2,2),0.5) );
	//pXML->Element("Goal1", fitness1);
    pXML->Element("Height", COMZ );
    pXML->Element("Distance", GetCurDistance());
    pXML->Element("xPos", SS.CurCM.x);
    pXML->Element("yPos", SS.CurCM.y);
    pXML->Element("simTime", GetCurTime());
	pXML->UpLevel();
	pXML->UpLevel();

	// std::cout << "dist: " << dist/LocalVXC.GetLatticeDim()  << std::endl;
	// // std::cout << "height: " << COMZ << std::endl;
	// std::cout << "fitness: " << dist/LocalVXC.GetLatticeDim() * COMZ << std::endl;

}

void CVX_SimGA::WriteAdditionalSimXML(CXML_Rip* pXML)
{
	pXML->DownLevel("GA");
		pXML->Element("Fitness", Fitness);
		pXML->Element("FitnessType", (int)FitnessType);
		pXML->Element("TrackVoxel", TrackVoxel);
		pXML->Element("FitnessFileName", FitnessFileName);
		pXML->Element("WriteFitnessFile", WriteFitnessFile);
        pXML->Element("SimLogFileName", SimLogFileName);
        pXML->Element("WriteSimLogFile", WriteSimLogFile);
	pXML->UpLevel();
}

bool CVX_SimGA::ReadAdditionalSimXML(CXML_Rip* pXML, std::string* RetMessage)
{
	if (pXML->FindElement("GA")){
		int TmpInt;
		if (!pXML->FindLoadElement("Fitness", &Fitness)) Fitness = 0;
		if (pXML->FindLoadElement("FitnessType", &TmpInt)) FitnessType=(FitnessTypes)TmpInt; else Fitness = 0;
		if (!pXML->FindLoadElement("TrackVoxel", &TrackVoxel)) TrackVoxel = 0;
		if (!pXML->FindLoadElement("FitnessFileName", &FitnessFileName)) FitnessFileName = "";
		if (!pXML->FindLoadElement("WriteFitnessFile", &WriteFitnessFile)) WriteFitnessFile = false;
        if (!pXML->FindLoadElement("SimLogFileName", &SimLogFileName)) SimLogFileName = "";
        if (!pXML->FindLoadElement("WriteSimLogFile", &WriteSimLogFile)) WriteSimLogFile = false;

		pXML->UpLevel();
	}

	return true;
}

/**
 * Writes the top-level element in the verbose simulation log.
 */
void CVX_SimGA::OpenSimLog() {
    pSimLog->DownLevel("SimLogData");
}

/**
 * Writes a single line to the verbose simulation log.
 */
void CVX_SimGA::WriteSimLogEntry(long int timeStep) {

    pSimLog->DownLevel("SimEntry");
    pSimLog->SetElAttribute("TimeStep", timeStep);

    // Write simulation state properties
    pSimLog->Element("Time", GetCurTime());
    pSimLog->Element("xPos", SS.CurCM.x);
    pSimLog->Element("yPos", SS.CurCM.y);
    pSimLog->Element("zPos", SS.CurCM.z);
    pSimLog->Element("Distance", GetCurDistance());
    pSimLog->Element("TotalDistance", SS.TotalDistanceTraversed);
    pSimLog->Element("VoxelsTouchingFloor", GetNumTouchingFloor());
    pSimLog->Element("MaxVoxelVelocity", SS.MaxVoxVel);
    pSimLog->Element("MaxVoxelDisplacement", SS.MaxVoxDisp);
    pSimLog->Element("MaxTrialDisplacement", SS.MaxTrialDisplacement);
    pSimLog->Element("xDisplacement", SS.TotalObjDisp.x);
    pSimLog->Element("yDisplacement", SS.TotalObjDisp.y);
    pSimLog->Element("zDisplacement", SS.TotalObjDisp.z);

    pSimLog->UpLevel();
}

/**
 * Closes and writes the verbose simulation log.
 *
 * @param filename The name of the simulation log file.
 */
void CVX_SimGA::CloseSimLog(std::string filename) {
    pSimLog->UpLevel();
    pSimLog->SaveFile(filename);
}
