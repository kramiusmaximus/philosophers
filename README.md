#Philosophers

##Introduction
One or more philosophers are sitting at a round table doing one of three things:
eating, thinking, or sleeping.  The philosophers sit at a circular table with a large bowl of spaghetti in the center. There are some forks on the table. As spaghetti is difficult to serve and eat with a single fork, it is assumed that a
philosopher must eat with two forks, one for each hand. The philosophers must never be starving. Every philosopher needs to eat. Philosophers don’t know when another philosopher is about to die. Each time a philosopher has finished eating, he will drop his forks and start sleeping. When a philosopher is done sleeping, he will start thinking. The simulation stops when a philosopher dies. 

##Project Requirements
1. Each program should have the same options:
   - number_of_philosophers - Number of Philosophers sitting at table, as well as the number of forks available
   - time_to_die -  is in milliseconds, if a philosopher doesn’t start eating ’time_to_die’
     milliseconds after starting his last meal or the beginning of the simulation, it
     dies
   - time_to_eat - Time it takes for a philosopher to finish eating
   - time_to_sleep - Time it takes for a philosopher to finish sleeping
   - number_of_times_each_philosopher_must_eat (Optional) - argument is optional, if all
     philosophers eat at least ’number_of_times_each_philosopher_must_eat’ the
     simulation will stop. If not specified, the simulation will stop only at the death
     of a philosopher

2. Each philosopher should be given a number from 1 to ’number_of_philosophers’
3. Philosopher number 1 is next to philosopher number ’number_of_philosophers’.
   Any other philosopher with the number N is seated between philosopher N - 1 and
   philosopher N + 1
4. Any change of status of a philosopher must be written as follows (with X replaced
   with the philosopher number and timestamp_in_ms the current timestamp in milliseconds)
   - timestamp_in_ms X has taken a fork
   - timestamp_in_ms X is eating
   - timestamp_in_ms X is sleeping
   - timestamp_in_ms X is thinking
   - timestamp_in_ms X died
5. You can’t have more than 10 ms between the death of a philosopher and when it
   will print its death.
6. One fork between each philosopher, therefore if they are multiple philosophers, there
   will be a fork at the right and the left of each philosopher.
7. To avoid philosophers duplicating forks, you should protect the forks state with a
   mutex for each of them.
8. Each philosopher should be a thread.

##Installation

##Usage

##Useful Stuff
- deadlock
- mutex
- http://www.stolyarov.info/books/pdf/progintro_e2v2.pdf 553 pg.
- https://leetcode.com/problems/the-dining-philosophers/
- gary explains dining philosophers
- usleep is inaccurate. Need own function.
