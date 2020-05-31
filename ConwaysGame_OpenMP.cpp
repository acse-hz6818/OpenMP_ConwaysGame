#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <omp.h>
using namespace std;

// CID: hz6818
// Email: hz6818@ic.ac.uk
// Compiler used: VSCode
// Operating System: MacOS
// The number of logical processors of my computer is: 4

//the average serial execution time : 0.247ms
//the average parallel execution time ran by 2 threads is: 0.124ms
//the average parallel execution time ran by 3 threads is: 0.095ms
//the average parallel execution time ran by 4 threads is: 0.081ms

//Therefore, the speedup ration for 2 threads: 1.99
//Therefore, the speedup ration for 3 threads: 2.60
//Therefore, the speedup ration for 4 threads: 3.05


//Note that this is a serial implementation with a periodic grid
vector< vector<bool> > grid, new_grid;
int imax, jmax;
int max_steps = 100;
int *localn,*ibegin;
int num_neighbours(int ii, int jj)
{
    int ix, jx;
    int cnt = 0;
    //No "omp parallel for" is needed here. 
    //It will increase the computational burden of each thread
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (i != 0 || j != 0)
            {
                ix = (i + ii + imax) % imax;
                jx = (j + jj + jmax) % jmax;
                if (grid[ix][jx]) cnt++;
            }
    return cnt;
}

void grid_to_file(int it)
{
    stringstream fname;
    fstream f1;
    fname << "output" << "_" << it << ".dat";
    f1.open(fname.str().c_str(), ios_base::out);
    for (int i = 0; i < imax; i++)
    {
        for (int j = 0; j < jmax; j++)
            f1 << grid[i][j] << "\t";
        f1 << endl;
    }
    f1.close();
}

void do_iteration(void)
{
    #pragma omp parallel
    {
        //Use omp parallel create parallel block. Obtain the thread id and start parallel computing each part
        int tid=omp_get_thread_num();
        for (int i = ibegin[tid]; i < ibegin[tid]+localn[tid]; i++)
        {
            for (int j = 0; j < jmax; j++)
            {
                new_grid[i][j] = grid[i][j];
                int num_n = num_neighbours(i, j);
                if (grid[i][j])
                {
                    if (num_n != 2 && num_n != 3)
                        new_grid[i][j] = false;
                }
                else if (num_n == 3) new_grid[i][j] = true;
            }
        }
    }

    grid.swap(new_grid);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    imax = 100;
    jmax = 100;
    grid.resize(imax, vector<bool>(jmax));
    new_grid.resize(imax, vector<bool>(jmax));

    //set an initial random collection of points - You could set an initial pattern
    #pragma omp parallel for
    for (int i = 0; i < imax; i++)
        for (int j = 0; j < jmax; j++) grid[i][j] = (rand() % 2);

    double ts,te;
    double t=0;

    //set numthreads
    int nthreads=atoi(argv[1]);
    omp_set_num_threads(nthreads);


    //calculates where each thread starts and how many thread need to compute
    localn=new int[nthreads];
    ibegin=new int[nthreads];

    int ave=imax/nthreads;
    for(int i=0;i<nthreads;i++)
    {
        if(i<imax%nthreads)
            localn[i]=ave+1;
        else
            localn[i]=ave;
    }

    ibegin[0]=0;
    for(int i=1;i<nthreads;i++)
    {
        ibegin[i]=ibegin[0]+localn[i-1];
    }


    for (int n = 0; n < max_steps; n++)
    {
        cout << "it: " << n << endl;
        //start time clock
        ts=omp_get_wtime();
        do_iteration();
        //end time clock
        te=omp_get_wtime();
        t+=te-ts;

        //Here, the function of to_file cannot be converted to parallel computing
        //Therefore, the total calculation time does not contain this function.
        grid_to_file(n);
    }
    printf("Finished calculate processing in %.3f ms.\n", t);

    delete []localn;
    delete []ibegin;
    return 0;
}
