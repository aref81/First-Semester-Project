#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

struct user{
    char name [100];
    double score;
};

struct game_ships{
    int x1;
    int y1;
    int x2;
    int y2;
};

struct game{
    int turn;
    char p1_name [100];
    char p2_name [100];
    int maps_id;
    char ** p1_map;
    char ** p2_map;
    double p1_score;
    double p2_score;
    struct game_ships* p1_ships;
    struct game_ships* p2_ships;
};

struct ship{
    int lenght;
    int quantity;
    double point;
};

struct mapsize{
    int columns;
    int lines;
};

struct theme{

};

struct setting{
    struct ship* ships;
    struct mapsize mapsize;
    int ship_types;
    struct theme theme;
};

void start_up();
void f_menu();
void pvp(struct setting* setting);
void pvc();
void loader(int choice);
void saver();
void print_scores();
void setting();
struct user* user_choice();
struct user* add_user(char temp [100]);
struct game* set_game(struct user* p1,struct user* p2,struct setting* setting);
char ** set_map (char* p_name,struct setting* setting,struct game_ships* p_ships);
void print_map (struct setting* setting,char ** p_map);
void print_op_map(struct setting* setting,char ** p_map);
int shoot (int x,int y,char** p_map,struct setting* setting,double* p_score);
void check_e (int x,int y,char ** p_map,struct setting* settings,double * p_score,struct game_ships* p_ships);
int check_end_game(struct game*game,struct setting* settings,struct user*p1,struct user* p2);
void sort_ships(struct game_ships* ships,int ships_num);
void update_user(struct user*p);

int main(){
    start_up();
    f_menu();
}

void start_up(){
    printf("%s","W");
    sleep(1);
    printf("E");
    sleep(1);
    printf("L");
    sleep(1);
    printf("L");
    sleep(1);
    printf("C");
    sleep(1);
    printf("O");
    sleep(1);
    printf("M");
    sleep(1);
    printf("E");
    sleep(1);
    printf("!\n");
    sleep(1);
    printf("%s","TO:\n");
    sleep(1);
    printf("%s","BATTLE SHIPS\n\n");
    sleep(2);
}

void f_menu (){
    int choice;
    struct setting* sett = (struct setting*)malloc(sizeof(struct setting));
    FILE * set = fopen("settings.txt","rb");
    FILE * ships = fopen("ships.txt","rb");
    if (set == NULL){
        ships = fopen("ships.txt","wb");
        set = fopen("settings.txt","wb");
        sett->mapsize.columns = 10;
        sett->mapsize.lines = 10;
        sett->ships= (struct ship*)calloc(sizeof(struct ship),4);
        sett->ships[0].lenght=5;
        sett->ships[0].quantity=1;
        sett->ships[1].lenght = 3;
        sett->ships[1].quantity=2;
        sett->ships[2].lenght=2;
        sett->ships[2].quantity=3;
        sett->ships[3].lenght=1;
        sett->ships[3].quantity=4;
        sett->ship_types = 4;
        for (int i = 0; i < sett->ship_types; ++i) {
            sett->ships[i].point = (double)(5*5)/sett->ships[i].lenght;
        }
        fwrite(sett,sizeof(struct setting),1,set);
        fwrite(sett->ships,sizeof(struct ship),sett->ship_types,ships);
        fclose(set);
        fclose(ships);
    }
    else{
        fread(sett,sizeof(struct setting),1,set);
        fclose(set);
        sett->ships = (struct ship*)calloc(sizeof(struct ship),sett->ship_types);
        fread(sett->ships,sizeof(struct ship),sett->ship_types,ships);
        fclose(ships);
    }
    while (1) {
        printf("1.Play With Friend\n2.Play With Bot\n3.Load Game\n4.Load Last Game\n5.Setting\n6.Score Board\n7.Exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
            {pvp(sett);
                break;}
//            case 2:
//            {pvc();
//                break;}
//            case 3:
//            case 4:
//            {loader(choice);
//                break;}
//            case 5:
//            {setting();
//                break;}
//            case 6:
//            {print_scores();
//                break;}
            case 7:
                return;
            default:
                printf("Unidentified Input!\nPlease Enter an Integer Between 1->7\n");
                sleep(1);

        }
    }
}

void pvp(struct setting* setting){
        struct game* c_game;
        int end_game = 1;
        printf("Please Choose The First Player:\n");
       struct user* p1 = user_choice();
       if (p1 == NULL){
           return;
       }
       printf("Please Choose The Second Player:\n");
       struct user* p2 = user_choice();
       if (p2 == NULL){
           return;
       }
       if(strcmp(p1->name,p2->name) == 0){
           printf("Dear %s You Cant Play With Your Self!\n",p1->name);
           return;
       }
       c_game=set_game(p1,p2,setting);
    for (int i = 0,choice = 0; i == 0 ; ++i) {
        printf("Who Would Go First?!\n1.%s     or     2.%s\n\nPlease Enter The Number of Player:", c_game->p1_name, c_game->p2_name);
        scanf("%d", &choice);
        if (choice == 1) {
            c_game->turn = 0;
            printf("%s goes first!\n",c_game->p1_name);
            i =1;
        } else if (choice == 2) {
            c_game->turn = 1;
            printf("%s goes first!\n",c_game->p2_name);
            i = 1;
        } else {
            printf("Wrong Input!\n");
            i = 0;
        }
    }
    while (end_game){
        c_game->turn %= 2;
        if (c_game->turn){
            int x;
            int y;
            for (int i = 0; !i ;) {
                printf("%s : %lf\n",c_game->p2_name,c_game->p2_score);
                print_op_map(setting, c_game->p1_map);
                printf("Please Enter The Coordinates You Want To Shoot!\n");
                printf("X :");
                scanf("%d", &x);
                printf("Y :");
                scanf("%d", &y);
                i = shoot(x,y,c_game->p1_map,setting,&c_game->p2_score);
            }
            check_e(x,y,c_game->p1_map,setting,&c_game->p2_score,c_game->p1_ships);
            print_op_map(setting, c_game->p1_map);
        }
        else{
            int x;
            int y;
            for (int i = 0; !i ;) {
                printf("%s : %lf\n",c_game->p1_name,c_game->p1_score);
                print_op_map(setting, c_game->p2_map);
                printf("Please Enter The Coordinates You Want To Shoot!\n");
                printf("X :");
                scanf("%d", &x);
                printf("Y :");
                scanf("%d", &y);
                i = shoot(x,y,c_game->p2_map,setting,&c_game->p1_score);
            }
            check_e(x,y,c_game->p2_map,setting,&c_game->p1_score,c_game->p2_ships);
            print_op_map(setting, c_game->p2_map);
        }
        end_game = check_end_game(c_game,setting,p1,p2);
        c_game->turn++;
    }
    update_user(p1);
    update_user(p2);
};

struct user* user_choice(){
    FILE *saves;
    char search_name[100];
    int i;
    saves = fopen("saves.txt","rb");
    if (saves == NULL){
        printf("couldn't open save file :(\n");
        saves = fopen("saves.txt","wb");
        fclose(saves);
        saves = fopen("saves.txt","rb");
    }

    fread(search_name,sizeof(search_name),1,saves);
    for (i = 1;!feof(saves); ++i) {
        printf("%d.%s\n",i,search_name);
        fseek(saves,sizeof(struct user) - sizeof(search_name),SEEK_CUR);
        fread(search_name,sizeof(search_name),1,saves);
    }
    if (i == 1){
        printf("No User Exists\n");
    }
    printf("Please Enter User's Name or\nEnter \"1\" To Add a New User\n");
    scanf("%s",search_name);
    fseek(saves,0,SEEK_SET);
    if (strcmp(search_name,"1") == 0){
        char new [100];
        printf("Please Enter New User's Name:\n");
        scanf("%s",new);
        while (strcmp(new,"1") == 0){
            printf("You Can't Choose \"1\" As User Name!\nPlease Enter Another Name:\n");
            scanf("%s",new);
        }
        char temp [100];
        fread(temp,sizeof(temp),1,saves);
        while (!feof(saves)){
            if (strcmp(temp,new) == 0){
                printf("The Entered Name Already Exist.\n"
                       "Please Enter Another Name:\n");
                scanf("%s",new);
                fseek(saves,0,SEEK_SET);
                fread(temp,sizeof(temp),1,saves);
                continue;
            }
            fseek(saves,sizeof(struct user) - sizeof(search_name),SEEK_CUR);
            fread(temp,sizeof(temp),1,saves);
        }
        fclose(saves);
        return add_user(new);
    }

    for (int i = 0; ;i++) {
        char temp_name [100];
        fread(temp_name,sizeof(temp_name),1,saves);
        if (feof(saves)){
            printf("name not found\nplease enter the correct name \nor \"1\" to add a new User \nor\"0\" to exit\n ");
            scanf("%s",search_name);
            if (strcmp(search_name,"0") == 0){
                fclose(saves);
                return NULL;
            }
            else if (strcmp(search_name,"1") == 0){
                char new [100];
                printf("Please Enter New User's Name:\n");
                scanf("%s",new);
                while (strcmp(new,"1") == 0){
                    printf("You Can't Choose \"1\" As User Name!\nPlease Enter Another Name:\n");
                    scanf("%s",new);
                }
                char temp [100];
                fread(temp,sizeof(temp),1,saves);
                while (!feof(saves)){
                    if (strcmp(temp,new) == 0){
                        printf("The Entered Name Already Exist.\n"
                               "Please Enter Another Name:\n");
                        scanf("%s",new);
                        fseek(saves,0,SEEK_SET);
                        fread(temp,sizeof(temp),1,saves);
                        continue;
                    }
                    fseek(saves,sizeof(struct user) - sizeof(search_name),SEEK_CUR);
                    fread(temp,sizeof(temp),1,saves);
                }
                fclose(saves);
                return add_user(new);
            }
            else {
                fseek(saves, 0, SEEK_SET);
                continue;
            }
        }
        else if(strcmp(temp_name,search_name) == 0){
            struct user* player = (struct user*)malloc(sizeof(struct user));
            fseek(saves,-sizeof(temp_name),SEEK_CUR);
            fread(player,sizeof(struct user),1,saves);
            fclose(saves);
            return player;
        }
        fseek(saves,sizeof(struct user)-sizeof(temp_name),SEEK_CUR);
    }

}

struct user* add_user(char temp [100]){
    FILE *saves;
    saves = fopen("saves.txt","ab");
    struct user* new = (struct user*) malloc(sizeof(struct user));
    strcpy(new->name,temp);
    new->score = 0;
    fwrite(new,sizeof(struct user),1,saves);
    fclose(saves);
    return new;
}

struct game* set_game(struct user* p1,struct user*p2,struct setting* setting){
    struct game* game = (struct game*)malloc(sizeof(struct game));
    int ship_num = 0;
    for (int i = 0; i <setting->ship_types ; ++i) {
        ship_num += setting->ships[i].quantity;
    }
    strcpy(game->p1_name,p1->name);
    strcpy(game->p2_name,p2->name);
    game->p1_score = 0;
    game->p2_score = 0;
    game->p1_ships = (struct game_ships*)calloc(sizeof(struct game_ships),ship_num);
    game->p2_ships = (struct game_ships*)calloc(sizeof(struct game_ships),ship_num);
    game->p1_map = set_map(game->p1_name,setting,game->p1_ships);
    game->p2_map = set_map(game->p2_name,setting,game->p2_ships);
    sort_ships(game->p1_ships,ship_num);
    sort_ships(game->p2_ships,ship_num);
    return game;
}

char ** set_map (char* p_name,struct setting* setting,struct game_ships* p_ships){
    char **p_map = (char **)calloc(sizeof(char*),setting->mapsize.lines);
    for (int i = 0; i < setting->mapsize.lines; ++i) {
        (p_map[i])= (char *)calloc(sizeof(char),setting->mapsize.columns);
    }
    int ship_num = 0;
    for (int i = 0; i < setting->mapsize.lines; ++i) {
        for (int j = 0; j < setting->mapsize.columns; ++j) {
            p_map[i][j] = '*';
        }
    }
    printf("%s\n\n",p_name);
    for (int i = 0; i < setting->ship_types; ++i) {
        for (int j = 0; j < setting->ships[i].quantity; ++j) {
            print_map(setting,p_map);
            int x1;
            int y1;
            int x2;
            int y2;
            if (j == 0){
                printf("Please Enter The coordinates of your %dst ship of length %d\n"
                       "out of X :1->%d and Y :1->%d :\n",j+1,setting->ships[i].lenght,setting->mapsize.columns,setting->mapsize.lines);
            }
            else if (j == 1){
                printf("Please Enter The coordinates of your %dnd ship of length %d\n"
                       "out of X :1->%d and Y :1->%d :\n",j+1,setting->ships[i].lenght,setting->mapsize.columns,setting->mapsize.lines);
            }
            else{
                printf("Please Enter The coordinates of your %dth ship of length %d\n"
                       "out of X :1->%d and Y :1->%d :\n",j+1,setting->ships[i].lenght,setting->mapsize.columns,setting->mapsize.lines);
            }
            if (setting->ships[i].lenght == 1){
                for (int k = 0; k != 1 ;) {
                    printf("X :");
                    scanf("%d", &x1);
                    printf("Y :");
                    scanf("%d", &y1);

                    if (p_map[y1 - 1][x1 - 1] != '*') {
                        printf("Blocks Already Filled!\n"
                               "Please Enter The Coordintates Again\n");
                        k = 0;
                    }
                    else{
                        p_map[y1-1][x1-1] = 'S';
                        p_ships[ship_num].x1 = x1-1;
                        p_ships[ship_num].x2 = x1-1;
                        p_ships[ship_num].y1 = y1-1;
                        p_ships[ship_num].y2 = y1-1;
                        ship_num++;
                        k =1;
                    }
                }
            }
            else {
                for (int k = 0, f = 0; k != 1 || f != 1;) {
                    printf("first:\n"
                           "X :");
                    scanf("%d", &x1);
                    printf("first:\n"
                           "Y :");
                    scanf("%d", &y1);
                    printf("last:\n"
                           "X :");
                    scanf("%d", &x2);
                    printf("last:\n"
                           "Y :");
                    scanf("%d", &y2);
                    if (!(((fabs(x1 - x2) + 1) == setting->ships[i].lenght) ||
                          ((fabs(y1 - y2) + 1) == setting->ships[i].lenght))) {
                        printf("Entered Coordinates Doesn't Match Ships Length\nPlease Enter Coordinates Again\n");
                        k = 0;
                        continue;
                    } else {
                        k = 1;
                    }
                    if (x1 > setting->mapsize.columns || x1 < 0 || x2 > setting->mapsize.columns || x2 < 0 ||
                        y1 > setting->mapsize.lines || y1 < 0 || y2 > setting->mapsize.lines || y2 < 0) {
                        printf("You Are Out Of Borders\nPlease Enter Coordinates out Of Given Values\n\n ");
                        k = 0;
                        continue;
                    } else {
                        k = 1;
                    }

                    if (x1 == x2 && y1 != y2) {
                        if (y1 > y2) {
                            for (int m = 0; m < setting->ships[i].lenght; ++m) {
                                if (p_map[y2 + m - 1][x1 - 1] != '*') {
                                    printf("Chosen Blocks Already Filled!\nPlease Enter Coordinates Again\n");
                                    f = 0;
                                    break;
                                } else {
                                    f = 1;
                                }
                            }
                            if (f == 1) {
                                for (int l = 0; l < setting->ships[i].lenght; ++l) {
                                    p_map[y2 + l - 1][x1 - 1] = 'S';
                                }
                                p_ships[ship_num].x1 = x1-1;
                                p_ships[ship_num].x2 = x2-1;
                                p_ships[ship_num].y1 = y1-1;
                                p_ships[ship_num].y2 = y2-1;
                                ship_num++;
                            }
                            else {
                                continue;
                            }
                        } else if (y2 > y1) {
                            for (int m = 0; m < setting->ships[i].lenght; ++m) {
                                if (p_map[y1 + m - 1][x1 - 1] != '*') {
                                    printf("Chosen Blocks Already Filled!\nPlease Enter Coordinates Again\n");
                                    f = 0;
                                    break;
                                } else {
                                    f = 1;
                                }
                            }
                            if (f == 1) {
                                for (int l = 0; l < setting->ships[i].lenght; ++l) {
                                        p_map[y1 + l - 1][x1 - 1] = 'S';
                                }
                                p_ships[ship_num].x1 = x1-1;
                                p_ships[ship_num].x2 = x2-1;
                                p_ships[ship_num].y1 = y1-1;
                                p_ships[ship_num].y2 = y2-1;
                                ship_num++;
                            }
                            else {
                                continue;
                            }
                        }
                    } else if (y1 == y2 && x1 != x2) {
                        if (x1 > x2) {
                            for (int m = 0; m < setting->ships[i].lenght; ++m) {
                                if (p_map[y1 - 1][x2 + m - 1] != '*') {
                                    printf("Chosen Blocks Already Filled!\nPlease Enter Coordinates Again\n");
                                    f = 0;
                                    break;
                                }
                                else {
                                    f = 1;
                                }
                            }
                            if (f == 1) {
                                for (int l = 0; l < setting->ships[i].lenght; ++l) {
                                        p_map[y1 - 1][x2 + l - 1] = 'S';
                                }
                                p_ships[ship_num].x1 = x1-1;
                                p_ships[ship_num].x2 = x2-1;
                                p_ships[ship_num].y1 = y1-1;
                                p_ships[ship_num].y2 = y2-1;
                                ship_num++;
                            }
                            else {
                                continue;
                            }
                        }
                        else if (x2 > x1) {
                            for (int m = 0; m < setting->ships[i].lenght; ++m) {
                                if (p_map[y1 - 1][x1 + m - 1] != '*') {
                                    printf("Chosen Blocks Already Filled!\nPlease Enter Coordinates Again\n");
                                    f = 0;
                                    break;
                                } else {
                                    f = 1;
                                }
                            }
                            if (f == 1) {
                                for (int l = 0; l < setting->ships[i].lenght; ++l) {
                                        p_map[y1 - 1][x1 + l - 1] = 'S';
                                }
                                p_ships[ship_num].x1 = x1-1;
                                p_ships[ship_num].x2 = x2-1;
                                p_ships[ship_num].y1 = y1-1;
                                p_ships[ship_num].y2 = y2-1;
                                ship_num++;
                            }
                            else {
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
    return p_map;
}

void print_map (struct setting* setting,char ** p_map){
    printf("  ");
    for (int i = 0; i < setting->mapsize.columns; ++i) {
        printf("%d ",i+1);
    }
    puts("");
    for (int i = 0; i < setting->mapsize.lines; ++i) {
        printf("%-2d",i+1);
        for (int j = 0; j < setting->mapsize.columns; ++j) {
                printf("%c ", p_map[i][j]);
        }
        puts("");
    }
}

void print_op_map (struct setting* setting,char ** p_map){
    printf("  ");
    for (int i = 0; i < setting->mapsize.columns; ++i) {
        printf("%d ",i+1);
    }
    puts("");
    for (int i = 0; i < setting->mapsize.lines; ++i) {
        printf("%-2d",i+1);
        for (int j = 0; j < setting->mapsize.columns; ++j) {
            if (p_map[i][j] == 'E' || p_map[i][j] == 'C' || p_map[i][j] == 'W') {
                printf("%c ", p_map[i][j]);
            }
            else{
                printf("* ");
            }
        }
        puts("");
    }
}

int shoot (int x,int y,char** p_map,struct setting* setting,double * p_score){
    if (x> setting->mapsize.columns || x<0 || y>setting->mapsize.lines || y < 0){
        printf("You Are Out Of Boarder!\n Please Enter The Coordinates You Want To Shoot in The Given Boarsers.\n\n");
        return 0;
    }
    if (p_map[y][x] == 'E' || p_map[y][x] == 'C' || p_map[y][x] == 'W'){
        printf("You Have Already Shot This Point!\n Please Choose Another Point To Attack.\n\n");
        return 0;
    }
    if (p_map[y-1][x-1] == '*'){
        p_map[y-1][x-1] = 'W';
        return 1;
    }
    else if (p_map[y-1][x-1] == 'S'){
        p_map[y-1][x-1] = 'E';
        (*p_score)++;
        return 1;
    }
}

void check_e (int x,int y,char ** p_map,struct setting* settings,double *p_score,struct game_ships* p_ships) {
    x--;
    y--;
    int check = 1;
    if (p_map[y][x] == 'W'){
        return;
    }
    else if (p_map[y][x] == 'E'){
        int i;
        for (i = 0;; ++i) {
            if (x>=p_ships[i].x1 && x<=p_ships[i].x2 && y>=p_ships[i].y1 && y<=p_ships[i].y2){
                break;
            }
        }
        for (int j = p_ships[i].x1; j <= p_ships[i].x2; ++j) {
            for (int k = p_ships[i].y1; k <= p_ships[i].y2 ; ++k) {
                if (p_map[k][j] != 'E'){
                  check = 0;
                    break;
                }
            }
            if (check == 0){
                break;
            }
        }
        if (check){
            for (int j = p_ships[i].x1; j <= p_ships[i].x2; ++j) {
                for (int k = p_ships[i].y1; k <= p_ships[i].y2 ; ++k) {
                    p_map[k][j] = 'C';
                    }
                }
            int j;
            for (j = 0; j < settings->ship_types; ++j) {
                if ((p_ships[i].x2 - p_ships[i].x1 +1 == settings->ships[j].lenght) || (p_ships[i].y2 - p_ships[i].y1 +1 == settings->ships[j].lenght)){
                    break;
                }
            }
            (*p_score) += settings->ships[j].point;
            }
        }
    }

int check_end_game(struct game* game,struct setting* settings,struct user* p1,struct user* p2){
    int p = 0;
    for (int i = 0; i < settings->mapsize.lines; ++i) {
        for (int j = 0; j < settings->mapsize.columns; ++j) {
            if (game->p1_map[i][j] == 'S'){
                p = 1;
            }
        }
    }
    if (p == 0){
        printf("Game is Over!\n\n"
               "****%s****\n\n"
               "is Winner!\n",game->p2_name);
        p2->score += game->p2_score;
        p1->score += game->p1_score/2;
        return 0;
    }
    p = 0;
    for (int i = 0; i < settings->mapsize.lines; ++i) {
        for (int j = 0; j < settings->mapsize.columns; ++j) {
            if (game->p2_map[i][j] == 'S'){
                p = 1;
            }
        }
    }
    if (p == 0){
        printf("Game is Over!\n\n"
               "****%s****\n\n"
               "is Winner!\n",game->p1_name);
        p1->score += game->p1_score;
        p2->score += game->p2_score/2;
        return 0;
    }

    return p;
}

void sort_ships(struct game_ships* ships,int ships_num){
    for (int i = 0; i < ships_num; ++i) {
        if (ships[i].x2 < ships[i].x1){
            int temp = ships[i].x1;
            ships[i].x1 = ships[i].x2;
            ships[i].x2 = temp;
        }
        if (ships[i].y2 < ships[i].y1){
            int temp = ships[i].y1;
            ships[i].y1 = ships[i].y2;
            ships[i].y2 = temp;
        }
    }
}

void update_user(struct user*p){
    FILE * saves = fopen("saves.txt","rb+");
    struct user* temp;
    fread(temp,sizeof(struct user),1,saves);
    while (!feof(saves)){
        if (strcmp(temp->name,p->name) == 0){
            break;
        }
        else{
            fread(temp,sizeof(struct user),1,saves);
        }
    }
    fseek(saves,-sizeof(struct user),SEEK_CUR);
    fwrite(p,sizeof(struct user),1,saves);
}