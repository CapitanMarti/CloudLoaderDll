#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include "CLogger/CLogger.h"
#include <filesystem>
#include <pcl/visualization/pcl_plotter.h>
#include <pcl/visualization/cloud_viewer.h>
#include "pcd_viewer.h"
#include "pcl/io/ply_io.h"
#include "pcl/io/auto_io.h"
#include "PCCommonFun.h"

#include "E57CloudReader.h"
using namespace pcl;
using namespace std;



extern "C" __declspec(dllexport) int* CloudLoader(const char* pFileName)
{

//    PointCloud<PointType>::Ptr pCloud(new PointCloud<PointType>);
    PointCloud<PointType>* pCloud=new PointCloud<PointType>;
    string sFileName = string(pFileName);

    int iErr;

    if (sFileName.rfind(string(".e57")) != string::npos || sFileName.rfind(string(".E57")) != string::npos)
    {
        iErr = ReadA57_d((const char*)(sFileName.c_str()), *pCloud);
    }
    else
        iErr = pcl::io::load((const char*)(sFileName.c_str()), *pCloud);


    if (iErr == -1)
        return NULL;


    //++++++++++++++++++++++++++
    /*
    int nSize = pCloud->points.size();
    FILE* pFile = fopen("D:\\Downloads\\CloudLoaderOutput0.txt", "wt");
    fprintf(pFile, "nSize=%d\n", nSize);
    fprintf(pFile, "point[22]=%f\n", pCloud->points[22].x);
    fprintf(pFile, "point[last]=%f\n", pCloud->points[nSize-1].z);
    fclose(pFile);
    */
    //++++++++++++++++++++++++++++++++

    return (int*)(pCloud);

}

extern "C" __declspec(dllexport) void TestCloudPointer(const int* pPointer)
{
    PointCloud<PointType>* pCloudObj = (PointCloud<PointType>*)pPointer;
    PointCloud<PointType>::Ptr pCloud(pCloudObj);

    //++++++++++++++++++++++++++
    /*
    int nSize = pCloud->points.size();
    FILE* pFile = fopen("D:\\Downloads\\CloudLoaderOutput1.txt", "wt");
        fprintf(pFile, "nSize=%d\n", nSize);
        fprintf(pFile, "point[22]=%f\n", pCloud->points[22].x);
        fprintf(pFile, "point[last]=%f\n", pCloud->points[nSize-1].z);
        fclose(pFile);
        */
    //++++++++++++++++++++++++++++++++
}

