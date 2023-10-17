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

int count = -1;// set global variable count to count lines in the file
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
        count = count + 1;//add 1 for each line in the file

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
void printMovieList(struct movie *list)//*list is a pointer to the first node in the linked list
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

//print a movie title
void printMovieTitle(struct movie* aMovie)
{
    printf("%s, \n", aMovie->title);
}

//print all movies that match the users year
//help from chat gpt with the found flag
void printTitleList(struct movie *list, int year)
{
    int found = 0;//flag to indicate if match was found
    while(list != NULL)
    {
        if(list->year == year)
        {
        printMovieTitle(list);
        found = 1;//movie was found
        } 

        list = list->next;
    }
    if(!found)
    {
        printf("No movies were made in this year\n");

    }
}

//function for printing info based on user chosen language
void choseLang(struct movie *list, char* language)
{
    while(list != NULL)
    {
        if(list->languages == language)
        {
            printf("%s, %d\n", list->title, list->year);
        }
        list = list->next;
    }
}

//function for printing user questions
void askQuestions()
{
    //show user options
    printf("\n");
    printf("1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n\n");
}

/* function for for returning user requested data*/
int userChoice(struct movie *list)
{
    askQuestions();
    int userNum;//variable to store user number
    printf("Choose an option from 1-4: ");
    scanf("%d", &userNum);//save user numner in userNum
    while(userNum <= 4)
    {
        if(userNum == 1)
        {
            int year;
            printf("Enter the year you would like to search for movies: ");
            scanf("%d", &year);
           
            /*call printTitleList with the LL head and user chosen year to return
            all movies made in that year, and error message if none found*/
            printTitleList(list, year);
            askQuestions();//ask questions again
            printf("Choose an option from 1-4: ");
            scanf("%d", &userNum);//get user num and continue in loop
        }
        if(userNum == 2)
        {

        }
        if(userNum == 3)
        {
            char* language;
            printf("Enter the language for which you want to see movies: ");
            scanf("%s", language);
            //printf("%s", language);
            choseLang(list, language);

        }
        if(userNum == 4)
        {
            printf("Have a nice day!\n");
            return 1;
        }
    }
}


/*Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list....*/
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample_1.csv\n");
        return 1;
    }

    struct movie *list = processFile(argv[1]);
    char* fileName = argv[1];
    //printMovieList(list);
    printf("Processed file %s and parsed data for %d movies.\n\n", fileName, count );
    //printf("hi im compiling\n");
    //printTitleList(list);
   
    userChoice(list);
    
    
    

    return 0;
}
