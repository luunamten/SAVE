using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameCs
{
    class MapNotification:Activity
    {
        const int X = 39;
        const int Y = 11;
        private Poster poster;
        string[] data;
        Map map;
        User user;
        public MapNotification(Poster poster, CentraProccessing cpu, Map map, User user, string label,string[] data)
        {
            st = Game.OUG;
            this.label = label;
            this.poster = poster;
            this.cpu = cpu;
            this.data = data;
            this.map = map;
            this.user = user;
        }


        public override void work()
        {
            
        }


        public override void getKey(char c)
        {

            switch (c)
            {
                case Game.ZERO_KEY:
                    cpu.popStack();
                    MultiMapActivity mma = new MultiMapActivity(user, map, cpu, label);
                    cpu.pushStack(mma);
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
