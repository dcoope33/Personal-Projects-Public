#include <stdio.h>

int main() {
    FILE *fp = fopen("Highscore", "wb");
    int score = 538;
    int unlock = 1;
    int lock = 0;
    fwrite(&score, 4, 1, fp);
    fseek(fp, 4, 0);
    fwrite(&unlock, 4, 1, fp);
    fseek(fp, 4, 4);
    fwrite(&lock, 4, 1, fp);
    fclose(fp);
}