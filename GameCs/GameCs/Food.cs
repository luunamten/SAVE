using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameCs
{
    abstract class Food
    {
        protected int x;
        protected int y;
        protected int changeTime;
        protected string figure;
        public abstract void work();
        public abstract void show();
        public Food(int x, int y)
        {
            changeTime = 0;
            this.x = x;
            this.y = y;
        }
        public int X
        {
            get
            {
                return x;
            }
        }
        public int Y
        {
            get
            {
                return y;
            }
        }
    }
}
