#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMutex>
struct RobotData
{
    double odo;                                            //累计行驶里程, 单位 m
    qint64 time;                                            //本次运行时间, 单位 ms
    qint64 total_time;                                   //累计运行时间, 单位 ms
    double controller_temp;                        //控制器温度, 单位 ℃
    double controller_humi;                        //控制器湿度, 单位 %
    double controller_voltage;                    //控制器电压, 单位 V
//    RobotInterface::OperatingModeStatus mode;//运行状态 0手动 1自动
    QString mode_string;                             //运行状态描述 --自定义
    double x;                                                 //机器人x坐标
    double y;                                                 //机器人y坐标
    double angle;                                          //机器人位置在地图上的角度
    double confidence;                                 //机器人的置信度
    double vx;                                               //机器人在机器人坐标系的 x 方向速度, 单位 m/s
    double vy;                                               //机器人在机器人坐标系的 y 方向速度, 单位 m/s
    double w;                                                //机器人在世界坐标系的角速度, 单位 rad/s
    double r_vx;                                            //到达x方向速度
    double r_vy;                                            //到达y方向速度
    double r_w;                                             //到达w方向速度
    double steer;                                           //机器人当前的舵角, 单位 rad, 仅舵轮机器人存在该字段
    bool blocked = false;                              //机器人是否被阻挡
//    RobotInterface::BlockedReason block_reason;                                    //被阻挡的原因 0=Ultrasonic, 1=Laser, 2=Fallingdown, 3=Collision},
    QString block_reason_string;                 //被阻挡的原因描述
    double block_x;                                       //(block_x)number{阻挡位置的 x 坐 标}
    double block_y;                                       //(block_y)number{阻挡位置的 y 坐标}
    double block_di;                                     //阻挡 DI 的 ID
    double block_ultrasonic_id;                   //阻挡超声 的 ID
    double battery_level;                              //机器人电池电量
    double battery_temp;                             //机器人电池温度
    bool charging = false;                             //机器人是否在充电
    double voltage;                                       //机器人电压
    double current;                                       //机器人电流
    bool brake = false;                                  ///机器人是否抱闸, 如果该字段不存在, 说明机器人没有抱闸功能
    QList<QPointF> laser_list;                      //key:laser_beams  激光点数据, 激光采样步长为 NetProtocol 中 laserStep 的值
    QList<QPointF> path_list;                       //key:path 按顺序的路径点数据, 依次连接就是机器人规划的路径

    QVariantList area_ids;                               //{机器人所在区 域的数组，数组可能为空}
    bool emergency = false;                          //机器人急停是否处于激活状态(机器人急停按钮是否按下)
    QVariantList DI;                                        //DI 数据, boolean 表示高低电平, 从 0 开始, 默认为 true (高电平)
    QVariantList DO;                                      //DO 数据, boolean 表示高低电平, 从 0 开始, 默认为 true (高电平)
    QList<int>DisabledDI;                            //模型文件中禁用的DI
    QList<int>SpecialDI;                               //模型文件中有特殊作用的DI
    QList<int>SpecialDO;                             //模型文件中有特殊作用DO
    QVariantList ultrasonic_nodes;               //超声节点数据
//    RobotInterface::TaskStatus task_status;                                       //0 = NONE, 1 = WAITING, 2 = RUNNING, 3 =SUSPENDED, 4 = COMPLETED, 5 = FAILED, 6 = CANCELED
    QString task_status_string;                    //机器人当前任务状态描述---自定义
//    RobotInterface::TaskTypeStatus task_type;                                          //任务类型, 0 = 没有任务, 1 = 自由导航到任意点, 2 = 自由导航到站点, 3 = 固定路径导航到站点, 4 = 其他
    QString task_type_string;                       //当前任务描述---自定义
    //当前任务要去的站点, 仅当 task_type 为 2 或 3 时该字段有效, task_status 为 RUNNING 时说明正在去这个站点,
    //task_status 为 COMPLETED 时说明已经到达这个站点, task_status 为 FAILED 时说明去这个站点失败,
    //task_status 为 SUSPENDED 说明去这个站点的任务暂停
    QString target_id;

    //number 当前任务要去的坐标点, 为一个包含三个元素的数组, 分别为在世界坐标系中的 x, y, r 坐标,
    //仅当 task_type 为 1 时该字段有效, task_status 为 RUNNING 时说明正在去这个坐标点,
    //task_status 为 COMPLETED 时说明已经到达这个坐标点, task_status 为 FAILED 时说明去这个坐标点失败,
    //task_status 为 SUSPENDED 说明去这个坐标点的任务暂停
    QVariantList target_point;

    //当前任务路径上已经经过的站点, 为站点的数组,
    //仅当 task_type 为 3 时该字段有效
    QStringList finished_path;

    //当前任务路径上尚未经过的站点, 为站点的数组,
    //仅当 task_type 为 3 时该字段有效
    QStringList unfinished_path;

//    RobotInterface::RelocStatus reloc_status; //机器人定位状态
    QString reloc_status_string;                  //机器人定位状态描述--自定义
//    RobotInterface::LoadMapStatus loadmap_status;                               //机器人加载地图状态
    QString loadmap_status_string;            //机器人加载地图状态--自定义
//    RobotInterface::SlamScanStatus slam_status;                                      //扫图状态
    QString slam_status_string;                   //扫图状态描述
    QVariant tasklist_status;                        //任务链返回状态json
    //polygons
    QVariantList decelerate_polygons;        //减速多边形
    QVariantList stop_polygons;                  //停止多边形
    QVariantList safe_polygons;                   //安全多边形
    //warning
    QVariantList fatals;                                  //告警码	Fatal的数组,	所有出现的	Fatal	告警都会出现在数组中
    QVariantList errors;                                  //告警码	Error的数组,	所有出现的	Error	告警都会出现在数组中
    QVariantList warnings;                            //告警码	Warning的数组,	所有出现的	Warning	告警都会出现在 数组中

    //robot_status_info_req
    QString ip;                                                //机器人ip
    QString id;                                                //机器人型号
    QString version;                                       //机器人服务端版本号
    QString dsp_version;                                //固件版本号
    QString map_version;                              //地图版本
    QString model_version;                           //模型版本
    QString netprotocol_version;                 //网络协议版本
    QString model;                                         //模型文件名称
    QString model_md5;                                //模型文件md5值
    QString current_map;                              //当前加载地图
    QString current_map_md5;                     //当前加载地图的md5

};
struct InfoMapData{
    int level;                                //0为第一层1为第二层依次类推
    QString key;                          //关键字
    QString displayText0;           //字段名
    QString displayText1;           //显示数据
    QString toolTip;                    //提示描述
    QVariant value;                     //对应数据
    QString unit;                         //单位
    QList<InfoMapData*>child;  //子类数据
};
Q_DECLARE_METATYPE(InfoMapData*) //声明
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    InfoMapData *addLevel(const QString &displayText0,
                          const QString &displayText1 = "",
                          const QString &toolTip = "",
                          InfoMapData *parent = Q_NULLPTR,
                          const QString &key = "",
                          const QVariant &var = QVariant());
    QString getDecimalPlaces(double d,int countBit);
    void initTreeView();
    void rbkWarningCode(const QVariantList & list,QList<InfoMapData*>&infoMapDataList);
    void recursiveCollapseExpand(const QModelIndex index,bool isExpanded);
private slots:
    void slotTreeViewHeaderClicked(const QModelIndex &index);
    void slot_robot_status_all1_data(const QString &ip,  const RobotData *info);
    void slotSectionClicked(int column);
    void slotExpandedClicked(bool checked);
private:
    Ui::MainWindow *ui;
    QMutex _mutex;
    bool _isExpanded = true;
     bool _isPressedSection = true;
};

#endif // MAINWINDOW_H
