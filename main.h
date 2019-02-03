#define __USE_LARGEFILE64
#include <sys/types.h>
#include <unistd.h>

#include<iostream>
#include<string>
#include <limits>
#include <list>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include<windows.h>
#include<process.h>

class Point
{
    float x;
    float y;
public:
    Point(float,float);
    float X();
    float Y();
};

class PointConnection
{
    uint32_t BeginPoint;
    uint32_t EndPoint;
public:
    PointConnection(uint32_t,uint32_t);
    void Set(uint32_t,uint32_t);
    void SetBegin(uint32_t);
    void SetEnd(uint32_t);
    uint32_t GetBegin();
    uint32_t GetEnd();
};

struct Path
{
    std::list <PointConnection> paths;
    float Cost;
};

class GeneticAlgorithm
{
    std::vector <Point> Points;
    std::list <Path> Paths;
    std::list <Path> NewPaths;
    uint32_t PointCount;
    uint32_t PathCount;
    uint32_t Generations;
    bool printbestpath;
    bool exit;
public:
    GeneticAlgorithm();
    std::list <Path> GetPaths();
private:
    void GetInput();
    void GetPoints();
    uint32_t GetUnsingedIntFromInput(std::string);
    float GetFloatFromInput(std::string);
    void PrintString(std::string);
    void PrintStringAndLine(std::string);
    bool AskIf(std::string);

    void GenerateRandomPaths();
    void PrintPathes();
    void PrintBestPath(uint32_t);

    void CountPathCosts();
    float CountDistance(Point,Point);
    void DestroyWeakestElements();

    void NextGeneration();
    void Mutate(Path);
    void SelectForCrossing();
    void GeneCrossing(Path,Path);
};
