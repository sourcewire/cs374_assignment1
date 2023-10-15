#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* struct for movie info*/
struct movie
{
    char* title;
    int year;
    char* languages;
    double rating_value;
    struct movie *next;
};

/*parse current line comma delimited and create movie struct with data in this line*/
struct movie *createMovie(char* currLine)//take currLine string as input and return pointer to student struct
{
    struct movie *currMovie = malloc(sizeof(struct movie));//create new movie of size movie struct

    //for use with strtok_r
    char* saveptr;

    //the first token is the title
    char* token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);//copy title from token to struct

    //second token is the year
    //help from chat gpt
    token = strtok_r(NULL, ",", &saveptr);
    if (token != NULL)
    {
        int year = atol(token);//convert token to integer

        //check if conversion was successful
        if (year != 0)
        {
            currMovie->year = year;
        }
    }

    //third token is the languages
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);//is a list, will need to edit

    //the last token is the rating value
    //help from chat gpt
    token = strtok_r(NULL, ",", &saveptr);
    if(token != NULL)
    {
        //convert token to double
        double rating_value = atof(token);

        if(rating_value != 0.0)
        {
            currMovie->rating_value = rating_value;
        }
    }


    //set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;//return pointer to current movie

}

/*return linked list of movies by parsing data from each line
of the file*/
struct movie *processFile(char* filePath)
{
    //open file for read only
    FILE *movieFile = fopen(filePath, "r");

    char* currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char* token;

    //head of linked list
    struct movie *head = NULL;
    //tail of linked list
    struct movie *tail = NULL;

    //read file line by line
    while((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        //get a new movie node corresponding to the current line
        struct movie *newNode = createMovie(currLine);

        //if this is the first node...
        if (head == NULL)
        {
            //set head and tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            //this is not the first node
            //add this node to list and advance the tail
            tail->next = newNode;
            tail = newNode;

        }
    }
    free(currLine);
    fclose(movieFile);
    return head;
}

//print data for a given movie
void printMovie(struct movie* aMovie)
{
    printf("%s, %d, %s, %lf, \n",
    aMovie->title,
    aMovie->year,
    aMovie->languages,
    aMovie->rating_value
    );
}

//print the linked list of movies
void printMovieList(struct movie *list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample_1.csv\n");
        return 1;
    }
    printf("hi im compiling\n");
    struct movie *list = processFile(argv[1]);
    printMovieList(list);
    

    return 0;
}
