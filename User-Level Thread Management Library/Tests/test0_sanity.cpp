/**
 * test0_sanity.coo - The test initializes a user-level threading library, starts a main thread (`m`), and spawns
 * two additional threads (`f` and `g`) after three quantums.
 * Each thread runs for a fixed number of quantums, prints its progress, and terminates after five quantums,with the
 * main thread terminating after ten quantums.
 *
 * Output should be:
 * ----------------------
 * Thread:m Number:(0) 0
 * Init Quantum num is: 1
 * m0 Quanta:1
 * m0 Quanta:2
 * m0 Quanta:3
 * m spawns f at (1) 1
 * m spawns g at (2) 2
 * f1 Quanta:1
 * g2 Quanta:1
 * m0 Quanta:4
 * f1 Quanta:2
 * g2 Quanta:2
 * m0 Quanta:5
 * f1 Quanta:3
 * g2 Quanta:3
 * m0 Quanta:6
 * f1 Quanta:4
 * g2 Quanta:4
 * m0 Quanta:7
 * f1 Quanta:5
 * f END
 * g2 Quanta:5
 * g END
 * m0 Quanta:8
 * m0 Quanta:9
 * m0 Quanta:10
 * Total Quantums: 20
 *
 */

#include "uthreads.h"
#include <iostream>

void f (void)
{
	int tid = uthread_get_tid();
	int i = 1;
	while(1)
	{
		if(i == uthread_get_quantums(tid))
		{
			std::cout << "f" << tid << " Quanta:" <<  i << std::endl;
			if (i == 5)
			{
				std::cout << "f END" << std::endl;
				uthread_terminate(tid);
			}
			i++;
		}

	}
}

void g (void)
{
	int tid = uthread_get_tid();
	int i = 1;
	while(1)
	{
		if(i == uthread_get_quantums(tid))
		{
			std::cout << "g" << tid << " Quanta:" <<  i << std::endl;
			if (i == 5)
			{
				std::cout << "g END" << std::endl;
				uthread_terminate(tid);
			}
			i++;
		}
	}
}


int main(void)
{
	try
	{
		uthread_init(1000000);
		int tid = uthread_get_tid();
		int i = 1;
		std::cout << "Thread:m Number:(0) " << tid << std::endl;
		std::cout << "Init Quantum num is: " << uthread_get_total_quantums() << std::endl;
		while(1)
		{
			if(i == uthread_get_quantums(tid))
			{
				std::cout << "m" << tid << " Quanta:" <<  i << std::endl;
				if (i == 3)
				{
					std::cout << "m spawns f at (1) " << uthread_spawn(f) << std::endl;
					std::cout << "m spawns g at (2) " << uthread_spawn(g) << std::endl;
				}
				if (i == 10)
				{
					std::cout << "Total Quantums: " << uthread_get_total_quantums() << std::endl;
					uthread_terminate(tid);
				}
				i++;
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cout << "Caught The Following Excpetion: \n" << e.what() << std::endl;
	}

}
