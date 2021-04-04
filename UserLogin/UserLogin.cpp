#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

int AdminOperation(sqlite3* db)
{
    int operation;

    printf("As an administrator, you can\n");
    printf("1. Change your username\n");
    printf("2. Change your password\n");
    printf("3. Regist administrator account\n");
    printf("4. Regist user account\n");
    
    printf("Select your operation:");
    scanf("%d", &operation);

    switch(operation)
    {
        case 1:
        {
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {
            break;
        }
        case 4:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return 0;
}

int UserOperation(sqlite3* db)
{
    int operation;
    
    printf("As a normal user, you can\n");
    printf("1. Change your username\n");
    printf("2. Change your password\n");
    printf("Select your operation:");
    scanf("%d", &operation);
    
    switch(operation)
    {
        case 1:
        {
            break;
        }
        case 2:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    return 0;
}

sqlite3* OpenDatabase(char* filename)
{
    sqlite3 *db;
    int openResult;

    openResult = sqlite3_open("userDB.db", &db);
    if(openResult != SQLITE_OK)
    {
        printf("User database open error! %s", sqlite3_errmsg(db));
        exit(1);
    }
    return db;
}


int Regist(sqlite3* db, int privilege)
{
    char registUserSql[1024];
    char** selectedResult;
    int selectedRow = 0;
    int selectedColumn = 0;
    char* selectError;

    memset(registUserSql, 0, sizeof(registUserSql));

    sqlite3_get_table(db, "select * from user", &selectedResult, &selectedRow, &selectedColumn, &selectError);

    int ID = atoi(selectedResult[(selectedColumn * selectedRow)]) + 1;
    char userName[50];
    char password[50];

    printf("Please input new username:");
    scanf("%s", userName);

    while(1)
    {
        int error = 0;
        for(int i = 0; i < selectedRow; i++)
        {
            if(strcmp(userName, selectedResult[(selectedColumn * i) + 1]) == 0)
            {
                printf("The username you input has been used, Please input another username:");
                scanf("%s", userName);
                error = 1;
            }
            break;
        }
        if(error == 0)
        {
            break;
        }
    }

    printf("Please input new password:");
    scanf("%s", password);

    int registResult;
    char* insertError;
    sprintf(registUserSql, "insert into user(ID, userName, password, privilege) values(%d, '%s', '%s', %d)", ID, userName, password, privilege);
    registResult = sqlite3_exec(db, registUserSql, NULL, NULL, &insertError);

    if(registResult != SQLITE_OK)
    {
        printf("Regist Error! %s", sqlite3_errmsg(db));
        exit(1);
    }

    return 0;
}

int Login(sqlite3* db)
{
    char loginSql[1024];
    char** selectedResult;
    int selectedRow = 0;
    int selectedColumn = 0;
    char* loginError;
    char userName[50];
    char password[50];

    printf("Please input username:");
    scanf("%s", userName);

    printf("Please input password:");
    scanf("%s", password);

    memset(loginSql, 0, sizeof(loginSql));

    sprintf(loginSql, "select * from user where userName = '%s'", userName);

    int selectResult;
    selectResult = sqlite3_get_table(db, loginSql, &selectedResult, &selectedRow, &selectedColumn, &loginError);

    if(strcmp(selectedResult[selectedColumn + 2], password) == 0)
    {
        printf("Login successfully!\n");
    }
    else
    {
        printf("Wrong password\n");
        return -1;
    }
    if(atoi(selectedResult[selectedRow * selectedColumn + 3]) == 0)
    {
        return 0;
    }
    if(atoi(selectedResult[selectedRow * selectedColumn + 3]) == 1)
    {
        return 1;
    }
   return -1;
}

int main(int argc, char* argv[])
{
    char* fileName = (char*)"userDB.db";
    sqlite3* db = OpenDatabase(fileName);

    int operation = 0;
    printf("------------------\n");
    printf("1. regist account\n2. login\n");
    printf("------------------\nSelect your operation:");
    
    scanf("%d", &operation);

    if(operation == 1)
    {
        int registResult;
        registResult = Regist(db, 1);
        if(registResult == 0)
        {
            printf("Regist successfully!\n");
        }
    }
    else if(operation == 2)
    {
        int loginResult;
        loginResult = Login(db);
        switch (loginResult)
        {
        case 0:
            printf("You are now login as an administrator.\n");
            AdminOperation(db);
            break;
        case 1:
            printf("You are now login as nomal user.\n");
            UserOperation(db);
            break;
        case -1:
            printf("Login failed.\n");
            break;
        }
    }
    else
    {
        printf("Error!\n");
        exit(1);
    }

    sqlite3_free(db);
    return 0;
}