#include "point_transf.h"
#include <math.h>
#define  N_DIMEN 4 //DIMENSIONALITY

//тип для хранения матриц преобразование в N-мерном пр-ве
typedef double t_matrix[N_DIMEN][N_DIMEN];

//тип для хранения вектора-точки в N-мерном пр-ве
typedef double t_vect[N_DIMEN];


int Mult(t_vect vec, const t_matrix a)
{
    t_vect res = {0};
    for(int i = 0; i < N_DIMEN; i++) {
        for(int j = 0; j < N_DIMEN; j++) {
            res[i] += a[i][j] * vec[j];
        }
    }
    for(int i = 0; i < N_DIMEN; i++) {
        vec[i] = res[i];
    }
    return 0;
}

void Mult_matrix(t_matrix res, const t_matrix a, const t_matrix b)
{
    for(int i = 0; i < N_DIMEN; i++) {
        for(int j = 0; j < N_DIMEN; j++) {
            res[i][j] = 0;
            for(int k = 0; k < N_DIMEN; k++) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void GetResultMatrix(t_matrix a, const Rotate &act)
{
    t_matrix ax = { { 1, 0, 0 },
                       { 0, cos(act.x_angle), -sin(act.x_angle) },
                       { 0, sin(act.x_angle), cos(act.x_angle) } };
    t_matrix ay = { { cos(act.y_angle), 0, sin(act.y_angle) },
                       { 0, 1, 0 },
                       { -sin(act.y_angle), 0, cos(act.y_angle) } };
    t_matrix az = { { cos(act.z_angle), -sin(act.z_angle), 0 },
                       { sin(act.z_angle), cos(act.z_angle), 0 },
                       { 0, 0, 1 } };
    t_matrix tmp;
    Mult_matrix(tmp, ax, ay);
    Mult_matrix(a, tmp, az);

}
void Copy_matrix(t_matrix res, const t_matrix a)
{
    for(int i = 0; i < N_DIMEN; i++) {
        for(int j = 0; j < N_DIMEN; j++) {
            res[i][j] = a[i][j];
        }
    }
}

void GetResultMatrix(t_matrix a, const Scale &act) {
    t_matrix m = { { act.kx, 0, 0 },
                   { 0, act.ky, 0 },
                   { 0, 0, act.kz } };
    Copy_matrix(a, m);
}
int From_vec_to_Point(tPoint &p, const t_vect &vec) {
    p.x = vec[0];
    p.y = vec[1];
    p.z = vec[2];
    return 0;
}
int From_Point_to_vec(t_vect &vec, const tPoint &p) {
    vec[0] = p.x;
    vec[1] = p.y;
    vec[2] = p.z;
    vec[3] = 1;
    return 0;
}

int Change_Point_with_matrix(tPoint &p, const t_matrix &m_rotate) {
    t_vect vec;
    int ret = 0;
    ret = From_Point_to_vec(vec, p);
    if(!ret)
        ret = Mult(vec, m_rotate);

    if(!ret)
        ret = From_vec_to_Point(p, vec);
    return ret;
}
int RotateModel(vector<pair<tPoint, tPoint>> &vect, const Rotate &act)
{
    t_matrix m;
    GetResultMatrix(m, act);
    int ret = 0;
    for(int i = 0; i < vect.size() && !ret; ++i) {
        ret = Change_Point_with_matrix(vect[i].first, m);
        ret = Change_Point_with_matrix(vect[i].second, m);
    }
   return ret;
}

int ScaleModel (vector<pair<tPoint, tPoint>> &vect, const Scale &act)
{
    t_matrix m;
    GetResultMatrix(m, act);
    int ret = 0;
    for(int i = 0; i < vect.size() && !ret; ++i) {
        ret = Change_Point_with_matrix(vect[i].first, m);
        ret = Change_Point_with_matrix(vect[i].second, m);
    }
   return ret;
}
