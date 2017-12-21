/*main.cpp*/

//
// Auto-complete using a binary search tree.
//
// << Zakee Jabbar (zjabba2)>>
// U. of Illinois, Chicago
// CS251, Spring 2017
// Project 03
//

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "bst.h"





//
// skipRestOfInput:
//
// Inputs and discards the remainder of the current line for the
// given input stream, including the EOL character(s).
//
void skipRestOfInput(FILE *stream)
{
  char restOfLine[256];
  int rolLength = sizeof(restOfLine) / sizeof(restOfLine[0]);

  fgets(restOfLine, rolLength, stream);
}

//
// getFileName: inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be opened, an
// error message is output and the program is exited.
//
char *getFileName()
{
  char filename[512];
  int  fnsize = sizeof(filename) / sizeof(filename[0]);

  // input filename from the keyboard:
  fgets(filename, fnsize, stdin);
  filename[strcspn(filename, "\r\n")] = '\0';  // strip EOL char(s):

  // make sure filename exists and can be opened:
  FILE *infile = fopen(filename, "r");
  if (infile == NULL)
  {
    printf("**Error: unable to open '%s'\n\n", filename);
    exit(-1);
  }

  fclose(infile);

  // duplicate and return filename:
  char *s = (char *)malloc((strlen(filename) + 1) * sizeof(char));
  strcpy(s, filename);

  return s;
}


int main()
{
  BST *tree = BSTCreate(); //creates tree
  long long weight;
  char *filename;
  char  cmd[64];
  char  text[512];
  int   textsize = sizeof(text) / sizeof(text[0]);
  char *token1;
  char *token2;

  printf("** Starting Autocomplete **\n");

  filename = getFileName();
  FILE *inputFile = fopen(filename, "r");


  fgets(text, textsize, inputFile);

  //creates the tree
  while(!feof(inputFile))
  {
      text[strcspn(text, "\r\n")] = '\0';  // strip EOL char(s):

      token1 = strtok(text, "\t");
      weight = atoll(token1);

      token2 = strtok(NULL, "\n");

      BSTInsert(tree, token2, weight);


      fgets(text, textsize, inputFile);

  }

  printf("** Ready **\n");

  scanf("%s", cmd);

  while (strcmp(cmd, "exit") != 0)
  {

    if (strcmp(cmd, "stats") == 0)
    {
      printf("**Tree count:  %d\n", BSTCount(tree));
      printf("**Tree Height: %d\n", BSTHeight(tree));
    }
    else if (strcmp(cmd, "add") == 0)
    {

      long long weight;
      char      part2[512];
      int       part2size = sizeof(part2) / sizeof(part2[0]);

      //
      // add weight text
      //
      scanf("%lld %s", &weight, text);
      fgets(part2, part2size, stdin);
      part2[strcspn(part2, "\r\n")] = '\0';  // strip EOL char(s):
      strcat(text, part2);

      int n = BSTInsert(tree, text, weight);
      if(n == 1)
      {
          printf("**Added\n");
      }
      if(n == 0)
      {
          printf("**Not added...\n");
      }

    }
    else if (strcmp(cmd, "find") == 0)
    {
      char part2[512];
      int  part2size = sizeof(part2) / sizeof(part2[0]);

      //
      // find text
      //
      scanf("%s", text);
      fgets(part2, part2size, stdin);
      part2[strcspn(part2, "\r\n")] = '\0';  // strip EOL char(s):
      strcat(text, part2);

      BSTNode *n = BSTSearch(tree, text);
      if(n != NULL)
      {
          printf("%s: %lld\n", n->Key, n->Value.weight);
      }
      else
      {
          printf("**Not found...\n");
      }

    }
    else if (strcmp(cmd, "suggest") == 0)
    {
      int  k;
      char part2[512];
      int  part2size = sizeof(part2) / sizeof(part2[0]);

      //
      // suggest k text
      //
      scanf("%d %s", &k, text);
      fgets(part2, part2size, stdin);
      part2[strcspn(part2, "\r\n")] = '\0';  // strip EOL char(s):
      strcat(text, part2);

      BSTNode *n = BSTPartialSearch(tree, text);
      if(n != NULL)
      {
          printf("** [Sub-tree root:  (%s,%lld)]\n", n->Key, n->Value.weight);
      }
      else if(n == NULL)
      {
          printf("**No suggestions...\n");
          scanf("%s", cmd);
          continue;

      }

      int totalCount = BSTNodeCount(n);
      printf("** [Sub-tree count: %d]\n", totalCount);

      int matches = 0;
      BSTValue *values = Matches(n, totalCount,text, &matches);
      printf("** [Num matches:    %d]\n", matches);
      int printValue = matches;
      if(matches > k) //if there are more matches than k it will only print out k matches
      {
          printValue = k;
      }


      //sorts the array in descending order
      int i, j;
      for(i = 0; i < matches - 1; i++)
      {
          int biggest = i;

        for (j = i+1; j < matches; j++)
        {
            if(values[j].weight > values[biggest].weight)
            {

                biggest = j;
            }
            else if(values[j].weight == values[biggest].weight)
            {
                if(strcmp(values[j].key, values[biggest].key) < 0)
                {
                    biggest = j;
                }
            }
        }

        BSTValue T = values[i];  //swaps the values
        values[i] = values[biggest];
        values[biggest] = T;
      }

      //prints the values in the sorted array based on k and matches
      for(i = 0; i < printValue; i++)
      {
          printf("%s: %lld\n", values[i].key, values[i].weight);
      }

    }
    else
    {
      printf("**unknown cmd, try again...\n");
    }

    scanf("%s", cmd);
  }

  printf("** Done **\n");

  return 0;
}

