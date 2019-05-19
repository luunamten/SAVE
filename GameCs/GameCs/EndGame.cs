using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameCs
{
    class EndGame : Activity
    {
        const int X =39;
        const int Y =11;
        private Poster poster;
        string[] data;
        public EndGame(Poster poster, CentraProccessing cpu, string label,string[] data)
        {
            st = Game.OUG;
            this.label = label;
            this.poster = poster;
            this.cpu = cpu;
            this.data = data;
        }


        public override void work()
        {
            drawAll();
        }


        public override void getKey(char c)
        {

            switch (c)
            {
                case Game.ZERO_KEY:
                    cpu.popStack();
                    Activity a = cpu.getNextNotification();
                    
                    if (a != null)
                    {
                        cpu.pushStack(cpu.getNextNotification());
                        cpu.popNotification();
                    }
                    else
                    {
                        cpu.clearInfo();
                        cpu.drawInfoFrame();
                    }
                    cpu.topOfStackWork();
                    break;
                case Game.REFRESH_KEY:
                    cpu.showAllInfo();
                    drawAll();
                    break;
            }
        }

        //lam viec lai
        public override void workFirst()
        {
            drawAll();
        }

        //ve lai toan vo poster
        public override void drawAll()
        {
            poster.showPoster();
            if (data != null)
            {
                int size = data.Length;
                int y = Y - size / 2;
                Console.ForegroundColor = ConsoleColor.Cyan;
                for (int i = 0; i < size; i++)
                {
                    Console.SetCursorPosition(X - data[i].Length / 2, y + i);
                    Console.Write(data[i]);
                }
            }
            cpu.addInfomation(InfoTable.TYPE.LEVEL, label, ConsoleColor.Yellow);
            cpu.addInfomation(InfoTable.TYPE.STATE, Game.OUG_DESCRIPTION, ConsoleColor.Red);
        }
    }
}
