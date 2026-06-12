#include <stdio.h>

// this code resets the Highscore to 0 and locks both the unlockable ships.
int main() {
    FILE *fp = fopen("Highscore", "wb");
    int score = 0;
    int unlock = 1;
    int lock = 0;
    fwrite(&score, 4, 1, fp);
    fseek(fp, 4, 0);
    fwrite(&lock, 4, 1, fp);
    fseek(fp, 4, 4);
    fwrite(&lock, 4, 1, fp);
    fclose(fp);
}
