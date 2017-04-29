#include "algo.h"
#include <vector>
#include <stack>
#include "paintscene.h"

using namespace std;

void FindNewPixels(paintScene *scene, const QColor &colorBrush,
                   stack<tPoint> &st, int xl, int xr, int y)
{
    tPoint tmp(xl, y);
    bool flag;
    while(tmp.x < xr) {
        flag = 0;
        while(tmp.x <= xr && scene->getPixelColor(tmp) != colorBrush
                          && scene->getPixelColor(tmp) != scene->colorLine)
        {
            flag = 1;
            tmp.x++;
        }
        if(flag == 1) {
            if(tmp.x <= xr && scene->getPixelColor(tmp) != colorBrush
                          && scene->getPixelColor(tmp) != scene->colorLine)
            {
                st.push(tmp);
            }
            else {
                st.push(tPoint(tmp.x - 1, y));
            }
        }
        tmp.x++;
    }

}

int SimpleAlgo(paintScene *scene, const QColor &colorBrush, int timePause)
{
    stack<tPoint> st;
    tPoint tmp, zt;
    int x, y;
    int xright, xleft;
    st.push(tPoint(scene->pixel));
    while(!st.empty()) {
        zt = st.top();
        //qDebug() << zt.x << zt.y;
        st.pop();
        if(scene->getPixelColor(zt) == colorBrush) {
            continue;
        }
        tmp = zt;
        while(scene->getPixelColor(tmp) != scene->colorLine) {
            scene->addPixelImage(tmp, colorBrush);
            tmp.x++;
        }
        xright = tmp.x - 1;
        tmp = zt;
        while(scene->getPixelColor(tmp) != scene->colorLine) {
            scene->addPixelImage(tmp, colorBrush);
            tmp.x--;
        }
        xleft = tmp.x + 1;
        FindNewPixels(scene, colorBrush, st, xleft, xright, zt.y - 1);
        //xleft = 0;
        FindNewPixels(scene, colorBrush, st, xleft, xright, zt.y + 1);
    }
    return 0;
}
