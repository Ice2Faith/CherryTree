#ifndef CHERRYTREE_H
#define CHERRYTREE_H
/*
绘制一棵树：如果需要实现自己的特性，可以继承后重写虚函数protected
使用方法：
QPixmap map=CherryTree(wid,hei)
.setLevel(level)
.setMainBoleWidth(width)
.setBackgroundColor(QColor(r,g,b))
.drawCherryTree(startPoint,endPoint);
然后在QPainter中.drawPixmap(posx,posy,map)即可。
*/
#include<QPainter>
#include<QPaintDevice>
#include<QPixmap>
#include<QImage>
#include<math.h>
#include<time.h>
class CherryTree
{
public:
    //产生一个这样大小的画布
    CherryTree(int width=720,int height=480);
    ~CherryTree();
    CherryTree & setSize(int width,int height);
    int getWidth();
    int getHeight();
    //设置树的高度层次
    CherryTree & setLevel(int level);
    int getLevel();
    //设置主干的宽度
    CherryTree & setMainBoleWidth(int width);
    int getMainBoleWidth();
    //设置背景颜色
    CherryTree & setBackgroundColor(QColor color);
    QColor getBackgroundColor();
    //设置树干宽度递减步数
    CherryTree & setBoleSubStep(int step);
    int getBoleSubStep();
    //设置树枝最大最小分支数量
    CherryTree & setMaxBoleBranch(int maxBranch);
    int getMaxBoleBranch();
    CherryTree & setMinBoleBranch(int minBranch);
    int getMinBoleBranch();
    //设置裁枝率（精度：万分之）
    CherryTree & setPruneRate(double rate);
    double getPruneRate();
    //获取绘制的图片
    QPixmap getPixmap();
    //绘制一张树的图片并返回，需要指定主干的起点和终点
    QPixmap drawCherryTree(QPoint startPoint,QPoint endPoint);
    //计算两点构成的直线的弧度
    double getLineDirection(const QPoint & startPoint,const QPoint & endPoint);
    //根据直线长度和方向，计算相对起点的终点
    QPoint getEndPoint(const QPoint & startPoint,int length,double direction);
    //弧度角度转换
    double AngleToRadian(double angle);
    double RadianToAngle(double radian);
    //获取绘制次数计数
    long getCountBole();
    long getCountLeaf();
    long getCountPrune();
private:
    long m_countBole;
    long m_countLeaf;
    long m_countPrune;
    int m_width;//画布尺寸
    int m_height;
    int m_level;//最大树高度层次
    int m_BoleWidth;//最大数粗度
    int m_widthSubStep;//每个树干递减的宽度
    int m_maxBoleBranch;//最大最小树枝数量
    int m_minBoleBranch;
    double m_pruneRate;//树枝剪裁率
    QColor m_bgColor;//画布背景颜色
    QPixmap m_map;//画布
    void InitDefaultArgument();//初始化默认参数
    void CreateMap();//创建画布
    void GCherryTree(QPainter * m_painter,QPoint pstart,QPoint pend,int width,int level);//绘制树入口
    void GCherryTreeNext(QPainter * m_painter,QPoint pstart,int length,double direct,int width,int level);//递归绘制
protected:
    virtual int getSubBoleLength(int length);//根据树的上一个树枝长度，计算返回这次绘制的树枝长度
    virtual double getSubBoleDeflectionAngle();//返回本次绘制的树枝和上一次树枝形成的夹角弧度
    virtual bool needPruneBole(double pruneRate);//返回是否需要进行裁枝
    virtual void drawPruneBole(QPainter * painter,QPoint endPoint,int width);//绘制被裁剪的树枝，给出裁剪点
    virtual void drawBefore(QPainter * painter);//在绘制树之前的绘制
    virtual void drawAfter(QPainter * painter);//在树绘制完毕之后的绘制
    virtual void drawBole(QPainter * painter,QPoint startPoint,QPoint endPoint,int boleWidth);//绘制树干，给出了起点终点和树干宽度
    virtual void drawLeaf(QPainter * painter,QPoint endPoint);//绘制终端叶子点，给出了终点
};

#endif // CHERRYTREE_H
