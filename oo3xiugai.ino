#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT//支持小爱同学
#define BLINKER_PRINT Serial
#include <Blinker.h> 
char auth[] = "7e2b9dd3eaeb";    

char ssid[] = "ChinaNGB-1015";
char pswd[] = "15000301391"; 
int GPIO = 0;                              //定义继电器输入引脚为GPIO/0 


// 新建组件对象
BlinkerButton Button1("btn-abc");           //注意：要和APP组件’数据键名’一致
  
// 按下BlinkerAPP按键即会执行该函数
void button1_callback(const String & state) 
{ 
  BLINKER_LOG("get button state: ", state);
  if(state=="on"){                         //如果命令为开
    digitalWrite(GPIO, LOW);                
  //反馈开关状态                
    Button1.color("#00FFFF");             //设置app按键是深蓝色
    Button1.print("on");}                 //输出开
  else if(state=="off"){
    digitalWrite(GPIO, HIGH);               
  //反馈开关状态              
    Button1.color("#FF0000");             //设置app按键是红色
    Button1.print("off");                
    }
     else if (state == BLINKER_CMD_BUTTON_TAP)  //如果state是点按（tap）
  {
    digitalWrite(GPIO, LOW);
    Blinker.delay(400);
    digitalWrite(GPIO, HIGH);
    BLINKER_LOG("Button tap!");           //串口打印开关状态是点按（tap）
    Button1.icon("icon_1");
    Button1.color("#FF0000");             //设置app按键点动是红色（点动后状态为关，所以一样设置红色）
    Button1.text("点动");                 //在app按键上显示提示语"点动"
    Button1.print();                      //反馈给app开关状态可以是"on"(打开); "off"(关闭)；或者不写
  }
}



//实时反馈
void heartbeat()
{
  BLINKER_LOG("状态同步!"); 
   if (digitalRead(GPIO)==LOW) 
    { 
      Button1.print("on");
      Button1.color("#00FFFF");            //设置app按键开是深蓝色
      Button1.text("开");
    }
    else 
    {
    Button1.print("off");
    Button1.color("#FF0000");              //设置app按键关是红色
    Button1.text("关");
    }
}
    //小爱电源类操作的回调函数:
    //当小爱同学向设备发起控制, 设备端需要有对应控制处理函数 
void miotPowerState(const String & state)
{ 
  BLINKER_LOG("need set power state: ", state); 
  if (state == BLINKER_CMD_ON) 
  { 
  digitalWrite(GPIO, LOW);
  BlinkerMIOT.powerState("on"); 
  BlinkerMIOT.print();
  } 
  else if (state == BLINKER_CMD_OFF) 
  { 
    digitalWrite(GPIO, HIGH);
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
    }
 }
    void setup()
    { 
      // 初始化串口，并开启调试信息,调试用,可以删除
      Serial.begin(115200); 
      BLINKER_DEBUG.stream(Serial); 
      // 初始化IO
      pinMode(GPIO, OUTPUT); 
      digitalWrite(GPIO, HIGH); 
      // 初始化blinker 
      Blinker.begin(auth, ssid, pswd); 
      Button1.attach(button1_callback);
     
      //小爱同学务必在回调函数中反馈该控制状态  
      BlinkerMIOT.attachPowerState(miotPowerState);//注册回调函数
      Blinker.attachHeartbeat(heartbeat);   //app定时向设备发送心跳包, 设备收到心跳包后会返回设备当前状态进行语音操作和app操作同步。
      }
      void loop()
      { Blinker.run();}
