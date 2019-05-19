using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameCs
{
    class PosterActivity : Activity
    {
        private Poster poster;
        
        public PosterActivity(Poster poster,CentraProccessing cpu,string label)
        {
            st = Game.OUG;
            this.label = label;
            this.poster = poster;
            this.cpu = cpu;
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
                    Poster p = new Poster("poster/menu.mp");
                    FirstMenu tpmenu = new FirstMenu(p, cpu);
                    cpu.pushStack(tpmenu);
                    cpu.topOfStackWork();
                    break;
                case Game.REFRESH_KEY:
                    cpu.drawInfoFrame();
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
            cpu.addInfomation(InfoTable.TYPE.LEVEL, label, ConsoleColor.Yellow);
            cpu.addInfomation(InfoTable.TYPE.STATE, Game.OUG_DESCRIPTION, ConsoleColor.Red);
        }
    }
}
