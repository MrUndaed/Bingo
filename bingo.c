#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Crearea matricelor din vector (atribuirea matricelor pe pozitiile unui vector, creem vector 3D)
int **creMat()
{
    int i, **mat = (int **)malloc(5 * sizeof(int *));
    for (i = 0; i < 5; i++)
        mat[i] = (int *)malloc(5 * sizeof(int));
    return mat;
}

// Citire matrice
void citMat(int **mat)
{
    int i, j;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            scanf("%d", &mat[i][j]);
    mat[2][2] = -mat[2][2];
}

// Print matrice pastigatoare
void prtWin(int **mat)
{
    int i, j;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
            if (mat[i][j] >= 0)
                printf("%d ", mat[i][j]);
            else
                printf("# ");
        printf("\n");
    }
}

// Swap
void swap(int *a, int *b)
{
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

// cauta la ce pozitie se afla coloana
int search(int *v, int x)
{
    int i;
    for (i = 0; i < 5; i++)
        if (v[i] == x)
            return i;
    return 0;
}

// Bubble sort (for collumn X asc/desc)
void ASC(int ***mat, int n, int x)
{
    int i, j, k;
    for (k = 0; k < n; k++)
        for (i = 0; i < 5; i++)
            for (j = 0; j < 4; j++)
                if (abs(mat[k][j][x]) > abs(mat[k][j + 1][x]))
                    swap(&mat[k][j][x], &mat[k][j + 1][x]);
}

void DSC(int ***mat, int n, int x)
{
    int i, j, k;
    for (k = 0; k < n; k++)
        for (i = 0; i < 5; i++)
            for (j = 0; j < 4; j++)
                if (abs(mat[k][j + 1][x]) > abs(mat[k][j][x]))
                    swap(&mat[k][j + 1][x], &mat[k][j][x]);
}

// Hasuram spatiile
void hash(int **mat, int pz, int y)
{
    int i;
    for (i = 0; i < 5; i++)
        if (mat[i][pz] == y)
            mat[i][pz] = -mat[i][pz];
}

// SHIFT-X-Y
void shiftXY(int ***mat, int n, int x, int y)
{
    int i, j, v[5], poz;
    for (i = 0; i < n; i++)
    {
        // atribuim vectorului elementele coloanei pentru a le permuta
        for (j = 0; j < 5; j++)
            v[j] = mat[i][j][x];

        if (!(y % 5))
            continue;
        else
            for (j = 0; j < 5; j++)
            {
                poz = (j + y) % 5;
                mat[i][poz][x] = v[j];
            }
    }
}

// SWAP-X-Y
void swapXY(int ***mat, int n, int x, int y, int *v)
{
    int i, j;
    if (x != y)
    {
        for (i = 0; i < n; i++)
            for (j = 0; j < 5; j++)
                swap(&mat[i][j][x], &mat[i][j][y]);

        swap(&v[x], &v[y]);
    }
}

// Verifica daca matricea a facut bingo
int points(int ***mat, int *pt, int n)
{
    int i, j, k;
    int c, l, d, ds;
    int poz = -1, max = -1;

    for (i = 0; i < n; i++)
    {
        d = 0, ds = 0;

        // Verificam BINGO pe diagonala secundara
        for (j = 0; j < 5; j++)
        {
            if (mat[i][4 - j][j] < 0)
                ds++;
            if (ds == 5)
                pt[i] += 10;
        }

        // Verificam BINGO pe linie
        for (j = 0; j < 5; j++)
        {
            l = 0;
            for (k = 0; k < 5; k++)
                if (mat[i][j][k] < 0)
                    l++;
            if (l == 5)
                pt[i] += 10, l = 0;
        }

        // Verificam BINGO pe coloana
        for (j = 0; j < 5; j++)
        {
            c = 0;
            for (k = 0; k < 5; k++)
                if (mat[i][k][j] < 0)
                    c++;
            if (c == 5)
                pt[i] += 10, c = 0;
        }

        // Verificam BINGO pe diagonala
        for (j = 0; j < 5; j++)
        {
            if (mat[i][j][j] < 0)
                d++;
            if (d == 5)
                pt[i] += 10;
        }
    }

    for (i = 0; i < n; i++)
        if (pt[i] > max && pt[i] > 0)
            max = pt[i], poz = i;

    if ((max >= 0))
    {
        printf("%d\n", poz);
        prtWin(mat[poz]);
        printf("%d\n", pt[poz]);
        return 1;
    }
    return 0;
}

// Completam matricea cu datele de sus^
int completare(int ***cards, int m, int n, int *v)
{
    int i, j, nr = 0, x = 0, y = 0, k = 0, h, d = 0;
    int *pt;
    char *p, c;
    char date[11], aux[11];

    pt = (int *)calloc(n, sizeof(int));

    for (h = 0; h < m; h++)
    {
        scanf("%s", date);
        strcpy(aux, date);

        p = strtok(aux, "-");
        k = x = y = 0;

        // verificam daca fct este de colorare(fct hash) sau de lucru pe matrice
        if (strlen(p) > 2)
        {
            // verificam nr de argumente al fct
            if (p[0] == 'A' ||
                p[0] == 'D' ||
                strcmp(p, "SHOW") == 0)
                k = 1; // ASC si DSC au un singur parametru x

            p = strtok(NULL, "-");

            for (i = 0; i < 2 - k; i++)
            { // pentru fct cu 2 parametri si posibili parametrii de 2 cifre
                if (!i)
                    for (j = 0; j < strlen(p); j++)
                        x = x * 10 + p[j] - '0';
                else
                    for (j = 0; j < strlen(p); j++)
                        y = y * 10 + p[j] - '0';
                p = strtok(NULL, "-");
            }

            p = strtok(date, "-");
            // printf("\n%d %d\n", x, y);
            if (!strcmp(p, "SWAP"))
                swapXY(cards, n, x, y, v);

            else if (!strcmp(p, "SHIFT"))
                shiftXY(cards, n, x, y);

            else if (!strcmp(p, "ASC"))
                ASC(cards, n, x);

            else if (!strcmp(p, "DSC"))
                DSC(cards, n, x);

            else if (!strcmp(p, "SHOW"))
            {

                prtWin(cards[x]);
            }
        }
        else
        {
            c = p[0];
            p = strtok(NULL, "-");
            // Pentru cazurile B/I/N/G/O
            switch (c)
            {
            case 'B':
                if (p[1] != '\0') // transformam nr din char in int
                    nr = (p[0] - '0') * 10 + (p[1] - '0');
                else
                    nr = p[0] - '0';

                for (j = 0; j < n; j++)
                    hash(cards[j], search(v, 0), nr);
                break;

            case 'I':
                if (p[1] != '\0')
                    nr = (p[0] - '0') * 10 + (p[1] - '0');
                else
                    nr = p[0] - '0';

                for (j = 0; j < n; j++)
                    hash(cards[j], search(v, 1), nr);
                break;

            case 'N':
                if (p[1] != '\0')
                    nr = (p[0] - '0') * 10 + (p[1] - '0');
                else
                    nr = p[0] - '0';

                for (j = 0; j < n; j++)
                    hash(cards[j], search(v, 2), nr);
                break;

            case 'G':
                if (p[1] != '\0')
                    nr = (p[0] - '0') * 10 + (p[1] - '0');
                else
                    nr = p[0] - '0';

                for (j = 0; j < n; j++)
                    hash(cards[j], search(v, 3), nr);
                break;

            case 'O':
                if (p[1] != '\0')
                    nr = (p[0] - '0') * 10 + (p[1] - '0');
                else
                    nr = p[0] - '0';

                for (j = 0; j < n; j++)
                    hash(cards[j], search(v, 4), nr);
                break;
            }
        }

        // Verificam daca s-a facut bingo
        d = points(cards, pt, n);
        if (d)
            return 1;
    }
    return 0;
}

int main()
{
    int n, m, ***cards, i, v[5] = {0, 1, 2, 3, 4};
    int d = 0;
    scanf("%d", &n); // nr de matrici date
    cards = (int ***)calloc(n, sizeof(int **));

    // creem matricile in vector
    for (i = 0; i < n; i++)
    {
        cards[i] = creMat(n);
        citMat(cards[i]);
    }

    scanf("%d", &m); // citim nr de comenzi date
    d = completare(cards, m, n, v);
    if (d)
        return 0;

    printf("NO WINNER\n");
    // eliberam spatiul din memorie
    free(cards);
    return 0;
}
