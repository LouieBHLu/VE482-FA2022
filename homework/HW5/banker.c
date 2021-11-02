#include <stdio.h>


int main(){
    int n, m, i, j, k;
    n = 5;
    m = 3;
    // Allocation Matrix
    int alloc[5][3] = { { 0, 1, 0 }, // P0    
                        { 2, 0, 0 }, // P1
                        { 3, 0, 2 }, // P2
                        { 2, 1, 1 }, // P3
                        { 0, 0, 2 } }; // P4
    // MAX Matrix
    int max[5][3] = { { 7, 5, 3 }, // P0    
                      { 3, 2, 2 }, // P1
                      { 9, 0, 2 }, // P2
                      { 2, 2, 2 }, // P3
                      { 4, 3, 3 } }; // P4
    // Available Resources
    int avail[3] = { 3, 3, 2 }; 
  
    int f[n], ans[n], ind = 0;
    for (k = 0; k < n; k++) {
        f[k] = 0;
    }
    int require[n][m];
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            require[i][j] = max[i][j] - alloc[i][j];
    }
    int y = 0;
    for (k = 0; k < 5; k++) {
        for (i = 0; i < n; i++) {
            if (f[i] == 0) {
                int flag = 0;
                for (j = 0; j < m; j++) {
                    if (require[i][j] > avail[j]){
                        flag = 1;
                         break;
                    }
                }
                if (flag == 0) {
                    ans[ind++] = i;
                    for (y = 0; y < m; y++)
                        avail[y] += alloc[i][y];
                    f[i] = 1;
                }
            }
        }
    }
  
    for (i = 0; i < n - 1; i++)
        printf(" P%d ->", ans[i]);
    printf(" P%d", ans[n - 1]);
  
    return (0);
}