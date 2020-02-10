#include "widget.h"
#include "ui_widget.h"
#include<QFileDialog>
#include<QMessageBox>
#include<windows.h>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("CherryTree dev Ugex");
    ui->widgetPaintArea->installEventFilter(this);
    ui->spinBoxLevel->setRange(1,100);
    ui->spinBoxLevel->setValue(15);
    ui->spinBoxMainWidth->setRange(1,100);
    ui->spinBoxMainWidth->setValue(8);
    ui->spinBoxMainRate->setRange(2,100);
    ui->spinBoxMainRate->setValue(4);
    ui->spinBoxMinBranch->setRange(1,100);
    ui->spinBoxMinBranch->setValue(2);
    ui->spinBoxMaxBranch->setRange(1,100);
    ui->spinBoxMaxBranch->setValue(2);
    ui->spinBoxWidthSub->setRange(0,100);
    ui->spinBoxWidthSub->setValue(1);
    ui->doubleSpinBoxPruneRate->setRange(0.0,1.0);
    ui->doubleSpinBoxPruneRate->setDecimals(4);
    ui->doubleSpinBoxPruneRate->setSingleStep(0.01);
    ui->doubleSpinBoxPruneRate->setValue(0.0);
    /*
        这里函数指针需要强转：
        QSpinBox::valueChanged
        转换类型为：void(QSpinBox::*)(int)
        推荐转换方式:
        static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged)
    */
    connect(ui->spinBoxMaxBranch,(void(QSpinBox::*)(int))&QSpinBox::valueChanged,this,[=](int val){
        if(val<ui->spinBoxMinBranch->value())
        {
            ui->spinBoxMaxBranch->setValue(ui->spinBoxMinBranch->value());
        }
    });

    connect(ui->spinBoxWidthSub,(void(QSpinBox::*)(int))&QSpinBox::valueChanged,this,[=](int val){
        if(val>ui->spinBoxMainWidth->value())
        {
            ui->spinBoxWidthSub->setValue(ui->spinBoxMainWidth->value());
        }
    });

    connect(ui->pushButtonRePaint,&QPushButton::clicked,this,[=](){
        PaintNewCheeryTree();
        ui->widgetPaintArea->update();
    });
    connect(ui->pushButtonSave,&QPushButton::clicked,this,[=](){
        QString filename=QFileDialog::getSaveFileName(this,"请输入保存图片的文件名");
        if(filename.isEmpty())
        {
            QMessageBox::warning(this,"Warnning","not select file,operate cancel.");
            return;
        }
        cherryTree.getPixmap().save(filename,"PNG");
    });

}

Widget::~Widget()
{
    delete ui;
}
void Widget::PaintNewCheeryTree(){
    int wid=ui->widgetPaintArea->width();
    int hei=ui->widgetPaintArea->height();
    int len=hei*1/ui->spinBoxMainRate->value();
    QPoint pstart(wid/2,hei);
    QPoint pend(wid/2,hei-len);
    long beftime=GetTickCount();
    cherryTree
            .setSize(wid,hei)
            .setLevel(ui->spinBoxLevel->value())
            .setMainBoleWidth(ui->spinBoxMainWidth->value())
            .setBoleSubStep(ui->spinBoxWidthSub->value())
            .setMinBoleBranch(ui->spinBoxMinBranch->value())
            .setMaxBoleBranch(ui->spinBoxMaxBranch->value())
            .setPruneRate(ui->doubleSpinBoxPruneRate->value())
            .drawCherryTree(pstart,pend);
    long nowtime=GetTickCount();
    usetime=nowtime-beftime;
}

void Widget::DrawCherryTree()
{
    QPainter rpainter(ui->widgetPaintArea);
    rpainter.drawPixmap(0,0,cherryTree.getPixmap());
    char buffer[1024]={0};
    sprintf(buffer,"树枝：%ld 树叶：%ld 修剪：%ld 用时：%ldms\0",cherryTree.getCountBole(),cherryTree.getCountLeaf(),cherryTree.getCountPrune(),usetime);
    ui->labelDrawInfo->setText(buffer);
}

bool Widget::eventFilter(QObject * obj, QEvent * e)
{
    if(ui->widgetPaintArea==obj && e->type()==QEvent::Show)
    {
        PaintNewCheeryTree();
    }
    if(ui->widgetPaintArea==obj && e->type()==QEvent::Paint)
    {
        DrawCherryTree();
        return true;
    }
    return QWidget::eventFilter(obj,e);
}


