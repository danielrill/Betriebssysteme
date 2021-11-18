

//Header Files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_LINE 1024

int array[MAX_LINE / 2];
int page_faults = 0;

//Initializes an int array with 9999
//Parameters - working size frame
//Returns - Nothing
void initialize_check(int working_set_size)
{
  page_faults = 0;
  int j = 0;
  for (j = 0; j < working_set_size; j++)
  {
    array[j] = 9999;
  }
}

//Checks if data is in the array for FIFO,Optimal,LRU
//Parameters: data, working size frame
//Returns: 1 if data is present
int check_present(int check, int working_set_size)
{
  int present = 0;
  int k = 0;
  while (k < working_set_size)
  {
    if (array[k] != check)
    {
    }
    else
    {
      present = 1;
      break;
    }
    k++;
  }
  return present;
}

//Checks if page is in the array for MFU
//Parameters: array, working size frame, data, counter
//Returns: true if  found otherwise false
bool page_found(int pages[], int working_set_size, int page_search, int *counter)
{
  int j = 0;
  for (j = 0; j < working_set_size; j++)
  {
    if (pages[j] == page_search)
    {
      *counter = j;
      return true;
    }
  }
  *counter = -1;
  return false;
}

//Checks if the page is blank for MFU
//Parameters: array, working size frame
int page_blank(int pages[], int working_set_size)
{
  int j = 0;
  for (j = 0; j < working_set_size; j++)
  {
    if (pages[j] == -1)
    {
      return 1;
    }
  }
  return 0;
}

//Implements First In First Out page fault algorithm
//Paramters: array of page requests, working size frame, length of the array
//Returns: Page faults in FIFO
int FIFO_pagefault(int pages[], int working_set_size, int length)
{
  //Initialize the array
  initialize_check(working_set_size);
  int i, j = 0;
  for (i = 0; i < length; i++)
  {
    //Finds and executes only if there is a page fault
    if (check_present(pages[i], working_set_size) == 0)
    {
      for (j = 0; j < working_set_size - 1; j++)
      {
        array[j] = array[j + 1];
      }
      array[j] = pages[i];
      page_faults = page_faults + 1;
    }
  }
  return page_faults;
}

//Implements Optimal page fault algorithm
//Parameters: array of page requests, working size frame, length of the array
//Returns: Page Faults in Optimal
int optimal_pagefault(int pages[], int working_set_size, int length)
{
  int i, j, k = 0;
  int close[MAX_LINE / 2];
  //Initialize the array
  initialize_check(working_set_size);
  while (k < length)
  {
    //Finds and executes only if there is a page fault
    if (check_present(pages[k], working_set_size) == 0)
    {
      for (i = 0; i < working_set_size; i++)
      {
        int find = 0;
        int page = array[i];
        j = k;
        while (j < length)
        {
          if (page == pages[j])
          {
            find = 1;
            close[i] = j;
            break;
          }
          else
          {
            find = 0;
          }
          j++;
        }
        if (!find)
        {
          close[i] = 9999;
        }
      }
      int maximum = -9999;
      int repeated;
      i = 0;
      while (i < working_set_size)
      {
        if (maximum < close[i])
        {
          repeated = i;
          maximum = close[i];
        }
        i++;
      }
      array[repeated] = pages[k];
      page_faults = page_faults + 1;
    }
    k++;
  }
  return page_faults;
}

//Implements Least Recently Found page fault algorithm
//Parameters: array of page requests, working size frame, length of the array
//Returns: Page Faults in LRU
int LRU_pagefault(int pages[], int working_set_size, int length)
{
  int i, j, k = 0;
  int close[MAX_LINE / 2];
  //Initialize the array
  initialize_check(working_set_size);
  while (k < length)
  {
    //Finds and executes only if there is a page fault
    if (check_present(pages[k], working_set_size) == 0)
    {
      for (i = 0; i < working_set_size; i++)
      {
        int find = 0;
        int page = array[i];
        j = k - 1;
        while (j >= 0)
        {
          if (page == pages[j])
          {
            find = 1;
            close[i] = j;
            break;
          }
          else
          {
            find = 0;
          }
          j--;
        }
        if (!find)
        {
          close[i] = -9999;
        }
      }
      int least = 9999;
      int repeated;
      i = 0;
      while (i < working_set_size)
      {
        if (close[i] < least)
        {
          repeated = i;
          least = close[i];
        }
        i++;
      }
      array[repeated] = pages[k];
      page_faults = page_faults + 1;
    }
    k++;
  }
  return page_faults;
}

//Implements Most Frequently Used  page fault algorithm
//Parameters: char array of page requests, working size frame
//Returns: Page Faults in MFU
int MFU_pagefault(int working_set_size, char copy_forMFU[])
{
  page_faults = 0;
  int pages[working_set_size];
  int array_copy[MAX_LINE];
  int pages_copy[working_set_size];
  int page_counter = 0;
  int prev_counter = 0;
  int final_counter = 0;
  int counter = 0;

  //Initialize the array
  int i = 0;
  for (i = 0; i < MAX_LINE; i++)
  {
    array_copy[i] = -1;
  }
  for (i = 0; i < working_set_size; i++)
  {
    pages_copy[i] = 0;
    pages[i] = -1;
  }

  int length = 0;
  char *token = strtok(copy_forMFU, " ");

  //Convert char array to integer array
  while (token != NULL)
  {
    array_copy[length] = atoi(token);
    token = strtok(NULL, " ");
    length++;
  }

  int check_pages = page_blank(pages, working_set_size);

  //Fill up the working set
  //Determine which page was used recently
  while (check_pages == 1)
  {
    if (!page_found(pages, working_set_size, array_copy[page_counter], &counter))
    {
      pages[prev_counter] = array_copy[page_counter];
      pages_copy[prev_counter]++;
      page_faults++;
      prev_counter++;
    }
    else if (page_found(pages, working_set_size, array_copy[page_counter], &counter))
    {
      pages_copy[counter]++;
    }
    page_counter++;
    check_pages = page_blank(pages, working_set_size);
  }

  //Do nothing if we find the value in the working set
  for (final_counter = page_counter; final_counter < length; final_counter++)
  {
    if (page_found(pages, working_set_size, array_copy[final_counter], &counter))
    {
      pages_copy[counter]++;
      continue;
    }

    //Remove least recently used from the array set, if there is no page
    int max_occur = pages_copy[0];
    int k = 0;
    counter = k;
    for (k = 1; k < working_set_size; k++)
    {
      if (max_occur < pages_copy[k])
      {
        max_occur = pages_copy[k];
        counter = k;
      }
    }
    max_occur = counter;
    pages[max_occur] = array_copy[final_counter];
    pages_copy[max_occur] = 1;
    page_faults++;
  }
  return page_faults;
}

int main(int argc, char *argv[])
{
  int working_set_size = 0;
  char *line = NULL;
  size_t line_length = MAX_LINE;
  char *filename;

  FILE *file;

  if (argc < 2)
  {
    printf("Error: You must provide a checkfile as an argument.\n");
    printf("Example: ./fp checkfile.txt\n");
    exit(EXIT_FAILURE);
  }

  filename = (char *)malloc(strlen(argv[1]) + 1);
  line = (char *)malloc(MAX_LINE);

  memset(filename, 0, strlen(argv[1] + 1));
  strncpy(filename, argv[1], strlen(argv[1]));

  printf("\nOpening file %s ...\n", filename);
  file = fopen(filename, "r");

  if (file)
  {
    int pages[MAX_LINE];
    int length = 0;
    char copy_forMFU[MAX_LINE];
    char *line2 = (char *)malloc(MAX_LINE);

    //Read in from the file
    while (fgets(line, line_length, file))
    {
      char *token;
      char *token2;

      strcpy(line2, line);
      token2 = strtok(line2, " ");
      token2 = strtok(NULL, "\r\n");
      strcpy(copy_forMFU, token2);

      token = strtok(line, " ");
      working_set_size = atoi(token);

      printf("\nWorking set size: %d\n\n", working_set_size);

      //Changes the character array into the integer array
      while (token != NULL)
      {
        token = strtok(NULL, " ");
        if (token != NULL)
        {
          pages[length] = atoi(token);
          length++;
        }
      }
      //Prints the coressponding page faults in correspnding algorithm
      printf("Page faults of FIFO: %5d\n", FIFO_pagefault(pages, working_set_size, length));
      printf("Page faults of LRU: %6d\n", LRU_pagefault(pages, working_set_size, length));
      printf("Page faults of MFU: %6d\n", MFU_pagefault(working_set_size, copy_forMFU));
      printf("Page faults of Optimal: %2d\n", optimal_pagefault(pages, working_set_size, length));
      length = 0;
      printf("\n");
    }
    free(line);
    fclose(file);
  }
  else
  {
    perror("Couldnt open file :(  ....\n");
  }
  return 0;
}


