#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
        QStandardItemModel *model = new QStandardItemModel(this);
        QStringList list;
        list<<QStringLiteral("属性")<<QStringLiteral("值");
        model->setHorizontalHeaderLabels(list);//列表头部
        model->horizontalHeaderItem(0);

        model->setColumnCount(list.size());//列数


        ui->treeView->setModel(model);
        ui->treeView->setColumnWidth(0,250);//列宽
        ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);//不可编辑
        //--

        /*谭创*/

        //TC

        QHeaderView *hv = ui->treeView->header();
        hv->setSortIndicatorShown(true); //显示排序三角
        hv->setSectionsClickable(true);     //设置标题可以被单击
        connect(hv,SIGNAL(sectionClicked(int)),this,SLOT(slotSectionClicked(int)));
        initTreeView();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotTreeViewHeaderClicked(const QModelIndex &index)
{
//    SCDebug<<index.row()<<index.column();
}
InfoMapData* MainWindow ::addLevel(const QString &displayText0,
                                          const QString &displayText1,
                                          const QString &toolTip,
                                          InfoMapData *parent,
                                          const QString &key,
                                          const QVariant &var
                                          )
{
    InfoMapData*data = new InfoMapData;
    data->key = key;
    data->displayText0 = displayText0;
    data->value = var;
    data->displayText1 = displayText1;
    if(toolTip.isEmpty()){
        //        data->toolTip = data->text;
    }else{
        data->toolTip = toolTip;
    }
    if(parent){
        data->level = 1;
        parent->child.append(data);
    }else{
        data->level = 0;
    }
    return data;
}
static QVariant getVar(InfoMapData*data)
{
    QVariant v;
    v.setValue(data);
    return v;
}


void MainWindow::slot_robot_status_all1_data(const QString &ip,  const RobotData *info)
{
    _mutex.lock();
    initTreeView();
    _mutex.unlock();
}
/** double转string保留小数位
 * @brief MainWindow::getDecimalPlaces
 * @param d
 * @param countBit 保留几位小数，默认3位
 * @return
 */
QString MainWindow::getDecimalPlaces(double d,int countBit)
{
    return QString::number(d,'f',countBit);
}
void MainWindow::initTreeView()
{
    QList<InfoMapData*>InfoMapDataList;
    //1 机器人运行信息
    InfoMapDataList.append(addLevel(QStringLiteral("机器人运行信息")));
    addLevel(QStringLiteral("运行模式"),"1","",InfoMapDataList.last());
    addLevel(QStringLiteral("累计里程(m)"),"2",QStringLiteral("机器人累计行驶里程(单位:m)"),InfoMapDataList.last());
    addLevel(QStringLiteral("累计运行时间"),"3",QStringLiteral("机器人累计运行时间"),InfoMapDataList.last());
    addLevel(QStringLiteral("本次运行时间"),"4",QStringLiteral("机器人本次运行时间"),InfoMapDataList.last());
    addLevel(QStringLiteral("定位状态"),"5",tr(""),InfoMapDataList.last());
    addLevel(QStringLiteral("地图载入状态"),"6",tr(""),InfoMapDataList.last());
    addLevel(QStringLiteral("扫图状态"),"7",tr(""),InfoMapDataList.last());
    addLevel(QStringLiteral("控制器温度(℃)"),"8",tr(""),InfoMapDataList.last());
    addLevel(QStringLiteral("控制器湿度(%)"),"9",tr(""),InfoMapDataList.last());
    addLevel(QStringLiteral("控制器电压(V)"),"10",tr(""),InfoMapDataList.last());


    //2

    //0
    InfoMapDataList.append(addLevel(QStringLiteral("机器人信息")));
    addLevel(QStringLiteral("ID"),"info->id",QStringLiteral("机器人ID"),InfoMapDataList.last());
    addLevel(QStringLiteral("型号"),"info->model",QStringLiteral("机器人型号"),InfoMapDataList.last());
    addLevel(QStringLiteral("Robokit版本"),"info->version",tr(""),InfoMapDataList.last());

    addLevel(QStringLiteral("固件版本"),"info->dsp_version",QStringLiteral("固件版本"),InfoMapDataList.last());
    addLevel(QStringLiteral("当前加载的地图"),"info->current_map",QStringLiteral("当前加载的地图"),InfoMapDataList.last());
    addLevel(QStringLiteral("地图版本"),"info->map_version",QStringLiteral("地图版本"),InfoMapDataList.last());
    addLevel(QStringLiteral("模型版本"),"info->model_version",QStringLiteral("模型版本"),InfoMapDataList.last());
    addLevel(QStringLiteral("网络协议版本"),"info->netprotocol_version",QStringLiteral("网络协议版本"),InfoMapDataList.last());


//    InfoMapDataList.append(addLevel(tr("Warning")));
//    rbkWarningCode(info->warnings,InfoMapDataList);

//    InfoMapDataList.append(addLevel(tr("Fatals")));
//    rbkWarningCode(info->fatals,InfoMapDataList);

//    InfoMapDataList.append(addLevel(tr("Errors")));
//    rbkWarningCode(info->errors,InfoMapDataList);


    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->treeView->model());

    if(model->rowCount()==0){
        for(int i=0;i<InfoMapDataList.size();++i)
        {
            auto var = InfoMapDataList.at(i);
            QStandardItem *item = new QStandardItem(var->displayText0);
            for(int j=0;j<var->child.size();++j)
            {
                auto childVar = var->child.at(j);
                QStandardItem *childItem0 = new QStandardItem(childVar->displayText0);
                childItem0->setToolTip(childVar->displayText0);
                QStandardItem *childItem1 = new QStandardItem(childVar->displayText1);
                childItem1->setToolTip(childVar->displayText1);
                QList<QStandardItem*>childItemList;
                childItemList<<childItem0<<childItem1;
                item->appendRow(childItemList);
            }
            model->appendRow(item);
            QModelIndex index = ui->treeView->model()->index(i,0);
            ui->treeView->setExpanded(index,_isExpanded);
        }
    }else{
        for(int i=0;i<InfoMapDataList.size();++i){
            QModelIndex index = model->index(i,0);
            auto var = InfoMapDataList.at(i);
            for(int j=0;j<var->child.size();++j){
                auto childVar = var->child.at(j);
                QModelIndex childIndex = index.child(j,1);
                QStandardItem *childItem1 = model->itemFromIndex(childIndex);
                if(!childItem1){
//                    SCWarning<<"error: QStandardItem is empty."<<i<<j<<childIndex;
                    continue;
                }
                childItem1->setText(childVar->displayText1);
                childItem1->setToolTip(childVar->displayText1);
            }
        }
    }

}

void MainWindow::rbkWarningCode(const QVariantList & list,QList<InfoMapData*>&infoMapDataList)
{
    for(int i=0;i<list.size();++i){
        QVariantMap map = list.at(i).toMap();
        for(int j = 0;j<map.keys().size();++j){

            QString  sKey =  map.keys().at(j);
            int key = sKey.toInt();
            qint64 time64 = map.value(sKey).toLongLong();
            QString rbkTime = QString::number(time64);
            QString rbkStr = QString::number(key);
            addLevel(QString("[%1][%2]").arg(rbkTime).arg(key),rbkStr,rbkStr,infoMapDataList.last());
        }
    }
}
void MainWindow::slotSectionClicked(int column)
{
//    SCDebug<<column;
    if(0 == column){
        slotExpandedClicked(_isPressedSection);//放大
        _isPressedSection = !_isPressedSection;//
    }
}
//递归展开
void MainWindow::recursiveCollapseExpand(const QModelIndex index,bool isExpanded)
{
    int rowCount = ui->treeView->model()->rowCount(index);

    for(int childNo=0; childNo<rowCount; ++childNo)
    {
        QModelIndex childIndex = index.child(childNo,0);
        if(ui->treeView->isExpanded(childIndex))
        {
            ui->treeView->setExpanded(childIndex,isExpanded);
            int childCount = ui->treeView->model()->rowCount(childIndex);
            if(childCount>0){
                recursiveCollapseExpand(childIndex,isExpanded);
            }
        }
    }
}
void MainWindow::slotExpandedClicked(bool checked)
{
    if(ui->treeView->model()){
        for(int i=0;i<ui->treeView->model()->rowCount();++i){
            QModelIndex index = ui->treeView->model()->index(i,0);
            ui->treeView->setExpanded(index,!checked);
        }
    }
}
