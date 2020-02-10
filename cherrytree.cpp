#include "cherrytree.h"
CherryTree::CherryTree(int width,int height)
{
    InitDefaultArgument();
    setSize(width,height);
}

CherryTree::~CherryTree()
{

}

void CherryTree::InitDefaultArgument()
{
    m_level=15;
    m_BoleWidth=8;
    m_bgColor=QColor(255,255,255);
    m_widthSubStep=1;
    m_minBoleBranch=2;
    m_maxBoleBranch=2;
    m_pruneRate=0;
    m_countBole=0;
    m_countLeaf=0;
    m_countPrune=0;
}
void CherryTree::CreateMap()
{
    m_map=QPixmap(m_width,m_height);
    m_map.fill(m_bgColor);
}


CherryTree & CherryTree::setSize(int width,int height)
{
    m_width=width;
    m_height=height;
    return *this;
}
int CherryTree::getWidth()
{
    return m_width;
}
int CherryTree::getHeight()
{
    return m_height;
}
CherryTree & CherryTree::setLevel(int level)
{
    if(level>=1)
        m_level=level;
    return *this;
}
int CherryTree::getLevel()
{
    return m_level;
}
CherryTree & CherryTree::setMainBoleWidth(int width)
{
    if(width>0)
        m_BoleWidth=width;
    return *this;
}
int CherryTree::getMainBoleWidth()
{
    return m_BoleWidth;
}
CherryTree & CherryTree::setBackgroundColor(QColor color)
{
    m_bgColor=color;
    return *this;
}

QColor CherryTree::getBackgroundColor()
{
    return m_bgColor;
}

CherryTree & CherryTree::setBoleSubStep(int step)
{
    if(step>=0)
        m_widthSubStep=step;
    return *this;
}

int CherryTree::getBoleSubStep()
{
    return m_widthSubStep;
}

CherryTree & CherryTree::setMaxBoleBranch(int maxBranch)
{
    if(maxBranch>=m_minBoleBranch)
        m_maxBoleBranch=maxBranch;
    return *this;
}

int CherryTree::getMaxBoleBranch()
{
    return m_maxBoleBranch;
}

CherryTree & CherryTree::setMinBoleBranch(int minBranch)
{
    if(minBranch>0)
        m_minBoleBranch=minBranch;
    return *this;
}

int CherryTree::getMinBoleBranch()
{
    return m_minBoleBranch;
}
CherryTree & CherryTree::setPruneRate(double rate)
{
    if(rate>=0.0 && rate<=1.0)
        m_pruneRate=rate;
    return *this;
}

double CherryTree::getPruneRate()
{
    return m_pruneRate;
}

QPixmap CherryTree::getPixmap()
{
    return m_map;
}
QPixmap CherryTree::drawCherryTree(QPoint startPoint,QPoint endPoint)
{
    CreateMap();
    QPainter painter(&m_map);
    drawBefore(&painter);
    GCherryTree(&painter,startPoint,endPoint,m_BoleWidth,m_level);
    drawAfter(&painter);
    return m_map;
}
double CherryTree::getLineDirection(const QPoint & startPoint,const QPoint & endPoint)
{
    QLine line(startPoint,endPoint);
    return atan2(line.dy()*1.0,line.dx()*1.0);
}

QPoint CherryTree::getEndPoint(const QPoint & startPoint,int length,double direction)
{
    return QPoint(startPoint.x()+length*cos(direction),startPoint.y()+length*sin(direction));
}

double CherryTree::AngleToRadian(double angle)
{
    return angle/180*3.1415926535;
}

double CherryTree::RadianToAngle(double radian)
{
    return radian/3.1415926535*180;
}
long CherryTree::getCountBole()
{
    return m_countBole;
}

long CherryTree::getCountLeaf()
{
    return m_countLeaf;
}

long CherryTree::getCountPrune()
{
    return m_countPrune;
}

void CherryTree::GCherryTree(QPainter * painter,QPoint pstart,QPoint pend,int width,int level)
{
    m_countBole=1;
    m_countLeaf=0;
    m_countPrune=0;
    srand((unsigned int)time(NULL));
    drawBole(painter,pstart,pend,width);
    double direct=getLineDirection(pstart,pend);
    QLine line(pstart,pend);
    GCherryTreeNext(painter,pend,sqrt(pow(line.dx(),2.0)+pow(line.dy(),2.0)),direct,width-m_widthSubStep,level-1);
}

void CherryTree::GCherryTreeNext(QPainter * painter,QPoint pstart,int length,double direct,int width,int level)
{
    if(level==0)
    {
        m_countLeaf++;
        drawLeaf(painter,pstart);
        return;
    }
    if(width<=0)
        width=1;

    int count=m_maxBoleBranch==m_minBoleBranch? m_minBoleBranch:rand()%(m_maxBoleBranch-m_minBoleBranch)+m_minBoleBranch;
    for(int i=0;i<count;i++)
    {
        if(needPruneBole(m_pruneRate))
        {
            m_countPrune++;
            drawPruneBole(painter,pstart,width);
            continue;
        }
        int plen=getSubBoleLength(length);
        double pfactor=getSubBoleDeflectionAngle();
        if(i%2)
        {
            pfactor=0.0-pfactor;
        }
        double pdirect=direct-pfactor;
        QPoint pend=getEndPoint(pstart,plen,pdirect);
        m_countBole++;
        drawBole(painter,pstart,pend,width);
        GCherryTreeNext(painter,pend,plen,pdirect,width-m_widthSubStep,level-1);
    }
}


int CherryTree::getSubBoleLength(int length)
{
    return length*(rand()%5+5)/10;
}

double CherryTree::getSubBoleDeflectionAngle()
{
    return (rand()%7+1)/10.0;
}

bool CherryTree::needPruneBole(double pruneRate)
{
    return rand()%100000<(pruneRate*100000);
}

void CherryTree::drawPruneBole(QPainter * painter,QPoint endPoint,int width)
{
    painter->setPen(QColor(0,0,255));
    painter->drawEllipse(endPoint,width,width);
}

void CherryTree::drawBefore(QPainter * painter)
{

}

void CherryTree::drawAfter(QPainter * painter)
{

}

void CherryTree::drawBole(QPainter * painter,QPoint startPoint,QPoint endPoint,int boleWidth)
{
    QPen pen(QBrush(QColor(rand()%60,rand()%120,rand()%60)),boleWidth);
    painter->setPen(pen);
    painter->drawLine(startPoint,endPoint);
}

void CherryTree::drawLeaf(QPainter * painter,QPoint endPoint)
{
    if(rand()%100<5)
    {
        painter->setPen(QColor(rand()%55+200,rand()%150+100,0));
        painter->drawEllipse(endPoint,rand()%4+2,rand()%4+2);
    }else
    {
        painter->setPen(QColor(rand()%55+200,rand()%100,rand()%155+100));
        painter->drawEllipse(endPoint,rand()%3+1,rand()%3+1);
    }
}
