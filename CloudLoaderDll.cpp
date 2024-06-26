#include "CLogger/CLogger.h"
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

static int s_nIter = 0;
static vector<PointCloud<PointType>::Ptr> a_sPointCloudStaticArray;


extern "C" __declspec(dllexport) int* CloudLoader(const char* pFileName)
{

    // Inizializzazione Logger
    WCHAR wPointCludName[32] = L"PointCloud_SharedMemory";
    WCHAR wPointMutexName[32] = L"PointCloud_SharedMutex";
    _CLOGGER_INIT_(true, string(""), wPointCludName, 1024 * 128, wPointMutexName);
    _CLogger("############## Start reading Point Cloud file ##############" << endl << endl);
    _CLogger("File name:" <<pFileName<< endl);



    PointCloud<PointType>* pCloud=new PointCloud<PointType>;
    string sFileName = string(pFileName);

    int iErr;

    if (sFileName.rfind(string(".e57")) != string::npos || sFileName.rfind(string(".E57")) != string::npos)
    {
        // Read some info data
        int iPointNum;
        ReadA57FileInfo((const char*)(sFileName.c_str()), iPointNum);
        _CLogger("Number of point: " << iPointNum << endl);

        // Read point cloud file itself
        iErr = ReadA57_d((const char*)(sFileName.c_str()), *pCloud);
    }
    else
        iErr = pcl::io::load((const char*)(sFileName.c_str()), *pCloud);


    if (iErr == -1)
    {
        _CLogger("ERROR DURING READING!!!!!" << endl <<endl );
        _CLOGGER_END_;

        return NULL;
    }


    _CLogger("... finished!" << endl << endl );
    _CLOGGER_END_;

    static PointCloud<PointType>::Ptr pCloudStatic(pCloud);

    return (int*)(pCloud);

}

extern "C" __declspec(dllexport) int* GetPointList(const int* pPointer, int& nPoint)
{
    // Inizializzazione Logger
    WCHAR wPointCludName[32] = L"PointCloud_SharedMemory";
    WCHAR wPointMutexName[32] = L"PointCloud_SharedMutex";
    _CLOGGER_INIT_(true, string(""), wPointCludName, 1024 * 128, wPointMutexName);

    PointCloud<PointType>* pCloudObj = (PointCloud<PointType>*)pPointer;
    PointCloud<PointType>::Ptr* pCloud =new PointCloud<PointType>::Ptr(pCloudObj);

    nPoint = (*pCloud)->points.size();
    _CLogger("Da GetPointList, Number of point: " << nPoint << endl);

//    int* piTmp = (int*)(&(pCloud->points[0]));
    int* piTmp = (int*)(&((*pCloud)->points[0]));

    _CLOGGER_END_;
    return piTmp;
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

extern "C" __declspec(dllexport) int FreeMemory(const int* pPointer)
{
    if (pPointer == NULL)
        return 0;
    try
    {
        /*
        PointCloud<PointType>* pCloudObj = (PointCloud<PointType>*)pPointer;
        delete pCloudObj;
        */
        PointCloud<PointType>* pCloudObj = (PointCloud<PointType>*)pPointer;
        PointCloud<PointType>::Ptr pCloud(pCloudObj);
        pCloud.reset();

        return 0;
    }
    catch (...)
    {
        return 1;
    }

}