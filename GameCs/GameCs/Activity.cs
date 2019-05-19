using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameCs
{
    //lop chua cac map, poster, menu, xu ly du lieu tu ban phim
    abstract class Activity
    {
        protected int st;
        protected string label;
        protected CentraProccessing cpu;
        public abstract void workFirst();
        public abstract void drawAll();
        public abstract void work();
        public abstract void getKey(char key);
    }
}
