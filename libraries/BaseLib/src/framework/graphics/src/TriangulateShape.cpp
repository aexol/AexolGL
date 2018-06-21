#include <aex/TriangulateShape.hpp>
#include <aex/Logger.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

namespace aex
{
static const float EPSILON = 0.0000000001f;

float TriangulateShape::Area(const Vector3dVector& contour)
{

    int n = contour.size();
    float A = 0.0f;

    for(int p = n - 1, q = 0; q < n; p = q++)
    {
        A += contour[p].x * contour[q].z - contour[q].x * contour[p].z;
    }

    return A * 0.5f;
}

bool TriangulateShape::InsideTriangle(float Ax, float Ay,
                                      float Bx, float By,
                                      float Cx, float Cy,
                                      float Px, float Py)
{
    float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
    float cCROSSap, bCROSScp, aCROSSbp;

    ax = Cx - Bx;
    ay = Cy - By;
    bx = Ax - Cx;
    by = Ay - Cy;
    cx = Bx - Ax;
    cy = By - Ay;
    apx = Px - Ax;
    apy = Py - Ay;
    bpx = Px - Bx;
    bpy = Py - By;
    cpx = Px - Cx;
    cpy = Py - Cy;

    aCROSSbp = ax * bpy - ay * bpx;
    cCROSSap = cx * apy - cy * apx;
    bCROSScp = bx * cpy - by * cpx;

    return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
};

bool TriangulateShape::Snip(const Vector3dVector& contour, int u, int v, int w, int n, int* V)
{
    int p;
    float Ax, Ay, Bx, By, Cx, Cy, Px, Py;

    Ax = contour[V[u]].x;
    Ay = contour[V[u]].z;

    Bx = contour[V[v]].x;
    By = contour[V[v]].z;

    Cx = contour[V[w]].x;
    Cy = contour[V[w]].z;

    if(EPSILON > (((Bx - Ax) * (Cy - Ay)) - ((By - Ay) * (Cx - Ax))))
    {
        return false;
    }

    for(p = 0; p < n; p++)
    {
        if((p == u) || (p == v) || (p == w))
        {
            continue;
        }

        Px = contour[V[p]].x;
        Py = contour[V[p]].z;

        if(InsideTriangle(Ax, Ay, Bx, By, Cx, Cy, Px, Py))
        {
            return false;
        }
    }

    return true;
}

bool TriangulateShape::Process(const Vector3dVector& contour, Vector3dVector& result, std::vector<aex::mesh_indices_type>& indices)
{

    indices.erase(indices.begin(), indices.end());

    int n = contour.size();

    if(n < 3)
    {
        return false;
    }

    int* V = new int[n];

    if(0.0f < Area(contour))
        for(int v = 0; v < n; v++)
        {
            V[v] = v;
        }
    else
        for(int v = 0; v < n; v++)
        {
            V[v] = (n - 1) - v;
        }

    int nv = n;
    int count = 2 * nv;

    for(int m = 0, v = nv - 1; nv > 2;)
    {

        if(0 >= (count--))
        {
            return false;
        }

        int u = v;

        if(nv <= u)
        {
            u = 0;
        }

        v = u + 1;

        if(nv <= v)
        {
            v = 0;
        }

        int w = v + 1;

        if(nv <= w)
        {
            w = 0;
        }

        if(Snip(contour, u, v, w, nv, V))
        {
            int a, b, c, s, t;

            a = V[u];
            b = V[v];
            c = V[w];

            result.push_back(contour[a]);
            result.push_back(contour[b]);
            result.push_back(contour[c]);
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);
            m++;

            for(s = v, t = v + 1; t < nv; s++, t++)
            {
                V[s] = V[t];
            }

            nv--;
            count = 2 * nv;

        }
    }

    delete[] V;
    return true;
}

std::vector<aex::mesh_indices_type> TriangulateShape::Process(const Vector3dVector& contour)
{

    std::vector<aex::mesh_indices_type> indices;
    int n = contour.size();

    if(n < 3)
    {
        return indices;
    }

    int* V = new int[n];

    if(0.0f < Area(contour))
        for(int v = 0; v < n; v++)
        {
            V[v] = v;
        }
    else
        for(int v = 0; v < n; v++)
        {
            V[v] = (n - 1) - v;
        }

    int nv = n;
    int count = 2 * nv;

    for(int m = 0, v = nv - 1; nv > 2;)
    {


        if(0 >= (count--))
        {
            std::vector<aex::mesh_indices_type> ret;
            return ret;
        }

        int u = v;

        if(nv <= u)
        {
            u = 0;
        }

        v = u + 1;

        if(nv <= v)
        {
            v = 0;
        }

        int w = v + 1;

        if(nv <= w)
        {
            w = 0;
        }

        if(Snip(contour, u, v, w, nv, V))
        {
            int a, b, c, s, t;

            a = V[u];
            b = V[v];
            c = V[w];

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);
            m++;

            for(s = v, t = v + 1; t < nv; s++, t++)
            {
                V[s] = V[t];
            }

            nv--;

            count = 2 * nv;
        }
    }



    delete[] V;

    return indices;
}

}
