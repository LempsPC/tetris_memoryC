/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*defines*/
#define MATRIX_X 10
#define MATRIX_Y 10

/*global variable declarations*/
char data[MATRIX_Y][MATRIX_X];

/*function prototypes*/
void printMatrix();
void initMatrix0();
int insert(char* name);
int scan();
void getMatrixFreeSpace(int *space);
void switchRows(int row1, int row2);
void sortRows(int *row_space);
void swapNumbers(int *xp, int *yp);
int help();
int getDataFromFile(char *filename, char **data);

/*return codes:
 * 3 - arguments incorrect
 * 4 - unable to open file
 * 5 - provided file is empty
 */

int main(int argc, char *argv[]) {

    /*command line parsing*/

    //check if user has entered no arguments
    if(argc != 2)
    {
        printf("Incorrect amount of arguments given to application\n");
        return help();
    }  else {int getDataFromFile();
        char fileNameToOpen[strlen(argv[1]) + 1];
        memcpy(fileNameToOpen, argv[1], strlen(argv[1]) + 1);

        /*
        printf("You have entered %d arguments\n", argc);
        for (int i = 0; i < argc; i++) {
            printf("%s\t", argv[i]);
        }
        printf("\n");
         printf("%s\n", fileNameToOpen);
        */
        char ** fileData = NULL;

        //int success = getDataFromFile(fileNameToOpen, fileData);
        //printf("%d\n", success);

        FILE * dataFile;
        char tempLine[120];
        //char ** info = NULL;
        int llen;
        int counter = 0;
        dataFile = fopen(fileNameToOpen, "r");
        //check if fopen could not open file
        if(!dataFile)
        {
            //if could not open file
            printf("Could not open file!\n");
            printf("Make sure file is in the same folder as executable, exiting application\n");
            return 4;
        }
        //checking if openef file is empty
        fseek (dataFile, 0, SEEK_END);
        int filesize = ftell(dataFile);
        if(filesize == 0)
        {
            printf("Provided file is empty, exiting application\n");
            return 5;

        }
        fseek (dataFile, 0, SEEK_SET);



        while (fgets(tempLine, 120, dataFile)) {

            if(tempLine[0] == 32 || tempLine[0] == 0 || tempLine[0] == 10)
            {
                printf("It's an empty line!\n");
            } else {
                // Allocate memory for pointer to tempLine just added
                fileData = realloc(fileData,(counter+1) * sizeof(char *));
                // And allocate memory for that tempLine itself!k

                char *firstWord = strtok(tempLine, " \t\n\f");
                printf("size of line %d is %d\n",counter,strlen(tempLine));
                llen = strlen(firstWord);
                printf("%s\n", firstWord);
                fileData[counter] = calloc(sizeof(char), llen + 1);
                // Copy the tempLine just read into that memory
                //strcpy(fileData[counter], tempLine);
                strcpy(fileData[counter], firstWord);
                //strncpy(fileData[counter], tempLine, strcspn(tempLine, " "));
                //adding end of string just in case it wont get added when copying first words
                //fileData[counter][strlen(firstWord)] = '\0';
                //printf("%s\n", fileData[counter]);

                counter++;
            }
        }

        for (int i = 0; i < counter ; i++) {
            printf("%d: %s \n",sizeof(fileData[i]),fileData[i]);
        }
        printf("\n");


        //printf("%s\n", fileData[0]);
        //int size = sizeof(fileData[0]);
        //printf(" array size: %d\n", size);


        initMatrix0();

        /*
        insert("dog");
        insert("camel");
        insert("chinchilla");
        insert("falcon");
        insert("deer");
        insert("on");
        insert("vana");
        */

        for(int i = 0; i < counter ; i++)
        {
            insert(fileData[i]);
        }

        printMatrix();

        for(int i = 0; i < counter ; i++)
        {
            free(fileData[i]);
        }
        free(fileData);

    }
    return 0;
}

//displays help "screen" and provides instructions on how to pass an argument
int help()
{
    printf("Uasge: program_name filename.txt");
    return 3;
}

//fills matrix with value of 43 indicating free space
void initMatrix0()
{
    int k = 0;
    for(int i = 0; i < MATRIX_Y; i++)
    {
        for(int j = 0; j < MATRIX_X ; j++)
        {
            //data[i][j] = k;
            data[i][j] = 43;
            k++;
        }

    }
}
//prints out matrix
void printMatrix()
{
    for(int i = 0; i < MATRIX_Y; i++)
    {
        for(int j = 0; j < MATRIX_X ; j++)
        {
            printf("%c\t" , data[i][j]);
        }
        printf("\n");
    }
}
//experimental, was used for developing
int scan()
{
    int counter = 0;

    for (int i = (MATRIX_Y - 1); i >= 0; --i) {
        for (int j = 0; j < MATRIX_X; j++) {
            printf("%d\t", data[i][j]);
            /*
            if (data[i][j] == 43) {
                //data ended
                break;
            } else{
                counter++;
            }
             */
        }
        printf("\n");
    }
    return counter;
}
//inserts word into matrix
int insert(char *name) {
    //printf("Trying to insert %s\n",name);
    if(strlen(name) > MATRIX_X)
    {
        printf("Too long\n");
        return 1; //too long word
    }



    if(data[MATRIX_Y-1][0] == 43) {
        memcpy(data[MATRIX_Y-1] , name, strlen(name));
    }
    else{
        //finding free space in matrix
        int success = 0;
        int free_space[MATRIX_Y];
        getMatrixFreeSpace(&free_space);
        /*
        for(int i = 0; i < MATRIX_Y; i++)
        {
            printf("%d ", free_space[i]);
        }
        printf("\n");
        */
        //finding first available slot to insert word
        int row_counter = (MATRIX_Y);
        for (int i = (MATRIX_Y - 1); i >= 0; i--)
        {
            //printf("Trying to put it in line %d\n", i);
            if(free_space[i] >= strlen(name))
            {
                //found spot
                memcpy(data[i]+MATRIX_X-free_space[i], name, strlen(name));
                success = 1;
                break;
            }
        }
        if(success) {
            //getting info about free space again
            getMatrixFreeSpace(&free_space);
            //sorting rows
            sortRows(&free_space);
            return 0;
        }
        else{
            return 2; // 2 means no room was left
        }

    }


}
//returns array of free space in every row
void getMatrixFreeSpace(int *space)
{
    for(int i = 0; i < MATRIX_Y; i++){

        int len = 0;
        for(int j = 0; j < MATRIX_X; j++)
        {
            if(data[i][j] == 43){
                break;
            }
            else{
                len++;
            }

        }
        space[i] =  MATRIX_X- len;
    }
}

//switches selected rows in matrix
void switchRows(int row1, int row2)
{
    char temp[MATRIX_X];
    memcpy(temp, data[row1], MATRIX_X);
    memcpy(data[row1], data[row2], MATRIX_X);
    memcpy(data[row2], temp, MATRIX_X);
}

//sorts rows with bubble sort, making sure most filled rows are in bottom
void sortRows(int *row_space)
{
    int i, j, check_counter;
    int swapped;
    for (i =0 ; i < MATRIX_Y-1; i++)
    {
        swapped = 0;
        for(j= 0; j < MATRIX_Y-i-1; j++)
        {
            if(row_space[j] < row_space[j+1])
            {
                //swap
                switchRows(j, j+1);
                swapNumbers(&row_space[j], &row_space[j+1]);
                swapped = 1;
            }
        }

        if(!swapped)
            break;
    }
    //printf("Bubble sort did %d checks\n", check_counter);
}

void swapNumbers(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int getDataFromFile(char *filename, char **data) {
    FILE * dataFile;
    char tempLine[121];
    //char ** info = NULL;
    int llen;
    int counter = 0;
    dataFile = fopen(filename, "r");
    //check if fopen could not open file
    if(!dataFile)
    {
        //if could not open file
        printf("Could not open file!\n");
        printf("Make sure file is in the same folder as executable, exiting application\n");
        return 4;
    }
    //checking if openef file is empty
    fseek (dataFile, 0, SEEK_END);
    int filesize = ftell(dataFile);
    if(filesize == 0)
    {
        printf("Provided file is empty, exiting application\n");
        return 5;

    }
    fseek (dataFile, 0, SEEK_SET);



    while (fgets(tempLine, 120, dataFile)) {

        // Allocate memory for pointer to tempLine just added
        data = realloc(data,(counter+1) * sizeof(char *));
        // And allocate memory for that tempLine itself!
        llen = strlen(tempLine);
        data[counter] = calloc(sizeof(char),llen+1);
        // Copy the tempLine just read into that memory
        strcpy(data[counter], tempLine);

        printf("%d characters in tempLine %d \n",llen,counter);
        counter++;
    }

    for (int i = 0; i < counter ; i++) {
        printf("%d: %s",i,data[i]);
    }
}

