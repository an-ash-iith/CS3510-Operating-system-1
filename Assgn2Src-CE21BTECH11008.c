#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

int store[100][10000];

typedef struct
{
    int first;
    int second;
    int third;
} arguments;

FILE *fptr;

// in write mode when we try to open the file then it will not give give error even though the file is not there
// then it will get created and then gets open while in read mode it was not like that...

//  fptr = fopen("log_file.txt","w");

int digit_count(int n)
{
    int count = 0;

    while (n > 0)
    {
        count++;
        n = n / 10;
    }

    return count;
}

bool check_prime(int num)
{
    //  int p = (int)sqrt((double) num);

    double p = sqrt(num);
    for (int i = 2; i <= (int)p; i++)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

void swap_fun(char *a, char *b)
{
    char x;
    x = *b;
    *b = *a;
    *a = x;
}

bool check_vampire(char *arr, char *ans, int n, int thread_no)
{
    // we will take those n when the n will be even
    //  printf("%s \n",arr);

    char part1[n / 2 + 1];
    char part2[n / 2 + 1];

    for (int i = 0; i < n / 2; i++)
    {
        part1[i] = arr[i];
        part2[i] = arr[n/2 + i];
    }
 
//  if(part1[n/2-1]=='0' && part2[n/2-1]=='0')
//  {
//     return false;
//  }
    //    printf("check 2 %d %d %d \n",atoi(part1),atoi(part2),atoi(ans));

    if (atoi(part1) * atoi(part2) == atoi(ans))
    {
        fprintf(fptr, "%s: Found by Thread %d\n", ans,thread_no+1);
        
        // fprintf()
        return true;
    }
    else
        return false;
}


bool permutate(char *arr, int n, int k, char *ans,int thread_no)
{

    // base case to end
    if (k == n)
    {
        // printing the string
        //  printf("%s \n",arr);

        if (check_vampire(arr, ans, n,thread_no) == true)

        {
            printf("Found y \n ");
            return true;
        }

        else
            return false;
    }

    for (int i = k; i < n; i++)
    {
        // ans[k]=arr[i];

        swap_fun(&arr[k], &arr[i]);

        if (permutate(arr, n, k + 1, ans,thread_no) == true)
        {
            return true;
        }
    
        else
            swap_fun(&arr[k], &arr[i]);
    }
    return false;
    // printf(" Hi ");
}


void *find_vampire_no(void *args)
{

    // printf("Enter into find_vampire_no \n");
    arguments *threadArgs = (arguments *)args;

    int N = threadArgs->first;
    int M = threadArgs->second;
    int thread_no= threadArgs->third;

    int size= N/M;

    for (int i = 1; i < size; i++)
    { 
        // printf(" %d ",store[thread_no][i]);
        int digit = digit_count(store[thread_no][i]);
        //  printf("%d \n",digit);

        if (digit % 2 != 0 || digit == 2)
        {
            continue;
        }
    
        if (check_prime(store[thread_no][i]) == true)
        {
            continue;
        }

        char *arr = (char *)malloc((digit + 1) * sizeof(char));
        char *ans = (char *)malloc((digit + 1) * sizeof(char));
  
//   pthread_mutex_lock(&mutex);
        sprintf(arr, "%d", store[thread_no][i]);
//   pthread_mutex_unlock(&mutex);
        strcpy(ans, arr);


        permutate(arr, digit, 0, ans,thread_no);

        free(ans);
        free(arr);

    
    }
    // free(threadArgs);
    // pthread_exit(NULL);
}

void find_vampire(int N, int M)
{
    // printf("Enter into find_vampire \n");

    pthread_t threads[M];
    arguments args;

    args.first = N;
    args.second = M;
    
// printf("%d , %d ",N,M);
    for (int i = 0; i < M; i++)
    {
        args.third= i;
        pthread_create(&threads[i], NULL, find_vampire_no, (void *)&args);
    }


      for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }
}



int main()
{

    fptr = fopen("OutFile.txt", "w");

    FILE *fptr_op;

    int N;
    int M;

    fptr_op = fopen("read.txt", "r");

    FILE *fptr;
    fptr = fopen("read.txt", "r");

    if (fptr != NULL)
    {
        printf("File handling is successfull \n");

        char data[100];

        fgets(data, 100, fptr);

        bool first = true;
        bool second = false;

        char arr1[20];
        char arr2[20];
        int i = 0;
        int j = 0;

        while (data[i] != '\0')
        {
            if (data[i] == ' ')
            {
                first = false;
            }

            if (first == true)

            {
                arr1[i] = data[i];
            }

            if (second == true)
            {
                arr2[j] = data[i];
                j++;
            }

            if (data[i] == ' ')
            {
                second = true;
            }
            i++;
        }

        N = atoi(arr1);
        M = atoi(arr2);
    }
    else

    {
        printf("File handling is unsuccessfull \n");
    }

    clock_t initial = clock();

    int numbersPerThread= N/M;
         int arr[M];

         for (int i = 0; i < M; i++)
         {
            arr[i]=0;
         }
         
    

        for (int j = 1; j <= N; j++)
        {
            arr[j%M]++;
        store[j%M][arr[j%M]] = j;

        }

//  pthread_mutex_init(&mutex, NULL);

    // calling the function
    find_vampire(N, M);

    fclose(fptr);
    fclose(fptr_op);
    // pthread_mutex_destroy(&mutex);
    // Record the ending time
    clock_t final = clock();

    // Calculate the elapsed time in seconds
    double time = ((double)(final - initial)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", time);

    return 0;
}