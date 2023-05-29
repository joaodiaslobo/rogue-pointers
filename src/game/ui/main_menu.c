#include <ncurses.h>
#include "engine_types.h"
#include "main_menu.h"
#include "draw.h"
#include "color.h"
#include "image.h"
#include "sys/time.h"
#include "components.h"

/*

* a104439, a104356 - Rita Camacho, João Lobo

* Menu principal do jogo.

*/
int main_menu(Terminal *terminal){
    int selection = -1;
    while(selection == -1){

        // Título
        Image image = load_image_from_file("assets/sprites/main_menu/title.sprite");
        
        Vector2D pos;
        pos.x = ((terminal->xMax - 62*2)/4);
        pos.y = ((terminal->yMax - 8)/2) - 15;

        draw_to_window(terminal->mainWindow, image, pos);     

        // Input de seleção
        char *options[] = {"PLAY", "ABOUT", "QUIT"};
        selection = main_menu_update(3, options, terminal->xMax - 12, terminal->yMax - 9, 5, terminal);
        
        if(selection == 2){
            // Confirmação no caso de saída
            if(modal_confim("Are you sure you want to quit the game?", 45, terminal->yMax, terminal->xMax)){
                return selection;
            }
            selection = -1;
        }
    }

    return selection;
}

int main_menu_update(int options, char *texts[], int width, int y, int x, Terminal *terminal){
    int selection = 0, key = 0;
    int lines  = options + 4;

    WINDOW * selectWindow = newwin(lines, width, y, x);
    box(selectWindow, 0, 0);
    refresh();
    wrefresh(selectWindow);
    keypad(selectWindow, true);

    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    unsigned long timeSinceLastFrame = 0;

    int direction = 0;
    int frame = 0;

    Vector2D pos;
    pos.x = ((terminal->xMax - 62*2)/4);
    pos.y = ((terminal->yMax - 8)/2) - 15;

    nodelay(selectWindow, TRUE);

    while (key != 10)
    {
        struct timeval now;
        gettimeofday(&now, NULL);

        unsigned long elapsedMicroseconds = (now.tv_sec - currentTime.tv_sec) * 1000000 + (now.tv_usec - currentTime.tv_usec);
        gettimeofday(&currentTime, NULL);

        timeSinceLastFrame += elapsedMicroseconds;

        if(timeSinceLastFrame >= 100000){
            timeSinceLastFrame = 0;
            if(direction){
                if(frame == 9){
                    direction = !direction;
                    frame--;
                }
                else{
                    frame++;
                }
            }
            else{
                if(frame == 0){
                    direction = !direction;
                    frame++;
                }
                else{
                    frame--;
                }
            }
            create_sparkles_animation(pos, frame, terminal);
        }

        for(int i = 0; i < options; i++){
            if(i == selection){
                wattron(selectWindow, A_BOLD);
                mvwprintw(selectWindow, i+2, 4, "[x] %s", texts[i]);
                wattroff(selectWindow, A_BOLD);
            } else {
                mvwprintw(selectWindow, i+2, 4, "[ ] %s", texts[i]);
            }
            wrefresh(selectWindow);
        }

        move(0, 0);

        key = wgetch(selectWindow);
        switch (key)
        {
            case KEY_UP:
                if(selection > 0){
                    selection--;
                } else {
                    selection = options - 1;
                }
                break;
            case KEY_DOWN:
                if(selection < options - 1){
                    selection++;
                } else {
                    selection = 0;
                }
                break;
            default:
                break;
        }
    }

    wclear(selectWindow);
    wrefresh(selectWindow);
    delwin(selectWindow);
    return selection;
}

void create_sparkles_animation(Vector2D pos, int frame, Terminal *terminal){
    char location[50];
    sprintf(location, "assets/sprites/main_menu/sparkles/%d.sprite", frame);
    Image frameImage = load_image_from_file(location);
    draw_to_window(terminal->mainWindow, frameImage, pos);
    wrefresh(terminal->mainWindow);
}