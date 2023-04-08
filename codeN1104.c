#include <stdio.h>

struct SoccerTeam {
    char name[50];
    int revenueThisYear;
    long revenueOverall;
};

struct SoccerTeam arsenalFC = {.name = "Arsenal", .revenueOverall = 12300000, .revenueThisYear = 2300000};
struct SoccerTeam barcelonaFC = {.name = "Barcelona", .revenueOverall = 1220000, .revenueThisYear = 9900000};
struct SoccerTeam parisSG = {.name = "Paris St", .revenueOverall = 1220000, .revenueThisYear = 55200000};

// changed ret type and revenueThisYear due to overflow from long to short assignment
long calculateSoccerRevenue(long revenueTillDate, int revenueThisYear) {
    return revenueTillDate + revenueThisYear;
}

int main() {

    struct SoccerTeam teams[] = {arsenalFC, barcelonaFC, parisSG};
    // variable type from short to long
    long maxRevenue = 0;
    // initialized maxRevenueTeam
    char *maxRevenueTeam = NULL;
    for (int i = 0; i < 3; i++) {
        long totalRevenue = calculateSoccerRevenue(teams[i].revenueOverall, teams[i].revenueThisYear);
        if (totalRevenue > maxRevenue) {
            maxRevenue = totalRevenue;
            maxRevenueTeam = teams[i].name;
        }
        printf("%s: %ld\n", teams[i].name, totalRevenue);
    }
    // %d to %ld to correctly assign long type
    printf("Team with max revenue (%ld) is: %s\n", maxRevenue, maxRevenueTeam);
    return (0);
}
