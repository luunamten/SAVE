using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace GameCs
{
    class SpeedMenu : Activity
    {
        const int X = 28;
        const int Y=4;
        int index;
        Poster poster;
        string[] item= { "1.Toc do thap" , "2.Toc do trung binh" , "3.Toc do cao", "4.Master" };

        public SpeedMenu(Poster poster,CentraProccessing cpu)
        {
            this.poster = poster;
            this.cpu = cpu;
            index = 0;
            label = "S.Menu";
            st = Game.OUG;
        }
        public override void getKey(char key)
        {
            switch(key)
            {
                case Game.UP_KEY:
                    itemUp();
                    break;
                case Game.DOWN_KEY:
                    itemDown();
                    break;
                case Game.ZERO_KEY:
                    cpu.setSpeed(100 - index * 25);
                    cpu.clearInfo();
                    cpu.beginDrawing();
                    break;
                case Game.BACK_KEY:
                    if (cpu.getPlayType() == CentraProccessing.TypePlay.TUDO)
                    {
                        cpu.emptySourceMap();
                    }
                    cpu.popStack();
                    cpu.drawTopOfStack();
                    break;
                case Game.REFRESH_KEY:
                    cpu.drawInfoFrame();
                    drawAll();
                    break;
            }
        }

        public override void work()
        {
            drawAll(); 
            
        }

        public override void workFirst()
        {
            drawAll();
        }

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
