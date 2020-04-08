#include<iostream>
#include<vector>
#include<string>
#include<stdio.h>
#include<map>
#include<math.h>
#include<iomanip>
#include<iterator>
#include<queue>
#include<string.h>
#include<algorithm>
#include<fstream>
#include<sstream>

using namespace std;

const int Ncity=30;
const int dragon=0,ninja=1,iceman=2,lion=3,wolf=4;
const int sword=0,bomb=1,arrow=2;
const int red=0,blue=1;
const int unmarked=-1;//表示为插旗
const char *team[2]={"red","blue"};
const int name[2][5]={{iceman,lion,wolf,ninja,dragon},
                      {lion,dragon,ninja,iceman,wolf}};

int tmp_gain_m[2];

char msg_buf[1000];

class Msg
{
public:
    int time,pos,order;//order用来判断同时同地发生的事情的先后顺序
    string content;

//构造函数，不需考虑顺序的消息
    Msg(int _time,int _pos):time(_time),pos(_pos),order(0),content(msg_buf){}

//构造函数，需要考虑先后顺序的消息
    Msg(int _time,int _pos,int _order):time(_time),pos(_pos),order(_order),content(msg_buf){}

//重载cout，输出消息
    friend ostream& operator<<(ostream &os,const Msg &m)
    {
        os<<m.content;
        return os;
    }

//判断Msg的输出顺序
    friend bool operator<(const Msg& m1,const Msg& m2)
    {
        if(m1.time==m2.time)//时间优先
        {
            if(m1.pos==m2.pos)//地点次之
            {
                return m1.order<m2.order;//同时同地看优先级
            }
            else
            {
                return m1.pos<m2.pos;
            }
        }
        else
        {
            return m1.time<m2.time;
        }
    }
};

class Warrior;
class Weapon;

class Game
{
public:
    int M;//司令部初始生命元,M<=10000
    int N;//两个司令部之间有N个城市，N<=20
    int R;//arrow的攻击力是R
    int K;//lion每经过一场未能杀死敌人的战斗，忠诚度就降低K。
    int T;//要求输出从0时0分开始，到时间T为止(包括T) 的所有事件。T以分钟为单位，0 <= T <= 5000
    int life[5];//五个整数，依次是 dragon 、ninja、iceman、lion、wolf 的初始生命值。它们都大于0小于等于10000
    int no[2];//红蓝司令部当前制造的武士的编号
    int warrior_attack[5];//五个整数，依次是 dragon 、ninja、iceman、lion、wolf 的攻击力。它们都大于0小于等于10000
    bool taken[2];//1表示已被占领
    int flag[Ncity];//表示城市插旗情况,-1表示未插旗
    int win_cnt[2][Ncity];//在某城市连续获胜的次数
    int city_element[Ncity]={0};//城市的生命元
    int m[2];//司令部剩余的生命元
    int hour;//当前小时
    int time_to_birth[2];//当前要制造的武士


    vector<Msg> msg_box;//存储待输出的消息
    vector<int> reward[2];//需要奖励生命元的武士所在的城市

    Warrior *warrior_of_city[2][Ncity];//指针类型，每个指针指向某阵营在该城市的勇士
    Warrior *reached[2][2];
    Warrior *tmp_city[2][Ncity];//临时指针，用于前进时武士所在城市的转移


    Game();//Game的初始化，读入数据M,N,R,K,T,
           //初始化生命元，攻击力，被占领情况
    
    ~Game();

    bool round();//回合开始

    void warrior_birth(int _side);//制造武士

    void send_element();//奖励生命元

    void city_generate_element()//城市制造10个生命元
    {
        for(int i=1;i<=N;++i)
        {
            city_element[i]+=10;
        }
    }

    void report_element(int _side)
    {
        //000:50 100 elements in red headquarter
        //000:50 120 elements in blue headquarter
        //表示在0点50分，红方司令部有100个生命元，蓝方有120个
        sprintf(msg_buf,"%03d:%02d %d elements in %s headquarter",hour,50,m[_side],team[_side]);
        msg_box.emplace_back(50,_side?N+1:0);
    }

    bool release_msg_box()
    {
        sort(msg_box.begin(),msg_box.end());//按输出顺序排序
        for(const Msg &m:msg_box)
        {
            if(m.time+hour*60<=T)//判断是否到时间了
            {
                cout<<m<<endl;//输出消息
            }
            else
            {
                return false;//回合结束
            }
        }
        msg_box.clear();//清空消息盒子
    }

};

class Weapon
{
public:
    Warrior *owner;//拥有者
    int force,durability;//攻击力和耐用性

    explicit Weapon(Warrior *_owner,int _force,int _durability):owner(_owner),force(_force),durability(_durability){}

    void worn();//武器报废

    virtual void use_weapon()
    {
        if(--durability==0)
        {
            worn();
        }
    }
};

class Warrior
{
public:
    int life;//武士的生命力
    int no;//武士的编号
    int pos;//武士所在的城市
    int side;//武士所在阵营
    int attack;//攻击力
    Game *game;//所在样例的指针
    Weapon *weapon[3];//拥有的武器

    void birth()
    {
        //输出样例： 000:00 blue lion 1 born
        //表示在 0点0分，编号为1的蓝魔lion武士降生
        sprintf(msg_buf,"%03d:%02d %s %s %d born",game->hour,0,team[side],type_str(),no);
        game->msg_box.emplace_back(0,pos);
    }

    virtual const char *type_str()=0;

    Warrior(int _side,int _attack,int _life,Game *_game):side(_side),attack(_attack),life(_life),game(_game)
    {
        for(int i=0;i<3;++i)
        {
            weapon[i]=nullptr;
        }
        no=++game->no[side];//武士在司令部中的编号
        int &to_birth=game->time_to_birth[side];//这次制造的武士的编号
        pos=side?game->N+1:0;//武士所在司令部
        int cost=game->life[name[side][to_birth]];//制造该武士消耗的生命元
        game->m[side]-=cost;//司令部生命元减去消耗的
        (++to_birth)%=5;//更新下一个要制造的武士
        game->warrior_of_city[side][pos]=this;//将game中对应指针指向这位武士
    }

    virtual ~Warrior()
    {
        game->warrior_of_city[side][pos]=nullptr;
        for(int i=0;i<3;++i)
        {
            delete weapon[i];
        }
    }

    virtual void escape(){}

    virtual void march()//前进
    {
        int direction=side?-1:+1;//确定前进的方向
        game->warrior_of_city[side][pos]=nullptr;
        pos+=direction;
        int dest=side?0:game->N+1;
        if(pos==dest)
        {
            //输出样例：001:10 red iceman 1 reached blue headquarter with 20 elements and force 30
            //(此时他生命值为20,攻击力为30）对于iceman,输出的生命值和攻击力应该是变化后的数值
            sprintf(msg_buf,"%03d:%02d %s %s %d reached %s headquarter with %d elements and force %d",
                    game->hour,10,team[side],type_str(),no,team[!side],life,attack);
            game->msg_box.emplace_back(10,pos);
            if(game->reached[!side][0])
            {
                game->reached[!side][1]=this;//该武士到达了地方司令部
                game->taken[!side]=true;//敌方司令部被占领
                //司令部被占领
                //输出样例：003:10 blue headquarter was taken
                sprintf(msg_buf,"%03d:%02d %s headquarter was taken",game->hour,10,team[!side]);
                game->msg_box.emplace_back(10,pos,1);
            }
            else
            {
                game->reached[!side][0]=this;
            }
        }
        else
        {
            game->tmp_city[side][pos]=this;
            // 武士前进到某一城市
            //000:10 red iceman 1 marched to city 1 with 20 elements and force 30
            //表示在 0点10分，红魔1号武士iceman前进到1号城市，此时他生命值为20,攻击力为30
            //对于iceman,输出的生命值和攻击力应该是变化后的数值
            sprintf(msg_buf,"%03d:%02d %s %s %d marched to city %d with %d elements and force %d",
                    game->hour,10,team[side],type_str(),no,pos,life,attack);
            game->msg_box.emplace_back(10,pos,side);
        }   
    }

    Warrior *get_enemy()//得到在同一个城市的敌人
    {
        return game->warrior_of_city[!side][pos];
    }

    void shot()
    {
        int direction=side?-1:+1;
        Warrior *enemy=game->warrior_of_city[!side][pos+direction];
        if(weapon[arrow]&&enemy)
        {
            enemy->life-=weapon[arrow]->force;
            weapon[arrow]->use_weapon();//使用武器，可用次数减少，判断是否报废
            if(enemy->life<=0)
            {
                //如果射出的箭杀死了敌人，则应如下输出：
                //000:35 blue dragon 1 shot and killed red lion 4
                //表示在 0点35分，编号为1的蓝魔dragon武士射出一支箭，杀死了编号为4的红魔lion。
                sprintf(msg_buf,"%03d:%02d %s %s %d shot and killed %s %s %d",game->hour,35,team[side],type_str(),
                        no,team[!side],enemy->type_str(),enemy->no);
                game->msg_box.emplace_back(35,pos,0);
            }
            else
            {
                //武士放箭
                //输出样例： 000:35 blue dragon 1 shot
                //表示在 0点35分，编号为1的蓝魔dragon武士射出一支箭。
                sprintf(msg_buf,"%03d:%02d %s %s %d shot",game->hour,35,team[side],type_str(),no);
                game->msg_box.emplace_back(35,pos);
            }
        }
    }

    bool offensive()
    {
        //HHHHHHHHHHHHHHHHHH
        if(game->flag[pos]==side||(game->flag[pos]==-1&&pos%2!=side))
        {
            return true;
        }
        return false;
    }

    virtual void prepare(){}

    void fight()
    {
        Warrior *enemy=get_enemy();
        prepare();
        enemy->prepare();
        active_attack();
        if(enemy->life<=0)
        {
            win();
            enemy->die();
        }
        else
        {
            enemy->attack_back();
            if(life<=0)
            {
                enemy->win();
                die();
            }
            else
            {
                tie();
                enemy->tie();
            }
        }   
    }

    void active_attack()//主动进攻
    {
        Warrior *enemy=get_enemy();
        enemy->life-=attack_damage();
        if(weapon[sword])
        {
            weapon[sword]->use_weapon();
        }
        //武士主动进攻
        //输出样例：000:40 red iceman 1 attacked blue lion 1 in city 1 with 20 elements and force 30
        //表示在0点40分，1号城市中，红魔1号武士iceman 进攻蓝魔1号武士lion,在发起进攻前，红魔1号武士iceman生命值为20，攻击力为 30
        sprintf(msg_buf,"%03d:%02d %s %s %d attacked %s %s %d in city %d with %d elements and force %d",
                game->hour,40,team[side],type_str(),no,team[!side],enemy->type_str(),enemy->no,pos,life,attack);
        game->msg_box.emplace_back(40,pos,0);
    }

    virtual void attack_back()//反击
    {
        
            Warrior *enemy=get_enemy();
            enemy->life-=attack_back_damage();
            if(weapon[sword])
            {
                weapon[sword]->use_weapon();
            }
            //武士反击
            //输出样例：001:40 blue dragon 2 fought back against red lion 2 in city 1
            //表示在1点40分，1号城市中，蓝魔2号武士dragon反击红魔2号武士lion
            sprintf(msg_buf,"%03d:%02d %s %s %d fought back against %s %s %d in city %d",
                    game->hour,40,team[side],type_str(),no,team[!side],enemy->type_str(),enemy->no,pos);
            game->msg_box.emplace_back(40,pos,1);
        
    }

    int attack_damage()//进攻伤害
    {
        int damage=attack;
        if(weapon[sword])//是否使用sword
        {
            damage+=weapon[sword]->force;
        }
        return damage;
    }

    virtual int attack_back_damage()//反击伤害
    {
        int damage=attack/2;
        if(weapon[sword])
        {
            damage+=weapon[sword]->force;
        }
        return damage;
    }

    void use_bomb()
    {
        if(!weapon[bomb])
        {
            return;
        }
        Warrior *enemy=get_enemy();
        if(!enemy)
        {
            return;
        }
        if((offensive()&&attack_damage()<enemy->life&&enemy->attack_back_damage()>=life)
            ||(!offensive()&&enemy->attack_damage()>=life))
        {
            weapon[bomb]->use_weapon();
        }
    }

    virtual void die()
    {
        //武士战死
        //输出样例：001:40 red lion 2 was killed in city 1
        //被箭射死的武士就不会有这一条输出。
        sprintf(msg_buf,"%03d:%02d %s %s %d was killed in city %d",game->hour,40,team[side],type_str(),no,pos);
        game->msg_box.emplace_back(40,pos,2);
        //delete this;

    }

    virtual void tie()//平局
    {
        game->win_cnt[0][pos]=game->win_cnt[1][pos]=0;
    }

    void gain_element()//非战斗获取生命元
    {
        game->m[side]+=game->city_element[pos];
        //000:30 blue lion 1 earned 10 elements for his headquarter
        sprintf(msg_buf,"%03d:%02d %s %s %d earned %d elements for his headquarter",
                game->hour,30,team[side],type_str(),no,game->city_element[pos]);
        game->msg_box.emplace_back(30,pos);
        game->city_element[pos]=0;
    }

    void win_element()
    {
        game->m[side]+=game->city_element[pos];
        //武士获取生命元( elements )
        //输出样例：001:40 blue dragon 2 earned 10 elements for his headquarter
        sprintf(msg_buf,"%03d:%02d %s %s %d earned %d elements for his headquarter",
                game->hour,40,team[side],type_str(),no,game->city_element[pos]);
        game->msg_box.emplace_back(40,pos,4);
        game->city_element[pos]=0;
    }

    virtual void win()
    {
        game->reward[side].push_back(pos);
        win_element();
        game->win_cnt[!side][pos]=0;
        if(++game->win_cnt[side][pos]==2)
        {
            raise_flag();
        }
    }

    void raise_flag()
    {
        if(game->flag[pos]==side)
        {
            return;
        }
        game->flag[pos]=side;
        //旗帜升起
        //输出样例：004:40 blue flag raised in city 4
        sprintf(msg_buf,"%03d:%02d %s flag raised in city %d",game->hour,40,team[side],pos);
        game->msg_box.emplace_back(40,pos,5);
    }

    void report_weapon()//武士报告武器情况
    {
        //000:55 blue wolf 2 has arrow(2),bomb,sword(23)
        //000:55 blue wolf 4 has no weapon
        //000:55 blue wolf 5 has sword(20)
        //表示在0点55分，蓝魔2号武士wolf有一支arrow（这支arrow还可以用2次），一个bomb，还有一支攻击力为23的sword。
        //蓝魔4号武士wolf没武器。
        //蓝魔5号武士wolf有一支攻击力为20的sword。
        //交代武器情况时，次序依次是：arrow,bomb,sword。如果没有某种武器，某种武器就不用提。报告时，先按从西向东的顺序所有的红武士报告，然后再从西向东所有的蓝武士报告。
        sprintf(msg_buf,"%03d:%02d %s %s %d has ",game->hour,55,team[side],type_str(),no);
        if(weapon[arrow])
        {
            sprintf(msg_buf+strlen(msg_buf),"arrow(%d),",weapon[arrow]->durability);
        }
        if(weapon[bomb])
        {
            sprintf(msg_buf+strlen(msg_buf),"bomb,");
        }
        if(weapon[sword])
        {
            sprintf(msg_buf+strlen(msg_buf),"sword(%d)",weapon[sword]->force);
        }
        if(!weapon[arrow]&&!weapon[bomb]&&!weapon[sword])
        {
            sprintf(msg_buf+strlen(msg_buf),"no weapon");
        }
        int len=(int)strlen(msg_buf);
        if(msg_buf[len-1]==',')
        {
            msg_buf[len-1]=0;
        }
        game->msg_box.emplace_back(55,-1,pos+side*100);
    }
};

void Weapon::worn()
{
    for(int i=0;i<3;++i)
    {
        if(owner->weapon[i]==this)
        {
            owner->weapon[i]=nullptr;
            break;
        }
    }
    delete this;
}

class Sword:public Weapon
{
public:
    explicit Sword(Warrior *_owner):Weapon(_owner,int(2*_owner->attack/10),-1)
    {
        owner->weapon[sword]=this;
        if(force==0)
        {
            worn();
        }
    }

    virtual void use_weapon()
    {
        force=(int)(8*force/10);
        if(force==0)
        {
            worn();
        }
    }
};

class Arrow:public Weapon
{
public:
    explicit Arrow(Warrior *_owner):Weapon(_owner,_owner->game->R,3)
    {
        owner->weapon[arrow]=this;
    }
};

class Bomb:public Weapon
{
public:
    explicit Bomb(Warrior *_owner):Weapon(_owner,0,1)
    {
        owner->weapon[bomb]=this;
    }

    virtual void use_weapon()
    {
        //武士使用bomb
        //输出样例： 000:38 blue dragon 1 used a bomb and killed red lion 7
        //表示在 0点38分，编号为1的蓝魔dragon武士用炸弹和编号为7的红魔lion同归于尽。
        sprintf(msg_buf,"%03d:%02d %s %s %d used a bomb and killed %s %s %d",
                owner->game->hour,38,team[owner->side],owner->type_str(),owner->no,
                team[!owner->side],owner->get_enemy()->type_str(),owner->get_enemy()->no);
        owner->game->msg_box.emplace_back(38,owner->pos);
        Warrior *pre_owner=owner;
        Weapon::use_weapon();
        delete pre_owner->get_enemy();
        delete pre_owner;
        //HHHHHHHHHHHHHHHHHHHHHHHHHHHHHH

    }
};

class Dragon:public Warrior
{
public:
    double morale;

    virtual const char *type_str()
    {
        return "dragon";
    }

    virtual void yell()
    {
        if(offensive()&&morale>=0.8)
        {
            //武士欢呼
            //输出样例：003:40 blue dragon 2 yelled in city 4
            sprintf(msg_buf,"%03d:%02d %s %s %d yelled in city %d",game->hour,40,team[side],type_str(),no,pos);
            game->msg_box.emplace_back(40,pos,3);
        }
    }

    Dragon(int side,Game *_game):Warrior(side,_game->warrior_attack[dragon],_game->life[dragon],_game)
    {
        morale=1.0*_game->m[side]/_game->life[dragon];
        int type=no%3;
        if(type==sword)
        {
            new Sword(this);
        }
        else if(type==arrow)
        {
            new Arrow(this);
        }
        else
        {
            new Bomb(this);
        }
        birth();
        //如果造出的是dragon，那么还要多输出一行，例：
        //Its morale is 23.34
        //表示该该dragon降生时士气是23. 34(四舍五入到小数点后两位)
        sprintf(msg_buf,"Its morale is %.2f",morale);
        _game->msg_box.emplace_back(0,pos,1);
    }

    virtual void win()
    {
        morale+=0.2;
        if(offensive()&&morale>=0.8)
        {
            yell();
        }
        Warrior::win();
    }

    virtual void tie()
    {
        morale-=0.2;
        if(offensive()&&morale>=0.8)
        {
            yell();
        }
        Warrior::tie();
    }
};

class Ninja:public Warrior
{
public:
    Ninja(int side,Game *_game):Warrior(side,_game->warrior_attack[ninja],_game->life[ninja],_game)
    {
        for(int i=0;i<2;i++)
        {
            int type=(no+i)%3;
            if(type==sword)
            {
                new Sword(this);
            }
            else if(type==arrow)
            {
                new Arrow(this);
            }
            else
            {
                new Bomb(this);
            }
        }
        birth();
    }

    virtual const char *type_str()
    {
        return "ninja";
    }

    virtual void attack_back()
    {
        return;
    }

    virtual int attack_back_damage()
    {
        return 0;
    }
};

class Iceman:public Warrior
{
public:
    int step_cnt;

    Iceman(int side,Game *_game):Warrior(side,_game->warrior_attack[iceman],_game->life[iceman],_game),step_cnt(0)
    {
        int type=no%3;
        if(type==sword)
        {
            new Sword(this);
        }
        else if(type==arrow)
        {
           new Arrow(this);
        }
        else
        {
            new Bomb(this);
        }
        birth();
    }
    
    virtual const char *type_str()
    {
        return "iceman";
    }

    virtual void march()
    {
        if(++step_cnt==2)
        {
            step_cnt=0;
            life=max(life-9,1);
            attack+=20;
        }
        Warrior::march();
    }
};

class Lion:public Warrior
{
public:
    int loyalty;
    int life_before_battle;
    int dest;//HHHHHHHHHHHHHHHHHHHHHHHHHH

    virtual const char *type_str()
    {
        return "lion";
    }

    Lion(int h,Game *_game):Warrior(h,_game->warrior_attack[lion],_game->life[lion],_game)
    {
        //HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
        dest=side?0:game->N+1;
        life_before_battle=life;
        loyalty=game->m[side];
        birth();
        //如果造出的是lion，那么还要多输出一行，例:
        //Its loyalty is 24
        sprintf(msg_buf,"Its loyalty is %d",loyalty);
        game->msg_box.emplace_back(0,pos,1);
    }

    virtual void tie()
    {
        Warrior::tie();
        loyalty-=game->K;
    }

    virtual void escape()
    {
        Warrior::escape();
        
        if(loyalty<=0&&pos!=dest)
        {
            //lion逃跑
            //输出样例： 000:05 blue lion 1 ran away
            //表示在 0点5分，编号为1的蓝魔lion武士逃走
            sprintf(msg_buf,"%03d:%02d %s %s %d ran away",game->hour,5,team[side],type_str(),no);
            game->msg_box.emplace_back(5,pos);
            game->warrior_of_city[side][pos]=nullptr;
            delete this;
        }
    }

    virtual void die()
    {
        if(get_enemy())
        {
            get_enemy()->life+=life_before_battle;
        }
        Warrior::die();
    }

    virtual void prepare()
    {
        life_before_battle=life;
    }
};

class Wolf:public Warrior
{
public:
    Wolf(int h,Game *_game):Warrior(h,_game->warrior_attack[wolf],_game->life[wolf],_game)
    {
        birth();
    }

    virtual const char *type_str()
    {
        return "wolf";
    }

    virtual void win()
    {
        Warrior::win();
        Warrior *enemy=get_enemy();
        if(!enemy)
        {
            return;
        }
        for(int i=0;i<3;i++)
        {
            if(!weapon[i]&&enemy->weapon[i])
           {
               weapon[i]=enemy->weapon[i];
               weapon[i]->owner=this;
               enemy->weapon[i]=nullptr;
           } 
        }
    }
};

Game::~Game()
{
    for(int i=0;i<N+2;++i)
    {
        delete warrior_of_city[0][i];
        delete warrior_of_city[1][i];
    }
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<2;j++)
        {
            delete reached[i][j];
        }
    }
}


Game::Game()
{
    tmp_gain_m[0]=tmp_gain_m[1]=0;
    hour=0;
    cin>>M>>N>>R>>K>>T;
    m[0]=m[1]=M;
    no[0]=no[1]=0;
    time_to_birth[0]=time_to_birth[1]=0;
    for(int i=0;i<5;i++)
    {
        cin>>life[i];//武士初始生命元

    }
    for(int i=0;i<5;++i)
    {
        cin>>warrior_attack[i];//武士初始攻击力
    }
    taken[0]=taken[1]=false;//占领情况初始化
    for(int i=0;i<N+2;++i)
    {
        city_element[i]=0;
        flag[i]=-1;//-1表示未插旗
        warrior_of_city[0][i]=warrior_of_city[1][i]=tmp_city[0][i]=tmp_city[1][i]=nullptr;
        win_cnt[0][i]=win_cnt[1][i]=0;//在城市i连续获胜次数
    }
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<2;j++)
        {
            reached[i][j]=nullptr;
        }
    }
}


bool Game::round()
{
    //在每个整点，即每个小时的第0分， 双方的司令部中各有一个武士降生。
    warrior_birth(red);
    warrior_birth(blue);
    for(int i=0;i<N+2;i++)
    {
        for(int j=0;j<2;j++)
        {
            //HHHHHHHHHHHHHHHHH
            if(warrior_of_city[j][i]&&warrior_of_city[j][i]->life>0)
            {
                warrior_of_city[j][i]->escape();
            }
        }
    }
    for(int i=0;i<=N;i++)
    {
        //HHHHHHHHHHHHHHHHH
        //改动
        if(warrior_of_city[0][i]&&warrior_of_city[0][i]->life>0)
        {
            warrior_of_city[0][i]->march();
        }
    }
    for(int i=N+1;i>=1;i--)
    {
        //HHHHHHHHHHHHHHHHH
        if(warrior_of_city[1][i]&&warrior_of_city[1][i]->life>0)
        {
            warrior_of_city[1][i]->march();
        }
    }
    for(int i=0;i<N+2;i++)
    {
        for(int j=0;j<2;j++)
        {
            warrior_of_city[j][i]=tmp_city[j][i];
            tmp_city[j][i]=nullptr;
        }
    }
    if(taken[0]||taken[1])
    {
        release_msg_box();
        return false;
    }
    city_generate_element();
    for(int i=1;i<=N;i++)
    {
        for(int j=0;j<2;j++)
        {
            if(warrior_of_city[j][i]&&!warrior_of_city[!j][i])
            {
                warrior_of_city[j][i]->gain_element();
            }
        }
    }
    for(int i=1;i<=N;i++)
    {
        for(int j=0;j<2;j++)
        {
            if(warrior_of_city[j][i])
            {
                warrior_of_city[j][i]->shot();
            }
        }
    }
    for(int i=1;i<=N;i++)
    {
        for(int j=0;j<2;j++)
        {
            if(warrior_of_city[j][i]&&warrior_of_city[j][i]->life<=0)
            {
                Warrior *loser=warrior_of_city[j][i];
                Warrior *winner=loser->get_enemy();
                if(winner&&winner->life>0)
                {
                    winner->win();
                }
                else
                {
                    delete winner;
                }
                delete loser;
                break;
            }
        }
    }
    for(int i=1;i<=N;i++)
    {
        for(int j=0;j<2;j++)
        {
            if(warrior_of_city[j][i])
            {
                warrior_of_city[j][i]->use_bomb();
            }
        }
    }
    for(int i=1;i<=N;i++)
    {
        if(warrior_of_city[0][i]&&warrior_of_city[1][i])
        {
            if(warrior_of_city[0][i]->offensive())
            {
                warrior_of_city[0][i]->fight();
            }
            else
            {
                warrior_of_city[1][i]->fight();
            }
        }
    }
    send_element();
    report_element(red);
    report_element(blue);
    for(int i=0;i<N+2;i++)
    {
        for(int j=0;j<2;j++)
        {
            if(warrior_of_city[j][i])
            {
                warrior_of_city[j][i]->report_weapon();
            }
        }
    }
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<2;j++)
        {
            if(reached[i][j])
            {
                reached[i][j]->report_weapon();
            }
        }
    }
    if(release_msg_box())
    {
        ++hour;
        return true;
    }
    else
    {
        return false;
    }
    
}

void Game::warrior_birth(int _side)
{
    int type=name[_side][time_to_birth[_side]];//当前要制造的武士
    int cost=life[type];//制造武士消耗的生命元
    if(m[_side]>=cost)
    {
        switch(type)//制造一个武士，所在阵营
        {
        case dragon:new Dragon(_side,this);break;
        case ninja:new Ninja(_side,this);break;
        case iceman:new Iceman(_side,this);break;
        case lion:new Lion(_side,this);break;
        case wolf:new Wolf(_side,this);break;
        }
    }
}


void Game::send_element()//奖励生命元
{
    //优先奖励靠近敌方阵营的武士
    //蓝司令部要倒序
    sort(reward[red].begin(),reward[red].end());
    sort(reward[blue].rbegin(),reward[blue].rend());
    for(int h=0;h<2;h++)
    {
        while(m[h]>=8&&!reward[h].empty())
        {
            warrior_of_city[h][reward[h].back()]->life+=8;
            m[h]-=8;
            reward[h].pop_back();
        }
        reward[h].clear();
    }
}

int main()
{
    int t;
    int test;
    freopen("in.txt","r",stdin);
    freopen("out1.txt","w",stdout);
    cin>>t;
    for(int i=1;i<=t;i++)
    {
        cin>>test;
        cout<<"Case "<<i<<':'<<endl;
        Game game;
        while(game.round());
    }
    fclose(stdin);
    fclose(stdout);
    return 0;
}