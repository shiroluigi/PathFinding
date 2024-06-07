#include "PathFinding.h"

int main()
{
    PathFinding p("pf");
    while (p.isRunningQuery())
    {
        p.run();
    }
}