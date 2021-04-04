#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

int my_callback(void *para, int columnCount, char **columnValue, char **columnName)
{
    for(int i = 0; i < columnCount; i++)
    {
        printf("%s:%s|",columnName[i],columnValue[i]);
    }
    printf("\n");
    return 0;
}





int main(int argc, char* argv[])
{
    sqlite3 *db;
    int ret;

    ret = sqlite3_open("studentDB.db", &db);

    if(ret != SQLITE_OK)
    {
        printf("Open database error! %s", sqlite3_errmsg(db));
        exit(1);
    }
    printf("Open database successfully!\n");

    char* errmsg;
#if 0
    char sql[] = "create table student(id int, name text, age int)";
    

    int creatResult = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if(creatResult != SQLITE_OK)
    {
        printf("Creat table error! %s", sqlite3_errmsg(db));
        exit(1);
    }
#endif
    char selectSql[] = "select * from student";

    sqlite3_exec(db, selectSql, my_callback, NULL, &errmsg);


#if 0
    int id;
    char name[20];
    int age;

    for(int i = 0; i < 3; i++)
    {
        memset(name, 0, 20);
        printf("Please input id:");
        scanf("%d", &id);
        
        printf("Please input name:");
        scanf("%s", name);

        printf("Please input age:");
        scanf("%d", &age);

        char insertSql[1024];
        memset(insertSql, 0, 1024);
        sprintf(insertSql, "insert into student(id, name, age) values(%d, '%s', %d)", id, name, age);

        char* insertErrMsg;
        int insertResult = sqlite3_exec(db, insertSql, NULL, NULL, &insertErrMsg);
        if(insertResult != SQLITE_OK)
        {
            printf("Insert table error! %s", sqlite3_errmsg(db));
            exit(1);
        }
    }
#endif



    
    sqlite3_close(db);
}