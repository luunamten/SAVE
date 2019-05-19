using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace GameCs
{
    //Menu dau tien
    class FirstMenu : Activity
    {
        const int X = 28;
        const int Y = 4;
        int index;
        string[] item= {"1.Choi tu do", "2.Choi chien dich" , "3.Cai dat" ,"4.Thoat game"};
        Poster poster;

        public FirstMenu(Poster poster, CentraProccessing cpu)
        {
            st = Game.OUG;
            label = "T.P.Menu";
            index = 0;
            this.poster = poster;
            this.cpu = cpu;
        }

        //lay du lieu ban phim
        public override void getKey(char key)
        {
            switch (key)
            {
                case Game.UP_KEY:
                    itemUp();
                    break;
                case Game.DOWN_KEY:
                    itemDown();
                    break;
                case Game.ZERO_KEY:
                    Poster p = new Poster(cpu.blankPoster);
                    switch (index) {
                        case 0:
                            cpu.setTypePlay(CentraProccessing.TypePlay.TUDO);
                            cpu.pushStack(new ChoseMapMenu(p, cpu));
                            break;

                        case 1:
                            cpu.setTypePlay(CentraProccessing.TypePlay.CHIENDICH);
                            cpu.pushStack(new SpeedMenu(p, cpu));
                            break;
                        case 2:
                            cpu.pushStack(new SettingMenu(p,cpu));
                            break;
                        case 3:
                            Environment.Exit(0);
                            break;
                    }
                    cpu.topOfStackWork();
                         
                    break;
                case Game.BACK_KEY:
                    cpu.popStack();
                    cpu.drawTopOfStack();                  
                    break;
                case Game.REFRESH_KEY:
                    cpu.drawInfoFrame();
                    drawAll();
                    break;
            }
        }

        //lam viec
        public override void work()
        {
            drawAll();
        }

        //lam viec lai
        public override void workFirst()
        {
            drawAll();
        }


        //ve menu
        public override void drawAll()
        {
            poster.showPoster();
            cpu.addInfomation(InfoTable.TYPE.LEVEL, label, ConsoleColor.Yellow);
            cpu.addInfomation(InfoTable.TYPE.STATE, Game.OUG_DESCRIPTION, ConsoleColor.Red);
            int size = item.Length;
            for (int i = 0; i < size; i++)
            {
                Console.ForegroundColor = ConsoleColor.White;
                Console.SetCursorPosition(X, Y + i);
                Console.WriteLine(item[i]);
            }
            Console.ForegroundColor = Game.SELECT_COLOR;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
        }

        //di chuyen len
        private void itemUp()
        {
            Console.ForegroundColor = ConsoleColor.White;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
            if (index > 0) index--;
            else if (index == 0) index = item.Length - 1;
            Console.ForegroundColor = Game.SELECT_COLOR;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
        }


        //di chuyen xuong
        private void itemDown()
        {
            Console.ForegroundColor = ConsoleColor.White;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
            if (index < item.Length - 1) index++;
            else if (index == item.Length - 1) index = 0;
            Console.ForegroundColor = Game.SELECT_COLOR;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
        }
    }
}
