using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace GameCs
{

    //Activity chua map choi che do tu do
    class SingleMapActivity:Activity
    {

        User user;
        Map canvas;
        char key;
        int part;
        int bigTime;
        public SingleMapActivity(User user, Map canvas, CentraProccessing cpu, string label)
        {
            key = '\0';
            bigTime = 0;
            this.cpu = cpu;
            this.label = label;
            this.user = user;
            this.canvas = canvas;
            this.canvas.addSnake(new UserSnake(5));
            part= canvas.getBigBaitTimeLimt/4;
        }

        public override void work()
        {
            if (reactKey()) return;
            if (st != Game.PAUSE && st != Game.G_OVER)
            {
                int flag = canvas.work();
                if (canvas.getBigBaitTime > 0 || bigTime > 0)
                {
                    bigTime = canvas.getBigBaitTime;
                    if (bigTime >= 3*part)
                    {
                        cpu.addInfomation(InfoTable.TYPE.TIME, bigTime.ToString(), ConsoleColor.White);
                    } else if (bigTime < 3* part && bigTime>= 2*part)
                    {
                        cpu.addInfomation(InfoTable.TYPE.TIME, bigTime.ToString(), ConsoleColor.Yellow);
                    } else if(bigTime < 2 * part && bigTime >= part)
                    {
                        cpu.addInfomation(InfoTable.TYPE.TIME, bigTime.ToString(), ConsoleColor.Magenta);
                    } else
                    {
                        cpu.addInfomation(InfoTable.TYPE.TIME, bigTime.ToString(), ConsoleColor.Red);
                    }

                }
                if (flag==Map.SNAKE_DIE)
                {
                    st = Game.G_OVER;
                    cpu.addInfomation(InfoTable.TYPE.STATE, Game.G_OVER_DESCRIPTION , ConsoleColor.Red);
                    return;
                }
                else if (flag != Map.NORMAL)
                {
                    user.raise(flag);
                    cpu.addInfomation(InfoTable.TYPE.FOOD, user.getFoods.ToString(), ConsoleColor.Green);
                    cpu.addInfomation(InfoTable.TYPE.SCORE, user.getScore.ToString(), ConsoleColor.Green);
                    if (flag == BigFood.SCRORE)
                    {
                        cpu.addInfomation(InfoTable.TYPE.TIME, "0", ConsoleColor.Red);
                    }
                }
                
            }
        }

        //khoi tao lai khi gameover
        private void reset()
        {
            canvas.reset();
            canvas.addSnake(new UserSnake(5));
            user.reset();
        }

        //lay phim nhap vao
        public override void getKey(char key)
        {
            this.key = key;
        }

        public User getUser
        {
            get
            {
                return user;
            }
        }

        //lam gi khi co phim nhap vao
        private bool reactKey()
        {
            if (key == '\0') return false;
            UserSnake n = canvas.getSnake;
            int dir = n.getDir;
            switch (key)
            {
                //di len
                case Game.UP_KEY:
                    if (st == Game.PLAY && dir != Snake.UP && dir != Snake.DOWN)
                    {
                        n.setDir(Snake.UP);
                    }
                    break;

                //di xuong
                case Game.DOWN_KEY:
                    if (st == Game.PLAY && dir != Snake.UP && dir != Snake.DOWN)
                    {
                        n.setDir(Snake.DOWN);
                        
                    }
                    break;
                //qua trai
                case Game.LEFT_KEY:
                    if (st == Game.PLAY && dir != Snake.RIGHT && dir != Snake.LEFT)
                    {
                        n.setDir(Snake.LEFT);
                        
                    }
                    break;

                //qua phai
                case Game.RIGHT_KEY:
                    if (st == Game.PLAY && dir != Snake.RIGHT && dir != Snake.LEFT)
                    {
                        n.setDir(Snake.RIGHT);
                     
                    }
                    break;
                case Game.ZERO_KEY:
                    if (st== Game.G_OVER)  //choi laij
                    {                       
                        reset();
                        workFirst();
                    }
                    else if (st == Game.PAUSE)  //dung game
                    {
                        if (canvas.isGameOver)
                        {
                            st = Game.G_OVER;
                            cpu.addInfomation(InfoTable.TYPE.STATE,Game.G_OVER_DESCRIPTION, ConsoleColor.Red);
                        }
                        else
                        {
                            st = Game.PLAY;
                            cpu.addInfomation(InfoTable.TYPE.STATE, Game.PLAY_DESCRIPTION, ConsoleColor.Magenta);
                        }
                    }
                    else if (st == Game.PLAY)   //tam ngung
                    {

                        st = Game.PAUSE;
                        cpu.addInfomation(InfoTable.TYPE.STATE, Game.PAUSE_DESCRIPTION, ConsoleColor.Magenta);
                    }
                    break;
                    //mo ban lua chon
                case Game.BACK_KEY:
                    Poster p = new Poster(cpu.blankPoster);
                    Option ntf = new Option(p, cpu, "Option");
                    cpu.pushStack(ntf);
                    cpu.topOfStackWork();
                    key = '\0';
                    return true;
                    //ve lai toan bo game
                case Game.REFRESH_KEY:
                    cpu.drawInfoFrame();
                    drawAll();
                    break;
            }
            key = '\0';
            return false;
        }

        //lam viec lai
        public override void workFirst()
        {
            st = Game.PAUSE;
            canvas.workFirst();
            cpu.addInfomation(InfoTable.TYPE.LEVEL, label, ConsoleColor.Yellow);
            cpu.addInfomation(InfoTable.TYPE.STATE, Game.PAUSE_DESCRIPTION, ConsoleColor.Magenta);
            cpu.addInfomation(InfoTable.TYPE.SCORE, user.getScore.ToString(), ConsoleColor.Green);
            cpu.addInfomation(InfoTable.TYPE.FOOD, user.getFoods.ToString(), ConsoleColor.Green);
            cpu.addInfomation(InfoTable.TYPE.NAME, canvas.getName, ConsoleColor.White);
            cpu.addInfomation(InfoTable.TYPE.TIME, canvas.getBigBaitTime.ToString(), ConsoleColor.Red);
        }


        //ve lai toan bo game
        public override void drawAll()
        {
            st = Game.PAUSE;
            canvas.show();
            cpu.addInfomation(InfoTable.TYPE.LEVEL, label, ConsoleColor.Yellow);
            cpu.addInfomation(InfoTable.TYPE.STATE, Game.PAUSE_DESCRIPTION, ConsoleColor.Magenta);
            cpu.addInfomation(InfoTable.TYPE.SCORE, user.getScore.ToString(), ConsoleColor.Green);
            cpu.addInfomation(InfoTable.TYPE.FOOD, user.getFoods.ToString(), ConsoleColor.Green);
            cpu.addInfomation(InfoTable.TYPE.NAME, canvas.getName, ConsoleColor.White);
            cpu.addInfomation(InfoTable.TYPE.TIME, canvas.getBigBaitTime.ToString(), ConsoleColor.Red);
        }
    }
}
