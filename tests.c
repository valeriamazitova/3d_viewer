#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "3d-viewer/parser.h"

START_TEST(viewer) {
    FILE * throw = NULL;
    char *file = "obj/cube.obj";
    viewer3D* obj = (viewer3D*) malloc(sizeof(viewer3D));
    if (obj != NULL) {
        obj->sizeArrayPoint = 1;
        obj->lastIndexPoint = 0;
        obj->arrayPoint = (double*)malloc(sizeof(double));
        obj->sizeArrayFacete = 1;
        obj->lastIndexFacete = 0;
        obj->arrayFacete = (int*)malloc(sizeof(int));
        obj->countFacete = 0;
    }
    if (!openFile(file, &throw)) {
        int result = parse(throw, obj);
        fclose(throw);
        if (!result) {
            int facets_count_check = 10;
            int vertex_count_check = 8;
            int arr_of_facets_check[60] = {0, 6, 6, 4, 4, 0, 0, 2, 2, 6, 6, 0, 0, 3, 3,
                                           2, 2, 0, 0, 1, 1, 3, 3, 0, 2, 7, 7, 6, 6, 2,
                                           2, 3, 3, 7, 7, 2, 4, 6, 6, 7, 7, 4, 4, 7, 7,
                                           5, 5, 4, 0, 4, 4, 5, 5, 0, 0, 5, 5, 1, 1, 0};
            double arr_of_vertexes_check[24] = {
                    -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0,
                    -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0};
            ck_assert_int_eq(facets_count_check, obj->countFacete);
            ck_assert_int_eq(vertex_count_check, obj->lastIndexPoint / 3);
            for (int i = 0; i < facets_count_check; i++) {
                ck_assert_int_eq(arr_of_facets_check[i], obj->arrayFacete[i]);
            }
            for (int i = 0; i < vertex_count_check; i++) {
                ck_assert_int_eq(arr_of_vertexes_check[i], obj->arrayPoint[i]);
            }
            movingCoordinatePoint(obj, 1.0, 1.0, 1.0);
            double move_of_vertexes_check[24] = {
                    0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 2.0, 0.0, 0.0, 2.0, 2.0, 2.0, 0.0,
                    0.0, 2.0, 0.0, 2.0, 2.0, 2.0, 0.0, 2.0, 2.0, 2.0};
            for (int i = 0; i < vertex_count_check; i++) {
                ck_assert_int_eq(move_of_vertexes_check[i], obj->arrayPoint[i]);
            }
            scaleViewer(obj, 2);
            double scale_of_vertexes_check[24] = {
                    0.0, 0.0, 0.0, 0.0, 0.0, 4.0, 0.0, 4.0, 0.0, 0.0, 4.0, 4.0, 4.0, 0.0,
                    0.0, 4.0, 0.0, 4.0, 4.0, 4.0, 0.0, 4.0, 4.0, 4.0};
            for (int i = 0; i < vertex_count_check; i++) {
                ck_assert_int_eq(scale_of_vertexes_check[i], obj->arrayPoint[i]);
            }
            rotateViewer(obj, 360, 'y');
            rotateViewer(obj, 360, 'z');
            rotateViewer(obj, 360, 'x');
            double rotate_of_vertexes_check[27] = {
                    0.0, 0.0, 0.0, 0.0, 0.0, 4.0, 0.0, 4.0, 0.0, 0.0, 4.0, 4.0, 4.0, 0.0,
                    0.0, 4.0, 0.0, 4.0, 4.0, 4.0, 0.0, 4.0, 4.0, 4.0};
            for (int i = 0; i < vertex_count_check; i++) {
                ck_assert_int_eq(rotate_of_vertexes_check[i], obj->arrayPoint[i]);
            }
        }
    }
    free(obj->arrayPoint);
    free(obj->arrayFacete);
    free(obj);
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("viewer");
    TCase *tc = tcase_create("viewer");
    SRunner *sr = srunner_create(s1);
    int result = 0;
    suite_add_tcase(s1, tc);
    tcase_add_test(tc, viewer);
    srunner_run_all(sr, CK_ENV);
    result = srunner_ntests_failed(sr);
    srunner_free(sr);
    return result == 0 ? 0 : 1;
}
