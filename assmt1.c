/* Cynthia Zhang 
 * Created on 20/04/2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define DIV "==========" /* stage header */
/* stage numbers */
#define STAGE_NUM_ONE 1
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_NUM_FIVE 5

#define READ_OK 1 /* reading point succeeded */
#define READ_FAILED 0 /* reading point failed */

#define MAX_NUM_POINTS 99 /* maximum number of points */
#define MAX_NUM_DIMENSIONS 10 /* maximum number of dimensions */

#define NORMALISE_COORDINATE_SUM 100 /* divides coordinates' sums in stage two */
#define DIV_BY_TEN 10 /* divides coordinates' sums in stage two */
#define NOT_DOMINTAE -1 /* flag to indicate a point is domintaed in stage 5 */

typedef double point_t[MAX_NUM_DIMENSIONS];

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* function prototypes */
void stage_one(point_t one_point, int* num_points, int num_dimensions);
void stage_two(point_t points[], int* num_points, int num_dimensions,
    double* coordinate_sums);
void stage_three(double* coordinate_sums, int num_points);
void stage_four(point_t points[], int num_points, int num_dimensions);
void stage_five(point_t points[], int num_points, int num_dimensions);

void print_stage_heading(int stage);
int read_one_point(point_t one_point, int num_dimensions);
void print_format(int point_id, int num_dimensions, point_t points[]);
void find_skyline_points(point_t points[], int num_points, int num_dimensions,
    int flag_points[], int dominate_num[]);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* main program binds it all together */
int main(int argc, char* argv[])
{

    /* storage for the input data */
    int num_dimensions;
    point_t points[MAX_NUM_POINTS];

    /* storage for statistics */
    int num_points = 0; /* number of points read in */
    double coordinate_sums[MAX_NUM_POINTS]; /* coordinate sums of the points */

    /* read the first line - number of dimensions */
    scanf("%d", &num_dimensions);

    /* stage 1 */
    stage_one(points[0], &num_points, num_dimensions);

    /* stage 2 */
    stage_two(points, &num_points, num_dimensions,
        coordinate_sums);

    /* stage 3 */
    stage_three(coordinate_sums, num_points);

    /* stage 4 */
    stage_four(points, num_points, num_dimensions);

    /* stage 5 */
    stage_five(points, num_points, num_dimensions);

    /* and done! */
    return 0;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* print out a stage heading, with a prior blank line once the stage number
   goes plus one 
*/
void print_stage_heading(int stage)
{
    /* main job is to print the stage number passed as an argument */
    if (stage > STAGE_NUM_ONE) {
        /* separate the stages */
        printf("\n");
    }
    printf("Stage %d\n%s\n", stage, DIV);
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* read the coordinates of one point, store them in one_point
*/
int read_one_point(point_t one_point, int num_dimensions)
{
    int i = 0;

    /* keep reading until num_dimensions numbers have been obtained */
    while (i < num_dimensions) {
        if (scanf("%lf", one_point + i) != 1) {
            return READ_FAILED;
        }
        i++;
    }

    return READ_OK;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* stage 1 - read in and print one point
*/
void stage_one(point_t one_point, int* num_points, int num_dimensions)
{

    /* print stage heading for stage 1 */
    print_stage_heading(STAGE_NUM_ONE);

    int i;

    /* read in all coordinates' values in point 1 */
    read_one_point(one_point, num_dimensions);

    *num_points += 1;

    /* print values of point1 in the required format */
    printf("Point 01: <");

    for (i = 0; i < num_dimensions; i++) {
        printf("%.2lf", one_point[i]);
        if (i != num_dimensions - 1) {
            printf(", ");
        }
    }
    printf(">\n");
    return;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* stage 2 - read in the rest of the points and visualise them
*/
void stage_two(point_t points[], int* num_points, int num_dimensions,
    double* coordinate_sums)
{

    /* print stage heading for stage 2 */
    print_stage_heading(STAGE_NUM_TWO);

    /* read in all points and store coordinats' values in points */
    /* one_point is an array that stores each point's coordinates */
    point_t one_point;
    int i, j = 1, k = 0;

    while (read_one_point(one_point, num_dimensions)) {
        *num_points += 1;
        for (i = 0; i < num_dimensions; i++) {
            points[j][i] = one_point[i];
        }
        j++;
    }

    /* calculate the sum of coordinates of each point */
    for (i = 0; i < *num_points; i++) {
        coordinate_sums[i] = 0;
        for (j = 0; j < num_dimensions; j++) {
            coordinate_sums[i] += points[i][j];
        }
    }

    /* print out visualised sum values divided by 100 in the required format */

    while (k < *num_points) {
        printf("Point %02d, sum of coordinates (/100): ", k + 1);
        printf("%5.2lf |", coordinate_sums[k] / NORMALISE_COORDINATE_SUM);
        for (i = 1; i < coordinate_sums[k] / NORMALISE_COORDINATE_SUM + 1; i++) {
            if (i % DIV_BY_TEN != 0) {
                printf("-");
            }
            else if (i % DIV_BY_TEN == 0) {
                printf("+");
            }
        }
        printf("\n");
        k++;
    }
    return;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* stage 3 - print statistics
*/
void stage_three(double* coordinate_sums, int num_points)
{

    /* print stage heading for stage 3 */
    print_stage_heading(STAGE_NUM_THREE);

    double max = 0.0;
    int max_point = 0, i;

    /* print total number of points */
    printf("Total: %d points\n", num_points);

    /* find the largest sum of all coordinates */
    for (i = 0; i <= num_points; i++) {
        if (coordinate_sums[i] > max) {
            max = coordinate_sums[i];
            max_point = i + 1;
        }
    }

    /* print the point with largest sum and value of the largest sum */
    printf("Point of largest sum of coordinates: %02d\n", max_point);
    printf("Largest sum of coordinates: %.2lf\n", max);
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* stage 4 - find and print the skyline points
*/
void stage_four(point_t points[], int num_points, int num_dimensions)
{

    /* print stage heading for stage 4 */
    print_stage_heading(STAGE_NUM_FOUR);

    int i;
    int flag_points[num_points], dominate_num[num_points];

    /* find all skyline points and store them in the array flag_points */
    find_skyline_points(points, num_points,
     num_dimensions, flag_points, dominate_num);

    printf("Skyline points:\n");

    /* print all coordinates' values of all skyline points*/
    for (i = 0; i < num_points; i++) {
        if (flag_points[i] == 0) {
            print_format(i, num_dimensions, points);
        }
    }
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* stage 5 - sort and print the skyline points by the number of points 
   they dominate
*/
void stage_five(point_t points[], int num_points, int num_dimensions)
{
    /* print stage heading for stage 5 */

    print_stage_heading(STAGE_NUM_FIVE);

    int i, j, max_point, max, max_is_set;
    int flag_points[num_points], dominate_num[num_points], mark[num_points];

    /* find all skyline points and store them in the array dominate_num */
    find_skyline_points(points, num_points,
     num_dimensions, flag_points, dominate_num);

    /* initialise all mark to be zero */
    memset(mark, 0, sizeof(int) * num_points);
    
    /* print all skyline points */
    printf("Sorted skyline points:\n");
    for (i = 0; i < num_points; i++) {
        max_is_set = 0;

        /* find the next maximum dominating point */
        for (j = 0; j < num_points; j++) {

            /*ensure maximum value has not been printed out before*/
            if (mark[j] == 0 && max_is_set == 0) {
                max = dominate_num[j];
                max_point = j;
                max_is_set = 1;
            }

            /* update max to keep track of the current max */
            else if (max_is_set && dominate_num[j] > max && mark[j] == 0) {
                max = dominate_num[j];
                max_point = j;
            }
        }

        /* stop if all skyline points has been printed */
        if (max == NOT_DOMINTAE) {
            break;
        }
        /* print the newly found skyline point */
        print_format(max_point, num_dimensions, points);
        printf("Number of points dominated: %d\n", max);

        /* mark this point as printed so it will not be printed again */
        mark[max_point] = 1;
    }
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* print all coordinates of a point by required format */
void print_format(int point_id, int num_dimensions, point_t points[])
{
    int j;

    printf("Point %02d: <", point_id + 1);
    for (j = 0; j < num_dimensions; j++) {
        printf("%.2lf", points[point_id][j]);
        if (j != num_dimensions - 1) {
            printf(", ");
        }
    }
    printf(">\n");
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* find all skyline points */
void find_skyline_points(point_t points[], int num_points,
    int num_dimensions, int flag_points[], int dominate_num[])
{

    int i, j, k, count, count_dominate;

    /* initialise array */
    memset(dominate_num, 0, sizeof(int) * num_points);

    /*find all points that are dominated*/
    for (i = 0; i < num_points; i++) {
        for (j = 0; j < num_points; j++) {
            if (i != j) {
                count = 0;
                count_dominate = 0;
                for (k = 0; k < num_dimensions; k++) {

                    /* if one dimension is dominated, proceed to the next 
					dimension */
                    if (points[i][k] <= points[j][k]) {
                        /* increase count by 1 every time one dimension of 
						the point is dominated */
                        count++;
                    }

                    /* increase count_dominate by 1 every time current
                     point's current dimension dominates */
                    if (points[i][k] >= points[j][k]) {
                        count_dominate++;
                    }
                }

                /* if all dimensions of that point are dominated, this point is 
				dominated */
                if (count == num_dimensions) {

                    /* mark this point in all array records as dominated */
                    flag_points[i] = 1;
                    dominate_num[i] = NOT_DOMINTAE;
                    break;
                }

                /* otherwise just initilise flag_points as 0 */
                if (count != num_dimensions) {
                    flag_points[i] = 0;
                }

                /* increment dominate_num if it dominates another point */
                if (count_dominate == num_dimensions) {
                    dominate_num[i]++;
                }
            }
        }
    }
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* algorithms are fun! */
