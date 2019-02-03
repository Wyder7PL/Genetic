#include "main.h"


int main()
{
    srand(time(NULL));
    GeneticAlgorithm aaa;
    return 0;
}

Point::Point(float X,float Y):x(X),y(Y)
{}

float Point::X()
{
    return x;
}
float Point::Y()
{
    return y;
}

PointConnection::PointConnection(uint32_t first,uint32_t last)
:BeginPoint(first),EndPoint(last){}

void PointConnection::Set(uint32_t first,uint32_t last)
{
    SetBegin(first);
    SetEnd(last);
}

void PointConnection::SetBegin(uint32_t first)
{
    BeginPoint = first;
}

void PointConnection::SetEnd(uint32_t last)
{
    EndPoint = last;
}
uint32_t PointConnection::GetBegin()
{
    return BeginPoint;
}
uint32_t PointConnection::GetEnd()
{
    return EndPoint;
}

GeneticAlgorithm::GeneticAlgorithm()
:exit(false)
{
    while(!exit)
    {
    GetInput();
    GenerateRandomPaths();
    for(uint32_t i = 0;i < Generations;i++)
    {
        NextGeneration();
        CountPathCosts();
        DestroyWeakestElements();
        if(printbestpath)
            PrintBestPath(i+1);
    }
    PrintPathes();
    exit = AskIf("Y - for exit, N - for another try ");
    }
}

std::list<Path> GeneticAlgorithm::GetPaths()
{
    return Paths;
}

void GeneticAlgorithm::GetInput()
{
    std::string InputMessage = "Enter how many points: ";
    do{
        PointCount = GetUnsingedIntFromInput(InputMessage);
        InputMessage = "The value should be bigger than 2, enter correct value : ";
    }while(PointCount<3);

    PrintStringAndLine("So there are " + std::to_string(PointCount) + " Points");
    GetPoints();
    InputMessage = "Enter number of paths : ";
    do{
        PathCount = GetUnsingedIntFromInput(InputMessage);
        InputMessage = "The value should be minimum 2, enter correct value : ";
    }while(PointCount<2);

    InputMessage = "Enter number of generations : ";
    do{
        Generations = GetUnsingedIntFromInput(InputMessage);
        InputMessage = "The value should be minimum 1, enter correct value : ";
    }while(PointCount<1);
    printbestpath = AskIf("Print Best Path of Generation?");
}

void GeneticAlgorithm::GetPoints()
{
    for(uint32_t i = 0;i < PointCount;i++)
    {
        float x = 0, y = 0;
        PrintStringAndLine("Enter "+std::to_string(i+1)+" point coordinates:");
        x = GetFloatFromInput("x: ");
        y = GetFloatFromInput("y: ");
        Point p(x,y);
        Points.push_back(p);
    }
}

uint32_t GeneticAlgorithm::GetUnsingedIntFromInput(std::string info)
{
    std::cout << info;
    uint32_t value;
    while(!(std::cin >> value)){
        std::cout << std::endl;
        std::cin.clear();
        std::cout << "Wrong value type, Please enter unsinget int value:";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}
float GeneticAlgorithm::GetFloatFromInput(std::string info)
{
    std::cout << info;
    float value=0;
    while(!(std::cin >> value))
    {
        std::cout << "Wrong value type, Please enter float value:";
        std::cin.clear();
        std::cin.ignore();
    }
    std::cin.clear();
    std::cin.ignore();
    return value;
}

void GeneticAlgorithm::PrintString(std::string info)
{
    std::cout << info;
}
void GeneticAlgorithm::PrintStringAndLine(std::string info)
{
    PrintString(info);
    std::cout<<std::endl;
}

bool GeneticAlgorithm::AskIf(std::string info)
{
    char answer;
    std::cout << info << " (y/n) ";
    while(!(std::cin >> answer)||!((answer!='y')!=(answer!='n')))
    {
        std::cout << "Wrong value type, Please enter right character:";
        std::cin.clear();
        std::cin.ignore();
    }
    std::cin.clear();
    std::cin.ignore();

    if(answer=='y')
        return true;
    return false;

}

void GeneticAlgorithm::GenerateRandomPaths()
{
    std::vector<uint32_t> PointOrder;
    for(uint32_t i = 0;i<PointCount;i++)
    {
        PointOrder.push_back(i);
    }
    uint32_t previous = 0;
    for(uint32_t i = 0;i<PathCount;i++)
    {
        Path p;
        std::vector<uint32_t> PointOrderCopy = PointOrder;
        for(uint32_t j = 0;j<PointCount;j++)
        {
            uint32_t random = rand()%(PointCount-j);
            uint32_t number = PointOrderCopy[random];
            if(j>0)
            {
                p.paths.push_back(PointConnection(previous,number));
            }
            previous = number;
            PointOrderCopy.erase(PointOrderCopy.begin()+random);
        }
        Paths.push_back(p);
    }
}

void GeneticAlgorithm::PrintPathes()
{
    PrintStringAndLine("__________________________");
    for(std::list<Path>::iterator ite = Paths.begin();ite!=Paths.end();ite++)
    {
        for(std::list <PointConnection>::iterator it2 = (*ite).paths.begin();it2!=(*ite).paths.end();it2++)
        {
            if(it2==(*ite).paths.begin())
                PrintString(std::to_string((*it2).GetBegin()+1)+" ");
            PrintString(std::to_string((*it2).GetEnd()+1)+" ");
        }
        PrintStringAndLine(" Distance "+std::to_string((*ite).Cost));
        PrintStringAndLine("__________________________");
    }

}

void GeneticAlgorithm::PrintBestPath(uint32_t generation)
{
    std::list<Path>::iterator ite = Paths.begin();
    PrintString("Best path of "+std::to_string(generation)+" generation: ");
    for(std::list <PointConnection>::iterator it2 = (*ite).paths.begin();it2!=(*ite).paths.end();it2++)
    {
        if(it2==(*ite).paths.begin())
            PrintString(std::to_string((*it2).GetBegin()+1)+" ");
        PrintString(std::to_string((*it2).GetEnd()+1)+" ");
    }
    PrintStringAndLine(" Distance "+std::to_string((*ite).Cost));
}

void GeneticAlgorithm::CountPathCosts()
{
    for(std::list<Path>::iterator ite = Paths.begin();ite!=Paths.end();ite++)
    {
        (*ite).Cost = 0;
        for(std::list <PointConnection>::iterator it2 = (*ite).paths.begin();it2!=(*ite).paths.end();it2++)
        {
            (*ite).Cost += CountDistance(Points[(*it2).GetBegin()],Points[(*it2).GetEnd()]);
        }
    }
    Paths.sort( []( const Path & a, Path & b ) { return a.Cost < b.Cost; } );
}

float GeneticAlgorithm::CountDistance(Point p1,Point p2)
{
    return pow(pow(p1.X()-p2.X(),2)+pow(p1.Y()-p2.Y(),2),0.5);
}

void GeneticAlgorithm::DestroyWeakestElements()
{
    while(PathCount<Paths.size())
    {
        Paths.pop_back();
    };
}

void GeneticAlgorithm::NextGeneration()
{
    NewPaths.clear();

    SelectForCrossing();
    for(std::list<Path>::iterator ite = Paths.begin();ite!=Paths.end();ite++)
    {
        Mutate((*ite));
    }

    for(std::list<Path>::iterator ite = NewPaths.begin();ite!=NewPaths.end();ite++)
    {
        Paths.push_back((*ite));
    }
}

void GeneticAlgorithm::Mutate(Path path)
{
    uint32_t FirstNumber = rand()%PointCount;
    uint32_t SecondNumber;
    do{
        SecondNumber = rand()%PointCount;
    }while(SecondNumber==FirstNumber);

    Path p = path;

    for(std::list <PointConnection>::iterator ite = p.paths.begin();ite!=p.paths.end();ite++)
    {
        uint32_t Pbegin = (*ite).GetBegin(), Pend = (*ite).GetEnd();
        if(Pbegin==FirstNumber)
            (*ite).SetBegin(SecondNumber);
        if(Pend==FirstNumber)
            (*ite).SetEnd(SecondNumber);
        if(Pbegin==SecondNumber)
            (*ite).SetBegin(FirstNumber);
        if(Pend==SecondNumber)
            (*ite).SetEnd(FirstNumber);
    }

    NewPaths.push_back(p);
}

void GeneticAlgorithm::SelectForCrossing()
{
    uint32_t NewSize = Paths.size();
    if(NewSize%2==1)
        NewSize--;
    std::list <Path> ToCross;
    for(std::list<Path>::iterator ite = Paths.begin();ite!=Paths.end();ite++)
    {
        if(NewSize>0)
        {
            if(rand()%2==0)
                ToCross.push_back((*ite));
            else
                ToCross.push_front((*ite));
            NewSize--;
        }
    }
    for(std::list<Path>::iterator ite = ToCross.begin();ite!=ToCross.end();)
    {
        GeneCrossing((*(ite++)),(*(ite++)));
    }
}

void GeneticAlgorithm::GeneCrossing(Path p1,Path p2)
{
    uint32_t Random = rand()%PointCount;
    uint32_t right = Random, left = Random;
    uint32_t lastpoint = (*(p1.paths.rbegin())).GetEnd();
    for(std::list <PointConnection>::iterator ite = p1.paths.begin();ite!=p1.paths.end();)
    {
        if((*ite).GetBegin()!=Random)
        {
            PointConnection pc(lastpoint,(*ite).GetBegin());
            lastpoint = (*ite).GetBegin();
            ite = p1.paths.erase(ite);
            p1.paths.push_back(pc);
        }
        else
            break;
    }
    lastpoint = (*(p2.paths.begin())).GetBegin();
    for(std::list <PointConnection>::reverse_iterator ite = p2.paths.rbegin();ite!=p2.paths.rend();)
    {
        if((*ite).GetEnd()!=Random)
        {
            uint32_t b = (*ite).GetBegin(),e = (*ite).GetEnd();
            PointConnection pc((*ite).GetEnd(),lastpoint);
            lastpoint = (*ite).GetEnd();
            p2.paths.erase(--((ite).base()));
            p2.paths.push_front(pc);
        }
        else
            break;
    }
    Path child;
    std::list<uint32_t> Repeated;
    std::list <PointConnection>::iterator ite1 = p1.paths.begin();
    std::list <PointConnection>::reverse_iterator ite2 = p2.paths.rbegin();
    do{
        if(ite1!=p1.paths.end())
        {
            bool repeated;
            do{
                repeated = false;
                for(std::list <uint32_t>::iterator ite = Repeated.begin();ite!=Repeated.end()&&!repeated;ite++)
                {
                    if((*ite1).GetEnd()==(*ite))
                        repeated = true;
                }
                if(repeated)
                    ite1++;
            }while(repeated&&ite1!=p1.paths.end());
            if(!repeated)
            {
                PointConnection pc(right,(*ite1).GetEnd());
                child.paths.push_back(pc);
                right = (*ite1).GetEnd();
                Repeated.push_back(right);
            }
        }
        if(ite2!=p2.paths.rend())
        {
            bool repeated;
            do{
                repeated = false;
                for(std::list <uint32_t>::iterator ite = Repeated.begin();ite!=Repeated.end()&&!repeated;ite++)
                {
                    if((*ite2).GetBegin()==(*ite))
                        repeated = true;
                }
                if(repeated)
                    ite2++;
            }while(repeated&&ite2!=p2.paths.rend());
            if(!repeated)
            {
                PointConnection pc((*ite2).GetBegin(),left);
                child.paths.push_front(pc);
                left = (*ite2).GetBegin();
                Repeated.push_back(left);
            }
        }
    }while(ite1!=p1.paths.end()||ite2!=p2.paths.rend());
    NewPaths.push_back(child);
}





