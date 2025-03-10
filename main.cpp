#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <termios.h>    //conio.h in windows (linux doesnt conio.h library)
#include <unistd.h>

using namespace std;

char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, & oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, & newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, & oldt);
    return ch;
}

const int maxbox_x = 40;
const int maxbox_y = 10;

int characterx = maxbox_x / 2;
int charactery = 2;

//this boxplaces array is an array used at the start of this project but now it has no use
int boxplaces[maxbox_x + 1][maxbox_y + 1]; //to not include the null character

int volume = 0;
double volume_indicator = (int)'🯰';
bool isOn = true;

void speeddraw(int dStart, int dEnd, unsigned int blockID) { //0 is the default value if you dont want any spaces at the start
    //block ID 1 = 🮆
    //block ID 2 = space
    //block ID 3 = stikmen

    for (int i = 0; i < dStart; i++) {
        printf(" ");
    }

    for (int i = 0; i < dEnd; i++) {
        if (blockID == 1) printf("🮆");
        if (blockID == 2) printf(" ");
        if (blockID == 3) printf("🯅");
        if (blockID == 8) printf("⏽");
        if (blockID == 9) printf("⭘");
        if (blockID == 10) printf("▕");
        if (blockID == 11) printf("▁");
        if (blockID == 12) printf("▏");
        if (blockID == 13) printf("▔");
    }
}

void printvolume() {
    switch (volume) {
    case 0:
        printf("🯰");
        break;
    case 1:
        printf("🯱");
        break;
    case 2:
        printf("🯲");
        break;
    case 3:
        printf("🯳");
        break;
    case 4:
        printf("🯴");
        break;
    case 5:
        printf("🯵");
        break;
    case 6:
        printf("🯶");
        break;
    case 7:
        printf("🯷");
        break;
    case 8:
        printf("🯸");
        break;
    case 9:
        printf("🯹");
        break;
    }
}

void _init_() {
    // adjusting box placement (no use)
    for (int j = 0; j < maxbox_y; j++) {
        for (int i = 0; i < maxbox_x; i++) {
            if (j == 2) boxplaces[i][j] = 0;
            else boxplaces[i][j] = 1;
        }
    }

    //reading (debug)
    for (int j = 0; j < maxbox_y; j++) {
        for (int i = 0; i < maxbox_x; i++) {
            printf("%d", boxplaces[i][j]);
        }
    }

    printf("\n\n");

}

void frame_generate() {
    system("clear");
    if (isOn == true) {
        for (int j = maxbox_y; j > 0; j--) {
            if (j == 1 || j == maxbox_y) speeddraw(0, maxbox_x, 1);
            else {

                //characterx = 1 y = 2
                if (j == charactery) {
                    speeddraw(0, 1, 1); speeddraw(characterx - 1, 1, 3); speeddraw(maxbox_x - (characterx + 2), 1, 1);
                } else {
                    speeddraw(0, 1, 1); speeddraw(0, maxbox_x - 2, 2); speeddraw(0, 1, 1);
                }

            }
            // printf("a");    //debug
            printf("\n");

        }
    }

    if (isOn == false) {
        for (int j = maxbox_y; j > 0; j--) {
            printf("\n");
        }
    }

    speeddraw(1, 3, 11);
    printf("\n");

    //so basically printing the on off button and volume
    if (isOn == true) {
        speeddraw(0, 1, 10); speeddraw(2, 1, 8); speeddraw(0, 1, 12); speeddraw(0, (2 * maxbox_x / 3 - 2), 2); printf("Volume: "); printvolume();
    }
    if (isOn == false) {
        speeddraw(0, 1, 10); speeddraw(0, 1, 9); speeddraw(2, 1, 12); speeddraw(0, (2 * maxbox_x / 3 - 2), 2); printf("Volume: "); printvolume();
    }
    printf("\n");
    speeddraw(1, 3, 13);

    printf("\n\n\n");
    /* speeddraw(0,1,1); speeddraw(0,38,2); speeddraw(0,1,1);   debug*/

}

int is_movement_okay(int dx, int dy) {
    if (characterx + dx < maxbox_x - 1 && characterx + dx > 1 && charactery + dy < maxbox_y && charactery + dy > 1 && isOn == true) return 1;
    else return 0;
}

int is_volume_okay(int dx) {
    if (volume + dx >= 0 && volume + dx <= 9) return 1;
    else return 0;
}

char get_arrow_key() {
    char ch = getch();
    if (ch == '\033') { // esc key
        if (getch() == '[') { // waiting for '[' character
            switch (getch()) { // third character determines the arrow direction
            case 'A':
                return 'w';
            case 'B':
                return 's';
            case 'C':
                return 'd';
            case 'D':
                return 'a';
            }
        }
    }
    return ch; // here returning the original input to use keys for volume control and on/off
}
bool power_switch() {
    if (isOn == true) {
        isOn = false;
        return false;
    }
    if (isOn == false) {
        isOn = true;
        return true;
    }
}
int main() {
    _init_();

    char key = '0';
    while (key != '^[[A' && key != '^[[B' && key != '^[[C' && key != '^[[D') {
        frame_generate();
        key = get_arrow_key();
        if (key == 'w' && is_movement_okay(0, 1)) {
            charactery++;
            key = '0';
        }
        if (key == 's' && is_movement_okay(0, -1)) {
            charactery--;
            key = '0';
        }
        if (key == 'd' && is_movement_okay(1, 0)) {
            characterx++;
            key = '0';
        }
        if (key == 'a' && is_movement_okay(-1, 0)) {
            characterx--;
            key = '0';
        }
        if (key == 'e' && is_volume_okay(1)) {
            volume++;
            key = '0';
        }
        if (key == 'q' && is_volume_okay(-1)) {
            volume--;
            key = '0';
        }
        if (key == 'x') {
            power_switch();
            key = '0';
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); //tick
    }

}
