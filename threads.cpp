#include <omp.h>

int num_threads()
{
    int n_threads;
#pragma omp parallel
    {
#pragma omp master
        {
            n_threads = omp_get_num_threads();
        }
    }
    return n_threads > 0 ? n_threads : -1;
}
