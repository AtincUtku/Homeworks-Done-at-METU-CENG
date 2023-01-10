#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <string.h>
#include <semaphore.h>
#include <stdio.h>
extern "C"
{
#include "hw2_output.h"
}

using namespace std;

void *private_routine(void *vargp);
void *smoker_routine(void *vargp);
void semaphore_constructor(int size_i, int size_j);
void smoking_constructor(int size_i, int size_j);
void non_smoking_constructor(int size_i, int size_j);
void order_routine(int size_i, int size_j);
struct timespec timeCalculator(struct timeval time_val, int my_time);

class asker
{
public:
    int gid;
    int si, sj;
    int time;
    vector<vector<int>> area;
};

class smoker
{
public:
    int sid;
    int time;
    vector<vector<int>> cell;
    vector<int> smoke_count;
};

class order
{
public:
    int current_order = 0; // 0 for default, 1 for break, 2 for continue, 3 for stop.
    // vector<int> break_orders;
    // vector<int> continue_orders;
    // vector<int> stop_orders;
    vector<pair<int, string>> orders; 
};

// sem_t a[65536][65536];
order ordered;
vector<asker> privates;
vector<smoker> smokers;
vector<vector<int>> grid;
vector<vector<sem_t>> sema;
vector<vector<sem_t>> smoking;
vector<vector<sem_t>> non_smoking;
pthread_cond_t break_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t continue_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t stop_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t smoker_cond = PTHREAD_COND_INITIALIZER;
// pthread_mutex_t break_mutex;
// pthread_mutex_t continue_mutex;
// pthread_mutex_t stop_mutex;
vector<vector<pthread_cond_t>> smoking_zone;
vector<vector<pthread_cond_t>> non_smoking_zone;
pthread_mutex_t part3_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t timed_wait_mutex = PTHREAD_MUTEX_INITIALIZER;

vector<vector<int>> smoker_count;
vector<vector<int>> private_count;
vector<vector<int>> smoker_wait;
vector<vector<int>> private_wait;




int main()
{

    hw2_init_notifier();
    // input for part 1

    int grid_i, grid_j;
    int control = 0;

    cin >> grid_i;
    cin >> grid_j;
    vector<int> temp;


    for (int i = 0; i < grid_i; i++)
    {
        for (int j = 0; j < grid_j; j++)
        {

            int cig;
            cin >> cig;
            temp.push_back(cig);
        }

        grid.push_back(temp);
        temp.clear();
    }

    int no_of_privates;
    cin >> no_of_privates;

    for (int i = 0; i < no_of_privates; i++)
    {
        asker soldier;
        int no_of_lines;

        cin >> soldier.gid;
        cin >> soldier.si;
        cin >> soldier.sj;
        cin >> soldier.time;
        cin >> no_of_lines;

        for (int j = 0; j < no_of_lines; j++)
        {
            vector<int> temp_soldier;

            int i_axis_temp, j_axis_temp;
            cin >> i_axis_temp;
            cin >> j_axis_temp;

            temp_soldier.push_back(i_axis_temp);
            temp_soldier.push_back(j_axis_temp);
            soldier.area.push_back(temp_soldier);
        }

        privates.push_back(soldier);
    }

    // input for part 2

    int number_of_orders;
    int temp_no_of_orders;

    // while (cin >> temp_no_of_orders)
    // {
    //     control = 1;
    //     number_of_orders=temp_no_of_orders;
    //     break;
    // }

    if (cin >> number_of_orders)
    {
    

        for (int i = 0; i < number_of_orders; i++)
        {
            int temp_order_time;
            string temp_order_name;
            pair<int, string> temp_pairs;

            cin >> temp_order_time;
            cin >> temp_order_name;

            temp_pairs = make_pair(temp_order_time, temp_order_name);
            ordered.orders.push_back(temp_pairs);

            
        }

        // input for part 3

        int number_of_smokers;
        int temp_no_of_smokers;

        // if (!control)
        // {
        //     while (cin >> temp_no_of_smokers)
        //     {
        //         number_of_smokers = temp_no_of_smokers;
        //         break;
        //     }
        // }

        if (cin >> number_of_smokers)
        {

            for (int i = 0; i < number_of_smokers; i++)
            {
                smoker tiryaki;
                int cell_count_tiryaki;

                cin >> tiryaki.sid;
                cin >> tiryaki.time;
                cin >> cell_count_tiryaki;

                for (int j = 0; j < cell_count_tiryaki; j++)
                {
                    vector<int> temp_tiryaki;
                    int x_tiryaki;
                    int y_tiryaki;
                    int tiryaki_smoke_count;
                    cin >> x_tiryaki;
                    cin >> y_tiryaki;
                    cin >> tiryaki_smoke_count;
                    temp_tiryaki.push_back(x_tiryaki);
                    temp_tiryaki.push_back(y_tiryaki);
                    tiryaki.cell.push_back(temp_tiryaki);
                    tiryaki.smoke_count.push_back(tiryaki_smoke_count);
                }

                smokers.push_back(tiryaki);
            }

        }

    }

    // end of input part

    smoker_count.resize(grid_i);
    private_count.resize(grid_i);
    smoker_wait.resize(grid_i);
    private_wait.resize(grid_i);
    smoking_zone.resize(grid_i);
    non_smoking_zone.resize(grid_i);
    for (int i=0; i < grid_i; i++)
    {
        smoker_count[i].resize(grid_j);
        smoker_wait[i].resize(grid_j);
        private_count[i].resize(grid_j);
        private_wait[i].resize(grid_j);
        smoking_zone[i].resize(grid_j);
        non_smoking_zone[i].resize(grid_j);

        for (int j=0; j < grid_j; j++)
        {
            smoker_count[i][j] = 0;
            smoker_wait[i][j] = 0;
            private_count[i][j] = 0;
            private_wait[i][j] = 0;
            smoking_zone[i][j] = PTHREAD_COND_INITIALIZER;
            non_smoking_zone[i][j] = PTHREAD_COND_INITIALIZER;

        }

    }


    semaphore_constructor(grid_i, grid_j);
    smoking_constructor(grid_i, grid_j);
    non_smoking_constructor(grid_i, grid_j);

    

    pthread_t private_threads[privates.size()];

    for (int i = 0; i < privates.size(); i++)
    {
        int *vargp = (int *)malloc(sizeof(*vargp));
        if (vargp == NULL)
        {
            exit(EXIT_FAILURE);
        }

        *vargp = i;


        pthread_create(&private_threads[i], NULL, private_routine, vargp);
    }

    
    pthread_t smoker_threads[smokers.size()];

    for (int i = 0; i < smokers.size(); i++)
    {
        int *vargp = (int *)malloc(sizeof(*vargp));
        if (vargp == NULL)
        {
            exit(EXIT_FAILURE);
        }

        *vargp = i;
        pthread_create(&smoker_threads[i], NULL, smoker_routine, vargp);
    }

    sort(ordered.orders.begin(), ordered.orders.end());

    

    order_routine(grid_i, grid_j);

    for (int i = 0; i < privates.size(); i++)
    {
        pthread_join(private_threads[i], NULL);
    }


    for (int i = 0; i < smokers.size(); i++)
    {
        pthread_join(smoker_threads[i], NULL);
    }

    for (int i = 0; i < grid_i; i++)
    {
        for (int j = 0; j < grid_j; j++)
        {
            sem_destroy(&sema[i][j]);
            pthread_cond_destroy(&smoking_zone[i][j]);
            pthread_cond_destroy(&non_smoking_zone[i][j]);
        }
    }


    pthread_mutex_destroy(&timed_wait_mutex);
    pthread_mutex_destroy(&part3_mutex);
    pthread_cond_destroy(&break_cond);
    pthread_cond_destroy(&continue_cond);
    pthread_cond_destroy(&smoker_cond);

    return 0;
}

void *private_routine(void *vargp)
{
    int index = *((int *)vargp);
    hw2_notify(PROPER_PRIVATE_CREATED, privates[index].gid, 0, 0);

    int gid = privates[index].gid;
    int si = privates[index].si;
    int sj = privates[index].sj;
    int my_time = privates[index].time;

    vector<vector<int>> area;

    int grid_i, grid_j;

    for (int i = 0; i < privates[index].area.size(); i++)
    {
        area.push_back(privates[index].area[i]);
    }


    int control = 0;

    // cerr<<gid<<endl;

    // semaphore kısmı


    for (int i = 0; i < area.size(); i++)
    {
        


       

        pthread_mutex_lock(&part3_mutex);

        if (ordered.current_order == 3)
        {
            hw2_notify(PROPER_PRIVATE_STOPPED, gid, 0, 0);
            pthread_mutex_unlock(&part3_mutex);
            free(vargp);
            pthread_exit(NULL);
        }

        if (ordered.current_order == 1)
        {
            hw2_notify(PROPER_PRIVATE_TOOK_BREAK, gid, 0, 0);
            pthread_cond_wait(&continue_cond, &part3_mutex);
            hw2_notify(PROPER_PRIVATE_CONTINUED, gid, 0, 0);

            if (ordered.current_order == 3)
            {
                hw2_notify(PROPER_PRIVATE_STOPPED, gid, 0, 0);
                pthread_mutex_unlock(&part3_mutex);
                free(vargp);
                pthread_exit(NULL);
            }
        }


        

        pthread_mutex_unlock(&part3_mutex);


        grid_i = area[i][0];
        grid_j = area[i][1];


        pthread_mutex_lock(&part3_mutex);

        for (int x = grid_i; x < (grid_i + si); x++)
        {
            for (int y = grid_j; y < (grid_j + sj); y++)
            {
                // sem_wait(&sema[x][y]);



                if (private_count[x][y] || smoker_count[x][y])
                {
                    ++private_wait[x][y];
                    pthread_cond_wait(&non_smoking_zone[x][y], &part3_mutex);
                    --private_wait[x][y];
                }

                if (ordered.current_order == 1)
                {
                    break;
                }

                if (ordered.current_order == 3)
                {
                    break;
                }

                private_count[x][y] = 1;

            }

            if (ordered.current_order == 1)
            {
                break;
            }

            if (ordered.current_order == 3)
            {
                break;
            }
        }

        pthread_mutex_unlock(&part3_mutex);

        if (ordered.current_order == 3)
        {
            hw2_notify(PROPER_PRIVATE_STOPPED, gid, 0, 0);
            free(vargp);
            pthread_exit(NULL);
        }

        if (ordered.current_order == 1)
        {
            i--;
            continue;
        }

        // pthread_mutex_unlock(&part3_mutex);

        hw2_notify(PROPER_PRIVATE_ARRIVED, gid, grid_i, grid_j);

                

        for (int x = grid_i; x < (grid_i + si); x++)
        {

            for (int y = grid_j; y < (grid_j + sj); y++)
            {
                while (grid[x][y] != 0)
                {
                    struct timeval time_val;
                    struct timespec ts;
                    int return_val;

                    gettimeofday(&time_val, NULL);
                    ts = timeCalculator(time_val, my_time);

                    pthread_mutex_lock(&timed_wait_mutex);
                    return_val = pthread_cond_timedwait(&break_cond, &timed_wait_mutex, &ts);

                    if (return_val == 0 || ordered.current_order == 1 || ordered.current_order == 3)
                    {
                        // this condition happens when break order occurs.

                        pthread_mutex_unlock(&timed_wait_mutex);


                        break;
                    }
                    pthread_mutex_unlock(&timed_wait_mutex);

                    // usleep(time*1000);
                    grid[x][y]--;

                    hw2_notify(PROPER_PRIVATE_GATHERED, gid, x, y);
                }



                if (ordered.current_order == 1)
                {
                    break;
                }

                if (ordered.current_order == 3)
                {
                    break;
                }

                
            }

            if (ordered.current_order == 1)
            {
                break;
            }

            if (ordered.current_order == 3)
            {
                break;
            }
        }


        if (ordered.current_order == 3)
        {
            hw2_notify(PROPER_PRIVATE_STOPPED, gid, 0, 0);
            free(vargp);
            pthread_exit(NULL);
        }

        if (ordered.current_order == 1)
        {
            i--;
            continue;
        }
        

        hw2_notify(PROPER_PRIVATE_CLEARED, gid, 0, 0);

        pthread_mutex_lock(&part3_mutex);

        for (int x = grid_i; x < (grid_i + si); x++)
        {
            for (int y = grid_j; y < (grid_j + sj); y++)
            {
                // sem_post(&sema[x][y]);

                //pthread_mutex_unlock(&part3_mutex);

                if (ordered.current_order == 1)
                {
                    
                    break;
                }

                if (ordered.current_order == 3)
                {
                    break;
                }


                private_count[x][y] = 0;
                
                if (smoker_wait[x][y])
                {
                    pthread_cond_signal(&smoking_zone[x][y]);
                }
                
                else
                {
                    pthread_cond_signal(&non_smoking_zone[x][y]);
                }

                
            }

            if (ordered.current_order == 1)
            {
                    break;
            }

            if (ordered.current_order == 3)
            {
                break;
            } 
        }

        pthread_mutex_unlock(&part3_mutex);

        if (ordered.current_order == 3)
        {
            hw2_notify(PROPER_PRIVATE_STOPPED, gid, 0, 0);
            free(vargp);
            pthread_exit(NULL);
        }

        if (ordered.current_order == 1)
        {
            i--;
            continue;
        }

        
    }

    
    hw2_notify(PROPER_PRIVATE_EXITED, gid, 0, 0);
    free(vargp);

    pthread_exit(NULL);
}

void *smoker_routine(void *vargp)
{
    int index = *((int *)vargp);
    int sid = smokers[index].sid;
    int my_time = smokers[index].time;
    vector<vector<int>> area;
    vector<int> smoke_count;

    int cell_i, cell_j;
    int control = 0;

    for (int i = 0; i < smokers[index].cell.size(); i++)
    {
        area.push_back(smokers[index].cell[i]);
    }

    for (int i=0; i< smokers[index].smoke_count.size(); i++)
    {
        smoke_count.push_back(smokers[index].smoke_count[i]);
    }

    hw2_notify(SNEAKY_SMOKER_CREATED, sid, 0, 0);


    for (int i=0; i<area.size(); i++)
    {
        cell_i = area[i][0];
        cell_j = area[i][1];

        if (ordered.current_order == 3)
        {
            hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
            free(vargp);

            pthread_exit(NULL);
        }
        

        // lock for the smokers smoking area, other smokers and proper privates cannot enter.
        sem_wait(&sema[cell_i][cell_j]);

        if (ordered.current_order == 3)
        {
            hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
        free(vargp);

            pthread_exit(NULL);
        }


        pthread_mutex_lock(&part3_mutex);

        


        for (int x = cell_i-1; x < cell_i+2; x++)
        {
            for (int y = cell_j-1; y < cell_j+2; y++)
            {
                
                if (x == cell_i && y == cell_j)
                {
                    continue;
                }

                else
                {
                    if (private_count[x][y] || private_wait[x][y])
                    {
                        smoker_wait[x][y]++;
                        pthread_cond_wait(&smoking_zone[x][y], &part3_mutex);
                        smoker_wait[x][y]--;
                        
                    }

                    if (ordered.current_order == 3)
                    {
                        hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
                        pthread_mutex_unlock(&part3_mutex);
                        free(vargp);

                        pthread_exit(NULL);
                    }

                    smoker_count[x][y]++;
                    pthread_cond_signal(&smoking_zone[x][y]);

                }
            }
        } 

        pthread_mutex_unlock(&part3_mutex);

        if (ordered.current_order == 3)
        {
            hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
            free(vargp);

			pthread_exit(NULL);
        }


        


        hw2_notify(SNEAKY_SMOKER_ARRIVED, sid, cell_i, cell_j);

        while (smoke_count[i] != 0)
        {

            struct timeval time_val;
            struct timespec ts;
            int return_val;

            gettimeofday(&time_val, NULL);
            ts.tv_sec = time(NULL) + my_time / 1000;
            ts.tv_nsec = time_val.tv_usec * 1000 + 1000 * 1000 * (my_time % 1000);
            ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
            ts.tv_nsec %= (1000 * 1000 * 1000);
            pthread_mutex_lock(&timed_wait_mutex);
            return_val = pthread_cond_timedwait(&smoker_cond, &timed_wait_mutex, &ts);
            if (return_val == 0 || ordered.current_order == 3)
            {
                        // this condition happens when break order occurs.
                pthread_mutex_unlock(&timed_wait_mutex);
                hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
                free(vargp);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&timed_wait_mutex);            
            
            
            if (ordered.current_order == 3)
            {
                hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
                free(vargp);
				pthread_exit(NULL);
            }
            
            // usleep(time*1000);
            grid[cell_i-1][cell_j-1]++;
            smoke_count[i]--;
            hw2_notify(SNEAKY_SMOKER_FLICKED, sid, cell_i-1, cell_j-1);

            if (smoke_count[i] == 0)
            {
                break;
            }

            gettimeofday(&time_val, NULL);
            ts.tv_sec = time(NULL) + my_time / 1000;
            ts.tv_nsec = time_val.tv_usec * 1000 + 1000 * 1000 * (my_time % 1000);
            ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
            ts.tv_nsec %= (1000 * 1000 * 1000);
            pthread_mutex_lock(&timed_wait_mutex);
            return_val = pthread_cond_timedwait(&smoker_cond, &timed_wait_mutex, &ts);
            if (return_val == 0 || ordered.current_order == 3)
            {
                        // this condition happens when break order occurs.
                pthread_mutex_unlock(&timed_wait_mutex);
                hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
                free(vargp);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&timed_wait_mutex);   

            // usleep(time*1000);
            grid[cell_i-1][cell_j]++;
            smoke_count[i]--;
            hw2_notify(SNEAKY_SMOKER_FLICKED, sid, cell_i-1, cell_j);

            if (smoke_count[i] == 0)
            {
                break;
            }

            gettimeofday(&time_val, NULL);
            ts.tv_sec = time(NULL) + my_time / 1000;
            ts.tv_nsec = time_val.tv_usec * 1000 + 1000 * 1000 * (my_time % 1000);
            ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
            ts.tv_nsec %= (1000 * 1000 * 1000);
            pthread_mutex_lock(&timed_wait_mutex);
            return_val = pthread_cond_timedwait(&smoker_cond, &timed_wait_mutex, &ts);
            if (return_val == 0 || ordered.current_order == 3)
            {
                        // this condition happens when break order occurs.
                pthread_mutex_unlock(&timed_wait_mutex);
                hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
                free(vargp);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&timed_wait_mutex);   

            //usleep(time*1000);
            grid[cell_i-1][cell_j+1]++;
            smoke_count[i]--;
            hw2_notify(SNEAKY_SMOKER_FLICKED, sid, cell_i-1, cell_j+1);

            if (smoke_count[i] == 0)
            {
                break;
            }

            gettimeofday(&time_val, NULL);
            ts.tv_sec = time(NULL) + my_time / 1000;
            ts.tv_nsec = time_val.tv_usec * 1000 + 1000 * 1000 * (my_time % 1000);
            ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
            ts.tv_nsec %= (1000 * 1000 * 1000);
            pthread_mutex_lock(&timed_wait_mutex);
            return_val = pthread_cond_timedwait(&smoker_cond, &timed_wait_mutex, &ts);
            if (return_val == 0 || ordered.current_order == 3)
            {
                        // this condition happens when break order occurs.
                pthread_mutex_unlock(&timed_wait_mutex);
                hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
                free(vargp);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&timed_wait_mutex);   

            //usleep(time*1000);
            grid[cell_i][cell_j+1]++;
            smoke_count[i]--;
            hw2_notify(SNEAKY_SMOKER_FLICKED, sid, cell_i, cell_j+1);

            if (smoke_count[i] == 0)
            {
                break;
            }

            gettimeofday(&time_val, NULL);
            ts.tv_sec = time(NULL) + my_time / 1000;
            ts.tv_nsec = time_val.tv_usec * 1000 + 1000 * 1000 * (my_time % 1000);
            ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
            ts.tv_nsec %= (1000 * 1000 * 1000);
            pthread_mutex_lock(&timed_wait_mutex);
            return_val = pthread_cond_timedwait(&smoker_cond, &timed_wait_mutex, &ts);
            if (return_val == 0 || ordered.current_order == 3)
            {
                        // this condition happens when break order occurs.
                pthread_mutex_unlock(&timed_wait_mutex);
                hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
                free(vargp);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&timed_wait_mutex);   

            // usleep(time*1000);
            grid[cell_i+1][cell_j+1]++;
            smoke_count[i]--;
            hw2_notify(SNEAKY_SMOKER_FLICKED, sid, cell_i+1, cell_j+1);

            if (smoke_count[i] == 0)
            {
                break;
            }

            gettimeofday(&time_val, NULL);
            ts.tv_sec = time(NULL) + my_time / 1000;
            ts.tv_nsec = time_val.tv_usec * 1000 + 1000 * 1000 * (my_time % 1000);
            ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
            ts.tv_nsec %= (1000 * 1000 * 1000);
            pthread_mutex_lock(&timed_wait_mutex);
            return_val = pthread_cond_timedwait(&smoker_cond, &timed_wait_mutex, &ts);
            if (return_val == 0 || ordered.current_order == 3)
            {
                        // this condition happens when break order occurs.
                pthread_mutex_unlock(&timed_wait_mutex);
                hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
                free(vargp);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&timed_wait_mutex);   

            // usleep(time*1000);
            grid[cell_i+1][cell_j]++;
            smoke_count[i]--;
            hw2_notify(SNEAKY_SMOKER_FLICKED, sid, cell_i+1, cell_j);

            if (smoke_count[i] == 0)
            {
                break;
            }

            gettimeofday(&time_val, NULL);
            ts.tv_sec = time(NULL) + my_time / 1000;
            ts.tv_nsec = time_val.tv_usec * 1000 + 1000 * 1000 * (my_time % 1000);
            ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
            ts.tv_nsec %= (1000 * 1000 * 1000);
            pthread_mutex_lock(&timed_wait_mutex);
            return_val = pthread_cond_timedwait(&smoker_cond, &timed_wait_mutex, &ts);
            if (return_val == 0 || ordered.current_order == 3)
            {
                        // this condition happens when break order occurs.
                pthread_mutex_unlock(&timed_wait_mutex);
                hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
                free(vargp);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&timed_wait_mutex);   

            //usleep(time*1000);
            grid[cell_i+1][cell_j-1]++;
            smoke_count[i]--;
            hw2_notify(SNEAKY_SMOKER_FLICKED, sid, cell_i+1, cell_j-1);

            if (smoke_count[i] == 0)
            {
                break;
            }

            gettimeofday(&time_val, NULL);
            ts.tv_sec = time(NULL) + my_time / 1000;
            ts.tv_nsec = time_val.tv_usec * 1000 + 1000 * 1000 * (my_time % 1000);
            ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
            ts.tv_nsec %= (1000 * 1000 * 1000);
            pthread_mutex_lock(&timed_wait_mutex);
            return_val = pthread_cond_timedwait(&smoker_cond, &timed_wait_mutex, &ts);
            if (return_val == 0 || ordered.current_order == 3)
            {
                        // this condition happens when break order occurs.
                pthread_mutex_unlock(&timed_wait_mutex);
                hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
                free(vargp);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&timed_wait_mutex);   


            //usleep(time*1000);
            grid[cell_i][cell_j-1]++;
            smoke_count[i]--;
            hw2_notify(SNEAKY_SMOKER_FLICKED, sid, cell_i, cell_j-1);
            

            if (smoke_count[i] == 0)
            {
                break;
            }
            
            
        }

        if (ordered.current_order == 3)
        {
            hw2_notify(SNEAKY_SMOKER_STOPPED, sid, 0, 0);
            free(vargp);
            pthread_exit(NULL);
        }

        hw2_notify(SNEAKY_SMOKER_LEFT, sid, 0, 0);

        pthread_mutex_lock(&part3_mutex);

        for (int x = cell_i-1; x < cell_i+2; x++)
        {
            for (int y = cell_j-1; y < cell_j+2; y++)
            {
                if (x == cell_i && y == cell_j)
                {
                    continue;
                }
                else
                {
                    smoker_count[x][y]--;
                    if (smoker_count[x][y] == 0)
                    {
                        pthread_cond_signal(&non_smoking_zone[x][y]);
                    }
                }
            }
        }

        pthread_mutex_unlock(&part3_mutex);

        // smoker_count--;

        // if (smoker_count == 0)
        // {

        //     for (int x = cell_i-1; x<cell_i+2; x++)
        //     {
        //         for (int y = cell_j-1; y<cell_j+2; y++)
        //         {
        //             sem_post(&non_smoking[x][y]);
        //         }
        //     }

        // }

        sem_post(&sema[cell_i][cell_j]);



        

    }

    hw2_notify(SNEAKY_SMOKER_EXITED, sid, 0, 0);
    free(vargp);

    pthread_exit(NULL);
}

void semaphore_constructor(int size_i, int size_j)
{
    sema.resize(size_i);

    for (int i = 0; i < size_i; i++)
    {
        sema[i].resize(size_j);

        for (int j = 0; j < size_j; j++)
        {
            sem_init(&sema[i][j], 0, 1);
        }
    }
}


void smoking_constructor(int size_i, int size_j)
{
    smoking.resize(size_i);

    for (int i = 0; i < size_i; i++)
    {
        smoking[i].resize(size_j);

        for (int j = 0; j < size_j; j++)
        {
            sem_init(&smoking[i][j], 0, 1);
        }
    }
}

void non_smoking_constructor(int size_i, int size_j)
{
    non_smoking.resize(size_i);

    for (int i = 0; i < size_i; i++)
    {
        non_smoking[i].resize(size_j);

        for (int j = 0; j < size_j; j++)
        {
            sem_init(&non_smoking[i][j], 0, 1);
        }
    }
}

void order_routine(int size_i, int size_j)
{
    int prev_time = 0;
    for (int i=0; i<ordered.orders.size(); i++)
    {
        if (ordered.current_order == 0)
        {
            usleep(ordered.orders[i].first * 1000);   

        }

        else if (ordered.current_order != 0)
        {
            int current_time = (ordered.orders[i].first - prev_time) * 1000;
            usleep(current_time);

        }

        prev_time = ordered.orders[i].first;
        pthread_mutex_lock(&part3_mutex);

        

        if (ordered.orders[i].second == "break")
        {
            hw2_notify(ORDER_BREAK, 0, 0, 0);
            // in case of break order
            if (ordered.current_order == 0 || ordered.current_order == 2)
            {
                ordered.current_order = 1;
                pthread_cond_broadcast(&break_cond);


                for (int x=0; x < size_i; x++)
                {
                    for (int y=0; y < size_j; y++)
                    {
                        pthread_cond_broadcast(&non_smoking_zone[x][y]);
                        pthread_cond_signal(&smoking_zone[x][y]);
                        private_count[x][y] = 0;
                    }
                }

            }

        }

        if (ordered.orders[i].second == "continue")
        {
            hw2_notify(ORDER_CONTINUE, 0, 0, 0);
            // in case of continue order
            if (ordered.current_order == 1)
            {
                // continue subroutine

                ordered.current_order = 2;
                
                    for (int x=0; x<size_i; x++)
                    {
                        for (int y=0; y<size_j; y++)
                        {
                            pthread_cond_broadcast(&continue_cond);
                            //pthread_cond_broadcast(&non_smoking_zone[x][y]);
                            
                        }
                    }
                
            }
        }

        if (ordered.orders[i].second == "stop")
        {
            ordered.current_order = 3;
            hw2_notify(ORDER_STOP, 0, 0, 0);
            // in case of stop order
            pthread_cond_broadcast(&break_cond);
            pthread_cond_broadcast(&continue_cond);
            pthread_cond_broadcast(&smoker_cond); 

            for (int x = 0; x < size_i; x++)
            {
                for (int y = 0; y < size_j; y++)
                {
                    pthread_cond_broadcast(&smoking_zone[x][y]);
                    pthread_cond_broadcast(&non_smoking_zone[x][y]);
                    sem_post(&sema[x][y]);
                    
                }
            }

            

        }
        pthread_mutex_unlock(&part3_mutex);

    }


}

struct timespec timeCalculator(struct timeval time_val, int my_time)
{
    struct timespec ts;
    ts.tv_sec = time(NULL) + my_time / 1000;
    ts.tv_nsec = time_val.tv_usec * 1000 + 1000 * 1000 * (my_time % 1000);
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);

    return ts;

}
